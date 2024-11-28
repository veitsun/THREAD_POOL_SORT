#ifndef SOLVER_H
#define SOLVER_H
#include "BaseSolver.h"
class Solver {
private:
  BaseSolver base;

public:
  int get_file_list(char *path, const char *filelist);
  void delete_directory(const char *path);
  void sort_list(const char *list_name, const char *dst_dir);
  int merge_orderd_files(int prefix, const char *dir_path);
  void copy_prefix_file(const char *path, int prefix, const char *dst_name);
};

#endif