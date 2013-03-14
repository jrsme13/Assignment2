//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------




matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;


Texture2D shaderTexture;
Texture2D depthMapTexture;
Texture2D depthMapTexture2;
Texture2D depthMapTexture3;

matrix lightViewMatrix;
matrix lightOrthoMatrix;

matrix lightViewMatrix2;
matrix lightProjectionMatrix2;

matrix lightViewMatrix3;
matrix lightProjectionMatrix3;


float3 cameraPos;
float4 ambient;

//Light1
float4 diffuseColor;
float3 lightPostition;
float3 specCol;
float specPower;

//light 2
float4 diffuseColor2;
float3 lightPosition2;
float3 specCol2;
float specPower2;

float4 diffuseColor3;
float3 lightPosition3;
float3 specCol3;
float specPower3;




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
	float3 lightPos: TEXCOORD1;
	float4 lightViewPosition: TEXCOORD2;
	float3 viewDir: TEXCOORD3;
	float3 lightPos2: TEXCOORD4;
	float4 lightViewPosition2: TEXCOORD5;
	float3 lightPos3: TEXCOORD6;
	float4 lightViewPosition3: TEXCOORD7;
	
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPos;
    

    input.position.w = 1.0f;


    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	output.lightViewPosition = mul(input.position, worldMatrix);
	output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightOrthoMatrix);
    
	output.lightViewPosition2 = mul(input.position, worldMatrix);
	output.lightViewPosition2 = mul(output.lightViewPosition2, lightViewMatrix2);
	output.lightViewPosition2 = mul(output.lightViewPosition2, lightProjectionMatrix2);

	output.lightViewPosition3 = mul(input.position, worldMatrix);
	output.lightViewPosition3 = mul(output.lightViewPosition3, lightViewMatrix3);
	output.lightViewPosition3 = mul(output.lightViewPosition3, lightProjectionMatrix3);


    output.tex = input.tex;
    

    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
	
  
    output.normal = normalize(output.normal);


	worldPos = mul(input.position, worldMatrix);
	

	output.lightPos = lightPostition.xyz - worldPos.xyz;
	output.lightPos = normalize(output.lightPos);

	output.lightPos2 = lightPosition2.xyz - worldPos.xyz;
	output.lightPos2 = normalize(output.lightPos2);

	output.lightPos3 = lightPosition3.xyz - worldPos.xyz;
	output.lightPos3 = normalize(output.lightPos3);

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
	float3 lightDir3;

    float lightIntensity;
	float lightIntensity2;
	float lightIntensity3;

    float4 color;
	float3 reflection;
	float3 reflection2;
	float3 reflection3;

	float4 specular;
	float4 specular2;
	float4 specular3;

	float bias;
	float2 orthoTexCoord;
	float depthValue;
	float lightDepthValue;
    

	
	bias = 0.0005f;
	
    

	// the ambient is the shadow
	
	color = ambient;


	specular = float4(0.0f,0.0f,0.0f,0.0f);

	orthoTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
    orthoTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

	if((saturate(orthoTexCoord.x) == orthoTexCoord.x) && (saturate(orthoTexCoord.y) == orthoTexCoord.y))
    {
		 depthValue = depthMapTexture.Sample(SampleTypeClamp, orthoTexCoord).r;
		 lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
		 lightDepthValue = lightDepthValue - bias;

		if(lightDepthValue < depthValue)
		{
			 lightDir = -input.lightPos;
			 lightIntensity = saturate(dot(input.normal, input.lightPos));

			if (lightIntensity > 0.0f)
			{
				//this is where the colour is actually set
				color +=(diffuseColor * lightIntensity);
				//color = saturate(color);
				reflection = normalize(2*lightIntensity*input.normal - lightDir);
				// uses the camera position to find the viewing direction to work out the specular.
				specular = pow(saturate(dot(reflection,input.viewDir)),specPower);
			}
		}
	}

	specular2 = float4(0.0f,0.0f,0.0f,0.0f);
	    
	orthoTexCoord.x =  input.lightViewPosition2.x / input.lightViewPosition2.w / 2.0f + 0.5f;
    orthoTexCoord.y = -input.lightViewPosition2.y / input.lightViewPosition2.w / 2.0f + 0.5f;

    if((saturate(orthoTexCoord.x) == orthoTexCoord.x) && (saturate(orthoTexCoord.y) == orthoTexCoord.y))
    {
        depthValue = depthMapTexture2.Sample(SampleTypeClamp, orthoTexCoord).r;

        lightDepthValue = input.lightViewPosition2.z / input.lightViewPosition2.w;
        lightDepthValue = lightDepthValue - bias;

        if(lightDepthValue < depthValue)
        {
			lightDir2 = -input.lightPos2;
            lightIntensity2 = saturate(dot(input.normal, input.lightPos2));
            if(lightIntensity2 > 0.0f)
            {
                color += (diffuseColor2 * lightIntensity2);
				//color = saturate(color);
				reflection2 = normalize(2*lightIntensity2*input.normal - lightDir2);

				specular2 = pow(saturate(dot(reflection2,input.viewDir)),specPower2);
            }
        }
    }

	specular3 = float4(0.0f,0.0f,0.0f,0.0f);
	    
	orthoTexCoord.x =  input.lightViewPosition3.x / input.lightViewPosition3.w / 2.0f + 0.5f;
    orthoTexCoord.y = -input.lightViewPosition3.y / input.lightViewPosition3.w / 2.0f + 0.5f;

    if((saturate(orthoTexCoord.x) == orthoTexCoord.x) && (saturate(orthoTexCoord.y) == orthoTexCoord.y))
    {
        depthValue = depthMapTexture3.Sample(SampleTypeClamp, orthoTexCoord).r;

        lightDepthValue = input.lightViewPosition3.z / input.lightViewPosition3.w;
        lightDepthValue = lightDepthValue - bias;

        if(lightDepthValue < depthValue)
        {
			lightDir3 = -input.lightPos3;
            lightIntensity3 = saturate(dot(input.normal, input.lightPos3));
            if(lightIntensity3 > 0.0f)
            {
                color += (diffuseColor3 * lightIntensity3);
				//color = saturate(color);
				reflection3 = normalize(2*lightIntensity3*input.normal - lightDir3);

				specular3 = pow(saturate(dot(reflection3,input.viewDir)),specPower3);
            }
        }
    }

    textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);


	//color = saturate(color);
    
    color = color * textureColor;
	
	color = saturate(color + specular+ specular2 + specular3);
	//color.rgb = input.normal.xyz;

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