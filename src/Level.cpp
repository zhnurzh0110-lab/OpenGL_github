// Builds the vault: room, lasers, guards, safe, CCTV, money, props.
// Everything is assembled from 4 primitive meshes (cube, quad, cylinder, sphere).
#include "Game.h"
#include <cmath>
#include <string>

namespace {
const glm::vec3 kLaserColor(1.0f, 0.06f, 0.06f);
}

Material Game::makeMaterial(const Texture* tex, glm::vec3 tint, float specular, float shininess) const
{
    Material m;
    m.tex = tex;
    m.tint = tint;
    m.specular = specular;
    m.shininess = shininess;
    return m;
}

SceneNode* Game::pivot(SceneNode* parent, const std::string& name, glm::vec3 pos, glm::vec3 rot)
{
    SceneNode* n = parent->addChild(name);
    n->position = pos;
    n->rotation = rot;
    return n;
}

SceneNode* Game::geo(SceneNode* parent, const std::string& name, const Mesh* mesh, glm::vec3 pos,
                     glm::vec3 size, const Material& mat, glm::vec3 rot)
{
    SceneNode* n = parent->addChild(name);
    n->mesh = mesh;
    n->position = pos;
    n->rotation = rot;
    n->scale = size;
    n->material = mat;
    return n;
}

void Game::addLaser(const std::string& name, LaserMotion motion, glm::vec3 base, float halfLength,
                    float yawDeg, float amp, float speed, float phase, int group, bool emitters)
{
    Material core;
    core.tex = &white_;
    core.mode = ShadeMode::Laser;
    core.emissive = kLaserColor * 1.6f;

    Material halo;
    halo.tex = &white_;
    halo.mode = ShadeMode::Glow;
    halo.emissive = kLaserColor;
    halo.alpha = 0.22f;

    Laser l;
    l.pivot = pivot(root_.get(), name, base, glm::vec3(0, yawDeg, 0));
    l.motion = motion;
    l.base = base;
    l.half = glm::vec3(halfLength, 0.02f, 0.02f);
    l.amp = amp;
    l.speed = speed;
    l.phase = phase;
    l.group = group;

    geo(l.pivot, "core", cube_.get(), glm::vec3(0), glm::vec3(2 * halfLength, 0.04f, 0.04f), core);
    geo(l.pivot, "halo", cube_.get(), glm::vec3(0), glm::vec3(2 * halfLength, 0.16f, 0.16f), halo);

    if (emitters) {   // little wall-mounted emitter boxes at both ends of full-width beams
        Material dark = makeMaterial(&metal_, glm::vec3(0.35f), 0.8f, 64.0f);
        dark.worldUV = true;
        dark.uvScale = 1.0f;
        for (float side : { -1.0f, 1.0f })
            geo(root_.get(), name + "_emitter", cube_.get(), glm::vec3(side * 5.9f, base.y, base.z),
                glm::vec3(0.2f, 0.2f, 0.2f), dark);
    }
    lasers_.push_back(l);
}

void Game::addGuard(glm::vec3 mountPos, float speed, float phase, int lightIndex)
{
    Material metal = makeMaterial(&metal_, glm::vec3(0.6f), 0.9f, 96.0f);
    metal.worldUV = true;
    Material lens;
    lens.tex = &white_;
    lens.mode = ShadeMode::Emissive;
    lens.emissive = glm::vec3(1.0f, 0.95f, 0.75f);

    SceneNode* mount = pivot(root_.get(), "guard_mount", mountPos);
    geo(mount, "guard_plate", cylinder_.get(), glm::vec3(0, 0.03f, 0), glm::vec3(0.3f, 0.08f, 0.3f), metal);

    SceneNode* arm = pivot(mount, "guard_arm", glm::vec3(0, -0.05f, 0));          // rotates about Y
    geo(arm, "guard_bar", cube_.get(), glm::vec3(0.25f, 0, 0), glm::vec3(0.5f, 0.06f, 0.06f), metal);

    SceneNode* head = pivot(arm, "guard_head", glm::vec3(0.5f, -0.08f, 0), glm::vec3(0, 0, 32.9f)); // tilted lamp
    geo(head, "guard_body", cylinder_.get(), glm::vec3(0, 0.0f, 0), glm::vec3(0.22f, 0.3f, 0.22f), metal);
    geo(head, "guard_lens", cylinder_.get(), glm::vec3(0, -0.16f, 0), glm::vec3(0.17f, 0.02f, 0.17f), lens);

    spots_.push_back({ arm, head, speed, phase, lightIndex });
}

void Game::addMoney(glm::vec3 pos)
{
    Material ped = makeMaterial(&metal_, glm::vec3(0.55f, 0.6f, 0.7f), 0.8f, 64.0f);
    ped.worldUV = true;
    Material bill = makeMaterial(&money_, glm::vec3(1.0f), 0.2f, 16.0f);
    bill.uvScale = 1.0f;
    Material band = makeMaterial(&white_, glm::vec3(0.9f, 0.85f, 0.6f), 0.2f, 16.0f);

    geo(root_.get(), "pedestal", cube_.get(), glm::vec3(pos.x, 0.35f, pos.z), glm::vec3(0.6f, 0.7f, 0.6f), ped);
    obstacles_.push_back({ glm::vec2(pos.x - 0.3f, pos.z - 0.3f), glm::vec2(pos.x + 0.3f, pos.z + 0.3f) });

    SceneNode* p = pivot(root_.get(), "money_pile", glm::vec3(pos.x, 0.82f, pos.z));
    geo(p, "bundle1", cube_.get(), glm::vec3(0, 0.00f, 0), glm::vec3(0.40f, 0.10f, 0.20f), bill);
    geo(p, "bundle2", cube_.get(), glm::vec3(0, 0.10f, 0), glm::vec3(0.40f, 0.10f, 0.20f), bill, glm::vec3(0, 20, 0));
    geo(p, "band", cube_.get(), glm::vec3(0, 0.05f, 0), glm::vec3(0.06f, 0.12f, 0.22f), band);
    piles_.push_back({ p, glm::vec3(pos.x, 0.82f, pos.z), false });
}

void Game::buildLevel()
{
    root_ = std::make_unique<SceneNode>("root");
    SceneNode* r = root_.get();

    // ------------------------------------------------------------ materials
    Material floorM = makeMaterial(&tiles_, glm::vec3(1.0f), 0.8f, 96.0f);
    floorM.worldUV = true; floorM.uvScale = 0.5f;
    Material wallM = makeMaterial(&concrete_, glm::vec3(1.0f), 0.15f, 16.0f);
    wallM.worldUV = true; wallM.uvScale = 0.35f;
    Material columnM = makeMaterial(&concrete_, glm::vec3(0.55f, 0.6f, 0.7f), 0.3f, 32.0f);
    columnM.worldUV = true; columnM.uvScale = 0.5f;
    Material metalM = makeMaterial(&metal_, glm::vec3(0.8f, 0.85f, 0.95f), 0.9f, 96.0f);
    metalM.worldUV = true; metalM.uvScale = 0.6f;
    Material darkM = makeMaterial(&white_, glm::vec3(0.07f, 0.07f, 0.09f), 0.5f, 48.0f);
    Material goldM = makeMaterial(&white_, glm::vec3(1.0f, 0.76f, 0.22f), 1.0f, 128.0f);
    Material yellowM = makeMaterial(&white_, glm::vec3(0.95f, 0.8f, 0.1f), 0.1f, 8.0f);

    // ------------------------------------------------------------ room shell
    geo(r, "floor",   cube_.get(), { 0, -0.1f, 0 },     { 12.0f, 0.2f, 20.0f }, floorM);
    geo(r, "ceiling", cube_.get(), { 0, 3.6f, 0 },      { 12.4f, 0.2f, 20.4f }, wallM);
    geo(r, "wall_back",  cube_.get(), { 0, 1.75f, -10.1f }, { 12.4f, 3.5f, 0.2f }, wallM);
    geo(r, "wall_front", cube_.get(), { 0, 1.75f, 10.1f },  { 12.4f, 3.5f, 0.2f }, wallM);
    geo(r, "wall_left",  cube_.get(), { -6.1f, 1.75f, 0 },  { 0.2f, 3.5f, 20.4f }, wallM);
    geo(r, "wall_right", cube_.get(), { 6.1f, 1.75f, 0 },   { 0.2f, 3.5f, 20.4f }, wallM);

    for (float z : { 4.0f, -1.5f, -6.0f })                       // structural columns
        for (float side : { -1.0f, 1.0f })
            geo(r, "column", cube_.get(), { side * 5.75f, 1.75f, z }, { 0.5f, 3.5f, 0.5f }, columnM);
    for (float z : { 7.5f, 2.0f, -3.5f, -8.0f })                 // ceiling beams
        geo(r, "beam", cube_.get(), { 0, 3.38f, z }, { 12.0f, 0.24f, 0.4f }, columnM);

    // hazard tape on the floor under the two full-width static lasers
    for (float z : { -1.5f, -6.0f })
        geo(r, "tape", cube_.get(), { 0, 0.006f, z }, { 12.0f, 0.012f, 0.25f }, yellowM);

    // ------------------------------------------------------------ ceiling lights
    Material fixtureM;
    fixtureM.tex = &white_;
    fixtureM.mode = ShadeMode::Emissive;
    fixtureM.emissive = glm::vec3(1.0f, 0.95f, 0.8f);
    const float lightZ[3] = { 5.5f, -1.0f, -7.0f };
    for (int i = 0; i < 3; ++i) {
        fixtures_.push_back(geo(r, "fixture", cube_.get(), { 0, 3.43f, lightZ[i] }, { 1.6f, 0.08f, 0.5f }, fixtureM));
        Light L;
        L.type = 1;
        L.pos = glm::vec3(0, 3.1f, lightZ[i]);
        L.color = glm::vec3(1.0f, 0.93f, 0.8f);
        L.intensity = 1.7f;
        lights_[i] = L;
    }

    // ------------------------------------------------------------ lasers
    // Zone A: two low sliding beams -> time the gaps at the side lanes (must go around)
    addLaser("laser_slideA1", LaserMotion::SlideX, { 0, 0.55f, 6.0f }, 4.5f, 0, 1.0f, 1.4f, 0.0f, 0, false);
    addLaser("laser_slideA2", LaserMotion::SlideX, { 0, 0.55f, 4.8f }, 4.5f, 0, 1.0f, 1.4f, 3.14159f, 0, false);

    // Zone B: two rotating beams around pillars; group 1 -> switch off at the wall panel
    for (int i = 0; i < 2; ++i) {
        float x = i == 0 ? -2.7f : 2.7f;
        geo(r, "pillar", cylinder_.get(), { x, 1.75f, 2.0f }, { 0.25f, 3.5f, 0.25f }, metalM);
        obstacles_.push_back({ glm::vec2(x - 0.2f, 1.8f), glm::vec2(x + 0.2f, 2.2f) });
        addLaser("laser_rot" + std::to_string(i), LaserMotion::Rotate, { x, 0.9f, 2.0f }, 2.6f, 0, 0.0f,
                 i == 0 ? 1.1f : -1.1f, i == 0 ? 0.0f : 1.5708f, 1, false);
    }

    // Zone C: sliding low beam + a full-width "limbo" beam you must crouch under
    addLaser("laser_slideC", LaserMotion::SlideX, { 0, 0.45f, -0.5f }, 4.5f, 0, 1.0f, 1.9f, 1.0f, 0, false);
    addLaser("laser_limbo", LaserMotion::Static, { 0, 1.1f, -1.5f }, 6.0f, 0, 0, 0, 0, 0, true);

    // Zone D: full-width beam sweeping up and down -> crouch in the window when it is high
    addLaser("laser_sweep", LaserMotion::SweepY, { 0, 0.95f, -3.8f }, 6.0f, 0, 0.75f, 1.6f, 0.0f, 0, true);

    // Zone E: the last laser, slide under it while the safe opens
    addLaser("laser_last", LaserMotion::Static, { 0, 1.05f, -6.0f }, 6.0f, 0, 0, 0, 0, 0, true);

    // ------------------------------------------------------------ money on pedestals
    addMoney({ -5.0f, 0, 7.6f });
    addMoney({ 5.0f, 0, 4.2f });
    addMoney({ -5.0f, 0, 0.6f });
    addMoney({ 5.0f, 0, -2.6f });
    addMoney({ -5.0f, 0, -5.0f });

    // ------------------------------------------------------------ guard spotlights
    Light spot;
    spot.type = 2;
    spot.color = glm::vec3(1.0f, 0.95f, 0.8f);
    spot.intensity = 6.0f;
    spot.innerCos = std::cos(glm::radians(11.0f));
    spot.outerCos = std::cos(glm::radians(21.0f));
    lights_[3] = spot;
    lights_[4] = spot;
    addGuard({ -3.0f, 3.46f, -0.5f }, 0.9f, 0.0f, 3);
    addGuard({ 3.0f, 3.46f, -5.0f }, -1.1f, 2.0f, 4);

    // ------------------------------------------------------------ the safe (hierarchy: safe -> hinge -> door -> wheel)
    SceneNode* safe = pivot(r, "safe", glm::vec3(0));
    geo(safe, "safe_back",   cube_.get(), { 0, 1.2f, -9.9f },   { 2.4f, 2.4f, 0.2f }, metalM);
    geo(safe, "safe_left",   cube_.get(), { -1.1f, 1.2f, -9.2f }, { 0.2f, 2.4f, 1.4f }, metalM);
    geo(safe, "safe_right",  cube_.get(), { 1.1f, 1.2f, -9.2f },  { 0.2f, 2.4f, 1.4f }, metalM);
    geo(safe, "safe_top",    cube_.get(), { 0, 2.3f, -9.2f },   { 2.4f, 0.2f, 1.4f }, metalM);
    geo(safe, "safe_bottom", cube_.get(), { 0, 0.1f, -9.2f },   { 2.4f, 0.2f, 1.4f }, metalM);
    obstacles_.push_back({ glm::vec2(-1.2f, -10.0f), glm::vec2(1.2f, -8.5f) });

    for (int i = 0; i < 3; ++i)                                         // gold bars inside
        geo(safe, "gold", cube_.get(), { -0.6f + 0.6f * i, 0.29f, -9.3f }, { 0.45f, 0.18f, 0.7f }, goldM);
    Material gem;
    gem.tex = &white_;
    gem.mode = ShadeMode::Emissive;
    gem.emissive = glm::vec3(0.35f, 0.95f, 1.0f);
    diamond_ = pivot(safe, "diamond_pivot", glm::vec3(0, 1.1f, -9.2f));
    geo(diamond_, "diamond", cube_.get(), glm::vec3(0), glm::vec3(0.38f), gem, glm::vec3(35, 0, 35));

    hinge_ = pivot(safe, "safe_hinge", glm::vec3(-1.2f, 1.2f, -8.5f));
    geo(hinge_, "safe_door", cube_.get(), { 1.2f, 0, 0.075f }, { 2.4f, 2.4f, 0.15f }, metalM);
    SceneNode* wheelTilt = pivot(hinge_, "wheel_tilt", { 1.2f, 0, 0.16f }, glm::vec3(90, 0, 0));
    wheelSpin_ = pivot(wheelTilt, "wheel_spin", glm::vec3(0));
    geo(wheelSpin_, "wheel_hub", cylinder_.get(), glm::vec3(0), { 0.4f, 0.12f, 0.4f }, goldM);
    geo(wheelSpin_, "wheel_bar1", cube_.get(), glm::vec3(0), { 1.1f, 0.1f, 0.1f }, goldM);
    geo(wheelSpin_, "wheel_bar2", cube_.get(), glm::vec3(0), { 0.1f, 0.1f, 1.1f }, goldM);

    Light glow;
    glow.type = 1;
    glow.pos = glm::vec3(0, 1.4f, -9.0f);
    glow.color = glm::vec3(1.0f, 0.8f, 0.35f);
    glow.intensity = 1.0f;
    glow.enabled = false;           // switched on when the door opens
    lights_[5] = glow;

    // ------------------------------------------------------------ alarm panel (right wall) and security monitor (left wall)
    Material panelM = makeMaterial(&metal_, glm::vec3(0.45f, 0.5f, 0.55f), 0.8f, 64.0f);
    panelM.worldUV = true;
    geo(r, "panel", cube_.get(), { 5.93f, 1.35f, 7.2f }, { 0.12f, 0.7f, 0.5f }, panelM);
    Material led;
    led.tex = &white_;
    led.mode = ShadeMode::Emissive;
    led.emissive = glm::vec3(1.0f, 0.1f, 0.1f);
    panelLed_ = geo(r, "panel_led", sphere_.get(), { 5.86f, 1.5f, 7.2f }, glm::vec3(0.12f), led);
    geo(r, "panel_button", cylinder_.get(), { 5.86f, 1.22f, 7.2f }, { 0.14f, 0.05f, 0.14f }, goldM, glm::vec3(0, 0, 90));

    geo(r, "monitor_body", cube_.get(), { -5.94f, 1.7f, 7.2f }, { 0.12f, 1.05f, 1.75f }, darkM);
    Material screenM;
    screenM.tex = &cctvFbo_->color();
    screenM.mode = ShadeMode::Screen;
    screen_ = geo(r, "monitor_screen", quad_.get(), { -5.872f, 1.7f, 7.2f }, { 1.55f, 0.87f, 1.0f }, screenM,
                  glm::vec3(0, 90, 0));
    screen_->hideInCCTV = true;      // the monitor must not appear inside its own picture

    // ------------------------------------------------------------ CCTV cameras (pivot yaws slowly)
    Material camM = makeMaterial(&white_, glm::vec3(0.12f), 0.7f, 64.0f);
    const glm::vec3 camPos[2] = { { 5.4f, 3.25f, 9.4f }, { -5.4f, 3.25f, -9.4f } };
    for (int i = 0; i < 2; ++i) {
        cctvPivot_[i] = pivot(r, "cctv_pivot", camPos[i], glm::vec3(0, i == 0 ? 18.0f : 198.0f, 0));
        geo(cctvPivot_[i], "cctv_body", cube_.get(), glm::vec3(0), { 0.22f, 0.2f, 0.45f }, camM);
        geo(cctvPivot_[i], "cctv_lens", cylinder_.get(), { 0, 0, -0.25f }, { 0.12f, 0.08f, 0.12f }, led, glm::vec3(90, 0, 0));
    }
}
