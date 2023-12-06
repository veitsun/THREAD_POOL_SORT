# thread-pool-sort
线程池排序

线程池参考：[thread-pool](https://github.com/mtrebi/thread-pool)

使用方法：
```
./gen_test.sh 10000             #在test文件夹生成一定规模的测试数据
g++ -pthread *.cpp -o sort      #编译源码
./sort test/ sorted.txt         #排序
```
输出：
```
1. file list write to filelist.txt.
2. delete tmp dir: /tmp/sort.
3. create tmp dir: /tmp/sort.
4. sort file chunk in /tmp/sort.
5. sort partial sorted file chunk.
6. copy result to sorted.txt.
```

