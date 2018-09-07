#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec4 xyHalfPixelSize_zwPixelSize;
uniform int kernelSize;

out vec4 finalColor;

//Gaussian blur functions from 
//https://software.intel.com/en-us/blogs/2014/07/15/an-investigation-of-fast-real-time-gpu-based-image-blur-algorithms

vec3 GaussianBlur63( sampler2D tex0, vec2 centreUV, vec2 halfPixelOffset, vec2 pixelOffset )                                                                           
{                                                                                                                                                                    
    vec3 colOut = vec3( 0, 0, 0 );                                                                                                                                   

    //Kernel 63x63
    const int stepCount = 16;
    float gWeights[stepCount];
    gWeights[0] = 0.05991;
    gWeights[1] = 0.07758;
    gWeights[2] = 0.07232;
    gWeights[3] = 0.06476;
    gWeights[4] = 0.05571;
    gWeights[5] = 0.04604;
    gWeights[6] = 0.03655;
    gWeights[7] = 0.02788;
    gWeights[8] = 0.02042;
    gWeights[9] = 0.01438;
    gWeights[10] = 0.00972;
    gWeights[11] = 0.00631;
    gWeights[12] = 0.00394;
    gWeights[13] = 0.00236;
    gWeights[14] = 0.00136;
    gWeights[15] = 0.00075;
    
    float gOffsets[stepCount];
    gOffsets[0] = 0.66555;
    gOffsets[1] = 2.49371;
    gOffsets[2] = 4.48868;
    gOffsets[3] = 6.48366;
    gOffsets[4] = 8.47864;
    gOffsets[5] = 10.47362;
    gOffsets[6] = 12.46860;
    gOffsets[7] = 14.46360;
    gOffsets[8] = 16.45860;
    gOffsets[9] = 18.45361;
    gOffsets[10] = 20.44863;
    gOffsets[11] = 22.44365;
    gOffsets[12] = 24.43869;
    gOffsets[13] = 26.43375;
    gOffsets[14] = 28.42881;
    gOffsets[15] = 30.42389;
    
                                                                                                                                                                     
    for( int i = 0; i < stepCount; i++ )                                                                                                                             
    {                                                                                                                                                                
        vec2 texCoordOffset = gOffsets[i] * pixelOffset;                                                                                                           
        vec3 col = texture2D( tex0, centreUV + texCoordOffset ).xyz + texture2D( tex0, centreUV - texCoordOffset ).xyz;                                                
        colOut += gWeights[i] * col;                                                                                                                               
    }                                                                                                                                                                
                                                                                                                                                                     
    return colOut;                                                                                                                                                   
}

vec3 GaussianBlur35( sampler2D tex0, vec2 centreUV, vec2 halfPixelOffset, vec2 pixelOffset )                                                                           
{                                                                                                                                                                    
    vec3 colOut = vec3( 0, 0, 0 );                                                                                                                                   
                                                                                                                                                                     
    //Kernel width 35 x 35
    const int stepCount = 9;
    float gWeights[9];
    gWeights[0] = 0.10855;
    gWeights[1] = 0.13135;
    gWeights[2] = 0.10406;
    gWeights[3] = 0.07216;
    gWeights[4] = 0.04380;
    gWeights[5] = 0.02328;
    gWeights[6] = 0.01083;
    gWeights[7] = 0.00441;
    gWeights[8] = 0.00157;
    
    float gOffsets[9];
    gOffsets[0] = 0.66293;
    gOffsets[1] = 2.47904;
    gOffsets[2] = 4.46232;
    gOffsets[3] = 6.44568;
    gOffsets[4] = 8.42917;
    gOffsets[5] = 10.41281;
    gOffsets[6] = 12.39664;
    gOffsets[7] = 14.38070;
    gOffsets[8] = 16.36501;
                                                                                                                                                                     
    for( int i = 0; i < stepCount; i++ )                                                                                                                             
    {                                                                                                                                                                
        vec2 texCoordOffset = gOffsets[i] * pixelOffset;                                                                                                           
        vec3 col = texture2D( tex0, centreUV + texCoordOffset ).xyz + texture2D( tex0, centreUV - texCoordOffset ).xyz;                                                
        colOut += gWeights[i] * col;                                                                                                                               
    }                                                                                                                                                                
                                                                                                                                                                     
    return colOut;                                                                                                                                                   
}          
vec3 GaussianBlur19( sampler2D tex0, vec2 centreUV, vec2 halfPixelOffset, vec2 pixelOffset )                                                                           
{                                                                                                                                                                    
    vec3 colOut = vec3( 0, 0, 0 );                                                                                                                                   

    //Kernel 19x19
    
    const int stepCount = 5;
    float gWeights[stepCount];
    gWeights[0] = 0.19955;
    gWeights[1] = 0.18945;
    gWeights[2] = 0.08376;
    gWeights[3] = 0.02321;
    gWeights[4] = 0.00403;

    float gOffsets[stepCount];
    gOffsets[0] = 0.65319;
    gOffsets[1] = 2.42547;
    gOffsets[2] = 4.36803;
    gOffsets[3] = 6.31412;
    gOffsets[4] = 8.26479;
                                                                                                                                                                     
    for( int i = 0; i < stepCount; i++ )                                                                                                                             
    {                                                                                                                                                                
        vec2 texCoordOffset = gOffsets[i] * pixelOffset;                                                                                                           
        vec3 col = texture2D( tex0, centreUV + texCoordOffset ).xyz + texture2D( tex0, centreUV - texCoordOffset ).xyz;                                                
        colOut += gWeights[i] * col;                                                                                                                               
    }                                                                                                                                                                
                                                                                                                                                                     
    return colOut;                                                                                                                                                   
}

vec3 GaussianBlur15( sampler2D tex0, vec2 centreUV, vec2 halfPixelOffset, vec2 pixelOffset )                                                                           
{                                                                                                                                                                    
    vec3 colOut = vec3( 0, 0, 0 );                                                                                                                                   

    //Kernel 15x15
    const int stepCount = 4;
    float gWeights[stepCount];
    gWeights[0] = 0.24961;
    gWeights[1] = 0.19246;
    gWeights[2] = 0.05148;
    gWeights[3] = 0.00645;
    
    float gOffsets[stepCount];
    gOffsets[0] = 0.64434;
    gOffsets[1] = 2.37885;
    gOffsets[2] = 4.29111;
    gOffsets[3] = 6.21661;
    
                                                                                                                                                                     
    for( int i = 0; i < stepCount; i++ )                                                                                                                             
    {                                                                                                                                                                
        vec2 texCoordOffset = gOffsets[i] * pixelOffset;                                                                                                           
        vec3 col = texture2D( tex0, centreUV + texCoordOffset ).xyz + texture2D( tex0, centreUV - texCoordOffset ).xyz;                                                
        colOut += gWeights[i] * col;                                                                                                                               
    }                                                                                                                                                                
                                                                                                                                                                     
    return colOut;                                                                                                                                                   
}

void main()
{
    finalColor.rgb = GaussianBlur63(texture0, fragTexCoord, xyHalfPixelSize_zwPixelSize.xy, xyHalfPixelSize_zwPixelSize.zw);
    finalColor.a = 1.0;
}