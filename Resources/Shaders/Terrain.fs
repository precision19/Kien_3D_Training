precision mediump float;
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;
varying vec2 v_uv;

void main(){
    vec4 BT, tex1, tex2, tex3;
    BT = texture2D(u_texture0, v_uv);
    tex1 = texture2D(u_texture1, v_uv);
    tex2 = texture2D(u_texture2, v_uv);
    tex3 = texture2D(u_texture3, v_uv);
    gl_FragColor = (BT.r * tex1 + BT.g * tex2 + BT.b * tex3)/(BT.r + BT.g + BT.b);
}