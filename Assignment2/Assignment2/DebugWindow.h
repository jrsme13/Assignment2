#pragma once

#ifndef _DEBUGWINDOW_H_
#define _DEBUGWINDOW_H_

#include <d3d10.h>
#include <d3dx10.h>


class DebugWindow
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	DebugWindow();
	DebugWindow(const DebugWindow&);
	~DebugWindow();

	bool Initialize(ID3D10Device*, int, int, int, int);
	void Shutdown();
	bool Render(ID3D10Device*, int, int);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D10Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(int, int);
	void RenderBuffers(ID3D10Device*);

private:
	ID3D10Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
};

#endif