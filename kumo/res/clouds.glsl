#ifdef BUILDING_VERTEX_SHADER

out vec2 var_uv;
out vec2 var_pos;

void main() {
  gl_Position = vec4(in_pos, 1.0);
  var_pos = in_pos.xy;
  var_uv = in_pos.xy * 0.5 + 0.5;
}

#endif

#ifdef BUILDING_FRAGMENT_SHADER

in vec2 var_uv;
in vec2 var_pos;

out vec4 fragColor;

vec4 sunColorIntensity = vec4(0.95, 0.95, 1.0, 0.05);
vec3 sunDir = normalize(vec3(0.0, 0.4, 0.9));
vec4 sphere = vec4(0.0, 0.0, 0.0, 1.0);

float getDensity(vec3 pos) {
  float posScale = 1.0 / 2.0;
  return max(0.0, textureLod(sampler_noiseTex, pos * posScale, 0.0).x);
}

float transmittance(float opticalDepth) {
  return exp(-2.0 * opticalDepth);
}

// henyey greenstein phase function
// gives clouds a bit of a silver lining and a more dynamic appearance
float phase(vec3 inLightVector, vec3 inViewVector, float inG)
{
  // todo: these normalizes might not be necessary so might be a good idea to remove them and just remember to do it at a higher level
  float cos_angle = dot(normalize(inLightVector), normalize(inViewVector));
  return ((1.0 - inG * inG ) / pow((1.0 + inG * inG - 2.0 * inG * cos_angle ), 3.0 / 2.0)) / 4.0 * 3.1415;
}

float lightPoint(vec3 pos, float opticalDepth) {
  float T = transmittance(opticalDepth);
  int lightSamples = 8;
  float lightDist = sphere.w;
  float lightStepSize = lightDist / lightSamples;
  float lightOpticalDepth = 0.0;
  for (int j = 0; j < lightSamples; ++j) {
    lightOpticalDepth += getDensity(pos + lightStepSize * j) * lightStepSize;
  }
  return lightOpticalDepth;
}

vec4 trace(vec3 start, vec3 end, float samples) {
  float opticalDepth = 0.0;
  vec3 light = vec3(0.0);
  
  vec3 ray = end - start;
  vec3 dir = normalize(ray);
  vec3 step = ray / samples;
  float stepSize = length(step);
  
  for (int i = 0; i < samples; ++i) {
    vec3 pos = start + step * i;
    float density = getDensity(pos);
    opticalDepth += density * stepSize;
    float lightOpticalDepth = lightPoint(pos, opticalDepth);
    float lightScattered = phase(sunDir, dir, 0.2) * lightOpticalDepth;
    float beer_factor = 1.0;
    float powder_factor = 1.0;
    float beer = exp(-lightScattered * beer_factor);
    float powder = (1.0 - exp(-2.0 * lightScattered * powder_factor));
    light += vec3(beer * powder);
  }
  
  float T = transmittance(opticalDepth);
  return vec4(light, T);
}

bool intersectSphere(vec3 origin, vec3 direction, vec4 sph, out vec2 hit) {
  float radius2 = sph.w*sph.w;
  
  vec3 L = sph.xyz - origin;

  float tca = dot(L, direction);
  //if (tca < 0)
  //  return false;

  float d2 = dot(L, L) - tca * tca;
  if (d2 > radius2)
    return false;
    
  float thc = sqrt(radius2 - d2);
  hit[0] = tca - thc;
  hit[1] = tca + thc;
  
  // ensure [0] is smaller than [1], by swapping if necessary
  float tmp = hit[0];
  hit[0] = min(hit[0], hit[1]);
  hit[1] = max(tmp, hit[1]);

  return true;
}

void calcRay(out vec3 rayOrigin, out vec3 rayDirection) {
  mat4 invMvp = inverse(uni_mvp);
  
  vec2 pos = var_uv * 2.0 - 1.0;

  vec4 f = invMvp * vec4(pos, -0.5, 1.0);
  vec3 start = f.xyz / f.w;

  f = invMvp * vec4(pos, 0.5, 1.0);
  vec3 end = f.xyz / f.w;
  
  rayOrigin = start;
  rayDirection = normalize(end - start);
}

void main() {
  vec3 origin, direction;
  calcRay(origin, direction);

  fragColor = vec4(vec3(var_uv, 0.0), 1.0);
  fragColor = vec4(vec3(direction), 0.9);
  
  vec2 hit;
  if (intersectSphere(origin, direction, sphere, hit) && hit.y > 0.0) {
    vec3 entry = origin + direction * max(0.0, hit.x);
    vec3 exit = origin + direction * hit.y;

#if 1
    vec4 lightTransmittance = trace(entry, exit, 64);
    fragColor = vec4(lightTransmittance.xyz, 1.0 - lightTransmittance.w);
#endif
    
#if 1
    vec3 normal = normalize(entry - sphere.xyz);
    float diffuse = dot(normal, sunDir);
    fragColor += 0.1 * vec4(vec3(sunColorIntensity.xyz * diffuse), 1.0);
#endif
  }
  else {
    fragColor = vec4(0.0);
  }
  
#if 0
  float noise = texture(sampler_noiseTex, vec3(var_uv, 0.3)).r;
  fragColor = vec4(vec3(noise), 1.0);
#endif
}

#endif