#ifndef MY_TOOL_HPP
#define MY_TOOL_HPP

#include <MRViewer/MRRibbonMenuItem.h>
#include <MRViewer/MRRibbonMenu.h>
#include <MRViewer/MRUIStyle.h>
#include <MRViewer/MRRibbonSchema.h>

namespace ExamplePlugin
{
	using namespace MR;

	class MyTool : public RibbonMenuItem
	{
	public:
		MyTool();
		virtual bool action() override;

	private:
	};

	MR_REGISTER_RIBBON_ITEM(MyTool)
};

#endif