#ifndef _ICOSAHEDRON_H_
#define _ICOSAHEDRON_H_

#include <stdint.h>

void IcosahedronCounts (uint32_t *vertices, uint32_t *faces, uint32_t *edges);
void IcosahedronVertices (float *vertices, uint32_t vertex_stride = 6);
void IcosahedronFaces (uint32_t *faces, uint32_t face_stride = 3);


#endif  // _ICOSAHEDRON_H_
