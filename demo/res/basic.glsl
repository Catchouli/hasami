uniform ViewSettings {
  mat4 test;
};

uniform mat4 mv;
uniform mat4 mvp;

#ifdef BUILDING_VERTEX_SHADER

in vec3 pos;
in vec3 nrm;

out vec3 var_nrm;

void main()
{
  gl_Position = mvp * vec4(pos, 1.0);
  var_nrm = normalize(mat3(mv) * nrm) * mat3(test);
}

#endif

#ifdef BUILDING_FRAGMENT_SHADER

in vec3 var_nrm;
out vec4 fragColor;

void main()
{
  fragColor = dot(var_nrm, vec3(0.0, 0.0, -1.0)) * vec4(0.7);
  //fragColor = vec4(var_nrm, 1.0);
}

#endif
