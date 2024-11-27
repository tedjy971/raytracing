#include <iostream>
#include <cmath>
#include "Vector3.hpp"

Vector3::Vector3() : x(0), y(0), z(0)
{
}

Vector3::Vector3(double iX, double iY, double iZ) : x(iX), y(iY), z(iZ)
{
}

const Vector3 Vector3::operator+(Vector3 const &vec) const
{
  Vector3 c;
  c.x = x + vec.x;
  c.y = y + vec.y;
  c.z = z + vec.z;
  c.invalidateCache();
  return c;
}

const Vector3 Vector3::operator-(Vector3 const &vec) const
{
  Vector3 c;
  c.x = x - vec.x;
  c.y = y - vec.y;
  c.z = z - vec.z;
  c.invalidateCache();

  return c;
}

const Vector3 Vector3::operator*(double const &f) const
{
  Vector3 c;
  c.x = x * f;
  c.y = y * f;
  c.z = z * f;
  c.invalidateCache();

  return c;
}

const Vector3 Vector3::operator/(double const &f) const
{
  Vector3 c;
  c.x = x / f;
  c.y = y / f;
  c.z = z / f;
  c.invalidateCache();

  return c;
}

Vector3& Vector3::operator=(Vector3 const &vec) {
  if (this != &vec) {  // Éviter l'auto-assignation
    x = vec.x;
    y = vec.y;
    z = vec.z;
    invalidateCache();
  }
  return *this;
}

double Vector3::length() const
{
  return std::sqrt(this->lengthSquared());
}

Vector3 Vector3::normalize() const {
  // Retourner la version cachée si elle existe
  if (cachedNormalized != nullptr) {
    return *cachedNormalized;
  }

  double lenSq = lengthSquared();
  if (lenSq < COMPARE_ERROR_CONSTANT) {
    cachedNormalized = new Vector3();
  } else {
    double invLength = 1.0 / std::sqrt(lenSq);
    cachedNormalized = new Vector3(x * invLength, y * invLength, z * invLength);
  }

  return *cachedNormalized;
}

double Vector3::dot(Vector3 const &vec) const
{
  return (x * vec.x + y * vec.y + z * vec.z);
}

const Vector3 Vector3::projectOn(Vector3 const &vec) const
{
  return vec * this->dot(vec);
}

const Vector3 Vector3::reflect(Vector3 const &normal) const
{
  Vector3 proj = this->projectOn(normal) * -2;
  Vector3 reflectDir = proj + *this;
  return reflectDir;
}

const Vector3 Vector3::cross(Vector3 const &b) const
{
  Vector3 c(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
  return c;
}

const Vector3 Vector3::inverse() const
{
  Vector3 c(1.0 / x, 1.0 / y, 1.0 / z);
  return c;
}

std::ostream &operator<<(std::ostream &_stream, Vector3 const &vec)
{
  return _stream << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
}
