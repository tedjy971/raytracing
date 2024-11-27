#pragma once
#include <vector>
#include "SceneObject.hpp"
#include "../raymath/Transform.hpp"
#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"
#include "../raymath/Ray.hpp"
#include "./Triangle.hpp"

class Mesh : public SceneObject
{
private:
  std::vector<Triangle *> triangles;

public:
  Mesh();
  ~Mesh();

  void loadFromObj(std::string path);

  virtual void applyTransform() override;
  virtual bool intersects(Ray &r, Intersection &intersection, CullingType culling) override;
  void computeBoundingBox() override
  {
    if (triangles.empty())
      return;

    boundingBox = triangles[0]->getBoundingBox();
    for (size_t i = 1; i < triangles.size(); ++i)
    {
      boundingBox.subsume(triangles[i]->getBoundingBox());
    }
  }
};
