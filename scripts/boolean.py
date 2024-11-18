# read, icp, and boolean

import meshlib.mrmeshpy as mrmeshpy
from meshlib import mrmeshnumpy as mn
import numpy as np

def main():
    # Read
    mesh1 = mrmeshpy.loadMesh("../meshes/cylinder_matrix_ideal_fh_ar.stl")
    mesh2 = mrmeshpy.loadMesh("../meshes/cylinder_matrix_defect_fh_ar.stl")
    
    # ICP
    # Prepare ICP parameters
    diagonal = mesh1.getBoundingBox().diagonal()
    icp_sampling_voxel_size = diagonal * 0.01  # To sample points from object
    icp_params = mrmeshpy.ICPProperties()
    icp_params.distThresholdSq = (diagonal * 0.1) ** 2  # Use points pairs with maximum distance specified
    icp_params.exitVal = diagonal * 0.0001  # Stop when this distance reached

    icp_params.iterLimit = 100   # Maximum number of iterations to perform

    # Calculate transformation
    icp = mrmeshpy.ICP(mesh2, mesh1,
                    mrmeshpy.AffineXf3f(), mrmeshpy.AffineXf3f(),
                    icp_sampling_voxel_size)
    icp.setParams(icp_params)
    xf = icp.calculateTransformation()
    
    # Transform floating mesh
    mesh2.transform(xf)
    
    # Output information string
    print(icp.getLastICPInfo())
    
    mrmeshpy.saveMesh(mesh2, "../meshes/cylinder_matrix_defect_fh_ar_icp.stl")

    # Boolean operation
    result = mrmeshpy.boolean(mesh1, mesh2, mrmeshpy.BooleanOperation.DifferenceAB)
    result_mesh = result.mesh

    mrmeshpy.saveMesh(result_mesh, "../meshes/out_boolean_cylinder_matrix_fh_ar.stl")

if __name__ == '__main__':
    main()
