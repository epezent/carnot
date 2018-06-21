uniform float u_feather;

void main() {
    float l = clamp((gl_TexCoord[0].x - u_feather) / u_feather, 0.0, 1.0);
    float t = clamp((gl_TexCoord[0].y - u_feather) / u_feather, 0.0, 1.0);
    float r = clamp((1.0f - gl_TexCoord[0].x) / u_feather, 0.0, 1.0);
    float b = clamp((1.0f - gl_TexCoord[0].y) / u_feather, 0.0, 1.0);
    gl_FragColor = vec4(1.0f,1.0f,1.0f,l*t*r*b) * gl_Color;
}

// uniform vec4 u_color1;
// uniform vec4 u_color2;
// uniform float u_angle;
// uniform sampler2D texture;

// void main() {
//     vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
//     vec2 center = gl_TexCoord[0].xy - 0.5f;
//     float radians = -0.0174532925199433f * u_angle;
//     float t = center.x * sin(radians) + center.y * cos(radians) + 0.5;
//     gl_FragColor = mix(u_color1, u_color2, t) * pixel;
// }
