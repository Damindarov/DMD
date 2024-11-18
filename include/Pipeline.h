/**
 * @file Pipeline.h
 * @author DMD team, IU
 * @brief header file for Pipeline class
 * @version 0.1
 * @date 2024-11-09
 * @dependencies: MeshLib - An open-source 3D geometry library for processing, editing,
 *                and manipulating 3D meshes. https://github.com/MeshInspector/MeshLib
 */

#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <optional>
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

/**
 * Pipeline class for our meshes processing pipeline.
 */

namespace DMD
{
    class Pipeline
    {
    public:
        Pipeline(const std::filesystem::path ideal_path, const std::filesystem::path defect_path);
        Pipeline();
        ~Pipeline();

        int run();

    private:
        std::filesystem::path ideal_mesh_path;
        std::filesystem::path defect_mesh_path;

        std::optional<MR::Mesh> loadMesh(const std::filesystem::path &path);
        void fillAndRebuildMesh(MR::Mesh &mesh);
        void fillHoles(MR::Mesh &mesh);
        MR::Expected<MR::Mesh> reBuild(MR::Mesh &mesh);
        void performLocalICP(MR::Mesh &ideal_mesh, MR::Mesh &defect_mesh);
        void performBooleanOperation(MR::Mesh &ideal_mesh, MR::Mesh &defect_mesh);
        void saveMesh(const MR::Mesh &result, const std::string &path);
        static bool onProgress(float v);
    };

} // namespace DMD