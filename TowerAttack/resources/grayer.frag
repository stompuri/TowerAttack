uniform sampler2D texture;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    if(pixel != vec4(1.0, 1.0, 1.0, 1.0) && pixel != vec4(0.0, 0.0, 0.0, 0.0))
        pixel = vec4(0.7, 0.7, 0.7, 1.0);

    gl_FragColor = pixel;
    // multiply it by the color
    //gl_FragColor = gl_Color * pixel;
}