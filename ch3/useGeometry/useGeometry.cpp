#include <iostream>
#include <cmath>
#include <corecrt_math_defines.h>

using namespace std;

#include <Eigen/Core>
#include <Eigen/Geometry>


using namespace Eigen;


int main(int argc, char** argv) {

	// Eigen/Geometry 模块提供了各种旋转和平移的表示
	// 3D旋转矩阵直接使用 Matrix3d 或 Matrix3f
	Matrix3d rotation_matrix = Matrix3d::Identity();

	// 旋转向量使用AngleAxis, 它底层部直接是Matrix，但运算可以当作矩阵（因为重载了运算符）
	AngleAxisd rotation_vector(M_PI / 4, Vector3d::UnitZ());  // 绕着 z 轴旋转45°
	
	cout.precision(3);
	cout << "rotation_matrix=\n" << rotation_vector.matrix() << endl; // 用.matrix()把旋转向量转换成旋转矩阵
	// 旋转矩阵也可以直接赋值
	rotation_matrix = rotation_vector.toRotationMatrix();

	// 用AngleAxis可以进行坐标变换
	Vector3d v(1, 0, 0);
	Vector3d v_rotated = rotation_vector * v;
	cout << "v(1, 0 ,0) after rotation (by angle axis) = " << v_rotated.transpose() << endl;
	// 或者用旋转矩阵
	v_rotated = rotation_matrix * v;
	cout << "v(1, 0 ,0) after rotation (by matrix) = " << v_rotated.transpose() << endl;

	// 欧拉角：可以将旋转矩阵直接转换成欧拉角
	Vector3d euler_angles = rotation_matrix.eulerAngles(2, 1, 0); // ZYX 顺序，即 yaw pitch roll 顺序
	cout << "yaw pitch roll = " << euler_angles.transpose() << endl;

	// 欧式变换矩阵使用 Eigen::Isometry
	Isometry3d T = Isometry3d::Identity();   // 虽然称为3D，实质上是4*4的矩阵
	T.rotate(rotation_vector);               // 按照 rotation_vector 进行旋转
	T.pretranslate(Vector3d(1, 3, 4));       // 把平移向量设成(1, 3, 4)
	cout << "Transform matrix = \n" << T.matrix() << endl;

	// 用变换矩阵进行坐标变换
	Vector3d v_transformed = T * v;  // 相当于 R*v+t
	cout << "v transformed = " << v_transformed.transpose() << endl;

	// 对于仿射变换和射影变换。使用 Eigen::Affine3d 和 Eigen::Projective3d 即可

	// 四元数
	// 可以直接把AngleAixs赋值给四元数，反之亦然
	Quaterniond q = Quaterniond(rotation_vector);
	cout << "quaternion from rotation vector = " << q.coeffs().transpose() << endl;// coffs 顺序 xyzw,w为实部

	// 也可以把旋转矩阵赋值给它
	q = Quaterniond(rotation_matrix);
	cout << "quaternion from rotation matrix = " << q.coeffs().transpose() << endl;// coffs 顺序 xyzw,w为实部

	// 使用四元数旋转阳光向量，使用重载乘法即可
	v_rotated = q * v; // 注意数学上是qvq^(-1)
	cout << "(1,0,0) after rotation = " << v_rotated.transpose() << endl;

	// 用常规向量乘法表示，则计算如下
	cout << "should be equal to " << (q * Quaterniond(0, 1, 0, 0) * q.inverse()).coeffs().transpose() << endl;
	return 0;
}