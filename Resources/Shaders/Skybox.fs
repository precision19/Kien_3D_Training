precision mediump float;
uniform samplerCube u_samplerCubeMap;
varying vec3 v_pos;
void main(void)
{
 gl_FragColor = textureCube(u_samplerCubeMap, v_pos);
}
