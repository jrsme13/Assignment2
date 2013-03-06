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
	float3 viewDir2: TEXCOORD2;
	
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
    float lightIntensity;
    float4 color;
	float3 reflection;
	float4 specular;

	
    
	

    textureColor = shaderTexture.Sample(SampleType, input.tex);


	
	color = ambient;

	specular = float4(0.0f,0.0f,0.0f,0.0f);
	    
    lightDir = -lightDirection;

    
    lightIntensity = saturate(dot(input.normal, lightDir));

	if (lightIntensity > 0.0f)
	{

		color +=(diffuseColor * lightIntensity);
		color = saturate(color);

		reflection = normalize(2*lightIntensity*input.normal - lightDir);

		specular = pow(saturate(dot(reflection,input.viewDir)),specPower);
	}

    
    color = color * textureColor;
	color = saturate(color + specular);
    

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