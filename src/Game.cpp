// Game logic, rendering passes and ImGui HUD.
#include "Game.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>
#include <cstdio>

namespace {
constexpr float kPlayerRadius = 0.25f;   // vs. walls and props
constexpr float kLaserRadius  = 0.20f;   // vs. laser beams
constexpr float kDoorSpeed    = 40.0f;   // degrees per second
const glm::vec3 kStart(0.0f, 0.0f, 8.6f);
}

Game::Game() = default;
Game::~Game() = default;

// =============================================================== setup
void Game::init(GLFWwindow* window)
{
    window_ = window;
    glEnable(GL_DEPTH_TEST);

    shader_ = std::make_unique<Shader>("phong.vert", "phong.frag");
    cube_ = Mesh::cube();
    quad_ = Mesh::quad();
    cylinder_ = Mesh::cylinder();
    sphere_ = Mesh::sphere();

    white_ = Texture::white();
    tiles_ = Texture::tiles();
    concrete_ = Texture::concrete();
    metal_ = Texture::metal();
    money_ = Texture::money();

    cctvFbo_ = std::make_unique<Framebuffer>(512, 288);

    lights_.resize(6);
    buildLevel();
    reset();
    setMouseCapture(true);
}

void Game::reset()
{
    for (auto& p : piles_) { p.taken = false; p.node->visible = true; }
    for (auto& l : lasers_) l.active = true;
    gridBOn_ = true;
    loot_ = 0;
    doorAngle_ = doorTarget_ = 0.0f;
    god_ = false;
    state_ = GameState::Playing;
    alarmLevel_ = 0.0f;
    respawn();
}

void Game::respawn()
{
    cam_.feet = kStart;
    cam_.yaw = -90.0f;
    cam_.pitch = 0.0f;
    cam_.eyeHeight = Camera::kStandEye;
    state_ = GameState::Playing;
    invulnerable_ = 1.0f;
    doorTarget_ = 0.0f;              // vault re-locks
}

void Game::triggerAlarm()
{
    state_ = GameState::Alarm;
    alarmTimer_ = 2.5f;
}

void Game::setMouseCapture(bool capture)
{
    captured_ = capture;
    firstMouse_ = true;
    glfwSetInputMode(window_, GLFW_CURSOR, capture ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    if (capture && glfwRawMouseMotionSupported()) glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

// =============================================================== input events
void Game::onKey(int key, int action)
{
    if (action != GLFW_PRESS) return;
    switch (key) {
    case GLFW_KEY_ESCAPE:
        if (captured_) setMouseCapture(false);
        else glfwSetWindowShouldClose(window_, GLFW_TRUE);
        break;
    case GLFW_KEY_L: lightsOn_ = !lightsOn_; break;                 // interaction 1: toggle vault lights
    case GLFW_KEY_E: interact(); break;                              // interaction 2/3: panel + safe
    case GLFW_KEY_V: cctvIdx_ = 1 - cctvIdx_; break;                 // interaction 4: switch CCTV camera
    case GLFW_KEY_R: reset(); break;
    case GLFW_KEY_H: showHelp_ = !showHelp_; break;
    case GLFW_KEY_G: god_ = !god_; break;
    case GLFW_KEY_F2:
        vsync_ = !vsync_;
        glfwSwapInterval(vsync_ ? 1 : 0);
        break;
    default: break;
    }
}

void Game::onCursorPos(double x, double y)
{
    if (!captured_) { lastX_ = x; lastY_ = y; return; }
    if (firstMouse_) { lastX_ = x; lastY_ = y; firstMouse_ = false; return; }
    if (state_ == GameState::Playing)
        cam_.look(static_cast<float>(x - lastX_), static_cast<float>(y - lastY_));
    lastX_ = x;
    lastY_ = y;
}

void Game::onMouseButton(int button, int action)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !captured_ && !ImGui::GetIO().WantCaptureMouse)
        setMouseCapture(true);
}

bool Game::nearPanel() const
{
    return glm::distance(glm::vec2(cam_.feet.x, cam_.feet.z), glm::vec2(5.9f, 7.2f)) < 2.0f;
}

bool Game::nearSafe() const
{
    return glm::distance(glm::vec2(cam_.feet.x, cam_.feet.z), glm::vec2(0.0f, -8.5f)) < 2.6f;
}

void Game::interact()
{
    if (state_ != GameState::Playing) return;
    if (nearPanel()) {                                   // disable / enable the rotating laser grid
        gridBOn_ = !gridBOn_;
        for (auto& l : lasers_) if (l.group == 1) l.active = gridBOn_;
    } else if (nearSafe() && doorAngle_ > 90.0f) {       // grab the diamond -> win
        state_ = GameState::Won;
    }
}

// =============================================================== per-frame update
void Game::update(float dt)
{
    time_ += dt;
    ++frames_;
    fpsTimer_ += dt;
    if (fpsTimer_ >= 0.5f) { fps_ = frames_ / fpsTimer_; frames_ = 0; fpsTimer_ = 0.0f; }

    if (state_ == GameState::Alarm) {
        alarmTimer_ -= dt;
        alarmLevel_ = std::clamp(0.7f + 0.3f * std::sin(time_ * 14.0f), 0.0f, 1.0f);
        if (alarmTimer_ <= 0.0f) respawn();
    } else {
        alarmLevel_ = std::max(0.0f, alarmLevel_ - dt * 3.0f);
    }
    if (invulnerable_ > 0.0f) invulnerable_ -= dt;

    updatePlayer(dt);
    animate(dt);

    // laser collision (point/segment vs. box in laser space)
    if (state_ == GameState::Playing && !god_ && invulnerable_ <= 0.0f)
        for (const auto& l : lasers_)
            if (l.hits(cam_.feet, cam_.bodyHeight(), kLaserRadius)) { triggerAlarm(); break; }

    updateLights();

    // contextual hint
    hint_.clear();
    if (state_ == GameState::Playing) {
        if (nearPanel()) hint_ = gridBOn_ ? "[E] Disable rotating lasers" : "[E] Enable rotating lasers";
        else if (nearSafe() && doorAngle_ > 90.0f) hint_ = "[E] Grab the diamond!";
    }
}

void Game::updatePlayer(float dt)
{
    crouching_ = captured_ && (glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS ||
                               glfwGetKey(window_, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS);
    cam_.updateCrouch(crouching_, dt);

    if (state_ != GameState::Playing || !captured_) return;

    glm::vec3 wish(0.0f);
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) wish += cam_.flatForward();
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) wish -= cam_.flatForward();
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) wish += cam_.flatRight();
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) wish -= cam_.flatRight();
    if (glm::length(wish) > 0.001f) {
        float speed = crouching_ ? 1.6f : (glfwGetKey(window_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 5.0f : 3.2f);
        cam_.feet += glm::normalize(wish) * speed * dt;      // frame-rate independent
    }
    resolveCollisions();

    // collect money
    for (auto& p : piles_)
        if (!p.taken && glm::distance(glm::vec2(cam_.feet.x, cam_.feet.z), glm::vec2(p.pos.x, p.pos.z)) < 0.95f) {
            p.taken = true;
            p.node->visible = false;
            ++loot_;
        }

    // the vault unlocks as the player approaches the last laser
    if (cam_.feet.z < -5.0f) doorTarget_ = 1.0f;
}

void Game::resolveCollisions()
{
    cam_.feet.x = std::clamp(cam_.feet.x, -5.5f, 5.5f);
    cam_.feet.z = std::clamp(cam_.feet.z, -9.6f, 9.5f);

    for (const auto& b : obstacles_) {
        glm::vec2 p(cam_.feet.x, cam_.feet.z);
        glm::vec2 closest = glm::clamp(p, b.min, b.max);
        glm::vec2 d = p - closest;
        float dist = glm::length(d);
        if (dist < kPlayerRadius) {
            p = dist > 1e-5f ? closest + d / dist * kPlayerRadius : p + glm::vec2(0.0f, kPlayerRadius);
            cam_.feet.x = p.x;
            cam_.feet.z = p.y;
        }
    }
}

void Game::animate(float dt)
{
    for (auto& l : lasers_) l.update(time_);

    for (auto& s : spots_) s.arm->rotation.y = glm::degrees(time_ * s.speed + s.phase);

    for (int i = 0; i < 2; ++i)
        cctvPivot_[i]->rotation.y = (i == 0 ? 18.0f : 198.0f) + 14.0f * std::sin(time_ * 0.5f + i);

    // safe door: swings open/closed, wheel spins with it
    float target = doorTarget_ * 105.0f;
    if (doorAngle_ < target) doorAngle_ = std::min(target, doorAngle_ + kDoorSpeed * dt);
    else                     doorAngle_ = std::max(target, doorAngle_ - kDoorSpeed * dt);
    hinge_->rotation.y = -doorAngle_;
    wheelSpin_->rotation.y = doorAngle_ * 4.0f;
    diamond_->rotation = glm::vec3(0.0f, time_ * 70.0f, 0.0f);
    diamond_->position.y = 1.1f + 0.05f * std::sin(time_ * 2.0f);

    for (size_t i = 0; i < piles_.size(); ++i) {
        piles_[i].node->rotation.y = time_ * 60.0f + i * 40.0f;
        piles_[i].node->position.y = piles_[i].pos.y + 0.04f * std::sin(time_ * 2.0f + (float)i);
    }
}

void Game::updateLights()
{
    for (int i = 0; i < 3; ++i) lights_[i].enabled = lightsOn_;
    lights_[5].enabled = doorAngle_ > 5.0f;

    for (const auto& s : spots_) {               // spot position/direction come from the scene graph
        glm::mat4 w = s.head->world();
        glm::vec3 dir = glm::normalize(glm::vec3(w * glm::vec4(0, -1, 0, 0)));
        lights_[s.lightIndex].dir = dir;
        lights_[s.lightIndex].pos = glm::vec3(w[3]) + dir * 0.1f;
    }

    const glm::vec3 alarmRed(1.0f, 0.05f, 0.05f);
    glm::vec3 fixtureOn(1.0f, 0.95f, 0.8f), fixtureOff(0.06f);
    for (auto* f : fixtures_)
        f->material.emissive = glm::mix(lightsOn_ ? fixtureOn : fixtureOff, alarmRed, alarmLevel_);

    panelLed_->material.emissive = gridBOn_ ? glm::vec3(1.0f, 0.1f, 0.1f) : glm::vec3(0.1f, 1.0f, 0.2f);
}

// =============================================================== rendering
void Game::uploadLights()
{
    int n = 0;
    char buf[48];
    auto name = [&](int idx, const char* field) {
        std::snprintf(buf, sizeof(buf), "uLights[%d].%s", idx, field);
        return std::string(buf);
    };
    for (const auto& L : lights_) {
        if (!L.enabled || n >= 8) continue;
        shader_->setInt(name(n, "type"), L.type);
        shader_->setVec3(name(n, "pos"), L.pos);
        shader_->setVec3(name(n, "dir"), L.dir);
        shader_->setVec3(name(n, "color"), L.color);
        shader_->setFloat(name(n, "intensity"), L.intensity);
        shader_->setFloat(name(n, "innerCos"), L.innerCos);
        shader_->setFloat(name(n, "outerCos"), L.outerCos);
        ++n;
    }
    shader_->setInt("uNumLights", n);
}

void Game::renderScene(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos, bool cctv)
{
    shader_->use();
    shader_->setMat4("uView", view);
    shader_->setMat4("uProj", proj);
    shader_->setVec3("uViewPos", viewPos);
    shader_->setFloat("uTime", time_);
    shader_->setFloat("uAlarm", alarmLevel_);
    shader_->setVec3("uAmbient", lightsOn_ ? glm::vec3(0.10f, 0.11f, 0.15f) : glm::vec3(0.03f, 0.03f, 0.045f));
    shader_->setInt("uTex", 0);
    uploadLights();

    root_->render(*shader_, false, cctv);                 // opaque pass

    glEnable(GL_BLEND);                                   // additive laser halos
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);
    root_->render(*shader_, true, cctv);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

glm::mat4 Game::cctvView(glm::vec3& eyeOut) const
{
    glm::mat4 w = cctvPivot_[cctvIdx_]->world();
    glm::vec3 fwd = glm::normalize(glm::vec3(w * glm::vec4(0.0f, -0.22f, -1.0f, 0.0f)));
    eyeOut = glm::vec3(w[3]) + fwd * 0.3f;
    return glm::lookAt(eyeOut, eyeOut + fwd, glm::vec3(0, 1, 0));
}

void Game::render(int fbW, int fbH)
{
    fbW_ = std::max(fbW, 1);
    fbH_ = std::max(fbH, 1);

    // pass 1: security camera -> texture (render-to-texture)
    cctvFbo_->bind();
    glViewport(0, 0, cctvFbo_->width(), cctvFbo_->height());
    glClearColor(0.02f, 0.02f, 0.03f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::vec3 camEye;
    glm::mat4 camView = cctvView(camEye);
    renderScene(camView, glm::perspective(glm::radians(75.0f), (float)cctvFbo_->width() / cctvFbo_->height(), 0.1f, 100.0f),
                camEye, true);
    Framebuffer::unbind();

    // pass 2: player view
    glViewport(0, 0, fbW_, fbH_);
    glClearColor(0.02f, 0.02f, 0.03f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderScene(cam_.view(), cam_.projection((float)fbW_ / fbH_), cam_.eye(), false);
}

// =============================================================== HUD (Dear ImGui)
void Game::drawUI()
{
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiWindowFlags overlay = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowPos(ImVec2(12, 12));
    ImGui::SetNextWindowBgAlpha(0.5f);
    if (ImGui::Begin("HUD", nullptr, overlay | ImGuiWindowFlags_NoInputs)) {
        ImGui::Text("FPS: %.0f", fps_);
        ImGui::Text("Loot: %d / %d", loot_, (int)piles_.size());
        ImGui::Text("Stance: %s", crouching_ ? "crouching" : "standing");
        ImGui::Text("Lights: %s   Rotating lasers: %s", lightsOn_ ? "ON" : "OFF", gridBOn_ ? "ARMED" : "OFF");
        if (god_) ImGui::TextColored(ImVec4(1, 0.8f, 0.2f, 1), "GOD MODE (lasers ignored)");
    }
    ImGui::End();

    if (showHelp_) {
        ImGui::SetNextWindowPos(ImVec2(12, io.DisplaySize.y - 12), ImGuiCond_Always, ImVec2(0, 1));
        ImGui::SetNextWindowBgAlpha(0.5f);
        if (ImGui::Begin("Controls", nullptr, overlay)) {
            ImGui::TextUnformatted("CONTROLS  (H hides this panel)");
            ImGui::Separator();
            ImGui::TextUnformatted("WASD move   Shift run   C / Ctrl crouch   Mouse look");
            ImGui::TextUnformatted("E interact (wall panel / safe)   L toggle lights");
            ImGui::TextUnformatted("V switch CCTV camera   R restart   Esc free mouse");
            ImGui::TextUnformatted("Goal: cross the laser room, reach the safe, press E");
            ImGui::Separator();
            if (ImGui::Checkbox("God mode (G)", &god_)) {}
            if (ImGui::Checkbox("VSync (F2)", &vsync_)) glfwSwapInterval(vsync_ ? 1 : 0);
        }
        ImGui::End();
    }

    ImDrawList* fg = ImGui::GetForegroundDrawList();
    ImVec2 centre(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    fg->AddCircleFilled(centre, 2.5f, IM_COL32(255, 255, 255, 200));

    auto centred = [&](const char* text, float size, float yOffset, ImU32 col) {
        ImVec2 ts = ImGui::GetFont()->CalcTextSizeA(size, 10000.0f, 0.0f, text);
        fg->AddText(ImGui::GetFont(), size, ImVec2(centre.x - ts.x * 0.5f, centre.y + yOffset), col, text);
    };

    if (!hint_.empty()) centred(hint_.c_str(), 26.0f, 60.0f, IM_COL32(255, 230, 120, 255));
    if (state_ == GameState::Alarm) {
        centred("ALARM!", 90.0f, -140.0f, IM_COL32(255, 40, 40, 255));
        centred("You touched a laser - back to the entrance...", 28.0f, -30.0f, IM_COL32(255, 200, 200, 255));
    } else if (state_ == GameState::Won) {
        char msg[64];
        std::snprintf(msg, sizeof(msg), "Loot collected: %d / %d", loot_, (int)piles_.size());
        centred("VAULT CRACKED!", 90.0f, -140.0f, IM_COL32(255, 215, 80, 255));
        centred(msg, 32.0f, -30.0f, IM_COL32(255, 255, 255, 255));
        centred("Press R to play again", 26.0f, 20.0f, IM_COL32(200, 200, 200, 255));
    } else if (!captured_) {
        centred("Click to capture the mouse", 26.0f, 100.0f, IM_COL32(255, 255, 255, 220));
    }
}
