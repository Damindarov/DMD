#include "mytool.hpp"

using namespace ExamplePlugin;

MyTool::MyTool()
  : RibbonMenuItem("My Tool") 
{}

bool MyTool::action()
{
	showModal("AAAAAAAA Hell to World!", MR::NotificationType::Info);
	return false; // true will lead Viewer to keep this tool activated in ribbon
}