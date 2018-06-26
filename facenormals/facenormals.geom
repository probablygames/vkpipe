#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout ( triangles ) in;
layout ( triangle_strip, max_vertices = 3 ) out;

layout(std140, binding = 0) uniform xbuf {
  mat4 mvp;
} ubuf;

layout(std140, binding = 1) uniform lbuf {
  vec3 dir;
  float ambient;
} light;

layout (location = 0) in vec2 in_texcoord[3];
layout (location = 0) out vec2 out_texcoord;
layout(location = 1) out float face_brightness;

void main()
{
  vec3 A = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
  vec3 B = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
  vec3 normal = normalize(cross(A,B));

  face_brightness = max(light.ambient, dot(normal, light.dir));

  gl_Position = gl_in[0].gl_Position;
  out_texcoord = in_texcoord[0];
  EmitVertex();

  gl_Position = gl_in[1].gl_Position;
  out_texcoord = in_texcoord[1];
  EmitVertex();

  gl_Position = gl_in[2].gl_Position;
  out_texcoord = in_texcoord[2];
  EmitVertex();

  EndPrimitive();
}

