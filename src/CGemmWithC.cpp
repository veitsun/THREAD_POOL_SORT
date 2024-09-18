#include "../include/CGemmWithC.h"
#include <iostream>

using namespace std;
void CGemmWithC::solveProblem(int M, int N, int K, float alpha, float **A, float **B, float beta, float **C){
        for ( int i = 0; i < M; i ++ ) {
                for (int j = 0; j < N; j++ ) {
			float sum = 0.0;
                        for (int k = 0; k < K; k ++) {
                                // 
				sum += (*A)[i * K + k] * (*B)[k * N + j];
                        }
			(*C)[i * N + j] = sum * alpha + beta * (*C)[i * N + j];
                }
        }
        // cout << "SolveProbelm 方法之后 的C矩阵 " << endl;
        // for (int i = 0; i < M * N; i++) {
        //         std::cout << (*C)[i] << " ";
        //         if((i + 1 ) % M == 0) {
        //         std::cout << std::endl;
        //         }
        // }
        std::cout << std::endl;
}

