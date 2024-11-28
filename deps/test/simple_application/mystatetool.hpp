#ifndef MY_STATE_TOOL_HPP
#define MY_STATE_TOOL_HPP

#include <MRViewer/MRRibbonMenuItem.h>
#include <MRViewer/MRRibbonMenu.h>
#include <MRViewer/MRUIStyle.h>
#include <MRViewer/MRRibbonSchema.h>
#include <MRViewer/MRSceneCache.h>
// #include <MRMesh/MRObjectsAccess.hpp>  //included to MRViewer/MRSceneCache.h
#include <MRMesh/MRObject.h>

namespace ExamplePlugin
{
	using namespace MR;

	class MyStateTool : public StatePlugin
	{
	public:
		MyStateTool();

		virtual void drawDialog(float menuScaling, ImGuiContext *) override;
		virtual bool onEnable_() override;
		virtual bool onDisable_() override;

		private : std::shared_ptr<MR::Object> obj_;
	};

	MR_REGISTER_RIBBON_ITEM(MyStateTool)
};

#endif