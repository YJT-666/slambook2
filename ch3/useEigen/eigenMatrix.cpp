#include <iostream>

using namespace std;
#include <ctime>

// Eigen 核心部分
#include <Eigen/Core>

// 稠密矩阵的代数运算（逆、特征值等）
#include <Eigen/Dense>

using namespace Eigen;

#define MATRIX_SIZE 50

/* 本程序演示了Eigen基本类型的使用 */
int main(int argc, char **argv){
    
    // 在 Eigen 中所有的向量和矩阵都是 Eigen::Matrix , 它是一个模板类。它的前三个参数为数据类型、行、列
    
    // 声明一个 2*3 的 float 矩阵
    Matrix<float, 2, 3> matrix_23;
    
    // 同时， Eigen 通过 typedef 提供了许多内置类型，不过底层仍是Eigen::Matrix
    // 例如， Vector3d 实质上是 Eigen::Matrix<double, 3, 1> 即三维向量
    Vector3d v_3d;
    // 这是一样的
    Matrix<float, 3, 1> vd_3d;
    
    // Matrix3d 实质上是 Eigen::Matrix<double, 3, 3>
    Matrix3d matrix_33  = Matrix3d::Zero(); // 初始化为0
    
    // 如果不确定矩阵的大小，可以使用动态大小的矩阵
    Matrix<double, Dynamic, Dynamic> matrix_dynamic;
    
    // 更简单的
    MatrixXd matrix_x;
    // 这种种类还有很多，这里不一一列举了
    
    // 下面是对 Eigen 矩阵的操作
    // 数据输入（初始化）
    matrix_23 << 1, 2, 3, 4, 5, 6;
    // 输出
    cout << "matrix 2x3 from 1 to 6: \n" << matrix_23 << endl;
    
    // 用 （） 访问矩阵中的元素
    cout << "print matrix 2x3: " << endl;
    for(int i = 0; i < 2; i++){
        for (int j = 0; j < 3; j++)
            cout << matrix_23(i,j) << "\t";
        cout << endl;
    }
    
    // 矩阵和向量相乘（实际上是矩阵和矩阵）
    v_3d << 3, 2, 1;
    vd_3d << 4, 5, 6;
    
    // 但是在 Eigen 里你不能混合两种不同类型的矩阵，像这样是错误的
    // Matrix<double, 2, 1> result_wrong_type = matrix_23 * v_3d;
    // 应该显示转换
    Matrix<double, 2, 1> result = matrix_23.cast<double>() * v_3d;
    cout << "[1,2,3;4,5,6]*[3,2,1]=" << result.transpose() << endl;
    
    
    // 同样，计算时不能搞错矩阵的维度
    
    // 一些矩阵运算
    // 四则运算就不演示了，直接+-*/ 即可
    matrix_33 = Matrix3d::Random();  // 随机数矩阵
    cout << "random matrix: \n" << matrix_33 << endl;
    cout << "transpose:\n" << matrix_33.transpose() << endl; // 转置
    cout << "sum: " << matrix_33.sum() << endl; // 各元素的和
    cout << "trace: " << matrix_33.trace() << endl; // 迹
    cout << "time 10: \n" << 10 * matrix_33 << endl; // 数乘
    cout << "inverse: \n" << matrix_33.inverse() << endl; // 逆
    cout << "det: " << matrix_33.determinant() << endl; // 行列式
    
    // 特征值
    // 实对称矩阵可以保证对角化成功
    SelfAdjointEigenSolver<Matrix3d> eigen_solver(matrix_33.transpose()*matrix_33);
    cout << "Eigen values = \n" << eigen_solver.eigenvalues() << endl;
    cout << "Eigen vectors \n" << eigen_solver.eigenvectors() << endl;
    
    // 解方程
    // 我们求解 matrix_NN * x = v_Nd 方程
    // N 的大小在前边宏里定义，它由随机数生成
    // 直接求逆自然是最直接的，但是运算量大
    
    Matrix<double, MATRIX_SIZE, MATRIX_SIZE> matrix_NN = 
        MatrixXd::Random(MATRIX_SIZE, MATRIX_SIZE);
    matrix_NN = matrix_NN * matrix_NN.transpose(); // 保证半正定
    Matrix<double, MATRIX_SIZE, 1> v_Nd = MatrixXd::Random(MATRIX_SIZE, 1);
    
    clock_t time_stt = clock(
    
    ); // 计时
    // 直接求逆
    Matrix<double, MATRIX_SIZE, 1> x = matrix_NN.inverse() * v_Nd;
    cout << "time of normal inverse is "
        << 1000 *(clock() - time_stt) / (double) CLOCKS_PER_SEC << "ms" << endl;
    cout << "x = " << x.transpose() << endl;
    
    // 通常用矩阵分解来求解，例如QR分解，速度会快很多
    time_stt = clock();
    x = matrix_NN.colPivHouseholderQr().solve(v_Nd);
    cout << "time of Qr decomposition is "
        << 1000 *(clock() - time_stt) / (double) CLOCKS_PER_SEC << "ms" << endl;
    cout << "x = " << x.transpose() << endl;
    
    // 对于正定矩阵，还可以用cholesky分解来解方程
    time_stt = clock();
    x = matrix_NN.ldlt().solve(v_Nd);
        cout << "time of ldlt decomposition is "
        << 1000 *(clock() - time_stt) / (double) CLOCKS_PER_SEC << "ms" << endl;
    cout << "x = " << x.transpose() << endl;
    
    return 0;
}
