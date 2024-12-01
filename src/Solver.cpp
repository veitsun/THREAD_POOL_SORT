#include "Solver.h"
#include "BaseSolver.h"
#include "ThreadPool.h"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <future>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

int Solver::get_file_list(char *path, const char *filelist) {

  DIR *dp;
  struct dirent *ep;
  // 表示目录条目的结构体， 通常包含一些字段，例如文件名和inode号
  char relative[128];
  char real[128];
  struct stat file_stat;
  FILE *file_filelist = fopen(filelist, "w");

  dp = opendir(path);
  if (dp != NULL) {
    while ((ep = readdir(dp))) { // readdir 用于从一个打开的
      // 目录流中读取下一个目录的条目
      if (ep->d_name[0] != '.') {
        strcpy(relative, path);
        strcat(relative, "/");
        strcat(relative, ep->d_name);
        realpath(relative, real); // 转换为绝对路径
        stat(real, &file_stat);
        if (S_ISREG(file_stat.st_mode))
          // count_lines(real, file_filelist);
          base.count_lines(real, file_filelist);
      }
    }
  } else {
    perror("couldnt open the directory");
    return -1;
  }
  closedir(dp);
  fclose(file_filelist);

  return 0;
}

void Solver::delete_directory(const char *path) {
  DIR *d = opendir(path);
  size_t path_len = strlen(path);
  int result = 0;

  if (d) {
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) {
      if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
        char *file_path = (char *)malloc(path_len + strlen(dir->d_name) + 2);
        if (file_path) {
          snprintf(file_path, path_len + strlen(dir->d_name) + 2, "%s/%s", path,
                   dir->d_name);
          if (dir->d_type == DT_DIR) {
            // 迭代删除子目录
            delete_directory(file_path);
          } else {
            // 删除文件
            result = remove(file_path);
            if (result != 0) {
              printf("Error deleting file : %s\n", file_path);
            }
          }
          free(file_path);
        }
      }
    }
    closedir(d);
  }
  // 删除目录
  rmdir(path);
}

void Solver::sort_list(const char *list_name, const char *dst_dir) {
  //  solver.sort_list("./filelist.txt", "./sort_test");
  ThreadPool pool(4);
  // 打开文件列表
  FILE *list = fopen(list_name, "r");
  char line_buffer[1024];
  char temp_file_name[128];
  std::vector<std::future<int>> results;

  pool.init();
  // 逐行读取文件列表，每一行包括
  // 原始文件名
  // 起始行号
  // 文件偏移量
  while (fgets(line_buffer, 1024, list)) {
    // strrchr 返回的是line_buffer字符串中最后一个出现的指定字符
    // atol 是将字符串转换为长整型的数值
    // offset 地址偏移量
    long offset = atol(strrchr(line_buffer, '\t') + 1);
    *strrchr(line_buffer, '\t') = 0;
    // 取出行数 start_line
    // atoi 是将字符串转换为int型的数值
    int start_line = atoi(strchr(line_buffer, '\t') + 1);
    *strchr(line_buffer, '\t') = 0;
    char name[128];
    strcpy(name, line_buffer);
    // 构造一个临时文件名，该文件名是由目标目录，
    // 一个前导0，文件名（去掉路径），和一个整数（行号）组成
    // 输出到数组 temp_file_name 中
    sprintf(temp_file_name, "%s/0%s%d", dst_dir, strrchr(name, '/') + 1,
            start_line);
    // 提交排序任务到线程池， 使用sort_worker作为工作线程
    // 参数有， name，开始行号，地址偏移量，100分组，临时文件名
    // strdup
    results.emplace_back(pool.submit(BaseSolver::sort_worker, strdup(name),
                                     start_line, offset, 100,
                                     strdup(temp_file_name)));
  }
  fclose(list);
  for (auto &&result : results) {
    // 遍历 results 容器中的每个 std::future 对象， 并调用 get()
    // 方法获取每个异步操作的结果，这通常用于在所有异步操作完成后处理其结果
    // 测试结果
    result.get();
  }
  pool.shutdown();
}

// 实现多文件的归并排序
int Solver::merge_orderd_files(int prefix, const char *dir_path) {
  BaseSolver bs;
  if (bs.count_prefix(prefix, dir_path) <= 1) {
    return prefix;
  }

  DIR *dp;
  struct dirent *ep;
  std::vector<std::future<int>> results;
  char file1[1024];
  char file2[1024];
  char output_file[1024];

  char buffer[1024];
  int i = 1;
  ThreadPool pool(4);
  pool.init();

  dp = opendir(dir_path);
  sprintf(buffer, "%d", prefix);
  size_t sz = strlen(buffer);

  if (dp != NULL) {
    while ((ep = readdir(dp))) {
      if (!strncmp(buffer, ep->d_name, sz)) {
        if (i % 2) {
          // 说明是第一个文件
          sprintf(file1, "%s/%s", dir_path, ep->d_name);
        } else {
          // 说明是第二个文件
          sprintf(file2, "%s/%s", dir_path, ep->d_name);
          sprintf(output_file, "%s/%d%s", dir_path, prefix + 1,
                  strrchr(file1, '/') + sz + 1);

          // 归并第一个文件和第二个文件，将工作函数放线程池中
          results.emplace_back(pool.submit(BaseSolver::merge_two_file,
                                           strdup(file1), strdup(file2),
                                           strdup(output_file)));
        }
        i++;
      }
    }
    if (i % 2 == 0) {
      sprintf(output_file, "%s/%d%s", dir_path, prefix + 1,
              strrchr(file1, '/') + sz + 1);
      results.emplace_back(pool.submit(BaseSolver::merge_two_file,
                                       strdup(file1), (char *)NULL,
                                       strdup(output_file)));
    }
    for (auto &&result : results) {
      result.get();
      // 等待所有的线程执行完毕
    }
    closedir(dp);
  } else {
    perror("couldnt open the directory");
    return -1;
  }

  pool.shutdown();

  return merge_orderd_files(prefix + 1, dir_path);
}

void Solver::copy_prefix_file(const char *path, int prefix,
                              const char *dst_name) {
  // path :./sort_test
  // prefix: result_prefix
  // dst_name: sorted.txt
  DIR *dp;
  struct dirent *ep;
  char buffer[1024];
  char path_buffer[1024];
  sprintf(buffer, "%d", prefix);
  size_t sz = strlen(buffer);

  FILE *dst_file;
  FILE *src_file;

  if (!path || prefix < 0 || !dst_name) {
    return;
  }
  dp = opendir(path);
  if (!dp) {
    perror("couldnt open the directory");
    return;
  }

  dst_file = fopen(dst_name, "w");
  if (!dst_file) {
    closedir(dp);
    perror("couldnt open the file");
    return;
  }

  while ((ep = readdir(dp))) {
    if (!strncmp(buffer, ep->d_name, sz)) {
      sprintf(path_buffer, "%s/", path);
      strcat(path_buffer, ep->d_name);
      src_file = fopen(path_buffer, "r");
      if (src_file) {
        while (fgets(buffer, 1024, src_file) != NULL) {
          fprintf(dst_file, "%s", buffer);
        }
        fclose(src_file);
      }
      break;
    }
  }

  fclose(dst_file);

  closedir(dp);
}