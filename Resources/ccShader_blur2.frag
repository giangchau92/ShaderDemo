#ifdef GL_ES
precision lowp float;
#endif

#define N 1
#define M 17

varying vec2 v_texCoord;

uniform vec2 u_resolution;

vec4 blur2(sampler2D t, vec2 p)
{
  vec4 sum = vec4(0.0);
  int count = 0;
  vec2 delta = vec2(1.0, 1.0) / u_resolution.xy;
  for (int i=-N; i < N; i++)
  {
    for (int j = -N; j < N; j++)
    {
      vec2 uv;
      
      if (uv.x < 0 || uv.x > 1 || uv.y < 0 || uv.y > 1)
        continue;
      sum += texture2D(t, uv);
      count ++;
    }
  }
  return sum / float(count);
}

vec4 getAround(sampler2D t, vec2 p)
{
  
}

vec4 blur3(sampler2D t, vec2 p)
{
  vec4 sum = vec4(0.0);
  vec2 delta = vec2(1.0, 1.0) / u_resolution.xy;
  vec2 uv;
  int count = 0;
  for (int i = -M; i <= M; i++)
  {
    if (abs(i) != M)
      continue;
    for (int j = -M ; j <= M; j++)
    {
      if (abs(j) != M)
        continue;
      uv.x = max(0, min(1, p.x + float(i) * N * delta.x));
      uv.y = max(0, min(1, p.y + float(j) * N * delta.y));
      sum += texture2D(t, uv);
      count++;
    }
  }
   return sum / float(count) ;
}

void main()
{
 	gl_FragColor = blur3(CC_Texture0, v_texCoord);
    //gl_FragColor =  texture2D(CC_Texture0, v_texCoord);
}