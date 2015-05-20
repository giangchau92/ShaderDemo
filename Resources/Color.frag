#ifdef GL_ES
precision lowp float;
#endif

varying vec2 v_texCoord;

uniform bool u_enable;

void main()
{
  vec4 color = texture2D(CC_Texture0, v_texCoord);
  if (u_enable && color.a > 0)
    color = vec4(1, 1, 1, color.a);
 	gl_FragColor = color;
}