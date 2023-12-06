#include "ThreadPool.h"
#include <algorithm>
#include <future>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

#define FILE_LIST_NAME "/tmp/filelist.txt"
#define TMP_FILE_DIR "/tmp/sort"
#define LINE_GROUPS 100
#define THREAD_NUM 4
#define LINE_BUF_SIZE 1024
#define PATH_BUF_SIZE 128

using namespace std;

void delete_directory(const char *path) {
  DIR *d = opendir(path);
  size_t path_len = strlen(path);
  int result = 0;

  if (d) {
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) {
      if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
        char *file_path = (char*)malloc(path_len + strlen(dir->d_name) + 2);
        if (file_path) {
          snprintf(file_path, path_len + strlen(dir->d_name) + 2, "%s/%s", path, dir->d_name);
          if (dir->d_type == DT_DIR) {
            delete_directory(file_path);
          } else {
            result = remove(file_path);
            if (result != 0) {
              printf("Error deleting file: %s\n", file_path);
            }
          }
          free(file_path);
        }
      }
    }
    closedir(d);
  }
  rmdir(path); 
}

void merge(int * a, int beg, int med, int end){
  int left = beg, right = med;
  size_t sz = end - beg;
  int temp[sz];
  int i = 0;
  while(left < med || right < end){
    if(right >= end || left < med && a[left] < a[right])
      temp[i ++] = a[left ++];
    else
      temp[i ++] = a[right ++];
  }
  memcpy(a + beg, temp, sz * sizeof(int));
}

void merge_sort(int * a, int beg, int end){
  if(end - beg <= 1)
    return;
  int med = beg + (end - beg) / 2;
  merge_sort(a, beg, med);
  merge_sort(a, med, end);
  merge(a, beg, med, end);
}

void copy_prefix_file(const char *path, int prefix, const char *dst_name) {
  DIR *dp;
  struct dirent *ep;
  char buffer[LINE_BUF_SIZE];
  char path_buffer[PATH_BUF_SIZE];
  sprintf(buffer, "%d", prefix);
  size_t sz = strlen(buffer);
  FILE *dst_file, *src_file;

  if(!path || prefix < 0 || !dst_name)
    return;

  dp = opendir(path);
  if(!dp) {
    perror("couldn't open the directory");
    return;
  }

  dst_file = fopen(dst_name, "w");
  if(!dst_file) {
    closedir(dp);
    perror("couldn't open the file");
    return;
  }

  while((ep = readdir(dp)))
    if(!strncmp(buffer, ep->d_name, sz)) {
      sprintf(path_buffer, "%s/", path);
      strcat(path_buffer, ep->d_name);
      src_file = fopen(path_buffer, "r");
      if(src_file) {
        while (fgets(buffer, LINE_BUF_SIZE, src_file) != NULL)
          fprintf(dst_file, "%s", buffer);
        fclose(src_file);
      }
      break;
    }

  closedir(dp);
  fclose(dst_file);
}

int count_lines(char *filename, FILE *filelist)
{
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("unable to open file");
    return -1;
  }

  int lines = 0;
  char buffer[LINE_BUF_SIZE];
  long offset = ftell(file);

  while (fgets(buffer, LINE_BUF_SIZE, file) != NULL) {
    if(!(lines % LINE_GROUPS)) {
      fprintf(filelist, "%s\t%d\t%ld\n", filename, lines, offset);
    }
    lines++;
    offset = ftell(file);
  }
  fclose(file);
  return lines;
}

int count_prefix(int prefix, const char *dir_path)
{
  DIR *dp;
  struct dirent *ep;
  int count = 0;
  char buffer[LINE_BUF_SIZE];
  sprintf(buffer, "%d", prefix);
  size_t sz = strlen(buffer);

  dp = opendir(dir_path);
  if(dp != NULL) {
    while((ep = readdir(dp)))
      if(!strncmp(buffer, ep->d_name, sz))
        ++count;
    closedir(dp);
  } else {
    perror("couldn't open the directory");
  }
  return count;
}

int merge_two_file(char *file1, char *file2, char *output) {
  char line_buffer1[LINE_BUF_SIZE];
  char line_buffer2[LINE_BUF_SIZE];
  FILE *f1 = fopen(file1, "r");
  FILE *f2 = fopen(file2, "r");
  FILE *fo = fopen(output, "w");

  if(!fo || (!f1 && !f2)) {
    perror("couldn't open file");
    if(fo) fclose(fo);
    if(f1) fclose(f1);
    if(f2) fclose(f2);
    return -1;
  }

  if(!f2) {
    while(fgets(line_buffer1, LINE_BUF_SIZE, f1))
      fprintf(fo, "%s", line_buffer1) ;
    fclose(f1); fclose(fo);
    return 0;
  } 

  if(!f1) {
    while(fgets(line_buffer1, LINE_BUF_SIZE, f2))
      fprintf(fo, "%s", line_buffer1);
    fclose(f2); fclose(fo);
    return 0;
  }

  char *s1 = fgets(line_buffer1, LINE_BUF_SIZE, f1);
  char *s2 = fgets(line_buffer2, LINE_BUF_SIZE, f2);

  while(s1 || s2) {
    if(!s2 || s1 && atoi(line_buffer1) <= atoi(line_buffer2)) {
      fprintf(fo, "%s", line_buffer1);
      s1 = fgets(line_buffer1, LINE_BUF_SIZE, f1);
    } else {
      fprintf(fo, "%s", line_buffer2);
      s2 = fgets(line_buffer2, LINE_BUF_SIZE, f2);
    }
  }

  fclose(f1); fclose(f2); fclose(fo);
  free(file1); free(file2); free(output);

  return 0;
}

char merge_orderd_files(int prefix, const char *dir_path)
{
  if(count_prefix(prefix, dir_path) <= 1) 
    return prefix;

  DIR *dp;
  struct dirent *ep;
  vector<future<int>> results;
  char file1[LINE_BUF_SIZE], file2[LINE_BUF_SIZE], output_file[LINE_BUF_SIZE];
  char buffer[LINE_BUF_SIZE];
  int i = 1;
  ThreadPool pool(THREAD_NUM);

  pool.init();
  dp = opendir(dir_path);
  sprintf(buffer, "%d", prefix);
  size_t sz = strlen(buffer);

  if(dp != NULL) {
    while((ep = readdir(dp))) {
      if(!strncmp(buffer, ep->d_name, sz)) {
        if(i % 2) {
          sprintf(file1, "%s/%s", dir_path, ep->d_name);
        } else {
          sprintf(file2, "%s/%s", dir_path, ep->d_name);
          sprintf(output_file, "%s/%d%s", dir_path, prefix + 1, strrchr(file1, '/') + sz + 1);
          //cout << file1 << ":" << file2 << ":" << output_file << endl;
          results.emplace_back(pool.submit(merge_two_file, strdup(file1), strdup(file2), strdup(output_file)));
        }
        ++ i;
      } 
    }
    if(i % 2 == 0) {
      sprintf(output_file, "%s/%d%s", dir_path, prefix + 1, strrchr(file1, '/') + sz + 1);
      //cout << file1 << "::" << output_file << endl;
      results.emplace_back(pool.submit(merge_two_file, strdup(file1), (char*)NULL, strdup(output_file)));
    }
    for(auto && result : results)
      result.get();
    closedir(dp);
  } else {
    perror("couldn't open the directory");
    return -1;
  }

  pool.shutdown();
  return merge_orderd_files(prefix + 1, dir_path);
}

int get_file_list(char *path, const char *filelist) {
  DIR *dp;
  struct dirent *ep;
  char relative[PATH_BUF_SIZE];
  char real[PATH_BUF_SIZE];
  struct stat file_stat;
  FILE *fl = fopen(filelist, "w");

  dp = opendir(path);
  if (dp != NULL) {
    while ((ep = readdir (dp)))
      if(ep->d_name[0] != '.') {
        strcpy(relative, path);
        strcat(relative, "/");
        strcat(relative, ep->d_name);
        realpath(relative, real);
        stat(real, &file_stat);
        if(S_ISREG(file_stat.st_mode))
          count_lines(real, fl);
      }
    closedir(dp);
  } else {
    perror("couldn't open the directory");
    return -1;
  }

  fclose(fl);

  return 0;
}

int sort_worker(char *name, int start_line, long offset, int lines, char *dst_name) {
  //cout << name << " " << start_line << " " << offset << " " << lines << " " << dst_name << endl;
  int data_buffer[lines];
  char line_buffer[LINE_BUF_SIZE];
  FILE *data_file = fopen(name, "r");
  int i;

  if(data_file == NULL) {
    perror("couldn't open file");
    return -1;
  }

  fseek(data_file, offset, SEEK_SET);
  for(i = 0; i < lines; ++i) {
    if(!fgets(line_buffer, LINE_BUF_SIZE, data_file))
      break;
    data_buffer[i] = atoi(line_buffer);
  }

  fclose(data_file);
  merge_sort(data_buffer, 0, i);
  //sort(data_buffer, data_buffer + lines);

  FILE *dst_file = fopen(dst_name, "w");
  if(dst_file == NULL) {
    perror("couldn't open file");
    return -1;
  }

  for(int j = 0; j < i; ++j) {
    fprintf(dst_file, "%d\n", data_buffer[j]);
  } 

  fclose(dst_file);
  free(name); free(dst_name);

  return 0;
}

void sort_list(const char *list_name, const char *dst_dir) {
  ThreadPool pool(THREAD_NUM);
  FILE *list = fopen(list_name, "r");
  char line_buffer[LINE_BUF_SIZE];
  char tmp_file_name[PATH_BUF_SIZE];
  vector<future<int>> results;
  fpos_t op;

  pool.init();

  while(fgets(line_buffer, LINE_BUF_SIZE, list)) {
    long offset = atol(strrchr(line_buffer, '\t') + 1);
    *strrchr(line_buffer, '\t') = 0;
    int start_line = atoi(strchr(line_buffer, '\t') + 1);
    *strchr(line_buffer, '\t') = 0;
    char name[PATH_BUF_SIZE];
    strcpy(name, line_buffer);
    sprintf(tmp_file_name, "%s/0%s%d", dst_dir, strrchr(name, '/') + 1, start_line);
    //cout << name << " " << start_line << " " << offset << " " << LINE_GROUPS << " " << tmp_file_name << endl;
    results.emplace_back(pool.submit(sort_worker, strdup(name), start_line, offset, LINE_GROUPS, strdup(tmp_file_name)));
  }

  fclose(list);
  for(auto && result : results)
    result.get();

  pool.shutdown();
}

int main(int argc, char *argv[])
{
  if(argc != 3) {
    printf("usage: sort <folder path> <destination file>\n");
    return -1;
  }

  char path[PATH_BUF_SIZE];

  strcpy(path, argv[1]);
  if(path[strlen(path) - 1] == '/')
    path[strlen(path) - 1] = 0;

  cout << "1. file list write to " << FILE_LIST_NAME << "." << endl;
  if(get_file_list(path, FILE_LIST_NAME)) {
    perror("get file list error");
    return -1;
  }
  cout << "2. delete tmp dir: " << TMP_FILE_DIR << "." << endl;
  delete_directory(TMP_FILE_DIR);
  cout << "3. create tmp dir: " << TMP_FILE_DIR << "." << endl;
  mkdir(TMP_FILE_DIR, 0777);
  cout << "4. sort file chunk in " << TMP_FILE_DIR << "." << endl;
  sort_list(FILE_LIST_NAME, TMP_FILE_DIR);
  cout << "5. sort partial sorted file chunk." << endl;
  int  result_prefix = merge_orderd_files(0, TMP_FILE_DIR);
  cout << "6. copy result to " << argv[2] << "." << endl;
  copy_prefix_file(TMP_FILE_DIR, result_prefix, argv[2]);
  return 0;
}
