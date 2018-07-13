#ifdef BUILDING_VERTEX_SHADER

out vec3 var_nrm;
out vec2 var_uv;

void main()
{
  gl_Position = uni_mvp * vec4(in_pos, 1.0);
  // gl_Position.x += 0.5 * sin(gl_Position.y * 1.0 + uni_time) * 0.1; //^ wave
  var_nrm = normalize((mat3(uni_model)) * in_nrm);
  var_uv = in_uv;
}

#endif

#ifdef BUILDING_FRAGMENT_SHADER

in vec3 var_nrm;
in vec2 var_uv;
out vec4 fragColor;

void main()
{
  vec4 col = vec4(1.0, 1.0, 1.0, 0.0);
  
#ifdef UNI_sampler_albedo
  vec3 albedo = texture(sampler_albedo, var_uv).rgb;
#else
  vec3 albedo = vec3(1.0);
#endif

  vec4 diffuse = vec4(albedo, 1.0) * col;
  fragColor = dot(var_nrm, vec3(0.0, 0.0, 1.0)) * vec4(0.7) * diffuse;
  fragColor = vec4(1.0);
}

#endif