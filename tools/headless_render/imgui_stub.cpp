// No-op ImGui so Game.cpp links in the headless test (the HUD is not drawn there).
#include "imgui.h"
namespace ImGui {
static ImGuiIO io; static ImDrawList dl; static ImFont font;
ImGuiIO& GetIO(){return io;} bool Begin(const char*,bool*,int){return false;} void End(){}
void Text(const char*,...){} void TextUnformatted(const char*){} void TextColored(ImVec4,const char*,...){}
void Separator(){} bool Checkbox(const char*,bool*){return false;} void SetNextWindowPos(ImVec2,int,ImVec2){} void SetNextWindowBgAlpha(float){}
ImDrawList* GetForegroundDrawList(){return &dl;} ImFont* GetFont(){return &font;}
}
