layout(std140) uniform ViewSettings {
  mat4 mv;
  mat4 mvp;
};

#ifdef BUILDING_VERTEX_SHADER
e
out vec3 var_nrm;

void main()
{
  gl_Position = _mvp * vec4(pos, 1.0);
  var_nrm = normalize(inverse(mat3(_mv)) * nrm);
  //var_nrm = nrm;
}

#endif

#ifdef BUILDING_FRAGMENT_SHADER

in vec3 var_nrm;
out vec4 fragColor;

void main()
{
  fragColor = dot(var_nrm, vec3(0.0, 0.0, -1.0)) * vec4(0.7);
  //fragColor = vec4(abs(var_nrm), 1.0);
}

#endif
