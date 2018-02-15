#extension GL_ARB_explicit_uniform_location : enable

#ifdef BUILDING_VERTEX_SHADER

layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 nrm;

layout (location = 0) uniform mat4 mvp;

out vec3 var_nrm;

void main()
{
  gl_Position = mvp * vec4(pos, 1.0);
  var_nrm = nrm;
}

#endif

#ifdef BUILDING_FRAGMENT_SHADER

in vec3 var_nrm;
out vec4 fragColor;

void main()
{
  fragColor = dot(var_nrm, vec3(0.0, 0.0, -1.0)) * vec4(1.0, 0.0, 0.0, 1.0);
}

#endif
