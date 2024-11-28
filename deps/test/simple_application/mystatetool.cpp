#include "mystatetool.hpp"

using namespace ExamplePlugin;

MyStateTool::MyStateTool()
  : StatePlugin("My State Tool") 
{

}

void MyStateTool::drawDialog(float menuScaling, ImGuiContext *)
{
	if (!ImGuiBeginWindow_({.width = 200 * menuScaling}))
		return;
	UI::transparentTextWrapped("Hell to World!");
	ImGui::EndCustomStatePlugin();
}

bool MyStateTool::onEnable_()
{
	obj_ = MR::getAllObjectsInTree<MR::Object>(&MR::SceneRoot::get(), MR::ObjectSelectivityType::Selected)[0];
	return true;
}

bool MyStateTool::onDisable_()
{
	obj_.reset();
	return true;
}