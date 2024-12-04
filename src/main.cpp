/**
 * @file main.cpp
 * @author DMD team, IU
 * @brief Runs the pipeline for reading, filling holes, rebuilding, icp registration, boolean and saving the output mesh.
 * @version 0.1
 * @date 2024-11-09
 * @dependencies: MeshLib - An open-source 3D geometry library for processing, editing,
 *                and manipulating 3D meshes. https://github.com/MeshInspector/MeshLib
 * @notes: This pipeline is using the ICP registration algorithm from MeshLib which is local registration based,
 *         but if we have the defective mesh too much disoriented from the ideal mesh, then we have to apply global registration first then apply local registration.
 */

#include <memory>
#include "Pipeline.h"

int main(int argc, char **argv)
{

    // default file paths for ideal and defective meshes
    std::filesystem::path ideal_path = "../meshes/cylinder_matrix_ideal.stl";   // ideal.stl
    std::filesystem::path defect_path = "../meshes/cylinder_matrix_defect.stl"; // defect.stl

    if (argc > 2)
    {
        ideal_path = argv[1];
        defect_path = argv[2];
        std::cout << "Using user given paths: " << ideal_path.string() << ", " << defect_path.string() << std::endl;
    }
    else
    {
        std::cout << "Usage: ./meshlib_main <ideal.stl> <defect.stl>" << std::endl;
        std::cout << "Using default paths: " << ideal_path.string() << ", " << defect_path.string() << std::endl;
    }

    // run pipeline on ideal and defective meshes
    // use unique pointer to avoid memory leaks if any
    std::unique_ptr<DMD::Pipeline> pipeline = std::make_unique<DMD::Pipeline>(ideal_path, defect_path);
    if (pipeline->run() == 0)
    {
        std::cout << "Pipline run success" << std::endl;
    }
    else
    {
        std::cout << "Failed to run the pipline" << std::endl;
    }

    /*
    // run pipeline on ideal and defective meshes
    DMD::Pipeline pipeline(ideal_path, defect_path);
    pipeline.run();
    */

    return 0;
}