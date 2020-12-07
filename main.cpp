#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <Eigen/Core>
#include <Eigen/Geometry>

constexpr unsigned int CIRCLE_FACES = 60;
constexpr float ANGLE_SECTION = 1;
constexpr unsigned int NUMBER_SECTIONS = 360 * 10 / ANGLE_SECTION;
constexpr float Y_INCREMENT = 10.0 * ANGLE_SECTION / 360;

template <unsigned int Edges>
class Circle
{
public:
  Circle(float radius)
  {
    for (unsigned int i(0); i < Edges; ++i)
    {
      _points(0, i) = cos(2 * M_PI * i / Edges) * radius; // X
      _points(1, i) = sin(2 * M_PI * i / Edges) * radius; // Y
      _points(2, i) = 0;                                  // Z
      _points(3, i) = 1;                                  // W
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

  unsigned int index(0);
  for (unsigned int i(0); i <= NUMBER_SECTIONS; ++i)
  {
    Circle<CIRCLE_FACES> c(.5);
    c.translate(50, i * Y_INCREMENT, 0);
    c.rotate(2 * M_PI / 360 * ANGLE_SECTION * i, 0, 1, 0);
    c.toOBJ(obj, index);
    index += CIRCLE_FACES;
  }

  for (unsigned int i(0); i <= NUMBER_SECTIONS; ++i)
  {
    Circle<CIRCLE_FACES> c(.5);
    c.translate(55, i * Y_INCREMENT + 2, 0);
    c.rotate(2 * M_PI / 360 * ANGLE_SECTION * i, 0, 1, 0);
    c.toOBJ(obj, i ? index : 0);
    index += CIRCLE_FACES;
  }

  return 0;
}
