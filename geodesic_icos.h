#ifndef _GEODESIC_ICOS_H_
#define _GEODESIC_ICOS_H_

#include <memory>

struct PolygonCounts
{
  uint32_t vertices, faces, edges;
};

struct GeodesicPolygon
{
  uint32_t vertices_stride;
  uint32_t faces_stride;
  uint32_t subdivisions;

  std::unique_ptr<PolygonCounts []> ns;
  std::unique_ptr<float []> vertices;
  std::unique_ptr<std::unique_ptr<uint32_t []> []> faces;

  GeodesicPolygon(
      uint32_t vertices_stride,
      uint32_t faces_stride,
      uint32_t subdivisions);

  void GeodesicIcosahedron();
};


#endif  // _GEODESIC_ICOS_H_
