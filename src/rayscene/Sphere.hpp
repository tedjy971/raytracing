#pragma once
#include "SceneObject.hpp"
#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"
#include "../raymath/Ray.hpp"

class Sphere : public SceneObject
{
private:
  Vector3 center;
  double radius;

public:
  Sphere(double r);
  ~Sphere();

  void computeBoundingBox() override {
    Vector3 center = transform.getPosition();
    Vector3 extent(radius, radius, radius);
    boundingBox = AABB(center - extent, center + extent);
  }

  virtual void applyTransform() override;
  virtual bool intersects(Ray &r, Intersection &intersection, CullingType culling) override;
  void countPrimes();
};
