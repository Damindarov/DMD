#include "local_icp.hpp"

using namespace ExamplePlugin;

LocalICP::LocalICP()
  : RibbonMenuItem("Local ICP") 
{}

bool LocalICP::action()
{
	auto selected_objects_in_tree = MR::getAllObjectsInTree<MR::Object>(&MR::SceneRoot::get(), MR::ObjectSelectivityType::Selected);

	if(selected_objects_in_tree.size() != 2)
	{
		showModal("Please select two objects", MR::NotificationType::Info);
		return false;
	}
	auto mesh_opt_1 = convertObjectToMesh(selected_objects_in_tree[0]);
	if(mesh_opt_1)
	{
		showModal("First selected object have to be ObjectMesh", MR::NotificationType::Info);
		return false;
	}

	auto mesh_opt_2 = convertObjectToMesh(selected_objects_in_tree[1]);
	if (mesh_opt_2)
	{
		showModal("Second selected object have to be ObjectMesh", MR::NotificationType::Info);
		return false;
	}
	//meshes.reserve(selected_objects_in_tree.size());// reserve space for all names
	
	MR::Mesh mesh_1 = **mesh_opt_1;
	MR::Mesh mesh_2 = **mesh_opt_2;

	performLocalICP(mesh_1, mesh_2);
	
	return true; // true will lead Viewer to keep this tool activated in ribbon
}

void LocalICP::performLocalICP(MR::Mesh &ideal_mesh, MR::Mesh &defect_mesh)
{
	// following ICP is adapted from examples/mesh_ICP.cpp
	std::cout << "\nPerforming local ICP..." << std::endl;

	// Prepare ICP parameters
	float diagonal = ideal_mesh.getBoundingBox().diagonal();
	MR::ICPProperties icpParams;
	icpParams.distThresholdSq = MR::sqr(diagonal * 0.1f); // Use points pairs with maximum distance specified
	icpParams.exitVal = diagonal * 0.003f;				  // Stop when distance reached

	MR::ICP icp(MR::MeshOrPoints{MR::MeshPart{defect_mesh}},
				MR::MeshOrPoints{MR::MeshPart{ideal_mesh}},
				MR::AffineXf3f(), MR::AffineXf3f(),
				diagonal * 0.01f); // To sample points from object
	icp.setParams(icpParams);
	MR::AffineXf3f xf = icp.calculateTransformation();
	defect_mesh.transform(xf);
}