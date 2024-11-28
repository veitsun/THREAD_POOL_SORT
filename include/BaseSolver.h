#ifndef BASESOLVER_H
#define BASESOLVER_H

#include <cstdio>
class BaseSolver {
public:
  int count_lines(char *filename, FILE *filelist);
  static int sort_worker(char *name, int start_line, long offset, int lines,
                         char *dst_name);
  int count_prefix(int prefix, const char *dir_path);
  static int merge_two_file(char *file1, char *file2, char *output);
};
#endif