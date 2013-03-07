#include "DepthShader.h"


DepthShader::DepthShader(void)
{

	_effect = 0;
	_technique = 0;
	_layout = 0;

	_worldMatrixPtr = 0;
	_viewMatrixPtr = 0;
	_projectionMatrixPtr = 0;
}

DepthShader::DepthShader(const DepthShader& other)
{

}

DepthShader::~DepthShader(void)
{
}


bool DepthShader::Initialize(ID3D10Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device,hwnd,L"../Assignment2/depth.fx");

	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the depth shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void DepthShader::Shutdown()
{
	ShutdownShader();

	return;
}

void DepthShader::Render(ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{

	SetShaderParameters(worldMatrix,viewMatrix,projectionMatrix);

	RenderShader(device,indexCount);

	return;

}

bool DepthShader::InitializeShader(ID3D10Device* device, HWND hwnd, WCHAR* filename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;

	D3D10_INPUT_ELEMENT_DESC polygonLayout[1];
	unsigned int numElements;
	D3D10_PASS_DESC passDesc;

	errorMessage = 0;


	result = D3DX10CreateEffectFromFile(filename, NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 
					    device, NULL, NULL, &_effect, &errorMessage, NULL);
	if(FAILED(result))
	{
		
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, filename);
		}
		else
		{
			MessageBox(hwnd, filename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	_technique = _effect->GetTechniqueByName("DepthTechnique");
	if(!_technique)
	{
		return false;
	}


	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	_technique->GetPassByIndex(0)->GetDesc(&passDesc);

	result = device->CreateInputLayout(polygonLayout, numElements, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &_layout);
	if(FAILED(result))
	{
		return false;
	}

	_worldMatrixPtr = _effect->GetVariableByName("worldMatrix")->AsMatrix();
	_viewMatrixPtr = _effect->GetVariableByName("viewMatrix")->AsMatrix();
	_projectionMatrixPtr = _effect->GetVariableByName("projectionMatrix")->AsMatrix();

	return true;
}


void DepthShader::ShutdownShader()
{

	_worldMatrixPtr = 0;
	_viewMatrixPtr = 0;
	_projectionMatrixPtr = 0;

	if(_layout)
	{
		_layout->Release();
		_layout = 0;
	}


	_technique = 0;

	if(_effect)
	{
		_effect->Release();
		_effect = 0;
	}

	return;
}

void DepthShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());

	bufferSize = errorMessage->GetBufferSize();

	fout.open("shader-error.txt");

	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	errorMessage->Release();
	errorMessage = 0;

	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}


void DepthShader::SetShaderParameters(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{

	_worldMatrixPtr->SetMatrix((float*)&worldMatrix);
	_viewMatrixPtr->SetMatrix((float*)&viewMatrix);
	_projectionMatrixPtr->SetMatrix((float*)&projectionMatrix);

	return;

}


void DepthShader::RenderShader(ID3D10Device* device, int indexCount)
{
	D3D10_TECHNIQUE_DESC techniqueDesc;
	unsigned int i;
	

	
	device->IASetInputLayout(_layout);

	
	_technique->GetDesc(&techniqueDesc);

	
	for(i=0; i<techniqueDesc.Passes; ++i)
	{
		_technique->GetPassByIndex(i)->Apply(0);
		device->DrawIndexed(indexCount, 0, 0);
	}

	return;

}