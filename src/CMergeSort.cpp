#include "CMergeSort.h"
#include <cstddef>
#include <cstring>

// Ordinary merge function for merge sort
void CMergeSort::merge(int *a, int beg, int med, int end) {
  int left = beg, right = med;
  size_t sz = end - beg;
  int temp[sz];
  int i = 0;
  while (left < med || right < end) {
    if ((right >= end) || (left < med && a[left] < a[right]))
      temp[i++] = a[left++];
    else
      temp[i++] = a[right++];
  }
  memcpy(a + beg, temp, sz * sizeof(int));
}

// Ordinary merge_sort function for merge sort
void CMergeSort::merge_sort(int *a, int beg, int end) {
  if (end - beg <= 1)
    return;
  int med = beg + (end - beg) / 2;
  merge_sort(a, beg, med);
  merge_sort(a, med, end);
  merge(a, beg, med, end);
}