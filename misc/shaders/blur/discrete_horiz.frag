#version 330 core

uniform sampler2D image;

out vec4 FragmentColor;

uniform float offset[5] = float[]( 0.0, 1.0, 2.0, 3.0, 4.0 );
uniform float weight[5] = float[]( 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 );

void main(void)
{
	FragmentColor = texture2D( image, vec2(gl_FragCoord)/1024.0 ) * weight[0];
	for (int i=1; i<5; i++) {
		FragmentColor += texture2D( image, ( vec2(gl_FragCoord)+vec2(offset[i], 0.0) )/1024.0 ) * weight[i];
		FragmentColor += texture2D( image, ( vec2(gl_FragCoord)-vec2(offset[i], 0.0) )/1024.0 ) * weight[i];
	}
}
