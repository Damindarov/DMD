
import open3d as o3d
import numpy as np

def main():
    pcd = o3d.io.read_point_cloud("../pcds/cylinder_matrix_defect_fh_ar.pcd")

    # example code for aligning the pcds PC with the z-axis
    # Compute the principal axis using PCA
    points = np.asarray(pcd.points)
    center = points.mean(axis=0)
    covariance_matrix = np.cov(points.T)
    eigenvalues, eigenvectors = np.linalg.eigh(covariance_matrix)

    # Sort eigenvectors based on eigenvalues
    idx = eigenvalues.argsort()[::-1]
    eigenvectors = eigenvectors[:, idx]

    # Align the principal component to the Z axis (3rd column of the identity matrix)
    rotation_matrix = eigenvectors.T  # Transpose to align the largest component with the Z-axis
    pcd.rotate(rotation_matrix, center=center)

    # remove the top half of the cloud
    min_bound = pcd.get_min_bound()
    max_bound = pcd.get_max_bound()
    z_mid = ((min_bound[2] + max_bound[2]) / 2) * 1.225
    points = np.asarray(pcd.points)
    remaining_points = points[points[:, 2] <= z_mid]

    remaining_pcd = o3d.geometry.PointCloud()
    remaining_pcd.points = o3d.utility.Vector3dVector(remaining_points)

    # save the remaining half of the cloud
    o3d.io.write_point_cloud("../pcds/remaining_half2.pcd", remaining_pcd)


if __name__ == "__main__":
    main()