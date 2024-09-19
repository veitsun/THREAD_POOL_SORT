#include "../include/CGemmWithCuda.h"
#include <iostream>
#include <cuda_runtime.h>

using namespace std;

class Vector {
private:
    float* data;
    int size;

public:
    // 构造函数（只在主机上调用）
    __host__ Vector(int size) : size(size) {
        cudaMallocManaged(&data, size * sizeof(float));
    }

    // 析构函数（只在主机上调用）
    __host__ ~Vector() {
        cudaFree(data);
    }

    // 访问元素（设备和主机都可调用）
    __host__ __device__ float& operator[](int index) {
        return data[index];
    }

    // 获取大小（设备和主机都可调用）
    __host__ __device__ int getSize() const {
        return size;
    }

    // 设备端向量加法核函数
    __device__ void add(Vector& other) {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx < size) {
            data[idx] += other[idx];
        }
    }

    // 启动内核的主机方法
    __host__ void launchAddKernel(Vector& other) {
        int threadsPerBlock = 256;
        int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;
        addKernel<<<blocksPerGrid, threadsPerBlock>>>(*this, other);
        cudaDeviceSynchronize();
    }

    // 内核函数
    static __global__ void addKernel(Vector a, Vector b) {
        a.add(b);
    }

};