#pragma once
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Framebuffer.h"
#include "Laser.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "SceneNode.h"
#include "Shader.h"
#include "Texture.h"

struct GLFWwindow;

enum class GameState { Playing, Alarm, Won };

struct Box2 { glm::vec2 min, max; };                    // XZ obstacle for player collision
struct MoneyPile { SceneNode* node; glm::vec3 pos; bool taken = false; };
struct GuardSpot { SceneNode* arm; SceneNode* head; float speed; float phase; int lightIndex; };

class Game {
public:
    Game();
    ~Game();

    void init(GLFWwindow* window);
    void update(float dt);
    void render(int fbWidth, int fbHeight);
    void drawUI();

    // GLFW event forwarding (see main.cpp)
    void onKey(int key, int action);
    void onCursorPos(double x, double y);
    void onMouseButton(int button, int action);

    float fps() const { return fps_; }

private:
    // ---- setup (Level.cpp) ----
    void buildLevel();
    Material makeMaterial(const Texture* tex, glm::vec3 tint, float specular, float shininess) const;
    SceneNode* pivot(SceneNode* parent, const std::string& name, glm::vec3 pos, glm::vec3 rot = glm::vec3(0.0f));
    SceneNode* geo(SceneNode* parent, const std::string& name, const Mesh* mesh, glm::vec3 pos,
                   glm::vec3 size, const Material& mat, glm::vec3 rot = glm::vec3(0.0f));
    void addLaser(const std::string& name, LaserMotion motion, glm::vec3 base, float halfLength,
                  float yawDeg, float amp, float speed, float phase, int group, bool emitters);
    void addGuard(glm::vec3 mountPos, float speed, float phase, int lightIndex);
    void addMoney(glm::vec3 pos);

    // ---- game logic (Game.cpp) ----
    void reset();
    void respawn();
    void triggerAlarm();
    void updatePlayer(float dt);
    void resolveCollisions();
    void animate(float dt);
    void updateLights();
    void interact();
    void setMouseCapture(bool capture);
    bool nearPanel() const;
    bool nearSafe() const;

    // ---- rendering ----
    void renderScene(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos, bool cctv);
    void uploadLights();
    glm::mat4 cctvView(glm::vec3& eyeOut) const;

    GLFWwindow* window_ = nullptr;

    // resources
    std::unique_ptr<Shader> shader_;
    std::unique_ptr<Mesh> cube_, quad_, cylinder_, sphere_;
    Texture white_, tiles_, concrete_, metal_, money_;
    std::unique_ptr<Framebuffer> cctvFbo_;
    std::unique_ptr<SceneNode> root_;

    // scene references
    std::vector<Laser> lasers_;
    std::vector<GuardSpot> spots_;
    std::vector<MoneyPile> piles_;
    std::vector<Box2> obstacles_;
    std::vector<SceneNode*> fixtures_;
    std::vector<Light> lights_;
    SceneNode* hinge_ = nullptr;
    SceneNode* wheelSpin_ = nullptr;
    SceneNode* diamond_ = nullptr;
    SceneNode* panelLed_ = nullptr;
    SceneNode* cctvPivot_[2] = { nullptr, nullptr };
    SceneNode* screen_ = nullptr;

    // state
    Camera cam_;
    GameState state_ = GameState::Playing;
    float time_ = 0.0f;
    float alarmTimer_ = 0.0f, alarmLevel_ = 0.0f, invulnerable_ = 0.0f;
    float doorAngle_ = 0.0f, doorTarget_ = 0.0f;     // degrees
    int   loot_ = 0;
    bool  lightsOn_ = true, gridBOn_ = true, god_ = false, showHelp_ = true, vsync_ = true;
    bool  crouching_ = false, captured_ = false, firstMouse_ = true;
    int   cctvIdx_ = 0;
    double lastX_ = 0.0, lastY_ = 0.0;
    int   fbW_ = 1280, fbH_ = 720;
    std::string hint_;

    // fps counter
    float fps_ = 0.0f, fpsTimer_ = 0.0f;
    int   frames_ = 0;
};
