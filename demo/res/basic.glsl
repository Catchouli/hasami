#ifdef BUILDING_VERTEX_SHADER

out vec3 var_nrm;
out vec2 var_uv;
out vec3 var_world;

void main()
{
  gl_Position = uni_mvp * vec4(in_pos, 1.0);
  // gl_Position.x += 0.5 * sin(gl_Position.y * 1.0 + uni_time) * 0.1; //^ wave
  var_nrm = normalize((mat3(uni_model)) * in_nrm);
  var_uv = in_uv;
  var_world = in_pos;
}

#endif

#ifdef BUILDING_FRAGMENT_SHADER

in vec3 var_nrm;
in vec2 var_uv;
in vec3 var_world;
out vec4 fragColor;

void main()
{
  vec3 albedo = vec3(1.0, 1.0, 1.0);

  vec2 zo = vec2(var_uv.x / 3.141, var_uv.y / 3.141 * 0.5);
  
#ifdef UNI_sampler_albedo
  albedo *= texture(sampler_albedo, var_uv.yx).rgb;
#endif

  float diffuse = dot(var_nrm, vec3(0.0, 0.0, 1.0));
  diffuse = 1.0;
  fragColor = vec4(diffuse * albedo, 1.0);
}

#endif