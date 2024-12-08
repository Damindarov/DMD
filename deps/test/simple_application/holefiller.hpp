#ifndef HOLE_FILLER_HPP
#define HOLE_FILLER_HPP

#include <MRViewer/MRRibbonMenuItem.h>
#include <MRViewer/MRRibbonMenu.h>
#include <MRViewer/MRUIStyle.h>
#include <MRViewer/MRRibbonSchema.h>

#include <MRViewer/MRSceneCache.h>
#include <MRMesh/MRMeshLoad.h>
#include <MRMesh/MRMesh.h>
#include <MRMesh/MRObjectMesh.h>
#include <MRMesh/MRMeshFillHole.h>
#include <MRVoxels/MRRebuildMesh.h>
#include <memory>
#include <iostream>

#include "helper_functions.hpp"

namespace ExamplePlugin
{
	using namespace MR;

	class HoleFiller : public RibbonMenuItem
	{
	public:
		HoleFiller();
		virtual bool action() override;

		void fillAndRebuildMesh(MR::Mesh &mesh);
		void fillHoles(MR::Mesh &mesh);
		MR::Expected<MR::Mesh> reBuild(MR::Mesh &mesh);
		static bool onProgress(float v);
	};

	MR_REGISTER_RIBBON_ITEM(HoleFiller)
};

#endif