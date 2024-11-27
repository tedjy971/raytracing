#include <iostream>
#include <cmath>
#include "Sphere.hpp"
#include "../raymath/Vector3.hpp"

Sphere::Sphere(double r) : SceneObject(), radius(r)
{
}

Sphere::~Sphere()
{
}

void Sphere::applyTransform()
{
  Vector3 c;
  this->center = this->transform.apply(c);
  computeBoundingBox(); // Recalculer après transformation
}

void Sphere::countPrimes() {
 int prime_counter = 0;
 for(int n = 2 ; n<1000 ; n++)
  {
    int count = 0;
    for (int i = 2; i <= i/2; i++)
    {
      if(n%i == 0) {
        count++;
      }
      if(count == 0)
      {
        prime_counter++;
      }
    }
  }
}

bool Sphere::intersects(Ray &r, Intersection &intersection, CullingType culling) {

  Vector3 L = center - r.GetPosition();
  double tca = L.dot(r.GetDirection());

  if (tca < 0) return false;

  double d2 = L.dot(L) - tca * tca;
  double radiusSquared = radius * radius;

  if (d2 > radiusSquared) return false;

  double thc = std::sqrt(radiusSquared - d2);
  double t = tca - thc;

  // Calcul du point d'intersection
  Vector3 P = r.GetPosition() + (r.GetDirection() * t);

  intersection.Position = P;
  intersection.Mat = this->material;
  intersection.Normal = (P - center).normalize();


  //junk function
  // countPrimes();
  return true;
}