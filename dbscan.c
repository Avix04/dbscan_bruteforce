#include <stdio.h>
#include <stdlib.h>

// Don't use pow for performance optimization
// Get the squared distance of two points to then be compared with epsilon squared
static double get_squared_distance(double x1, double y1, double x2, double y2) {
  double dx = x1 - x2;
  double dy = y1 - y2;
  
  return (dx * dx) + (dy * dy);
}

// Checks the distance between two points and in case populates the neighbor array, returns the count of indexes in the array
static int check_points_in_radius(int target_index, int n, double* x_coords, double* y_coords, double epsilon, int* neighbor_indices) {
  double target_point_x = x_coords[target_index];
  double target_point_y = y_coords[target_index];

  double epsilon_squared = epsilon * epsilon;

  int count = 0;

  for (int i = 0; i < n; i++) {
    double point_x = x_coords[i];
    double point_y = y_coords[i];
    double squared_distance = get_squared_distance(target_point_x, target_point_y, point_x, point_y);

    if (squared_distance <= epsilon_squared) {
      neighbor_indices[count] = i;
      count++;
    }
  }

  return count;
}

// Runs the algorithm
void run_dbscan(int n, double* x_coords, double* y_coords, double epsilon, int min_points, int* labels) {
  int* visited = (int*)calloc(n, sizeof(int));
  int* inqueue = (int*)calloc(n, sizeof(int));

  int* queue = (int*)malloc(n * sizeof(int));
  int* neighbor_indices = (int*)malloc(n * sizeof(int));
    
  int current_cluster_id = 0;

  // Loop through all the points
  for (int i = 0; i < n; i++) {
    // Check if a point has been visited
    if (visited[i]) {
      continue;
    }

    visited[i] = 1;

    int point_count = check_points_in_radius(i, n, x_coords, y_coords, epsilon, neighbor_indices);

    // If it has not been visited we check if it is a core point
    if (point_count < min_points) {
      labels[i] = -1;
    } else {
      current_cluster_id++;
      labels[i] = current_cluster_id;

      int read_index = 0;
      int write_index = 0;

      // We add all the neighbor points to the queue
      for (int j = 0; j < point_count; j++) {
        if (inqueue[neighbor_indices[j]]) {
          continue;
        }
        queue[j] = neighbor_indices[j];
        write_index++;
        inqueue[neighbor_indices[j]] = 1;
      }

      // We loop through the queue until its empty and perform the same check for each point in it
      while (read_index < write_index) {
        int current_point = queue[read_index];
        read_index++;

        // A point that is in the queue is part of its cluster if it is not already been claimed from another one
        if (labels[current_point] == -1 || labels[current_point] == 0) {
          labels[current_point] = current_cluster_id;
        }
      
        // If the point hasn't been visited we check if it is a core point
        if (visited[current_point] == 0) {
          visited[current_point] = 1;
          int new_point_count = check_points_in_radius(current_point, n, x_coords, y_coords, epsilon, neighbor_indices);
          if (new_point_count >= min_points) {
            // If it is a core point we push its neighbour points at the end of the queue
            for (int k = 0; k < new_point_count; k++) {
              if (inqueue[neighbor_indices[k]]) {
                continue;
              }
              queue[write_index] = neighbor_indices[k];
              write_index++;
              inqueue[neighbor_indices[k]] = 1;
            }
          }
        }
      }
    }
  }

  free(visited);
  free(queue);
  free(neighbor_indices);
}