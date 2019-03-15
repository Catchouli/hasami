#ifdef BUILDING_VERTEX_SHADER

out vec2 var_uv;
out vec4 var_col;

void main()
{
  gl_Position = uni_mvp * vec4(in_pos.xy, 0.0, 1.0);
  var_uv = in_uv;
  
  // unpack color
  uint col = floatBitsToUint(in_col);
  uint r = (col & 0xFFu);
  uint g = (col & 0xFF00u) >> 8;
  uint b = (col & 0xFF0000u) >> 16;
  uint a = (col & 0xFF000000u) >> 24;
  var_col = vec4(float(r)/255.0, float(g)/255.0, float(b)/255.0, float(a)/255.0);
}

#endif

#ifdef BUILDING_FRAGMENT_SHADER

in vec2 var_uv;
in vec4 var_col;
out vec4 fragColor;

void main()
{
#ifdef UNI_sampler_albedo
  vec4 texSample = texture(sampler_albedo, var_uv);
#else
  vec4 texSample = vec4(1.0);
#endif

  fragColor = vec4(texSample) * var_col;
}

#endif