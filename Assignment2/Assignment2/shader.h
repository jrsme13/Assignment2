#pragma once

#ifndef _SHADER_H_
#define _SHADER_H_


#include <d3d10.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;


class shader
{
public:
	shader();
	shader(const shader&);
	~shader();


	bool Initialize(ID3D10Device*, HWND);
	void Shutdown();
	void Render(ID3D10Device*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,ID3D10ShaderResourceView*);


private:

	ID3D10Effect* _effect;
	ID3D10EffectTechnique* _technique;
	ID3D10InputLayout* _layout;

	ID3D10EffectMatrixVariable* _worldMatrixPtr;
	ID3D10EffectMatrixVariable* _viewMatrixPtr;
	ID3D10EffectMatrixVariable* _projectionMatrixPtr;
	ID3D10EffectShaderResourceVariable* _texturePtr;




	bool InitializeShader(ID3D10Device*, HWND, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	void SetShaderParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,ID3D10ShaderResourceView*);
	void RenderShader(ID3D10Device*, int);
};


#endif

