#ifndef BOOL_OPERATION_HPP
#define BOOL_OPERATION_HPP

#include <MRViewer/MRRibbonMenuItem.h>
#include <MRViewer/MRRibbonMenu.h>
#include <MRViewer/MRUIStyle.h>
#include <MRViewer/MRRibbonSchema.h>

#include <MRMesh/MRMeshBoolean.h>
#include <MRViewer/MRSceneCache.h>
#include <MRMesh/MRMesh.h>
#include <MRMesh/MRICP.h>
#include <memory>
#include <iostream>

#include "helper_functions.hpp"

namespace ExamplePlugin
{
	using namespace MR;

	class BoolOperation : public RibbonMenuItem
	{
	public:
		BoolOperation();

		// \brief Called when the menu item is clicked
		virtual bool action() override;

	private:
		//MR::Mesh performBoolOperation(MR::Mesh &ideal_mesh, MR::Mesh &defect_mesh);
	};

	MR_REGISTER_RIBBON_ITEM(BoolOperation)
};

#endif