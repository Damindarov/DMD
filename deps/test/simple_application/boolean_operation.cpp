#include "boolean_operation.hpp"

using namespace ExamplePlugin;

BoolOperation::BoolOperation()
	: RibbonMenuItem("Boolean Operation")
{
}

bool BoolOperation::action()
{
	auto selected_objects_in_tree = MR::getAllObjectsInTree<MR::Object>(&MR::SceneRoot::get(), MR::ObjectSelectivityType::Selected);

	if (selected_objects_in_tree.size() != 2)
	{
		showModal("Please select two objects", MR::NotificationType::Info);
		return false;
	}

	if (auto obj_mesh_1 = dynamic_pointer_cast<MR::ObjectMesh>(selected_objects_in_tree[0]))
	{
		if (auto obj_mesh_2 = dynamic_pointer_cast<MR::ObjectMesh>(selected_objects_in_tree[1]))
		{
			// getting non-const mesh pointers from selected objects
			std::shared_ptr<MR::Mesh> mesh_1 = obj_mesh_1->varMesh();
			std::shared_ptr<MR::Mesh> mesh_2 = obj_mesh_2->varMesh();

			MR::Mesh result_mesh = performBoolOperation(*mesh_1, *mesh_2);
			std::shared_ptr<MR::ObjectMesh> result_obj_mesh = std::make_shared<MR::ObjectMesh>();
			
			// here we can skip type casting check because be already know that
			//  both objects are ObjectMesh that are child of VisualObject class
			result_obj_mesh->asType<VisualObject>()->setDirtyFlags(DIRTY_ALL);

			MR::SceneRoot::get().addChild(result_obj_mesh);
		} // if(auto obj_mesh_2 = selected_objects_in_tree[1].asType<MR::ObjectMesh>())
		else
		{
			showModal("Second selected object have to be ObjectMesh", MR::NotificationType::Info);
			return false;
		}
	} // if(auto obj_mesh_1 = selected_objects_in_tree[0].asType<MR::ObjectMesh>())
	else
	{
		showModal("First selected object have to be ObjectMesh", MR::NotificationType::Info);
		return false;
	}
	return true; // true will lead Viewer to keep this tool activated in ribbon
}

/**
 * @brief Performs a boolean operation (Difference) on the ideal and defect meshes.
 *
 * @param ideal_mesh Reference to the ideal mesh.
 * @param defect_mesh Reference to the defect mesh.
 */
inline MR::Mesh BoolOperation::performBoolOperation(MR::Mesh &ideal_mesh, MR::Mesh &defect_mesh)
{
	std::cout << "Performing boolean operation (DifferenceAB)..." << std::endl;
	MR::BooleanResult result = MR::boolean(ideal_mesh, defect_mesh, MR::BooleanOperation::DifferenceAB);
	if (!result.valid())
	{
		std::cerr << result.errorString << std::endl;
		return MR::Mesh();
	}
	// save result to STL file
	return *result;
}