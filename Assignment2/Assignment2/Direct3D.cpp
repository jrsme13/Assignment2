#include "Direct3D.h"


Direct3D::Direct3D()
{
	_device = 0;
	_swapChain = 0;
	_renderTargetView = 0;
	_depthStencilBuffer = 0;
	_depthStencilState = 0;
	_depthStencilView = 0;
	_rasterState = 0;
}

Direct3D::Direct3D(const Direct3D& other)
{

}


Direct3D::~Direct3D()
{
}

bool Direct3D::intialize(int screenWidth, int screenHeight,bool vsync, HWND hwnd, bool fullscreen,
							float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D10Texture2D* backBufferPtr;
	D3D10_TEXTURE2D_DESC descDepth;
	D3D10_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D10_VIEWPORT viewport;
	float fieldOfView, screenAspect;
	D3D10_RASTERIZER_DESC rasterDesc;


	// Store the vsync setting.
	_vsync_enabled = vsync;


	// Setting up swap chain


    ZeroMemory( &swapChainDesc, sizeof(swapChainDesc) );
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//bit for vysnc

	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

	if(fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;

	result = D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, 
					       &swapChainDesc, &_swapChain, &_device);
	if(FAILED(result))
	{
		MessageBox(hwnd,NULL,L"D3D10CreateDeviceAndSwapChain Failed",MB_OK);
		return false;
	}

	result = _swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		MessageBox(hwnd,L"Getbuffer",L"Error",MB_OK);
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = _device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView);
	if(FAILED(result))
	{
		MessageBox(hwnd,L"create traget view",L"Error",MB_OK);
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	//-------------------------------------
	// Depth buffer

	ZeroMemory(&descDepth, sizeof(descDepth));

	descDepth.Width = screenWidth;
    descDepth.Height = screenHeight;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D10_USAGE_DEFAULT;
    descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

	result = _device->CreateTexture2D(&descDepth, NULL, &_depthStencilBuffer);
	if(FAILED(result))
	{
		MessageBox(hwnd,L"create texture 2d",L"Error",MB_OK);
		return false;
	}

	// Depth Stencil setup

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
	if(FAILED(result))
	{
		MessageBox(hwnd,L"create depth stencil state",L"Error",MB_OK);
		return false;
	}

	// Set the depth stencil state on the D3D device.
	_device->OMSetDepthStencilState(_depthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = _device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
	if(FAILED(result))
	{
		MessageBox(hwnd,L"create depth stencil view",L"Error",MB_OK);
		return false;
	}


	// Bind the render target view and depth stencil buffer to the output render pipeline.
	_device->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	//-------------------------------------------------------

	//setting up rater bit for deciding which polygons to be drawn

	rasterDesc.AntialiasedLineEnable = false;
	//rasterDesc.CullMode = D3D10_CULL_BACK;
	rasterDesc.CullMode = D3D10_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D10_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = _device->CreateRasterizerState(&rasterDesc, &_rasterState);
	if(FAILED(result))
	{
		MessageBox(hwnd,L"create rasterizer",L"Error",MB_OK);
		return false;
	}

	// Now set the rasterizer state.
	_device->RSSetState(_rasterState);

	//--------------------------------------------------------

	// setting the viewport


	_viewport.Width = screenWidth;
    _viewport.Height = screenHeight;
    _viewport.MinDepth = 0.0f;
    _viewport.MaxDepth = 1.0f;
    _viewport.TopLeftX = 0;
    _viewport.TopLeftY = 0;

	// Create the viewport.
	_device->RSSetViewports(1, &_viewport);

	//---------------------------------------------------------

	// matrix setup

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	D3DXMatrixIdentity(&_worldMatrix);
	D3DXMatrixOrthoLH(&_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;

}


void Direct3D::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if(_swapChain)
	{
		_swapChain->SetFullscreenState(false, NULL);
	}


	if(_rasterState)
	{
		_rasterState->Release();
		_rasterState = 0;
	}

	if(_depthStencilView)
	{
		_depthStencilView->Release();
		_depthStencilView = 0;
	}

	if(_depthStencilState)
	{
		_depthStencilState->Release();
		_depthStencilState = 0;
	}

	if(_depthStencilBuffer)
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = 0;
	}

	if(_renderTargetView)
	{
		_renderTargetView->Release();
		_renderTargetView = 0;
	}

	if(_swapChain)
	{
		_swapChain->Release();
		_swapChain = 0;
	}

	if(_device)
	{
		_device->Release();
		_device = 0;
	}

	return;


}


void Direct3D::SetupScene(float red, float green, float blue, float aplah)
{


	float bgColour[] = {red, green, blue, aplah};

	_device->ClearRenderTargetView(_renderTargetView, bgColour);
    
	// Clear the depth buffer.
	_device->ClearDepthStencilView(_depthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void Direct3D::DrawScene()
{

	_swapChain->Present(0, 0);
	

	return;
}


ID3D10Device* Direct3D::GetDevice()
{
	return _device;
}

void Direct3D::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = _projectionMatrix;
	return;
}

void Direct3D::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = _orthoMatrix;
	return;
}

void Direct3D::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = _worldMatrix;
	return;
}



ID3D10DepthStencilView* Direct3D::GetDepthStencilView()
{
	return _depthStencilView;
}

void Direct3D::SetBackBufferRenderTarget()
{

	_device->OMSetRenderTargets(1,&_renderTargetView,_depthStencilView);
	return;

}


void Direct3D::ResetViewport()
{
	// Set the viewport.
    _device->RSSetViewports(1, &_viewport);

	return;
}