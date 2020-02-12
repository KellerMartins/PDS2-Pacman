#version 100
precision mediump float;

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec2 PixelSize;

varying vec4 finalColor;

float BSpline( float x )
{
	float f = x;
	if( f < 0.0 )
	{
		f = -f;
	}

	if( f >= 0.0 && f <= 1.0 )
	{
		return ( 2.0 / 3.0 ) + ( 0.5 ) * ( f* f * f ) - (f*f);
	}
	else if( f > 1.0 && f <= 2.0 )
	{
		return 1.0 / 6.0 * pow( ( 2.0 - f  ), 3.0 );
	}
	return 1.0;
} 

//From https://www.codeproject.com/Articles/236394/Bi-Cubic-and-Bi-Linear-Interpolation-with-GLSL
vec4 BiCubic( sampler2D textureSampler, vec2 TexCoord )
{
    vec4 nSum = vec4( 0.0, 0.0, 0.0, 0.0 );
    vec4 nDenom = vec4( 0.0, 0.0, 0.0, 0.0 );
    float a = fract( TexCoord.x / PixelSize.x );
    float b = fract( TexCoord.y / PixelSize.y );
    for( int m = -1; m <=2; m++ )
    {
        for( int n =-1; n<=2; n++)
        {
			vec4 vecData = texture2D(textureSampler, 
								     TexCoord + vec2(PixelSize.x * float( m ), 
									 PixelSize.y * float( n )));
			float f  = BSpline( float( m ) - a );
			vec4 vecCooef1 = vec4( f,f,f,f );
			float f1 = BSpline ( -( float( n ) - b ) );
			vec4 vecCoeef2 = vec4( f1, f1, f1, f1 );
            nSum = nSum + ( vecData * vecCoeef2 * vecCooef1  );
            nDenom = nDenom + (( vecCoeef2 * vecCooef1 ));
        }
    }
    return nSum / nDenom;
}



void main(void) {
    gl_FragColor = BiCubic(texture0, fragTexCoord);
}