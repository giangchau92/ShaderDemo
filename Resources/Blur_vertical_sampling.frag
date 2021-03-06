#ifdef GL_ES
precision lowp float;
#endif

#define N 126

varying vec2 v_texCoord;

uniform vec2 u_resolution;


vec4 verticalBlur(sampler2D t, vec2 p)
{
  vec4 sum = vec4(0.0);
  int count = 0;
  vec2 delta = vec2(1.0, 1.0) / u_resolution.xy;
  for (int i = -N; i <= N; i+=2)
  {
      vec2 uv;
      uv.x = p.x;
      if (i == N)
        uv.y = p.y - float(i) * delta.y;
      else
        uv.y = p.y - (float(i) * delta.y - delta.y / 2);
      if (uv.y < 0 || uv.y > 1)
        continue;
      sum += texture2D(t, uv);
      count ++;
  }
  return sum / float(count);
}

void main()
{
 	gl_FragColor = verticalBlur(CC_Texture0, v_texCoord);
}