attribute vec3 a_posL;
attribute vec2 a_uv;
uniform mat4 u_WVP;
varying vec2 v_uv;
uniform sampler2D u_texture4;
void main(void)
{
	 vec3 vertex = a_posL;
	 vertex.y = (texture2D(u_texture4, a_uv).r)*10.0;
	 gl_Position = u_WVP * vec4(vertex, 1.0);
	 vec3 v_pos = a_posL;
	 v_uv = a_uv;
}
