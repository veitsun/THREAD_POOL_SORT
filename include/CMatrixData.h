#ifndef CMatrixData_H
#define CMatrixData_H
#include <string>
#include <vector>

// struct s_matrix{    // 矩阵结构体类型
//     int m;
//     int n;
//     float *data;    // 用一维数组存储矩阵的信息
// };


class CMatrixData{
private:
    //s_matrix *matrix;   // 矩阵信息，数据类型是上面定义矩阵结构体类型
    int m;
    int n;
    //float *data;
    std::vector<float> data;
    
public:
    CMatrixData(int m, int n);
    ~CMatrixData();
    void createData();  // 生成矩阵的信息
    void read_file(const std::string& filename, std::vector<float>& data); // 读取文件数据
    float string_to_float(const std::string& str);
    void show();
};

#endif