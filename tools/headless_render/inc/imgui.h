#pragma once
#include <cstdio>
struct ImVec2{float x,y; ImVec2(float a=0,float b=0):x(a),y(b){}}; struct ImVec4{float x,y,z,w; ImVec4(float a,float b,float c,float d):x(a),y(b),z(c),w(d){}};
typedef unsigned ImU32; typedef int ImGuiWindowFlags; typedef int ImGuiCond;
#define IM_COL32(r,g,b,a) ((ImU32)(r))
#define IMGUI_CHECKVERSION()
enum{ImGuiWindowFlags_NoDecoration=1,ImGuiWindowFlags_AlwaysAutoResize=2,ImGuiWindowFlags_NoSavedSettings=4,ImGuiWindowFlags_NoFocusOnAppearing=8,ImGuiWindowFlags_NoNav=16,ImGuiWindowFlags_NoMove=32,ImGuiWindowFlags_NoInputs=64,ImGuiCond_Always=1};
struct ImFont{ ImVec2 CalcTextSizeA(float,float,float,const char*) const {return ImVec2();} };
struct ImDrawList{ void AddCircleFilled(ImVec2,float,ImU32){} void AddText(ImFont*,float,ImVec2,ImU32,const char*){} };
struct ImGuiIO{ ImVec2 DisplaySize; bool WantCaptureMouse=false; const char* IniFilename; };
struct ImDrawData;
namespace ImGui{ ImGuiIO& GetIO(); bool Begin(const char*,bool*,int); void End(); void Text(const char*,...); void TextUnformatted(const char*); void TextColored(ImVec4,const char*,...);
 void Separator(); bool Checkbox(const char*,bool*); void SetNextWindowPos(ImVec2,int=0,ImVec2=ImVec2()); void SetNextWindowBgAlpha(float);
 ImDrawList* GetForegroundDrawList(); ImFont* GetFont(); void CreateContext(); void DestroyContext(); void StyleColorsDark(); void NewFrame(); void Render(); ImDrawData* GetDrawData(); }
