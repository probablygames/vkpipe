#include "icosahedron.h"

#include <math.h>

#include "vkutils.h"
#include "vkmacros.h"
#include "vecutil.h"

static uint32_t const N_VERTICES = 12;
static uint32_t const N_FACES = 20;
static uint32_t const N_EDGES = 30;


void
IcosahedronCounts (
    uint32_t *vertices, uint32_t *faces, uint32_t *edges)
{
  *vertices = N_VERTICES;
  *faces = N_FACES;
  *edges = N_EDGES;
}

template <typename T> void
Sph(double lat, double lon, double r, vec<T> v){
  v.xyz(
      r * cos(lat) * cos(lon),
      r * sin(lat),
      r * cos(lat) * sin(lon));
}

void
IcosahedronVertices (float *vertices, uint32_t vertex_stride)
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
  double r = 1.0f;

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
IcosahedronFaces (uint32_t *faces, uint32_t face_stride)
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
