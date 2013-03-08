//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------




matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;


Texture2D shaderTexture;
Texture2D depthMapTexture;


matrix lightViewMatrix;
matrix lightOrthoMatrix;


float3 cameraPos;
float4 ambient;

//Light1
float4 diffuseColor;
float3 lightDirection;
float3 specCol;
float specPower;

//light 2
float4 diffuseColor2;
float3 lightDirection2;
float3 specCol2;
float specPower2;






// Texture Sample state
SamplerState SampleTypeClamp
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};


SamplerState SampleTypeWrap
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
	float4 lightViewPosition: TEXCOORD2;
	
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

	output.lightViewPosition = mul(input.position, worldMatrix);
	output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightOrthoMatrix);
    
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

	float bias;
	float2 orthoTexCoord;
	float depthValue;
	float lightDepthValue;
    

	
	bias = 0.001f;
	
    


	
	color = ambient;

	orthoTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
    orthoTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

	if((saturate(orthoTexCoord.x) == orthoTexCoord.x) && (saturate(orthoTexCoord.y) == orthoTexCoord.y))
    {
		 depthValue = depthMapTexture.Sample(SampleTypeClamp, orthoTexCoord).r;
		 lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
		 lightDepthValue = lightDepthValue - bias;

		if(lightDepthValue < depthValue)
		{
			 lightDir = -lightDirection;
			 lightIntensity = saturate(dot(input.normal, lightDir));

			if (lightIntensity > 0.0f)
			{

				color +=(diffuseColor * lightIntensity);
				

				reflection = normalize(2*lightIntensity*input.normal - lightDir);

				specular = pow(saturate(dot(reflection,input.viewDir)),specPower);
			}
		}
	}

	specular = float4(0.0f,0.0f,0.0f,0.0f);
	specular2 = float4(0.0f,0.0f,0.0f,0.0f);
	    
    //lightDir = -lightDirection;
	lightDir2 = -lightDirection2;
    
    //lightIntensity = saturate(dot(input.normal, lightDir));
	lightIntensity2 = saturate(dot(input.normal, lightDir2));

	

		if (lightIntensity2 > 0.0f)
	{

		//color +=(diffuseColor2 * lightIntensity2);
		//color = saturate(color);

		reflection2 = normalize(2*lightIntensity2*input.normal - lightDir2);

		specular2 = pow(saturate(dot(reflection2,input.viewDir)),specPower2);
	}
	
	textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);
    

	color.rgb = input.lightViewPosition.w;
    //color = saturate(color);
    //color = color * textureColor;
	//color = saturate(color + specular + specular2);
    

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