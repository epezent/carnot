attribute vec4 position;
attribute vec4 inputTextureCoordinate;

uniform float texelWidthOffset;
uniform float texelHeightOffset;

varying vec2 blurCoordinates[5];

void main()
{
    gl_Position = position;

    vec2 singleStepOffset = vec2(texelWidthOffset, texelHeightOffset);
    blurCoordinates[0] = inputTextureCoordinate.xy;
    blurCoordinates[1] = inputTextureCoordinate.xy + singleStepOffset * 1.407333;
    blurCoordinates[2] = inputTextureCoordinate.xy - singleStepOffset * 1.407333;
    blurCoordinates[3] = inputTextureCoordinate.xy + singleStepOffset * 3.294215;
    blurCoordinates[4] = inputTextureCoordinate.xy - singleStepOffset * 3.294215;
}
