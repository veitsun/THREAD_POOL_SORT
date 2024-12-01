#include "BaseSolver.h"
#include "CMergeSort.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>

int BaseSolver::count_lines(char *filename, FILE *filelist) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("unable to open file");
    return -1;
  }

  int lines = 0;
  char buffer[1024];
  long offset = ftell(file); // 用于获取当前文件指针在文件中的偏移量
  // 也就是当前位置
  while (fgets(buffer, 1024, file) != NULL) {
    if (!(lines % 100)) {
      fprintf(filelist, "%s\t%d\t%ld\n", filename, lines, offset);
      // 写入filelist.txt文件中
    }
    lines++;
    offset = ftell(file);
    // 获取当前位置的偏移量
  }
  fclose(file);
  return lines;

  // return 0;
}

int BaseSolver::sort_worker(char *name, int start_line, long offset, int lines,
                            char *dst_name) {
  // 参数有， name，开始行号，地址偏移量，100分组，临时文件名
  // name ： /home/sunwei/thread-pool-sort/test/num
  int data_buffer[lines];
  char line_buffer[1024];
  FILE *data_file = fopen(name, "r");
  int i;
  if (data_file == NULL) {
    perror("couldnt open file");
    return -1;
  }

  // fseek 是用于移动文件指针的函数调用
  fseek(data_file, offset, SEEK_SET);
  for (i = 0; i < lines; i++) {
    if (!fgets(line_buffer, 1024, data_file)) {
      break;
    }
    data_buffer[i] = atoi(line_buffer); // 将字符串转换为整型
  }

  fclose(data_file);

  //
  CMergeSort sortserver;
  sortserver.merge_sort(data_buffer, 0, i);

  FILE *dst_file = fopen(dst_name, "w");
  if (dst_file == NULL) {
    perror("couldnt open file");
    return -1;
  }

  // 将排序好的数据写回到目标临时文件中
  for (int j = 0; j < i; j++) {
    fprintf(dst_file, "%d\n", data_buffer[j]);
  }
  fclose(dst_file);
  free(name);
  free(dst_name);

  return 0;
}

int BaseSolver::count_prefix(int prefix, const char *dir_path) {
  DIR *dp;
  struct dirent *ep;
  int count = 0;
  char buffer[1024];
  sprintf(buffer, "%d", prefix); // 将整数prefix 放到字符串buffer中
  size_t sz = strlen(buffer);

  dp = opendir(dir_path);
  if (dp != NULL) {
    while ((ep = readdir(dp))) {
      if (!strncmp(buffer, ep->d_name, sz)) {
        count++;
      }
    }
    closedir(dp);
  } else {
    perror("couldnt open the directory");
  }

  return count;
}

int BaseSolver::merge_two_file(char *file1, char *file2, char *output) {
  char line_buffer1[1024];
  char line_buffer2[1024];
  FILE *f1 = fopen(file1, "r");
  FILE *f2 = fopen(file2, "r");
  FILE *fo = fopen(output, "w");

  if (!fo || (!f1 && !f2)) {
    perror("couldn't open file");
    if (fo)
      fclose(fo);
    if (f1)
      fclose(f1);
    if (f2)
      fclose(f2);
    return -1;
  }

  if (!f2) {
    while (fgets(line_buffer1, 1024, f1))
      fprintf(fo, "%s", line_buffer1);
    fclose(f1);
    fclose(fo);
    return 0;
  }

  if (!f1) {
    while (fgets(line_buffer1, 1024, f2))
      fprintf(fo, "%s", line_buffer1);
    fclose(f2);
    fclose(fo);
    return 0;
  }

  char *s1 = fgets(line_buffer1, 1024, f1);
  char *s2 = fgets(line_buffer2, 1024, f2);

  while (s1 || s2) { // Ordinary merge operation
    if ((!s2) || (s1 && atoi(line_buffer1) <= atoi(line_buffer2))) {
      fprintf(fo, "%s", line_buffer1);
      s1 = fgets(line_buffer1, 1024, f1);
    } else {
      fprintf(fo, "%s", line_buffer2);
      s2 = fgets(line_buffer2, 1024, f2);
    }
  }

  fclose(f1);
  fclose(f2);
  fclose(fo);
  free(file1);
  free(file2);
  free(output);

  return 0;
}