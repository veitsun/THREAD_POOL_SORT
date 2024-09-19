#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include "../include/CGemmWithC.h"
#include "CMatrixData.h"
#include <algorithm>


void convert_vector_to_pointer(const std::vector<float>& vec, float* ptr) {
    // 使用 std::copy 复制 vector 数据到指针
    std::copy(vec.begin(), vec.end(), ptr);
}

int main(int argc , char ** argv){

	/*
	// M , N , K, aplha , A, B, beta, C
	CGemmWithC girl;
	CMatrixData matrixdataA(5, 5);
	CMatrixData matrixdataB(5, 5);
	CMatrixData matrixdataC(5, 5);
	matrixdataA.createData();
	matrixdataB.createData();
	matrixdataC.createData();
	//matrixdata.show();
	float *A = new float[matrixdataA.getdata().size()];
	float *B = new float[matrixdataB.getdata().size()];
	float *C = new float[matrixdataC.getdata().size()];
	// 将 vector 数据复制到指针
    convert_vector_to_pointer(matrixdataA.getdata(), A);
	convert_vector_to_pointer(matrixdataB.getdata(), B);
	convert_vector_to_pointer(matrixdataC.getdata(), C);
	girl.solveProblem(5, 5, 5, 1.0, &A, &B, 1.0, &C);


	std::copy(C, C + 25, matrixdataC.getdata().begin());
	matrixdataA.show();
	matrixdataB.show();
	matrixdataC.show();

	//for(int i = 0; i < )
	
	std::cout << "sunwei over" << std::endl;
	delete[] A;
	delete[] B;
	delete[] C;
	*/

	
	return 0;
}
