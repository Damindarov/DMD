/**
 * @file simple_boolean.cpp
 * @author DMD team, IU
 * @brief This program applys simple boolean (DifferenceAB) operation inbetween the ideal and defect meshes. Assuming that ideal and defect meshes are in the same coordinates
 * @version 0.1
 * @date 2024-11-08
 * @dependencies: MeshLib - An open-source 3D geometry library for processing, editing,
 *                and manipulating 3D meshes. https://github.com/MeshInspector/MeshLib
 */

#include <iostream>
#include <filesystem>

#include <MRMesh/MRMesh.h>
#include <MRMesh/MRMeshLoad.h>
#include <MRMesh/MRMeshBoolean.h>
#include <MRMesh/MRMeshSave.h>
#include <MRMesh/MRUVSphere.h>

// file paths for ideal and defective meshes
std::filesystem::path ideal_mesh_path = "../meshes/cylinder_matrix_ideal_fh_ar.stl";   // ideal.stl
std::filesystem::path defect_mesh_path = "../meshes/cylinder_matrix_defect_icpgl.stl"; // defect.stl correctted transformation using global and local icps

// file path for output mesh
std::filesystem::path out_mesh_path = "../meshes/simple_boolean_output.stl";

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        ideal_mesh_path = argv[1];
        defect_mesh_path = argv[2];
        std::cout << "Using user given paths: " << ideal_mesh_path.string() << ", " << defect_mesh_path.string() << std::endl;
    }
    else
    {
        std::cout << "Usage:./meshlib_simple_boolean <ideal.stl> <defect.stl>" << std::endl;
        std::cout << "Using default paths: " << ideal_mesh_path.string() << ", " << defect_mesh_path.string() << std::endl;
    }
    // read the ideal mesh
    MR::Mesh ideal_mesh = *MR::MeshLoad::fromAnyStl(ideal_mesh_path);
    // read the defect mesh
    MR::Mesh defect_mesh = *MR::MeshLoad::fromAnyStl(defect_mesh_path);

    std::cout << "Performing boolean operation (DifferenceAB)..." << std::endl;
    // perform boolean operation (Difference)
    MR::BooleanResult result = MR::boolean(ideal_mesh, defect_mesh, MR::BooleanOperation::DifferenceAB);
    MR::Mesh resultMesh = *result;
    if (!result.valid())
        std::cerr << result.errorString << std::endl;

    std::cout << "Saving the result mesh to: " << out_mesh_path << std::endl;
    // save result to STL file
    MR::MeshSave::toAnySupportedFormat(resultMesh, out_mesh_path);

    return 0;
}