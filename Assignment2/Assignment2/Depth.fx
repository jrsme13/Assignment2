//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------




matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;




// TYPEDEFS //

struct VertexInputType
{
    float4 position : POSITION;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 depthPos : TEXCOORD0;
	
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType DepthVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	output.depthPos = output.position;

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 DepthPixelShader(PixelInputType input) : SV_Target
{
	float depthValue;
    float4 color;


	depthValue = input.depthPos.z/ input.depthPos.w;

	color = float4(depthValue,depthValue,depthValue,1.0f);

	return color;
}


////////////////////////////////////////////////////////////////////////////////
// Technique - the axtual drawing
////////////////////////////////////////////////////////////////////////////////
technique10 DepthTechnique
{
    pass pass0
    {
        SetVertexShader(CompileShader(vs_4_0, DepthVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, DepthPixelShader()));
        SetGeometryShader(NULL);
    }
}