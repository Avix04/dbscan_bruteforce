#ifndef DBSCAN_H
#define DBSCAN_H

#include <stdbool.h>

typedef struct {
  double x;
  double y;
  int cluster_id;
  bool visited;
} Point;

void run_dbscan(int n, double* x_coords, double* y_coords, double epsilon, int min_points, int* labels);
#endif