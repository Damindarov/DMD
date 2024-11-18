/**
 * @file global_local_icp.cpp
 * @author DMD team, IU
 * @brief This program applys global registration first then apply local registration to the ideal and defect meshes.
 * @version 0.1
 * @date 2024-11-08
 * @dependencies: MeshLib - An open-source 3D geometry library for processing, editing,
 *               and manipulating 3D meshes. https://github.com/MeshInspector/MeshLib
 */

#include <iostream>
#include <functional>
#include <filesystem>
#include <sstream>

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
#include <MRMesh/MRMultiwayICP.h>
#include <MRMesh/MRPointCloud.h>
#include <MRMesh/MRPointsLoad.h>
#include <MRMesh/MRPointsSave.h>
#include <MRMesh/MRString.h>

// file paths for ideal and defective meshes
std::filesystem::path ideal_mesh_path = "../meshes/cylinder_matrix_ideal_fh_ar.stl";   // detal_ideal.stl
std::filesystem::path defect_mesh_path = "../meshes/cylinder_matrix_defect_fh_ar.stl"; // detal_defect.stl

/**
 * @brief Callback function to report progress during operations.
 *
 * @param v A float value representing the progress (0.0 to 1.0).
 * @return true if the operation should continue, false otherwise.
 */
bool onProgress(float v)
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

std::string affineToString(const MR::AffineXf3f &transform)
{
    std::ostringstream oss;
    auto translation = transform.b;
    auto rotation = transform.A;
    oss << "Translation: (" << translation.x << ", "
        << translation.y << ", "
        << translation.z << "), "
        << "Rotation: (" << rotation.x.x << ", " << rotation.x.y << ", " << rotation.x.z << ", "
        << rotation.y.x << ", " << rotation.y.y << ", " << rotation.y.z << ", "
        << rotation.z.x << ", " << rotation.z.y << rotation.z.z << ")";
    return oss.str();
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
        std::cout << "Usage:./meshlib_global_local_icp <ideal.stl> <defect.stl>" << std::endl;
        std::cout << "Using default paths: " << ideal_mesh_path.string() << ", " << defect_mesh_path.string() << std::endl;
    }

    // read the ideal mesh
    MR::Mesh ideal_mesh = *MR::MeshLoad::fromAnyStl(ideal_mesh_path);

    // read the defect mesh
    MR::Mesh defect_mesh = *MR::MeshLoad::fromAnyStl(defect_mesh_path);

    float diagonal = ideal_mesh.getBoundingBox().diagonal();

    // apply global ICP in between the ideal and defect meshs
    std::cout << "\nPerforming global ICP between Ideal and Defect Meshes..." << std::endl;
    MR::MultiwayICPSamplingParameters sampling_parameters;
    sampling_parameters.samplingVoxelSize = diagonal * 0.01f;
    sampling_parameters.cb = onProgress;

    // create the ICP objects
    MR::ICPObjects objects;
    objects.push_back(MR::MeshOrPointsXf(ideal_mesh));
    objects.push_back(MR::MeshOrPointsXf(defect_mesh));

    // initialize MultiwayICP
    MR::MultiwayICP multiwayICP(objects, sampling_parameters);

    MR::ICPProperties icpParams1;
    icpParams1.distThresholdSq = MR::sqr(diagonal * 0.1f); // Use points pairs with maximum distance specified
    icpParams1.exitVal = diagonal * 0.003f;                // Stop when distance reached
    icpParams1.iterLimit = 1000;

    multiwayICP.setParams(icpParams1);

    // Calculate transformations
    MR::Vector<MR::AffineXf3f, MR::ObjId> transformations = multiwayICP.calculateTransformationsFixFirst();

    std::cout << "\nTransformations size: " << transformations.size() << std::endl;

    MR::AffineXf3f xf = transformations.back();

    std::cout << "Transformation: " << affineToString(xf) << std::endl;

    // apply the transformation obtained from the global icp
    defect_mesh.transform(xf);

    MR::MeshSave::toAnySupportedFormat(defect_mesh, "../meshes/cylinder_matrix_defect_icpg.stl");

    // apply local ICP in between the ideal and defect meshs
    std::cout << "Performing local ICP between Ideal and Defect Meshes..." << std::endl;
    // following ICP is adapted from examples/mesh_ICP.cpp
    // Prepare ICP parameters
    float diagonal2 = ideal_mesh.getBoundingBox().diagonal();
    float icpSamplingVoxelSize = diagonal2 * 0.01f; // To sample points from object
    MR::ICPProperties icpParams;
    icpParams.distThresholdSq = MR::sqr(diagonal2 * 0.1f); // Use points pairs with maximum distance specified
    icpParams.exitVal = diagonal2 * 0.003f;                // Stop when distance reached
    icpParams.iterLimit = 1000;

    // Calculate transformation
    MR::ICP icp(
        MR::MeshOrPoints{MR::MeshPart{defect_mesh}},
        MR::MeshOrPoints{MR::MeshPart{ideal_mesh}},
        MR::AffineXf3f(), MR::AffineXf3f(),
        icpSamplingVoxelSize);
    icp.setParams(icpParams);
    MR::AffineXf3f xf2 = icp.calculateTransformation();

    // apply the transformation obtained from the local icp
    defect_mesh.transform(xf2);
    std::cout << "Transformation: " << affineToString(xf2) << std::endl;

    MR::MeshSave::toAnySupportedFormat(defect_mesh, "../meshes/cylinder_matrix_defect_icpgl.stl");

    // for (const auto& transform : transformations) {
    //     std::cout << "Transformation: " << affineToString(transform) << std::endl;
    // }

    return 0;
}