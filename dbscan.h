#ifndef DBSCAN_H
#define DBSCAN_H

// n: the number of points, x_coords: array of all the x coordinates, y_coords: array of all the y coordinates, epsilon: the value of the radius, min_points: the min number of points to consider a point a core point, labels: an array of ints to store the cluster id of each point
void run_dbscan(int n, double* x_coords, double* y_coords, double epsilon, int min_points, int* labels);
#endif