//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------




matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;
Texture2D shaderTexture;


float4 diffuseColor;
float3 lightDirection;
float4 ambient;

float4 diffuseColor2;
float3 lightDirection2;

float3 cameraPos;
float3 specCol;
float specPower;

float3 specCol2;
float specPower2;

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
	float3 viewDir: TEXCOORD1;
	
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

	output.viewDir = cameraPos.xyz - worldPos.xyz;
	

	output.viewDir = normalize(output.viewDir);
	

    return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ColorPixelShader(PixelInputType input) : SV_Target
{
    float4 textureColor;
	float3 lightDir;
	float3 lightDir2;

    float lightIntensity;
	float lightIntensity2;

    float4 color;
	float3 reflection;
	float3 reflection2;

	float4 specular;
	float4 specular2;

	
    
	

    textureColor = shaderTexture.Sample(SampleType, input.tex);


	
	color = ambient;

	specular = float4(0.0f,0.0f,0.0f,0.0f);
	specular2 = float4(0.0f,0.0f,0.0f,0.0f);
	    
    lightDir = -lightDirection;
	lightDir2 = -lightDirection2;
    
    lightIntensity = saturate(dot(input.normal, lightDir));
	lightIntensity2 = saturate(dot(input.normal, lightDir2));

	if (lightIntensity > 0.0f)
	{

		color +=(diffuseColor * lightIntensity);
		//color = saturate(color);

		reflection = normalize(2*lightIntensity*input.normal - lightDir);

		specular = pow(saturate(dot(reflection,input.viewDir)),specPower);
	}

		if (lightIntensity2 > 0.0f)
	{

		color +=(diffuseColor2 * lightIntensity2);
		//color = saturate(color);

		reflection2 = normalize(2*lightIntensity2*input.normal - lightDir2);

		specular2 = pow(saturate(dot(reflection2,input.viewDir)),specPower2);
	}
	
    
    color = saturate(color);
    color = color * textureColor;
	color = saturate(color + specular + specular2);
    

    return color;
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