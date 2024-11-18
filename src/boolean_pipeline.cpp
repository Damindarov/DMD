/**
 * @file boolean_pipeline.cpp
 * @author DMD team, IU
 * @brief The pipeline for reading, filling holes, rebuilding, icp registration, boolean and saving the output mesh.
 * @version 0.1
 * @date 2024-11-07
 * @dependencies: MeshLib - An open-source 3D geometry library for processing, editing,
 *                and manipulating 3D meshes. https://github.com/MeshInspector/MeshLib
 * @notes: This pipeline is using the ICP registration algorithm from MeshLib which is local registration based,
 *         but if we have the defective mesh too much disoriented from the ideal mesh, then we have to apply global registration first then apply local registration.
 */

#include <iostream>
#include <functional>

#include <MRMesh/MRMesh.h>
#include <MRMesh/MRMeshLoad.h>
#include <MRMesh/MRMeshFillHole.h>
#include <MRVoxels/MRRebuildMesh.h>
#include <MRMesh/MRMeshPart.h>
#include <MRMesh/MRMeshSave.h>
#include <MRMesh/MRBox.h>
#include <MRMesh/MRICP.h>
#include <MRMesh/MRMeshBoolean.h>
#include <MRMesh/MRCube.h>
#include <MRMesh/MRVector3.h>

// bool cancelRequested = false;

// file paths for ideal and defective meshes
std::filesystem::path ideal_mesh_path = "../meshes/cylinder_matrix_ideal.stl";   // detal_ideal.stl
std::filesystem::path defect_mesh_path = "../meshes/cylinder_matrix_defect.stl"; // detal_defect.stl

// // rebuilding mesh progress bar
// bool progressCallback(float progress)
// {
//     std::cout << "Progress: " << (progress * 100) << "%" << std::endl;
//     //std::cout << "\r" << std::flush << " reBuilding the mesh - Processing: " << (progress * 100) << "% completed.";
//     if (cancelRequested) {
//         std::cout << "Operation cancelled!" << std::endl;
//         return false;
//     }
//     return true;
// }

// print progress every 10%
int gProgress = -1;
bool onProgress(float v)
{
    int progress = (int)(10.f * v);
    if (progress != gProgress)
    {
        gProgress = progress;
        // printf( "%d%%...\n", progress * 10 );
        std::cout << "\r" << std::flush << ((progress + 1) * 10) << "% completed.";
    }
    return true;
}

void resetProgress(void)
{
    gProgress = -1;
}

// fills the holes in the mesh
void fillHoles(MR::Mesh &mesh)
{
    // fillHoles is the adaptation from examples/fillHoles.cpp
    // Find single edge for each hole in mesh
    std::vector<MR::EdgeId> holeEdges = mesh.topology.findHoleRepresentiveEdges();

    for (MR::EdgeId e : holeEdges)
    {
        // Setup filling parameters
        MR::FillHoleParams params;
        params.metric = MR::getUniversalMetric(mesh);
        // Fill hole represented by `e`
        MR::fillHole(mesh, e, params);
    }
}

// rebuilds the mesh
MR::Expected<MR::Mesh> reBuild(MR::Mesh &mesh)
{
    // rebuildMesh params setting
    MR::RebuildMeshSettings settings;
    settings.decimate = false;
    settings.voxelSize = 0.278;
    settings.progress = onProgress;

    // convert mesh to MeshPart
    MR::MeshPart mesh2(mesh);

    // rebuild and return
    return MR::rebuildMesh(mesh2, settings);
}

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
        std::cout << "Usage:./meshlib_boolean_pipeline <ideal.stl> <defect.stl>" << std::endl;
        std::cout << "Using default paths: " << ideal_mesh_path.string() << ", " << defect_mesh_path.string() << std::endl;
    }

    // read the ideal mesh
    MR::Mesh ideal_mesh = *MR::MeshLoad::fromAnyStl(ideal_mesh_path);

    std::cout << "Filling holes on Ideal mesh..." << std::endl;
    // fill holes on ideal mesh
    fillHoles(ideal_mesh);

    std::cout << "Rebuilding Ideal mesh..." << std::endl;
    // rebuild ideal mesh
    MR::Mesh ideal_mesh2;
    if (auto result = reBuild(ideal_mesh))
    {
        ideal_mesh2 = *result;
    }
    else
    {
        std::cout << "Error: cannot rebuild the ideal mesh" << std::endl;
    }

    // read the defect mesh
    MR::Mesh defect_mesh = *MR::MeshLoad::fromAnyStl(defect_mesh_path);

    std::cout << "\nFilling holes on Defect mesh..." << std::endl;
    // fill holes on defect mesh
    fillHoles(defect_mesh);

    std::cout << "Rebuilding Defect mesh..." << std::endl;
    // rebuild defect mesh
    MR::Mesh defect_mesh2;
    if (auto result = reBuild(defect_mesh))
    {
        defect_mesh2 = *result;
    }
    else
    {
        std::cout << "Error: cannot rebuild the defect mesh" << std::endl;
    }

    std::cout << "\nPerforming local ICP between Ideal and Defect Meshes..." << std::endl;
    // following ICP is adapted from examples/mesh_ICP.cpp
    // Prepare ICP parameters
    float diagonal = ideal_mesh2.getBoundingBox().diagonal();
    float icpSamplingVoxelSize = diagonal * 0.01f; // To sample points from object
    MR::ICPProperties icpParams;
    icpParams.distThresholdSq = MR::sqr(diagonal * 0.1f); // Use points pairs with maximum distance specified
    icpParams.exitVal = diagonal * 0.003f;                // Stop when distance reached

    // Calculate transformation
    MR::ICP icp(
        MR::MeshOrPoints{MR::MeshPart{defect_mesh2}},
        MR::MeshOrPoints{MR::MeshPart{ideal_mesh2}},
        MR::AffineXf3f(), MR::AffineXf3f(),
        icpSamplingVoxelSize);
    icp.setParams(icpParams);
    MR::AffineXf3f xf = icp.calculateTransformation();

    // Transform floating mesh
    defect_mesh2.transform(xf);

    std::cout << "Saving the repaired meshes..." << std::endl;
    // save rebuild and transformed meshes
    MR::MeshSave::toAnySupportedFormat(ideal_mesh2, "../meshes/fillHoles_reBuild_ideal_mesh.stl");
    MR::MeshSave::toAnySupportedFormat(defect_mesh2, "../meshes/fillHoles_reBuild_defect_icp_mesh.stl");

    std::cout << "Performing boolean operation (DifferenceAB)..." << std::endl;
    // perform boolean operation (Difference)
    MR::BooleanResult result = MR::boolean(ideal_mesh2, defect_mesh2, MR::BooleanOperation::DifferenceAB);
    MR::Mesh resultMesh = *result;
    if (!result.valid())
        std::cerr << result.errorString << std::endl;

    std::cout << "Saving the result mesh..." << std::endl;
    // save result to STL file
    MR::MeshSave::toAnySupportedFormat(resultMesh, "../meshes/out_boolean.stl");

    // params for making a cube
    // MR::Vector3f cube_size(100.0f, 100.0f, 100.0f);
    // MR::Vector3f cube_base(0.0f, 0.0f, 0.0f);

    // // create cube mesh
    // MR::Mesh cube = MR::makeCube(cube_size, cube_base);

    // // following ICP is adapted from examples/mesh_ICP.cpp
    // // Prepare ICP parameters
    // diagonal = resultMesh.getBoundingBox().diagonal();
    // icpSamplingVoxelSize = diagonal * 0.01f; // To sample points from object

    // icpParams.distThresholdSq = MR::sqr( diagonal * 0.1f ); // Use points pairs with maximum distance specified
    // icpParams.exitVal = diagonal * 0.003f; // Stop when distance reached

    // // Calculate transformation
    // MR::ICP icp2(
    //     MR::MeshOrPoints{ MR::MeshPart{ cube } },
    //     MR::MeshOrPoints{ MR::MeshPart{ resultMesh } },
    //     MR::AffineXf3f(), MR::AffineXf3f(),
    //     icpSamplingVoxelSize );
    // icp2.setParams( icpParams );
    // xf = icp2.calculateTransformation();

    // // transform the cube to the ideal mesh
    // cube.transform(xf);

    // // save the cube mesh
    // MR::MeshSave::toAnySupportedFormat( cube, "../meshes/cube.stl" );

    // // perform boolean operation (Difference)
    // MR::BooleanResult filtered = MR::boolean(resultMesh, cube, MR::BooleanOperation::Intersection );
    // MR::Mesh filteredMesh = *filtered;
    // if ( !filtered.valid() )
    //     std::cerr << filtered.errorString << std::endl;

    // // save filtered_mesh to STL file
    // MR::MeshSave::toAnySupportedFormat( filteredMesh, "../meshes/out_boolean_filtered.stl" );

    return 0;
}