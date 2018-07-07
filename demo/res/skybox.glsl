#ifdef BUILDING_VERTEX_SHADER

out vec3 var_nrm;
out vec2 var_uv;

void main()
{
  gl_Position = uni_projection * mat4(mat3(uni_view)) * vec4(in_pos, 1.0);
  var_uv = in_uv;
}

#endif

#ifdef BUILDING_FRAGMENT_SHADER

in vec2 var_uv;
out vec4 fragColor;

void main()
{
#ifdef UNI_sampler_albedo
  vec3 albedo = texture(sampler_albedo, var_uv).rgb;
#else
  vec3 albedo = vec3(0.0);
#endif

  fragColor = vec4(albedo, 1.0);
}

#endif