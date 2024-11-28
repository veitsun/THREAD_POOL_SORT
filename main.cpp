#include "include/Solver.h"
#include "include/ThreadPool.h"
#include <algorithm>
#include <cstddef>
#include <dirent.h>
#include <fcntl.h>
#include <future>
#include <iostream>
#include <iterator>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("usage: sort <folder path> <destination file>\n");
    return -1;
  }

  char path[128];
  Solver solver;

  strcpy(path, argv[1]);
  if (path[strlen(path) - 1] == '/')
    path[strlen(path) - 1] = 0;

  // 得到 filelist 文件， 遍历目录中所有文件，得到目录列表
  if (solver.get_file_list(path, "./filelist.txt")) {
    perror("get file list error");
    return -1;
  }
  // 删除原来有的 sort_test 目录， 新建一个空的 sort_test 目录
  solver.delete_directory("./sort_test");
  mkdir("./sort_test", 0777);
  // 归并排序每单个文件
  solver.sort_list("./filelist.txt", "./sort_test");
  // 合并排序好的文件
  int result_prefix = solver.merge_orderd_files(0, "./sort_test");
  // 复制结果到最终文件中
  solver.copy_prefix_file("./sort_test", result_prefix, argv[2]);
  // solver.delete_directory("./sort_test");
  cout << "排序完成" << endl;
  cout << "排序结果在 sorted.txt 文件中" << endl;
  return 0;
}
