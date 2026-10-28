// Laser Vault Heist - Computer Graphics Fundamentals final project
// OpenGL 3.3 core | GLFW | GLAD | GLM | Dear ImGui
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <cstdio>
#include <exception>
#include <string>

#include "Game.h"

namespace {
Game* g_game = nullptr;
}

int main()
{
    if (!glfwInit()) {
        std::fprintf(stderr, "Failed to initialise GLFW\n");
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Laser Vault Heist", nullptr, nullptr);
    if (!window) {
        std::fprintf(stderr, "Failed to create window (OpenGL 3.3 core required)\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::fprintf(stderr, "Failed to load OpenGL functions\n");
        return 1;
    }
    glEnable(GL_MULTISAMPLE);

    int exitCode = 0;
    try {
        Game game;
        g_game = &game;

        // Our callbacks are installed first; ImGui chains to them (install_callbacks = true).
        glfwSetKeyCallback(window, [](GLFWwindow*, int key, int, int action, int) {
            if (g_game) g_game->onKey(key, action);
        });
        glfwSetCursorPosCallback(window, [](GLFWwindow*, double x, double y) {
            if (g_game) g_game->onCursorPos(x, y);
        });
        glfwSetMouseButtonCallback(window, [](GLFWwindow*, int button, int action, int) {
            if (g_game) g_game->onMouseButton(button, action);
        });

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::GetIO().IniFilename = nullptr;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        game.init(window);

        double last = glfwGetTime(), titleTimer = 0.0;
        while (!glfwWindowShouldClose(window)) {
            double now = glfwGetTime();
            float dt = static_cast<float>(now - last);
            last = now;
            if (dt > 0.1f) dt = 0.1f;      // avoid huge steps after window drags / breakpoints

            glfwPollEvents();
            game.update(dt);

            int w, h;
            glfwGetFramebufferSize(window, &w, &h);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            game.render(w, h);
            game.drawUI();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);

            titleTimer += dt;
            if (titleTimer > 0.5) {                 // FPS counter in the window title (R7)
                titleTimer = 0.0;
                char title[96];
                std::snprintf(title, sizeof(title), "Laser Vault Heist  |  %.0f FPS", game.fps());
                glfwSetWindowTitle(window, title);
            }
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        g_game = nullptr;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "Fatal error: %s\n", e.what());
        exitCode = 1;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return exitCode;
}
