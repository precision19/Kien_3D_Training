attribute vec3 a_posL;
attribute vec3 color;
varying vec3 v_col;
void main()
{
vec4 posL = vec4(a_posL, 1.0);
gl_Position = posL;
v_col = color;
}
   