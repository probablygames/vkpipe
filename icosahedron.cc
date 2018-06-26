#include <math.h>

#include "vkutils.h"
#include "vkmacros.h"

static uint32_t const N_VERTICES = 12;
static uint32_t const N_FACES = 20;

template <typename T>
struct vec {
  T &x, &y, &z, &w, &u, &v;

  vec(T *a) : x(a[0]), y(a[1]), z(a[2]), w(a[3]), u(a[4]), v(a[5]) {}

  template<typename T1> void tri(T1 a, T1 b, T1 c) { x = a; y = b; z = c; }
  template<typename T1> void xyz(T1 a, T1 b, T1 c) { x = a; y = b; z = c; w = 1; }
  template<typename T1> void uv(T1 a, T1 b) { u = a; v = b; }
  void copy(vec<T> const& o) { x = o.x; y = o.y; z = o.z; w = o.w; u = o.u; v = o.v; }
};


template <typename T>
struct array1d {
  array1d(T *a, uint32_t stride) : a(a), stride(stride) {}
  T *a;
  uint32_t stride;

  vec<T> operator()(uint32_t n) { return vec<T>(a + stride*n); }
};


template <typename T> void
Sph(double lat, double lon, double r, vec<T> v){
  v.xyz(
      r * cos(lat) * cos(lon),
      r * sin(lat),
      r * cos(lat) * sin(lon));
  v.uv(lat, lon);
}

void
IcosahedronVertices (float *vertices, uint32_t vertex_stride = 6)
{
  /*
  Spherical coordinates:
    If two vertices are taken to be at the north and south poles (latitude ±90°), 
    then the other ten vertices are at latitude ±arctan( 1 / 2) ≈ ±26.57°. These
    ten vertices are at evenly spaced longitudes (36° apart), alternating
    between north and south latitudes.
  */

  array1d<float> vx(vertices, vertex_stride);
  double lat, lon; // radians, of course. equator is 0 radians lat.
  double dlon = (2.0 * M_PI) / 5.0;
  double r = 0.5f;

  // North pole
  Sph(M_PI / 2.0, 0.0f, r, vx(0));

  // Northern Hemisphere
  lat = atan2(1, 2);
  lon = 0.0f;
  for (uint32_t k = 1; k <= 5; ++k) {
    Sph(lat, lon, r, vx(k));
    lon += dlon;
  }

  // Southern Hemisphere
  lat = -atan2(1, 2);
  lon = dlon / 2.0;
  for (uint32_t k = 6; k <= 10; ++k) {
    Sph(lat, lon, r, vx(k));
    lon += dlon;
  }

  // South pole
  Sph(-M_PI / 2.0, 0.0f, r, vx(11));

  vec<float> a(vx(11));
  /* Alternatively:
    a sphere of radius r=sqrt(1+f^2), where f={1+sqrt(5)}/2, vxs at
    [0,±1,±f], [±1,±f, 0], and [±f,0,±1] 

  But I like the north/south poles being a vertices.
  */
}

void
IcosahedronFaces (uint32_t *faces, uint32_t face_stride = 3)
{
  array1d<uint32_t> f(faces, face_stride);
  int A = 10, B = 11, C = 12, D = 13, E = 14,
      F = 15, G = 16, H = 17, I = 18, J = 19;
  /* Ascii icosahedron:
    
        0     0     0     0     0  
      / 0 \ / 1 \ / 2 \ / 3 \ / 4 \
     1 --- 2 --- 3 --- 4 --- 5 --- 1
      \ 5 /A\ 6 /B\ 7 /C\ 8 /D\ 9 /E\
        6 --- 7 --- 8 --- 9 --- A --- 6
         \ F / \ G / \ H / \ I / \ J /
           B     B     B     B     B
    
        Faces are:
        
        0: 0, 1, 2,
        1: 0, 2, 3,
        2: 0, 3, 4,
        3: 0, 4, 5,
        4: 0, 5, 1,
        
        5: 1, 6, 2,
        6: 2, 7, 3,
        7: 3, 8, 4,
        8: 4, 9, 5,
        9: 5, A, 1,    
        
        A: 2, 6, 7,
        B: 3, 7, 8,
        C: 4, 8, 9,
        D: 5, 9, A,
        E: 1, A, 6,  
        
        F: 6, B, 7,
        G; 7, B, 8,
        H: 8, B, 9,
        I: 9, B, A,
        J: A, B, 6
  */

  f(0).tri( 0, 1, 2 );
  f(1).tri( 0, 2, 3 );
  f(2).tri( 0, 3, 4 );
  f(3).tri( 0, 4, 5 );
  f(4).tri( 0, 5, 1 );
        
  f(5).tri( 1, 6, 2 );
  f(6).tri( 2, 7, 3 );
  f(7).tri( 3, 8, 4 );
  f(8).tri( 4, 9, 5 );
  f(9).tri( 5, A, 1 );
        
  f(A).tri( 2, 6, 7 );
  f(B).tri( 3, 7, 8 );
  f(C).tri( 4, 8, 9 );
  f(D).tri( 5, 9, A );
  f(E).tri( 1, A, 6 );
        
  f(F).tri( 6, B, 7 );
  f(G).tri( 7, B, 8 );
  f(H).tri( 8, B, 9 );
  f(I).tri( 9, B, A );
  f(J).tri( A, B, 6 );
}

void
UnrollGeometry(
    float *vertices, uint32_t vertices_stride,
    uint32_t *faces, uint32_t faces_stride, 
    float *uvertices, uint32_t uvertices_stride)
{
  array1d<float> v(vertices, vertices_stride);
  array1d<uint32_t> f(faces, faces_stride);
  array1d<float> uv(uvertices, uvertices_stride);

  vec<float> a(v(11));
  for (uint32_t k = 0; k < N_FACES; ++k) {
    uint32_t v0 = f(k).x, v1 = f(k).y, v2 = f(k).z;
    uint32_t u0 = 3*k+0, u1 = 3*k+1, u2 = 3*k+2;

    uv(u0).copy(v(v0));
    uv(u1).copy(v(v1));
    uv(u2).copy(v(v2));
  }
}


int
CreateScene(
    VkPhysicalDevice physical_device,
    VkDevice device,
    VkExtent2D surface_extent,

    VkBuffer *vertex_buffer,
    VkDeviceMemory *vertex_memory,
    uint32_t *n_vertices,

    VkImage *texture_image,
    VkDeviceMemory *texture_memory,
    VkImageView *texture_image_view)
{

  float vertices[N_VERTICES * 6];
  IcosahedronVertices (vertices);

  uint32_t faces[N_FACES * 3];
  IcosahedronFaces (faces);

  float vertex_data[N_FACES * 3 * 6];
  UnrollGeometry (vertices, 6, faces, 3, vertex_data, 6);

  *n_vertices = N_FACES * 3;
  uint32_t vertex_buffer_size = (*n_vertices) * 6 * sizeof(float); // x y z w u v;
  re(CreateAndAllocateVertexBuffer(
      physical_device, device, vertex_buffer_size,
      vertex_buffer, vertex_memory));

  re(MapAndInitializeMemory (
      device, *vertex_memory, vertex_data, sizeof(vertex_data)));

  VkFormat texture_format = VK_FORMAT_B8G8R8A8_UNORM; //VK_FORMAT_R8G8B8A8_UNORM;
  VkExtent2D tex_extent = surface_extent;
  tr(CreateAndAllocateImage2D(
      physical_device,
      device, tex_extent, texture_format,
      VK_IMAGE_TILING_LINEAR,
      VK_IMAGE_USAGE_SAMPLED_BIT,
      (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
      texture_image, texture_memory));

  tr(CreateImage2DView(
      device, *texture_image, texture_format, VK_IMAGE_ASPECT_COLOR_BIT,
      texture_image_view));

  uint32_t texture_size, texture_pitch;
  GetImagePitch (device, *texture_image, &texture_size, &texture_pitch);
  std::unique_ptr<uint8_t []> texture(new uint8_t [texture_size]);

  for (uint32_t x = 0; x < tex_extent.width; ++x) {
    for (uint32_t y = 0; y < tex_extent.height; ++y) {
      uint8_t red = (255 * x) / tex_extent.width;
      uint8_t green = (255 * (tex_extent.width - x)) / tex_extent.width;
      uint8_t blue = (255 * y) / tex_extent.height;
      uint8_t alpha = (255 * (tex_extent.height - y)) / tex_extent.height;

      texture[0 + 4 * x + texture_pitch * y] = blue;
      texture[1 + 4 * x + texture_pitch * y] = green;
      texture[2 + 4 * x + texture_pitch * y] = red;
      texture[3 + 4 * x + texture_pitch * y] = alpha;
    }
  }

  re(MapAndInitializeMemory (
      device, *texture_memory, texture.get(), texture_size));
  return 0;

}
