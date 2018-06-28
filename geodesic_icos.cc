#include "geodesic_icos.h"
#include "icosahedron.h"
#include "vecutil.h"

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <math.h>

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <utility>

void
SubdividePolygonCounts(
    PolygonCounts const *in,
    PolygonCounts *out)
{
  out->vertices = in->vertices + in->edges;
  out->faces = 4 * in->faces;
  out->edges = 2 * in->edges + 3 * in->faces;
}

uint32_t
SubdividePolygon (
    uint32_t vertices_stride, uint32_t faces_stride,
    float *vertices, uint32_t n_vertices,
    uint32_t const *in_faces,  uint32_t n_faces,
    uint32_t *out_faces)
{
  array1d<uint32_t const> ifaces (in_faces, faces_stride);
  array1d<uint32_t> ofaces (out_faces, faces_stride);
  array1d<float> vxs (vertices, vertices_stride);

  uint32_t nvxs = n_vertices;
  std::map<std::pair<uint32_t, uint32_t>, uint32_t> edge_vxs;

  std::function<uint32_t(uint32_t, uint32_t)> vx(
      [&edge_vxs, &vxs, &nvxs] (uint32_t u, uint32_t v) {
    auto uv = (u < v) ? std::make_pair(u, v) : std::make_pair(v, u);
    if (edge_vxs.find(uv) == edge_vxs.end()) {
      vxs(nvxs).copy(glm::normalize(
          0.5f * (glm::vec3(vxs(u)) + glm::vec3(vxs(v)))));

      edge_vxs[uv] = nvxs++;
    }
    return edge_vxs[uv];
  });

  for (uint32_t nf = 0; nf < n_faces; ++nf) {
    uint32_t v0 = ifaces(nf).x, v1 = ifaces(nf).y, v2 = ifaces(nf).z;
    uint32_t v01 = vx(v0, v1), v12 = vx(v1, v2), v02 = vx(v0, v2);

    ofaces(4*nf + 0).tri(v0, v01, v02); 
    ofaces(4*nf + 1).tri(v01, v1, v12); 
    ofaces(4*nf + 2).tri(v02, v12, v2); 
    ofaces(4*nf + 3).tri(v02, v01, v12); 
  }
  std::cerr << " NVXS=" << nvxs <<
      ",  EDGES=" << edge_vxs.size() <<
      ", FACES= " << 4 * n_faces << std::endl;
  return nvxs;
}

GeodesicPolygon::
GeodesicPolygon(
    uint32_t vertices_stride,
    uint32_t faces_stride,
    uint32_t subdivisions)
  : vertices_stride (vertices_stride),
    faces_stride (faces_stride),
    subdivisions (subdivisions) {}

void
GeodesicPolygon::
GeodesicIcosahedron()
{
  ns.reset (new PolygonCounts [subdivisions + 1]);
  IcosahedronCounts (&ns[0].vertices, &ns[0].faces, &ns[0].edges);

  faces.reset (new std::unique_ptr<uint32_t []> [subdivisions + 1]);

  faces[0].reset (new uint32_t [3 * ns[0].faces]);
    std::cerr
       << "Level " << 0 << ": nvxs=" << ns[0].vertices
       << ", nedges="  << ns[0].edges
       << ", nfaces=" << ns[0].faces << std::endl;

  for (uint32_t s = 1; s <= subdivisions; ++s) {
    SubdividePolygonCounts (&ns[s-1], &ns[s]);
    std::cerr
       << "Level " << s << ": nvxs=" << ns[s].vertices
       << ", nedges="  << ns[s].edges
       << ", nfaces=" << ns[s].faces << std::endl;
    faces[s].reset (new uint32_t [faces_stride * ns[s].faces]);
  }

  vertices.reset (new float[vertices_stride * ns[subdivisions].vertices]);

  IcosahedronVertices (vertices.get(), vertices_stride);
  IcosahedronFaces (faces[0].get(), faces_stride);

  for (uint32_t s = 1; s <= subdivisions; ++s) {
    std::cerr << "Subdividing s=" << s << std::endl;
    uint32_t nvxs = SubdividePolygon (
        vertices_stride, faces_stride,
        vertices.get(), ns[s-1].vertices,
        faces[s-1].get(), ns[s-1].faces,
        faces[s].get());
    std::cerr << "Subdivided Polygon -> nvxs = " << nvxs << std::endl;
  }
}
