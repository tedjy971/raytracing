#pragma once
#include "SceneObject.hpp"
#include "../raymath/Vector3.hpp"
#include "../raymath/Ray.hpp"

class Plane : public SceneObject {
private:
  Vector3 point;
  Vector3 normal;

public:
  Plane(Vector3 p, Vector3 n);

  ~Plane() override;

  bool intersects(Ray &r, Intersection &intersection, CullingType culling) override;

  bool intersectsBoundingBox(Ray &r) const override {
    return true;
  }
};
