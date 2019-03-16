uniform vec4 u_color1;
uniform vec4 u_color2;
uniform sampler2D texture;

void main() {
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec2 center = gl_TexCoord[0].xy - 0.5f;
    float t = clamp(2.0f * length(center), 0.0f, 1.0f);
    gl_FragColor = mix(u_color1, u_color2, t) * pixel;
}
