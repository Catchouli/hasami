#ifdef BUILDING_VERTEX_SHADER

out vec3 var_nrm;
out vec2 var_uv;

void main()
{
  gl_Position = uni_mvp * vec4(in_pos, 1.0);
  var_nrm = normalize((mat3(uni_m)) * in_nrm);
  var_uv = in_uv;
}

#endif

#ifdef BUILDING_FRAGMENT_SHADER

in vec3 var_nrm;
in vec2 var_uv;
out vec4 fragColor;

void main()
{
  vec4 diffuse = vec4(texture(sampler_diffuse, var_uv).rgb, 1.0);
  fragColor = dot(var_nrm, vec3(0.0, 0.0, 1.0)) * vec4(0.7) * diffuse;
}

#endif
