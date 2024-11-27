#include <iostream>
#include "AABB.hpp"

AABB::AABB() : Min(Vector3()), Max(Vector3()) {}

AABB::AABB(Vector3 min, Vector3 max) : Min(min), Max(max) {}

AABB::~AABB() {}

AABB &AABB::operator=(AABB const &box)
{
    Min = box.Min;
    Max = box.Max;
    return *this;
}

void AABB::subsume(AABB const &other)
{
    Min.x = std::min(Min.x, other.Min.x);
    Min.y = std::min(Min.y, other.Min.y);
    Min.z = std::min(Min.z, other.Min.z);

    Max.x = std::max(Max.x, other.Max.x);
    Max.y = std::max(Max.y, other.Max.y);
    Max.z = std::max(Max.z, other.Max.z);
}

bool AABB::intersects(Ray &r) const
{
    Vector3 dirfrac;
    // Calculer l'inverse de la direction pour éviter les divisions
    dirfrac.x = 1.0f / r.GetDirection().x;
    dirfrac.y = 1.0f / r.GetDirection().y;
    dirfrac.z = 1.0f / r.GetDirection().z;

    // Calculer les intersections avec les faces de la boîte
    float t1 = (Min.x - r.GetPosition().x) * dirfrac.x;
    float t2 = (Max.x - r.GetPosition().x) * dirfrac.x;
    float t3 = (Min.y - r.GetPosition().y) * dirfrac.y;
    float t4 = (Max.y - r.GetPosition().y) * dirfrac.y;
    float t5 = (Min.z - r.GetPosition().z) * dirfrac.z;
    float t6 = (Max.z - r.GetPosition().z) * dirfrac.z;

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    // Si tmax < 0, le rayon intersecte la boîte mais derrière l'origine
    // Si tmin > tmax, le rayon ne intersecte pas la boîte
    return !(tmax < 0 || tmin > tmax);
}

std::ostream &operator<<(std::ostream &_stream, AABB const &box)
{
    return _stream << "Min(" << box.Min << ")-Max(" << box.Max << ")";
}