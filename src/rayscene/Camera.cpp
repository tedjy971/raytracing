#include <iostream>
#include <cmath>
#include "Camera.hpp"

#include <thread>

#include "../raymath/Ray.hpp"

struct RenderSegment
{
public:
  int rowMin;
  int rowMax;
  Image *image;
  double height;
  double intervalX;
  double intervalY;
  int reflections;
  Scene *scene;
  Vector3 origin;
};

Camera::Camera() : position(Vector3())
{
}

Camera::Camera(Vector3 pos) : position(pos)
{
}

Camera::~Camera()
{
}

Vector3 Camera::getPosition()
{
  return position;
}

void Camera::setPosition(Vector3 &pos)
{
  position = pos;
}

/**
 * Render a segment (set of rows) of the image
 */
void renderSegment(RenderSegment *segment)
{

  for (int y = segment->rowMin; y < segment->rowMax; ++y)
  {
    double yCoord = (segment->height / 2.0) - (y * segment->intervalY);

    for (int x = 0; x < segment->image->width; ++x)
    {
      double xCoord = -0.5 + (x * segment->intervalX);

      Vector3 coord(xCoord, yCoord, 0);
      Vector3 origin(0, 0, -1);
      Ray ray(origin, coord - origin);

      Color pixel = segment->scene->raycast(ray, ray, 0, segment->reflections);
      segment->image->setPixel(x, y, pixel);
    }
  }
}

void Camera::render(Image &image, Scene &scene) {
  const double ratio = static_cast<double>(image.width) / image.height;
  const double height = 1.0 / ratio;
  const double intervalX = 1.0 / image.width;
  const double intervalY = height / image.height;

  scene.prepare();

  // Calculer le nombre optimal de threads
  const unsigned int threadCount = std::thread::hardware_concurrency();
  const int rowsPerThread = image.height / threadCount;

  std::vector<RenderSegment> segments;
  std::vector<std::thread> threads;

  // Réserver l'espace pour éviter les réallocations
  segments.reserve(threadCount);
  threads.reserve(threadCount);

  // Création des segments et threads
  for (unsigned int i = 0; i < threadCount; ++i) {
    RenderSegment seg;
    // Calcul des lignes pour ce thread

    seg.rowMin = i * rowsPerThread;
    seg.rowMax = (i == threadCount-1) ? image.height
                                         : (i + 1) * rowsPerThread;
    seg.image = &image;
    seg.height = height;
    seg.intervalX = intervalX;
    seg.intervalY = intervalY;
    seg.reflections = Reflections;
    seg.scene = &scene;
    seg.origin = Vector3(0, 0, -1);

    segments.push_back(seg);
    threads.emplace_back(renderSegment, &segments.back());
  }

  // Attendre tous les threads
  for (auto& thread : threads) {
    thread.join();
  }
}

std::ostream &operator<<(std::ostream &_stream, Camera &cam)
{
  Vector3 pos = cam.getPosition();
  return _stream << "Camera(" << pos << ")";
}
