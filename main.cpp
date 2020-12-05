#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include <Eigen/Core>
#include <Eigen/Geometry>

template <unsigned int Edges>
class Circle
{
public:
  Circle(float radius)
  {
    for (unsigned int i(0); i < Edges; ++i)
    {
      _points(0, i) = cos(2 * M_PI * i / Edges); // X
      _points(1, i) = sin(2 * M_PI * i / Edges); // Y
      _points(2, i) = 0;                         // Z
      _points(3, i) = 1;                         // W
    }
    std::cout << _points << std::endl;
  }

  void translate(float x, float y, float z)
  {
    Eigen::Transform<float, 3, Eigen::Affine> transform(Eigen::Translation<float, 3>(x, y, z));
    Eigen::Matrix4f matrix = transform.matrix();
    _points = matrix * _points;
  }

  void toOBJ(std::string const &path)
  {
    std::ofstream f(path);
    for (unsigned int i(0); i < Edges; ++i)
      f << "v " << _points(0, i) << " " << _points(1, i) << " " << _points(2, i) << '\n';
  }

private:
  Eigen::Matrix<float, 4, Edges> _points;
};

class MarbleMachine
{
public:
  MarbleMachine() = default;

private:
};

int main(int ac, char **av)
{
  std::cout << "Eigen version: " << EIGEN_MAJOR_VERSION << "." << EIGEN_MINOR_VERSION << std::endl;

  Circle<360> c1(1);
  c1.translate(5, 0, 0);
  c1.toOBJ("translate_circle.obj");

  return 0;
}
