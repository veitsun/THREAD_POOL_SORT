#include <cuda_runtime.h>
// #include <stdio.h>

__global__ void gemm_kernel(int M, int N, int K, float alpha, float *A,
                            float *B, float beta, float *C) {
  int row = blockIdx.y * blockDim.y + threadIdx.y;
  int col = blockIdx.x * blockDim.x + threadIdx.x;

  if (row < M && col < N) {
    float temp = 0.0;
    for (int k = 0; k < K; k++) {
      temp += A[row * K + k] * B[k * N + col];
    }
    C[row * N + col] = alpha * temp + beta * C[row * N + col];
  }
}

void gemm_cuda(int M, int N, int K, float alpha, float *A, float *B, float beta,
               float *C) {
  float *d_A, *d_B, *d_C;

  // 分配 GPU 内存
  cudaMalloc((void **)&d_A, M * K * sizeof(float));
  cudaMalloc((void **)&d_B, K * N * sizeof(float));
  cudaMalloc((void **)&d_C, M * N * sizeof(float));

  // 复制矩阵到 GPU
  cudaMemcpy(d_A, A, M * K * sizeof(float), cudaMemcpyHostToDevice);
  cudaMemcpy(d_B, B, K * N * sizeof(float), cudaMemcpyHostToDevice);
  cudaMemcpy(d_C, C, M * N * sizeof(float), cudaMemcpyHostToDevice);

  // 定义线程块和网格
  dim3 blockSize(16, 16);
  dim3 gridSize((N + blockSize.x - 1) / blockSize.x,
                (M + blockSize.y - 1) / blockSize.y);

  // 调用 CUDA kernel
  gemm_kernel<<<gridSize, blockSize>>>(M, N, K, alpha, d_A, d_B, beta, d_C);

  // 复制结果回 CPU
  cudaMemcpy(C, d_C, M * N * sizeof(float), cudaMemcpyDeviceToHost);

  // 释放 GPU 内存
  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);
}
