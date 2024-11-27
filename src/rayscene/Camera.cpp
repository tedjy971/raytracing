#include <iostream>
#include "Camera.hpp"

#ifdef USE_MULTITHREADING
#include <thread>
#endif

#include "../raymath/Ray.hpp"

struct RenderSegment {
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

Camera::Camera() : position(Vector3()) {
}

Camera::Camera(Vector3 pos) : position(pos) {
}

Camera::~Camera() {
}

Vector3 Camera::getPosition() {
  return position;
}

void Camera::setPosition(Vector3 &pos) {
  position = pos;
}

/**
 * Render a segment (set of rows) of the image
 */
void renderSegment(RenderSegment *segment) {
  // Précalcul des valeurs constantes
  const int width = segment->image->width;
  const double halfHeight = segment->height / 2.0;
  const Vector3 baseOrigin(0, 0, -1);

  // Précalcul des coordonnées X pour éviter les calculs répétés
  std::vector<double> xCoords(width);
  for (int x = 0; x < width; ++x) {
    xCoords[x] = -0.5 + (x * segment->intervalX);
  }

  // Objets réutilisables
  Vector3 direction;
  Ray ray(baseOrigin, direction);

  for (int y = segment->rowMin; y < segment->rowMax; ++y) {
    const double yCoord = halfHeight - (y * segment->intervalY);

    for (int x = 0; x < width; ++x) {
      // Mettre à jour la direction directement
      direction = Vector3(xCoords[x], yCoord, 0) - baseOrigin;
      ray.SetDirection(direction);

      Color pixel = segment->scene->raycast(ray, ray, 0, segment->reflections);
      segment->image->setPixel(x, y, pixel);
    }
  }
}

void Camera::render(Image &image, Scene &scene) {
  // Précalcul de toutes les valeurs constantes
  const double ratio = static_cast<double>(image.width) / image.height;
  const double height = 1.0 / ratio;
  const double intervalX = 1.0 / image.width;
  const double intervalY = height / image.height;

  scene.prepare();

#ifdef USE_MULTITHREADING
  // Utiliser la moitié des cores disponibles pour éviter la surcharge
  const unsigned int threadCount = std::max(1u, std::thread::hardware_concurrency() );
  const int rowsPerThread = image.height / threadCount;

  std::vector<RenderSegment> segments;
  std::vector<std::thread> threads;
  segments.reserve(threadCount);
  threads.reserve(threadCount);

  // Configuration commune pour tous les segments
  RenderSegment baseSeg;
  baseSeg.image = &image;
  baseSeg.height = height;
  baseSeg.intervalX = intervalX;
  baseSeg.intervalY = intervalY;
  baseSeg.reflections = Reflections;
  baseSeg.scene = &scene;
  baseSeg.origin = Vector3(0, 0, -1);

  for (unsigned int i = 0; i < threadCount; ++i) {
    RenderSegment seg = baseSeg;  // Copie de la configuration de base
    seg.rowMin = i * rowsPerThread;
    seg.rowMax = (i == threadCount-1) ? image.height : (i + 1) * rowsPerThread;

    segments.push_back(seg);
    threads.emplace_back(renderSegment, &segments.back());
  }

  for (auto& thread : threads) {
    thread.join();
  }
#else

std::cout << "1th" << std::endl;
  RenderSegment seg;
  seg.rowMin = 0;
  seg.rowMax = image.height;
  seg.image = &image;
  seg.height = height;
  seg.intervalX = intervalX;
  seg.intervalY = intervalY;
  seg.reflections = Reflections;
  seg.scene = &scene;
  seg.origin = Vector3(0, 0, -1);

  renderSegment(&seg);
#endif
}

std::ostream &operator<<(std::ostream &_stream, Camera &cam) {
  Vector3 pos = cam.getPosition();
  return _stream << "Camera(" << pos << ")";
}
