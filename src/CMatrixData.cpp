#include "../include/CMatrixData.h"
#include <cstdlib>


CMatrixData::CMatrixData(int m, int n){
    this->m = m;
    this->n = n;
    this->data = (float *)malloc(m * n * sizeof(float));
}

CMatrixData::~CMatrixData(){
    if(this->data != nullptr) {
        free(this->data);
    }
}

void CMatrixData::createData(){
    //this->matrix
}

