attribute vec3 a_posL;
attribute vec2 a_uv;
uniform mat4 u_WVP;
varying vec3 v_pos;
void main(void)
{
 gl_Position = u_WVP * vec4(a_posL, 1.0);
 v_pos = a_posL;
}
