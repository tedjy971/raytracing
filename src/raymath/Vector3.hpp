#pragma once

#include <iostream>
// #include "../Eigen/Dense"

#define COMPARE_ERROR_CONSTANT 0.000001

class Vector3 {
private:
  mutable double cachedLength = -1;
  mutable Vector3 *cachedNormalized = nullptr; // Cache pour le vecteur normalisé

public:
  double x = 0;
  double y = 0;
  double z = 0;

  Vector3();

  Vector3(double x, double y, double z);

  ~Vector3() {
    delete cachedNormalized;
  }

  const Vector3 operator+(Vector3 const &vec) const;

  const Vector3 operator-(Vector3 const &vec) const;

  const Vector3 operator*(double const &f) const;

  const Vector3 operator/(double const &f) const;

  Vector3 &operator=(Vector3 const &vec);

  double length() const;

  inline double lengthSquared() const {
    return x * x + y * y + z * z;
  }

  Vector3 normalize() const;

  double dot(Vector3 const &vec) const;

  const Vector3 projectOn(Vector3 const &vec) const;

  const Vector3 reflect(Vector3 const &normal) const;

  const Vector3 cross(Vector3 const &vec) const;

  const Vector3 inverse() const;

  // Mise à jour de invalidateCache
  inline void invalidateCache() const {
    cachedLength = -1;
    delete cachedNormalized;
    cachedNormalized = nullptr;
  }


  friend std::ostream &operator<<(std::ostream &_stream, Vector3 const &vec);
};
