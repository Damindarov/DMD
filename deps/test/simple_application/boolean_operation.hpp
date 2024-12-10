#ifndef BOOLEAN_OPERATION_HPP
#define BOOLEAN_OPERATION_HPP

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

	class BooleanOperation : public RibbonMenuItem
	{
	public:
		LocalICP();

		// \brief Called when the menu item is clicked
		virtual bool action() override;

	private:
		void performBooleanOperation(MR::Mesh &ideal_mesh, MR::Mesh &defect_mesh);
	};

	MR_REGISTER_RIBBON_ITEM(BooleanOperation)
};

#endif