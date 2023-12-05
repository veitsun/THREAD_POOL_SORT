# thread-pool-sort
线程池海量数据排序

线程池参考：[thread-pool](https://github.com/mtrebi/thread-pool)

使用方法：
```
./gen_test.sh 10000             #在test文件夹生成一定规模的测试数据
g++ -pthread *.cpp -o sort      #编译源码
./sort test/                    #排序
```
输出：
```
1. file list write to filelist.txt.
2. delete tmp dir: /tmp/sort.
3. create tmp dir: /tmp/sort.
4. sort file chunk in /tmp/sort.
5. sort partial sorted file chunk.
9      #最终结果存在/tmp/sort文件夹下前缀为这一行的数字的文件里，此处为9意为总计归并了9次
6. copy result to sorted.txt.   #TODO
```

