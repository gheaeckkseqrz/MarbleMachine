#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
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
  }

  void translate(float x, float y, float z)
  {
    Eigen::Transform<float, 3, Eigen::Affine> transform(Eigen::Translation<float, 3>(x, y, z));
    Eigen::Matrix4f matrix = transform.matrix();
    _points = matrix * _points;
  }

  void rotate(float angle, float x, float y, float z)
  {
    Eigen::Transform<float, 3, Eigen::Affine> transform(Eigen::AngleAxis<float>(angle, Eigen::Vector3f(x, y, z)));
    Eigen::Matrix4f matrix = transform.matrix();
    _points = matrix * _points;
  }

  void toOBJ(std::ostream &ss, unsigned int index)
  {
    for (unsigned int i(0); i < Edges; ++i)
      ss << "v " << _points(0, i) << " " << _points(1, i) << " " << _points(2, i) << '\n';
    if (index)
    {
      for (unsigned int i(0); i < Edges; ++i)
      {
        ss << "f " << index - Edges + i + 1 << " " << index - Edges + ((i + 1) % Edges) + 1 << " " << index + ((i + 1) % Edges) + 1 << '\n';
        ss << "f " << index - Edges + i + 1 << " " << index + i + 1 << " " << index + ((i + 1) % Edges) + 1 << '\n';
      }
    }
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

  std::ofstream obj("marble.obj");

  Circle<4> c1(1);
  c1.translate(5, 0, 0);
  c1.toOBJ(obj, 0);

  Circle<4> c2(1);
  c2.translate(5, 0, 0);
  c2.rotate(M_PI / 8, 0, 1, 0);
  c2.toOBJ(obj, 4);

  return 0;
}
