import ctypes
import os
import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_blobs, make_moons, make_circles
from sklearn.cluster import DBSCAN

# 1. Load the compiled C library
lib_ext = '.dll' if os.name == 'nt' else '.so'
lib_path = os.path.join(os.path.dirname(__file__), f'dbscan{lib_ext}')
lib = ctypes.CDLL(lib_path)

# 2. Define the exact argument types for the C function
lib.run_dbscan.argtypes = [
    ctypes.c_int,                                                         # n
    np.ctypeslib.ndpointer(dtype=np.float64, ndim=1, flags='C_CONTIGUOUS'), # x_coords
    np.ctypeslib.ndpointer(dtype=np.float64, ndim=1, flags='C_CONTIGUOUS'), # y_coords
    ctypes.c_double,                                                      # epsilon
    ctypes.c_int,                                                         # min_points
    np.ctypeslib.ndpointer(dtype=np.int32, ndim=1, flags='C_CONTIGUOUS')    # labels
]

# 3. Create a clean Python wrapper class
class FastDBSCAN:
    def __init__(self, eps=0.5, min_samples=5):
        self.eps = eps
        self.min_samples = min_samples
        
    def fit_predict(self, X):
        n_points = X.shape[0]
        
        x_coords = np.ascontiguousarray(X[:, 0], dtype=np.float64)
        y_coords = np.ascontiguousarray(X[:, 1], dtype=np.float64)
        
        labels = np.zeros(n_points, dtype=np.int32)
        
        lib.run_dbscan(n_points, x_coords, y_coords, self.eps, self.min_samples, labels)
        
        return labels

# --- Testing and Visualization ---
if __name__ == "__main__":
    print("Generating data...")
    
    # --- DATASET SELECTION ---
    # Uncomment the ONE you want to test and its recommended parameters:

    # 1. Blobs (Standard clusters)
    # X, _ = make_blobs(n_samples=1000, centers=4, cluster_std=0.50, random_state=42)
    # my_eps, my_min_pts = 0.4, 5
    
    # 2. Moons (Interlocking curves)
    # X, _ = make_moons(n_samples=1000, noise=0.05, random_state=42)
    # my_eps, my_min_pts = 0.1, 5
    
    # 3. Circles (A ring inside a ring)
    X, _ = make_circles(n_samples=1000, factor=0.5, noise=0.05, random_state=42)
    my_eps, my_min_pts = 0.15, 5

    # -------------------------

    print("Running Custom C DBSCAN...")
    custom_model = FastDBSCAN(eps=my_eps, min_samples=my_min_pts)
    custom_labels = custom_model.fit_predict(X)

    print("Running Scikit-Learn DBSCAN...")
    sklearn_model = DBSCAN(eps=my_eps, min_samples=my_min_pts)
    sklearn_labels = sklearn_model.fit_predict(X)

    # Plot them side-by-side
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 7))
    
    ax1.scatter(X[:, 0], X[:, 1], c=custom_labels, cmap='viridis', s=25, edgecolor='k', linewidth=0.5)
    ax1.set_title(f"My Custom C DBSCAN (O(n²))")
    
    ax2.scatter(X[:, 0], X[:, 1], c=sklearn_labels, cmap='viridis', s=25, edgecolor='k', linewidth=0.5)
    ax2.set_title(f"Official Scikit-Learn DBSCAN")

    plt.tight_layout()
    plt.show()