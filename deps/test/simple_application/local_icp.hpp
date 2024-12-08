#ifndef LOCAL_ICP_HPP
#define LOCAL_ICP_HPP

#include <MRViewer/MRRibbonMenuItem.h>
#include <MRViewer/MRRibbonMenu.h>
#include <MRViewer/MRUIStyle.h>
#include <MRViewer/MRRibbonSchema.h>

#include <MRViewer/MRSceneCache.h>
#include <MRMesh/MRMesh.h>
#include <MRMesh/MRICP.h>
#include <memory>
#include <iostream>

#include "helper_functions.hpp"

namespace ExamplePlugin
{
	using namespace MR;

	class LocalICP : public RibbonMenuItem
	{
	public:
		LocalICP();
		
		// \brief Called when the menu item is clicked
		virtual bool action() override;
	private:

		// \brief Perform local ICP (finds diffeteces between two meshes)
		void performLocalICP(MR::Mesh &ideal_mesh, MR::Mesh &defect_mesh);
	};

	MR_REGISTER_RIBBON_ITEM(LocalICP)
};

#endif