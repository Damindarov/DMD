#ifndef MY_STATE_TOOL_HPP
#define MY_STATE_TOOL_HPP

#include <MRViewer/MRRibbonMenuItem.h>
#include <MRViewer/MRRibbonMenu.h>
#include <MRViewer/MRUIStyle.h>
#include <MRViewer/MRRibbonSchema.h>

namespace ExamplePlugin
{
	using namespace MR;

	class MyStateTool : public StatePlugin
	{
	public:
		MyStateTool();

		virtual void drawDialog(float menuScaling, ImGuiContext *) override;
	};

	MR_REGISTER_RIBBON_ITEM(MyStateTool)
};

#endif