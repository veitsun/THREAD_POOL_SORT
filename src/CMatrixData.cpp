#include "../include/CMatrixData.h"
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>



CMatrixData::CMatrixData(int m, int n){
    this->m = m;
    this->n = n;
    //this->data = (float *)malloc(m * n * sizeof(float));
}

CMatrixData::~CMatrixData(){
    // if(this->data != nullptr) {
    //     free(this->data);
    // }
}

void CMatrixData::createData(){
    int num = m * n; // 一维数组维数
    read_file("random_numbers.txt", this->data);

}

void CMatrixData::read_file(const std::string& filename, std::vector<float>& data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    //std::vector<float> data;
    while (std::getline(file, line)) {
        // std::cout << line << std::endl;
        float temp = string_to_float(line);
        data.emplace_back(temp);
    }
    file.close();
}

float CMatrixData::string_to_float(const std::string& str) {
    try {
        return std::stof(str);
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Invalid argument: " << ia.what() << std::endl;
    } catch (const std::out_of_range& oor) {
        std::cerr << "Out of range: " << oor.what() << std::endl;
    }
    return 0.0f; // 返回默认值
}

void CMatrixData::show() {
    for (int i = 0; i < m * n; i++) {
        std::cout << this->data[i] << " ";
        if((i + 1 ) % m == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

std::vector<float>& CMatrixData::getdata() {
    return this->data;
}

