#pragma once
#include "SceneObject.hpp"
#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/Transform.hpp"

class Triangle : public SceneObject
{
private:
  Vector3 A;
  Vector3 B;
  Vector3 C;

  Vector3 tA;
  Vector3 tB;
  Vector3 tC;

public:
  Triangle(Vector3 a, Vector3 b, Vector3 c);
  ~Triangle();

  int ID;

  virtual void applyTransform() override;

  void computeBoundingBox() override
  {
    Vector3 min(
        std::min(std::min(tA.x, tB.x), tC.x),
        std::min(std::min(tA.y, tB.y), tC.y),
        std::min(std::min(tA.z, tB.z), tC.z));
    Vector3 max(
        std::max(std::max(tA.x, tB.x), tC.x),
        std::max(std::max(tA.y, tB.y), tC.y),
        std::max(std::max(tA.z, tB.z), tC.z));
    boundingBox = AABB(min, max);
  }

  virtual bool intersects(Ray &r, Intersection &intersection, CullingType culling) override;
};
