#ifndef _VECUTIL_H_
#define _VECUTIL_H_

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

// not to be confused with glm::vec3 &c.
template <typename T>
struct vec {
  T &x, &y, &z, &w, &u, &v;

  vec(T *a) : x(a[0]), y(a[1]), z(a[2]), w(a[3]), u(a[4]), v(a[5]) {}

  template<typename T1> void tri(T1 a, T1 b, T1 c) { x = a; y = b; z = c; }
  template<typename T1> void xyz(T1 a, T1 b, T1 c) { x = a; y = b; z = c; w = 1; }
  template<typename T1> void uv(T1 a, T1 b) { u = a; v = b; }
  void copy(vec<T> const& o) { x = o.x; y = o.y; z = o.z; w = o.w; u = o.u; v = o.v; }
  void copy(glm::vec3 const& o) {x = o.x; y = o.y; z = o.z; w = 1;}
  void dir (glm::vec3 const& o) {x = o.x; y = o.y; z = o.z; w = 0;}
  operator glm::vec3() { glm::vec3 r(x, y, z); return r; }
};

template <typename T>
struct array1d {
  array1d(T *a, uint32_t stride) : a(a), stride(stride) {}
  T *a;
  uint32_t stride;

  vec<T> operator()(uint32_t n) { return vec<T>(a + stride*n); }
};


#endif  // _VECUTIL_H_
