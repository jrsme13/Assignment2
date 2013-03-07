#include "shader.h"


shader::shader(void)
{

	_effect = 0;
	_technique = 0;
	_layout = 0;

	_worldMatrixPtr = 0;
	_viewMatrixPtr = 0;
	_projectionMatrixPtr = 0;
	
	_texturePtr = 0;



	_lightDirPtr = 0;
	_diffusePtr = 0;

	_lightDirPtr2 = 0;
	_diffusePtr2 = 0;


	_lightPosPtr = 0;

	_ambientPtr = 0;

	_cameraPositionPtr = 0;
	_specularColorPtr = 0;
	_specularPowerPtr = 0;
	_specularColorPtr2 = 0;
	_specularPowerPtr2 = 0;

	_depthMapTexturePtr = 0;
	_lightViewMatrixPtr = 0;
	_lightProjectionMatrixPtr = 0;


}

shader::shader(const shader& other)
{

}


shader::~shader(void)
{
}


bool shader::Initialize(ID3D10Device* device, HWND hwnd)
{
	bool result;


	// Initialize the shader that will be used to draw the triangle.
	result = InitializeShader(device, hwnd, L"../Assignment2/Shader.fx");
	if(!result)
	{
		MessageBox(hwnd, L"Could not the intialize the shader", L"Error", MB_OK);
		return false;
	}

	return true;
}


void shader::Shutdown()
{
	// Shutdown the shader effect.
	ShutdownShader();

	return;
}


void shader::Render(ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,D3DXMATRIX lightViewMatrix,D3DXMATRIX lightProjectionMatrix, 
	ID3D10ShaderResourceView* texture,ID3D10ShaderResourceView* depthMapTexture,D3DXVECTOR3 lightDir, D3DXVECTOR4 diffuse,D3DXVECTOR4 ambient,  D3DXVECTOR3 cameraPos, D3DXVECTOR4 specularColor,
	float specularPower,D3DXVECTOR3 lightDir2,D3DXVECTOR4 diffuse2,D3DXVECTOR4 specColor2,float specPower2)
{
	// Set the shader parameters that it will use for rendering.
	SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix,lightViewMatrix, lightProjectionMatrix,texture,depthMapTexture,lightDir,diffuse, ambient, cameraPos, specularColor, specularPower,
							lightDir2,diffuse2,specColor2,specPower2);

	// Now render the prepared buffers with the shader.
	RenderShader(device, indexCount);

	return;
}


bool shader::InitializeShader(ID3D10Device* device, HWND hwnd,WCHAR* filename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	D3D10_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D10_PASS_DESC passDesc;


	// Initialize the error message.
	errorMessage = 0;

	// Load the shader in from the file.
	result = D3DX10CreateEffectFromFile(filename, NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 
					    device, NULL, NULL, &_effect, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, filename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, filename, L"Missing Shader File", MB_OK);
		}

		return false;
	}


	_technique = _effect->GetTechniqueByName("ColorTechnique");
	if(!_technique)
	{
		return false;
	}


	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;


	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Get the description of the first pass described in the shader technique.
	_technique->GetPassByIndex(0)->GetDesc(&passDesc);

	// Create the input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, 
					   &_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Get pointers to the three matrices inside the shader so we can update them from this class.
	_worldMatrixPtr = _effect->GetVariableByName("worldMatrix")->AsMatrix();
	_viewMatrixPtr = _effect->GetVariableByName("viewMatrix")->AsMatrix();
	_projectionMatrixPtr = _effect->GetVariableByName("projectionMatrix")->AsMatrix();

	_texturePtr = _effect->GetVariableByName("shaderTexture")->AsShaderResource();

	_lightDirPtr = _effect->GetVariableByName("lightDirection")->AsVector();
	_diffusePtr = _effect->GetVariableByName("diffuseColor")->AsVector();

	_lightDirPtr2 = _effect->GetVariableByName("lightDirection2")->AsVector();
	_diffusePtr2 = _effect->GetVariableByName("diffuseColor2")->AsVector();

	_ambientPtr = _effect->GetVariableByName("ambient")->AsVector();
	_cameraPositionPtr =_effect->GetVariableByName("cameraPos")->AsVector();
	_specularColorPtr = _effect->GetVariableByName("specCol")->AsVector();
	_specularPowerPtr = _effect->GetVariableByName("specPower")->AsScalar();

	_specularColorPtr2 = _effect->GetVariableByName("specCol2")->AsVector();
	_specularPowerPtr2 = _effect->GetVariableByName("specPower2")->AsScalar();

	_lightViewMatrixPtr = _effect->GetVariableByName("lightViewMatrix")->AsMatrix();
	_lightProjectionMatrixPtr = _effect->GetVariableByName("lightOrthoMatrix")->AsMatrix();
	_depthMapTexturePtr = _effect->GetVariableByName("depthMapTexture")->AsShaderResource();
	return true;

}

void shader::ShutdownShader()
{
	// Release the pointers to the matrices inside the shader.
	_diffusePtr = 0;
	_lightDirPtr= 0;
	_diffusePtr2 = 0;
	_lightDirPtr2= 0;
	_lightPosPtr = 0;
	_texturePtr = 0;
	_cameraPositionPtr = 0;
	_specularColorPtr = 0;
	_specularPowerPtr = 0;
	_specularColorPtr2 = 0;
	_specularPowerPtr2 = 0;
	_worldMatrixPtr = 0;
	_viewMatrixPtr = 0;
	_projectionMatrixPtr = 0;
	_ambientPtr = 0;

	_depthMapTexturePtr = 0;

	
	_lightViewMatrixPtr = 0;
	_lightProjectionMatrixPtr = 0;

	// Release the pointer to the shader layout.
	if(_layout)
	{
		_layout->Release();
		_layout = 0;
	}

	// Release the pointer to the shader technique.
	_technique = 0;

	// Release the pointer to the shader.
	if(_effect)
	{
		_effect->Release();
		_effect = 0;
	}

	return;
}


void shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}


void shader::SetShaderParameters(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,D3DXMATRIX lightViewMatrix,D3DXMATRIX lightProjectionMatrix,
	ID3D10ShaderResourceView* texture,ID3D10ShaderResourceView* depthMapTexture,D3DXVECTOR3 lightDir, D3DXVECTOR4 diffused,D3DXVECTOR4 ambient,D3DXVECTOR3 cameraPos, D3DXVECTOR4 specularColor, float specularPower,
	D3DXVECTOR3 lightDir2,D3DXVECTOR4 diffuse2,D3DXVECTOR4 specColor2,float specPower2)
{
	// Set the world matrix variable inside the shader.
	_worldMatrixPtr->SetMatrix((float*)&worldMatrix);

	// Set the view matrix variable inside the shader.
	_viewMatrixPtr->SetMatrix((float*)&viewMatrix);

	// Set the projection matrix variable inside the shader.
	_projectionMatrixPtr->SetMatrix((float*)&projectionMatrix);

	_texturePtr->SetResource(texture);
	_ambientPtr->SetFloatVector((float*)&ambient);

	_lightDirPtr->SetFloatVector((float*)&lightDir);
	_diffusePtr->SetFloatVector((float*)&diffused);

	_lightDirPtr2->SetFloatVector((float*)&lightDir2);
	_diffusePtr2->SetFloatVector((float*)&diffuse2);


	_cameraPositionPtr->SetFloatVector((float*)&cameraPos);


	_specularColorPtr->SetFloatVector((float*)&specularColor);
	_specularPowerPtr->SetFloat(specularPower);

	_specularColorPtr2->SetFloatVector((float*)&specColor2);
	_specularPowerPtr2->SetFloat(specPower2);

	_lightViewMatrixPtr->SetMatrix((float*)&lightViewMatrix);
	_lightProjectionMatrixPtr->SetMatrix((float*)&lightProjectionMatrix);
	_depthMapTexturePtr->SetResource(depthMapTexture);

	return;
}


void shader::RenderShader(ID3D10Device* device, int indexCount)
{
	D3D10_TECHNIQUE_DESC techniqueDesc;
	unsigned int i;
	

	// Set the input layout.
	device->IASetInputLayout(_layout);

	// Get the description structure of the technique from inside the shader so it can be used for rendering.
	_technique->GetDesc(&techniqueDesc);

	// Go through each pass in the technique (should be just one currently) and render the triangles.
	for(i=0; i<techniqueDesc.Passes; ++i)
	{
		_technique->GetPassByIndex(i)->Apply(0);
		device->DrawIndexed(indexCount, 0, 0);
	}

	return;
}

