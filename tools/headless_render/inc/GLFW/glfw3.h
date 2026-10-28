#pragma once
struct GLFWwindow;
constexpr int GLFW_CONTEXT_VERSION_MAJOR=1;
constexpr int GLFW_CONTEXT_VERSION_MINOR=2;
constexpr int GLFW_CURSOR=3;
constexpr int GLFW_CURSOR_DISABLED=4;
constexpr int GLFW_CURSOR_NORMAL=5;
constexpr int GLFW_KEY_A=6;
constexpr int GLFW_KEY_C=7;
constexpr int GLFW_KEY_D=8;
constexpr int GLFW_KEY_E=9;
constexpr int GLFW_KEY_ESCAPE=10;
constexpr int GLFW_KEY_F2=11;
constexpr int GLFW_KEY_G=12;
constexpr int GLFW_KEY_H=13;
constexpr int GLFW_KEY_L=14;
constexpr int GLFW_KEY_LEFT_CONTROL=15;
constexpr int GLFW_KEY_LEFT_SHIFT=16;
constexpr int GLFW_KEY_R=17;
constexpr int GLFW_KEY_S=18;
constexpr int GLFW_KEY_V=19;
constexpr int GLFW_KEY_W=20;
constexpr int GLFW_MOUSE_BUTTON_LEFT=21;
constexpr int GLFW_OPENGL_CORE_PROFILE=22;
constexpr int GLFW_OPENGL_FORWARD_COMPAT=23;
constexpr int GLFW_OPENGL_PROFILE=24;
constexpr int GLFW_PRESS=25;
constexpr int GLFW_RAW_MOUSE_MOTION=26;
constexpr int GLFW_SAMPLES=27;
constexpr int GLFW_TRUE=28;
template<class...A> inline GLFWwindow* glfwCreateWindow(A...){ return nullptr; }
template<class...A> inline int glfwDestroyWindow(A...){ return 0; }
template<class...A> inline int glfwGetFramebufferSize(A...){ return 0; }
template<class...A> inline int glfwGetKey(A...){ return 0; }
template<class...A> inline void* glfwGetProcAddress(A...){ return nullptr; }
template<class...A> inline double glfwGetTime(A...){ return 0; }
template<class...A> inline int glfwInit(A...){ return 0; }
template<class...A> inline int glfwMakeContextCurrent(A...){ return 0; }
template<class...A> inline int glfwPollEvents(A...){ return 0; }
template<class...A> inline int glfwRawMouseMotionSupported(A...){ return 0; }
template<class...A> inline int glfwSetCursorPosCallback(A...){ return 0; }
template<class...A> inline int glfwSetInputMode(A...){ return 0; }
template<class...A> inline int glfwSetKeyCallback(A...){ return 0; }
template<class...A> inline int glfwSetMouseButtonCallback(A...){ return 0; }
template<class...A> inline int glfwSetWindowShouldClose(A...){ return 0; }
template<class...A> inline int glfwSetWindowTitle(A...){ return 0; }
template<class...A> inline int glfwSwapBuffers(A...){ return 0; }
template<class...A> inline int glfwSwapInterval(A...){ return 0; }
template<class...A> inline int glfwTerminate(A...){ return 0; }
template<class...A> inline int glfwWindowHint(A...){ return 0; }
template<class...A> inline int glfwWindowShouldClose(A...){ return 0; }
