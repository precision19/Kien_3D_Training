attribute vec3 a_posL;
attribute vec2 a_uv;
varying vec2 v_uv;
uniform mat4 u_WVP;
void main()
{
vec4 posL = u_WVP*vec4(a_posL, 1.0);
gl_Position = posL;
v_uv = a_uv;
}
   