uniform vec4 u_color1;
uniform vec4 u_color2;
uniform float u_angle;
uniform sampler2D texture;

void main() {
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec2 center = gl_TexCoord[0].xy - 0.5f;
    float radians = -0.0174532925199433f * u_angle;
    float t = center.x * sin(radians) + center.y * cos(radians) + 0.5;
    gl_FragColor = mix(u_color1, u_color2, t) * pixel;
}

//=============================================================================
// OLD VERSION(S)
//=============================================================================

// uniform vec4 u_rect;
// uniform vec4 u_color1;
// uniform vec4 u_color2;
// uniform float u_angle;

// layout(origin_upper_left) in vec4 gl_FragCoord;

// void main() {
//     vec2 position = clamp((gl_FragCoord.xy - u_rect.xy) / u_rect.zw, 0.0f, 1.0f);
//     vec2 center = position - 0.5f;
//     float radians = -0.0174532925199433f * u_angle;
//     float t = center.x * sin(radians) + center.y * cos(radians) + 0.5;
//     gl_FragColor = mix(u_color1, u_color2, t);
// }
