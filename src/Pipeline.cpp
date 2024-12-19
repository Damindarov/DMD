/**
 * @file Pipeline.cpp
 * @author DMD team, IU
 * @brief Implementation of Pipeline class
 * @version 0.1
 * @date 2024-11-09
 * @dependencies: MeshLib - An open-source 3D geometry library for processing, editing,
 *                and manipulating 3D meshes. https://github.com/MeshInspector/MeshLib
 */

#include "Pipeline.h"

/**
 * @brief Constructor for Pipeline class
 *
 * @param ideal_path the path to true mesh
 * @param defect_path the path to defective/deformed mesh
 */

namespace DMD
{

    Pipeline::Pipeline(const std::filesystem::path ideal_path, const std::filesystem::path defect_path)
        : ideal_mesh_path(ideal_path), defect_mesh_path(defect_path) {}

    Pipeline::Pipeline()
    {
        std::cout << "No paths provided for ideal and defect meshes. Using default paths"
                  << std::endl;

        ideal_mesh_path = "../meshes/cylinder_matrix_ideal.stl";
        defect_mesh_path = "../meshes/cylinder_matrix_defect.stl";
    }

    /**
     * @brief Destroy the Pipeline:: Pipeline object
     */
    Pipeline::~Pipeline() = default;

    /**
     * @brief Executes the mesh processing pipeline.
     *
     * This function loads the ideal and defective meshes, fills holes in both meshes,
     * rebuilds them, performs local Iterative Closest Point (ICP) alignment, saves
     * the repaired and transformed meshes, performs boolean and saves the output
     */
    int Pipeline::run()
    {
        // read the ideal mesh and the defect mesh
        auto ideal_mesh = loadMesh(ideal_mesh_path);
        auto defect_mesh = loadMesh(defect_mesh_path);

        // apply our pipeline to these meshes
        if (ideal_mesh && defect_mesh)
        {
            fillAndRebuildMesh(*ideal_mesh);
            fillAndRebuildMesh(*defect_mesh);
            performLocalICP(*ideal_mesh, *defect_mesh);

            // save rebuild and transformed meshes
            std::cout << "Saving the repaired meshes..." << std::endl;
            MR::MeshSave::toAnySupportedFormat(*ideal_mesh, "../meshes/fillHoles_reBuild_ideal_mesh.stl");
            MR::MeshSave::toAnySupportedFormat(*defect_mesh, "../meshes/fillHoles_reBuild_defect_icp_mesh.stl");

            performBooleanOperation(*ideal_mesh, *defect_mesh);

            

            return 0;
        }
        else
        {
            return -1;
        }
    }

    /**
     * @brief Loads a mesh from a given file path.
     *
     * @param path The file path to the STL mesh file.
     * @return std::optional<MR::Mesh> An optional containing the loaded mesh if successful, or empty if failed.
     */
    std::optional<MR::Mesh> Pipeline::loadMesh(const std::filesystem::path &path)
    {
        auto mesh = MR::MeshLoad::fromAnyStl(path);
        if (!mesh)
        {
            std::cerr << "Error loading mesh from " << path << std::endl;
            return std::nullopt;
        }
        return *mesh;
    }

    /**
     * @brief Fills holes in the given mesh and rebuilds it.
     *
     * @param mesh Reference to the mesh to be filled and rebuilt.
     */
    void Pipeline::fillAndRebuildMesh(MR::Mesh &mesh)
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

    /**
     * @brief Fills holes in the given mesh.
     *
     * @param mesh Reference to the mesh in which holes will be filled.
     */
    void Pipeline::fillHoles(MR::Mesh &mesh)
    {
        auto holeEdges = mesh.topology.findHoleRepresentiveEdges();
        for (MR::EdgeId e : holeEdges)
        {
            MR::FillHoleParams params;
            params.metric = MR::getUniversalMetric(mesh);
            MR::fillHole(mesh, e, params);
        }
    }

    /**
     * @brief Rebuilds the given mesh.
     *
     * @param mesh Reference to the mesh to be rebuilt.
     * @return MR::Expected<MR::Mesh> The rebuilt mesh or an error if the process fails.
     */
    MR::Expected<MR::Mesh> Pipeline::reBuild(MR::Mesh &mesh)
    {
        // rebuildMesh params setting
        MR::RebuildMeshSettings settings;
        settings.decimate = false;
        settings.voxelSize = 0.278;     // in mm
        settings.progress = onProgress; // callback for progress

        MR::MeshPart meshPart(mesh);
        return MR::rebuildMesh(meshPart, settings);
    }

    /**
     * @brief Performs local ICP alignment between the ideal and defect meshes.
     *
     * @param ideal_mesh Reference to the ideal mesh.
     * @param defect_mesh Reference to the defect mesh to be aligned.
     */
    void Pipeline::performLocalICP(MR::Mesh &ideal_mesh, MR::Mesh &defect_mesh)
    {
        // following ICP is adapted from examples/mesh_ICP.cpp
        std::cout << "\nPerforming local ICP..." << std::endl;

        // Prepare ICP parameters
        float diagonal = ideal_mesh.getBoundingBox().diagonal();
        MR::ICPProperties icpParams;
        icpParams.distThresholdSq = MR::sqr(diagonal * 0.1f); // Use points pairs with maximum distance specified
        icpParams.exitVal = diagonal * 0.003f;                // Stop when distance reached

        MR::ICP icp(MR::MeshOrPoints{MR::MeshPart{defect_mesh}},
                    MR::MeshOrPoints{MR::MeshPart{ideal_mesh}},
                    MR::AffineXf3f(), MR::AffineXf3f(),
                    diagonal * 0.01f); // To sample points from object
        icp.setParams(icpParams);
        MR::AffineXf3f xf = icp.calculateTransformation();
        defect_mesh.transform(xf);
    }

    /**
     * @brief Performs a boolean operation (Difference) on the ideal and defect meshes.
     *
     * @param ideal_mesh Reference to the ideal mesh.
     * @param defect_mesh Reference to the defect mesh.
     */
    void Pipeline::performBooleanOperation(MR::Mesh &ideal_mesh, MR::Mesh &defect_mesh)
    {
        std::cout << "Performing boolean operation (DifferenceAB)..." << std::endl;
        MR::BooleanResult result = MR::boolean(ideal_mesh, defect_mesh, MR::BooleanOperation::DifferenceAB);
        if (!result.valid())
        {
            std::cerr << result.errorString << std::endl;
            return;
        }
        // save result to STL file
        MR::Mesh resultMesh = *result;
        saveMesh(resultMesh, "../meshes/out_boolean.stl");
    }

    /**
     * @brief Performs filtering out all unnecessary geometry outside of the working area.
     *
     * @param mesh The mesh to be filtered.
     * @param chop_area The working area. Default area is a cube, suitable for "cylinder matrix" default stl.
     */
    MR::Mesh Pipeline::chop(MR::Mesh mesh, MR::Mesh chop_area) {

        MR::Mesh result = MR::boolean(chop_area, mesh, MR::BooleanOperation::Intersection).mesh;

        return result;
    }

    MR::Mesh Pipeline::chop(MR::Mesh mesh) {
        MR::Vector3f size = {45.0f, 45.0f, 45.0f};
        MR::Vector3f angles = {55.5f/180*MR::PI, -18.0f/180*MR::PI, -19.6f/180*MR::PI};
        MR::Vector3f position = {-7.0f, 30.0f, -65.0f};

        MR::Mesh chop_area = MR::makeCube(size);

        auto M = MR::AffineXf3f::linear(MR::Matrix3f::rotationFromEuler(angles));
        chop_area.transform(M);

        M = MR::AffineXf3f::translation(position);
        chop_area.transform(M);

        MR::MeshSave::toAnySupportedFormat(chop_area, "chop_area.stl" );

        return chop(mesh, chop_area);
    }

    /**
     * @brief Saves the resulting mesh to a specified file path.
     *
     * @param result The mesh to be saved.
     * @param path The file path where the mesh will be saved.
     */
    void Pipeline::saveMesh(const MR::Mesh &result, const std::string &path)
    {
        MR::MeshSave::toAnySupportedFormat(result, path);
        std::cout << "Saved the result mesh to " << path << std::endl;
    }

    /**
     * @brief Callback function to report progress during operations.
     *
     * @param v A float value representing the progress (0.0 to 1.0).
     * @return true if the operation should continue, false otherwise.
     */
    bool Pipeline::onProgress(float v)
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

} // namespace DMD