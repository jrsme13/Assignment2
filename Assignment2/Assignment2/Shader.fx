//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

#define NUM_LIGHTS 4


matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;
Texture2D shaderTexture;


float4 diffuseColor[NUM_LIGHTS];
//float3 lightDirection;
float3 lightPos[NUM_LIGHTS];
float4 ambient;

float3 cameraPos;
float3 specCol;
float specPower;

// Texture Sample state

SamplerState SampleType
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

// TYPEDEFS //

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal: NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal: NORMAL;
	float3 lightPos1: TEXCOORD1;
	float3 lightPos2: TEXCOORD2;
	float3 lightPos3: TEXCOORD3;
	float3 lightPos4: TEXCOORD4;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPos;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the input color for the pixel shader to use.
    output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);


	worldPos = mul(input.position, worldMatrix);

	output.lightPos1 = lightPos[0].xyz - worldPos.xyz;
	output.lightPos2 = lightPos[1].xyz - worldPos.xyz;
	output.lightPos3 = lightPos[2].xyz - worldPos.xyz;
	output.lightPos4 = lightPos[3].xyz - worldPos.xyz;

	output.lightPos1 = normalize(output.lightPos1);
	output.lightPos2 = normalize(output.lightPos2);
	output.lightPos3 = normalize(output.lightPos3);
	output.lightPos4 = normalize(output.lightPos4);

    return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ColorPixelShader(PixelInputType input) : SV_Target
{
    float4 textureColor;
	//float3 lightDir;
    float lightIntensity1,lightIntensity2,lightIntensity3,lightIntensity4;
    float4 color,color1,color2,color3,color4;
	//float3 reflection;
	//float4 specular;

	lightIntensity1 = saturate(dot(input.normal, input.lightPos1));
    lightIntensity2 = saturate(dot(input.normal, input.lightPos2));
    lightIntensity3 = saturate(dot(input.normal, input.lightPos3));
    lightIntensity4 = saturate(dot(input.normal, input.lightPos4));
    

	color1 = diffuseColor[0] * lightIntensity1;
    color2 = diffuseColor[1] * lightIntensity2;
    color3 = diffuseColor[2] * lightIntensity3;
    color4 = diffuseColor[3] * lightIntensity4;

    textureColor = shaderTexture.Sample(SampleType, input.tex);

	color = saturate(color1 + color2 + color3 + color4) * textureColor;
	return color;
	//color = ambient;

	//specular = float4(0.0f,0.0f,0.0f,0.0f);
	    
    //diretional light lightDir = -lightDirection;

    
    //lightIntensity = saturate(dot(input.normal, lightDir));

	//if (lightIntensity > 0.0f)
	//{

		//color +=(diffuseColor * lightIntensity);
		//color = saturate(color);

		//reflection = normalize(2*lightIntensity*input.normal - lightDir);

		//specular = pow(saturate(dot(reflection,input.viewDir)),specPower);
	//}

    // Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
    

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    //color = color * textureColor;
	//color = saturate(color + specular);
    

    
}


////////////////////////////////////////////////////////////////////////////////
// Technique - the axtual drawing
////////////////////////////////////////////////////////////////////////////////
technique10 ColorTechnique
{
    pass pass0
    {
        SetVertexShader(CompileShader(vs_4_0, ColorVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, ColorPixelShader()));
        SetGeometryShader(NULL);
    }
}