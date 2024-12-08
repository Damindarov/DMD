#include "holefiller.hpp"

using namespace ExamplePlugin;

HoleFiller::HoleFiller()
  : RibbonMenuItem("Hole Filler") 
{}

bool HoleFiller::action()
{
	auto selected_objects_in_tree = MR::getAllObjectsInTree<MR::Object>(&MR::SceneRoot::get(), MR::ObjectSelectivityType::Selected);

	for (auto &&obj : selected_objects_in_tree)
	{
		std::shared_ptr<MR::ObjectMesh> obj_mesh = std::static_pointer_cast<MR::ObjectMesh>(obj->cloneTree());
		std::shared_ptr<MR::Mesh> mesh = obj_mesh->varMesh();
		fillAndRebuildMesh(*mesh);
		if (auto visObj = obj_mesh->asType<VisualObject>())
			visObj->setDirtyFlags(DIRTY_ALL);
		obj_mesh->setName(obj_mesh->name() + "_holes_filled")
		MR::SceneRoot::get().addChild(obj_mesh);
	}

	//loadMesh(selected_objects_in_tree);
	showModal("Holes filled", MR::NotificationType::Info);
	return false; // true will lead Viewer to keep this tool activated in ribbon
}

void HoleFiller::fillAndRebuildMesh(MR::Mesh &mesh)
{
	std::cout << "\nFilling holes in mesh..." << std::endl;
	fillHoles(mesh);
	std::cout << "Rebuilding mesh..." << std::endl;
	auto rebuilt_mesh = reBuild(mesh);
	if (!rebuilt_mesh)
	{
		std::cerr << "Error: cannot rebuild the mesh" << std::endl;
	}
	mesh = *rebuilt_mesh;
}

void HoleFiller::fillHoles(MR::Mesh &mesh)
{
	auto holeEdges = mesh.topology.findHoleRepresentiveEdges();
	for (MR::EdgeId e : holeEdges)
	{
		MR::FillHoleParams params;
		params.metric = MR::getUniversalMetric(mesh);
		MR::fillHole(mesh, e, params);
	}
}

MR::Expected<MR::Mesh> HoleFiller::reBuild(MR::Mesh &mesh)
{
	// rebuildMesh params setting
	MR::RebuildMeshSettings settings;
	settings.decimate = false;
	settings.voxelSize = 0.278;		// in mm
	settings.progress = onProgress; // callback for progress

	MR::MeshPart meshPart(mesh);
	return MR::rebuildMesh(meshPart, settings);
}

bool HoleFiller::onProgress(float v)
{
	static int gProgress = -1;
	int progress = static_cast<int>(10.f * v);
	if (progress != gProgress)
	{
		gProgress = progress;
		std::cout << "\r" << std::flush << ((progress + 1) * 10) << "% completed.";
	}
	return true;
}