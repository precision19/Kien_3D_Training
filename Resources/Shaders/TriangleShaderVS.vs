attribute vec3 a_posL;
attribute vec3 color;
attribute vec2 a_uv;
varying vec2 v_uv;
varying vec3 v_col;
void main()
{
vec4 posL = vec4(a_posL, 1.0);
gl_Position = posL;
v_col = color;
v_uv = a_uv;
}
   