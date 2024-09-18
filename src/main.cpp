#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include "../include/CGemmWithC.h"
#include "CMatrixData.h"


int main(int argc , char ** argv){
	// M , N , K, aplha , A, B, beta, C
	
	CGemmWithC girl;
	CMatrixData matrixdata(5, 5);
	matrixdata.createData();
	matrixdata.show();
	
	std::cout << "sunwei over" << std::endl;
	return 0;
}
