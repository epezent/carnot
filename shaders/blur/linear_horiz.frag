#version 330 core

uniform sampler2D image;
uniform vec2 resolution;

out vec4 FragmentColor;

uniform float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
uniform float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );

void main(void)
{
	FragmentColor = texture2D( image, vec2(gl_FragCoord)/resolution.yx ) * weight[0];
	for (int i=1; i<3; i++) {
		FragmentColor += texture2D( image, ( vec2(gl_FragCoord)+vec2(offset[i], 0.0) )/resolution.x ) * weight[i];
		FragmentColor += texture2D( image, ( vec2(gl_FragCoord)-vec2(offset[i], 0.0) )/resolution.x ) * weight[i];
	}
}

/*
sf::Shader hBlur, vBlur;
hBlur.loadFromFile("../shaders/blur/linear_horiz.frag", sf::Shader::Fragment);
vBlur.loadFromFile("../shaders/blur/linear_vert.frag", sf::Shader::Fragment);
hBlur.setUniform("image", sf::Shader::CurrentTexture);
hBlur.setUniform("resolution", sf::Glsl::Vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
vBlur.setUniform("image", sf::Shader::CurrentTexture);
vBlur.setUniform("resolution", sf::Glsl::Vec2(WINDOW_WIDTH, WINDOW_HEIGHT));

...

sf::RenderTexture rTexture, hBlurTex, vBlurTex;
rTexture.clear(sf::Color::White);

// Render to RenderTexture
rTexture.draw(...);
rTexture.display();
sf::Sprite sprite(rTexture.getTexture());
for (std::size_t i = 0; i < iterations; ++i) {
    // horizontal pass
    hBlurTex.clear();
    hBlurTex.draw(sprite,&hBlur);
    hBlurTex.display();
    sprite.setTexture(hBlurTex.getTexture(),true);
    // vertical pass
    vBlurTex.clear();
    vBlurTex.draw(sprite,&vBlur);
    vBlurTex.display();
    sprite.setTexture(vBlurTex.getTexture());
}
window.draw(sprite);
 */
