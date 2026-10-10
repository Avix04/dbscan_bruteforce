# Custom C-Based DBSCAN (O(n²))

It has a time complexity of O(n²) and is not recommended for large datasets.
It uses a flat nested-loop structure for radius searches.

## Prerequisites

To compile and run this project, you will need:

1. GCC (GNU Compiler Collection): To compile the C code.
2. Python 3.x: To run the visualization wrapper.
3. Python Packages: `numpy, matplotlib, scikit-learn`

You can install the required Python packages using:
`pip install numpy matplotlib scikit-learn`

## Compilation and Configuration

Before running the Python script, you must compile the C code into a dynamic shared library that Python's `ctypes` can interface with.

For Linux / macOS:
`gcc -shared -o dbscan.so -fPIC dbscan.c`

For Windows:
`gcc -shared -o dbscan.dll dbscan.c`

## Running the Algorithm

`python main.py`

Inside `main.py`, you can uncomment different datasets (Blobs, Moons, Circles) to see how the algorithm handles various geometric shapes compared to the official `scikit-learn` implementation.
