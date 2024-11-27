#include "mystatetool.hpp"

using namespace ExamplePlugin;

MyStateTool::MyStateTool()
  : StatePlugin("My State Tool") 
{}

void MyStateTool::drawDialog(float menuScaling, ImGuiContext *)
{
	if (!ImGuiBeginWindow_({.width = 200 * menuScaling}))
		return;
	UI::transparentTextWrapped("Hell to World!");
	ImGui::EndCustomStatePlugin();
}