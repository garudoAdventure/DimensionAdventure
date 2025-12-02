#include "Shader.h"
using namespace DirectX;
#include "Directx.h"
#include "MathStruct.h"

#pragma comment(lib, "d3dcompiler.lib")

Shader::Shader(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
  _device = device;
  _deviceContext = deviceContext;

  ID3DBlob* compiledVS;
  D3DCompileFromFile(L"./vertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &compiledVS, nullptr);
  _device->CreateVertexShader(compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), nullptr, &_vertexShader);

  D3D11_INPUT_ELEMENT_DESC layout[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "BONE_IDX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
  };
  _device->CreateInputLayout(&layout[0], 6, compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), &_inputLayout);
  _deviceContext->IASetInputLayout(_inputLayout);

  compiledVS->Release();

  {
    ID3DBlob* compiledPS;
    D3DCompileFromFile(L"./pixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &compiledPS, nullptr);
    _device->CreatePixelShader(compiledPS->GetBufferPointer(), compiledPS->GetBufferSize(), nullptr, &_pixelShader);
    compiledPS->Release();
  }
  {
    ID3DBlob* compiledPS;
    D3DCompileFromFile(L"./blurPS.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &compiledPS, nullptr);
    _device->CreatePixelShader(compiledPS->GetBufferPointer(), compiledPS->GetBufferSize(), nullptr, &_blurShader);
    compiledPS->Release();
  }
  {
    ID3DBlob* compiledPS;
    D3DCompileFromFile(L"./getLuminancePS.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &compiledPS, nullptr);
    _device->CreatePixelShader(compiledPS->GetBufferPointer(), compiledPS->GetBufferSize(), nullptr, &_luminanceShader);
    compiledPS->Release();
  }
  
  
  _deviceContext->VSSetShader(_vertexShader, NULL, 0);
  _deviceContext->PSSetShader(_pixelShader, NULL, 0);

  {
    D3D11_BUFFER_DESC desc;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = sizeof(Transpose);
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    _device->CreateBuffer(&desc, nullptr, &_matrixBuffer);
  }

  {
    D3D11_BUFFER_DESC desc;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = sizeof(Light);
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    _device->CreateBuffer(&desc, nullptr, &_lightBuffer);
  }

  D3D11_SAMPLER_DESC samplerDesc = {};
  samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.MipLODBias = 0;
  samplerDesc.MaxAnisotropy = 16;
  samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  samplerDesc.MinLOD = 0;
  samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
  _device->CreateSamplerState(&samplerDesc, &_samplerWrapState);

  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  _device->CreateSamplerState(&samplerDesc, &_samplerClampState);

  _deviceContext->PSSetSamplers(0, 1, &_samplerWrapState);

  const float fov = PI / 4;
  const float aspect = 1280.0f / 720.0f;
  const float nearZ = 0.1f;
  const float farZ = 200.0f;
  _orthoMatrix = XMMatrixOrthographicLH(16.0f * 3.0f, 9.0f * 3.0f, nearZ, farZ);
  _perspectiveMatrix = XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
}

Shader::~Shader() {
  SAFE_RELEASE(_pixelShader);
  SAFE_RELEASE(_matrixBuffer);
  SAFE_RELEASE(_lightBuffer);
  SAFE_RELEASE(_inputLayout);
  SAFE_RELEASE(_vertexShader);
  SAFE_RELEASE(_samplerWrapState);
  SAFE_RELEASE(_samplerClampState);
}

void Shader::setSamplerState(SamplerState state) {
  switch (state) {
    case SamplerState::WRAP:
      _deviceContext->PSSetSamplers(0, 1, &_samplerWrapState);
      break;
    case SamplerState::CLAMP:
      _deviceContext->PSSetSamplers(0, 1, &_samplerClampState);
      break;
  }
}

void Shader::begin() {
  _deviceContext->VSSetShader(_vertexShader, nullptr, 0);
  _deviceContext->PSSetShader(_pixelShader, nullptr, 0);
  _deviceContext->IASetInputLayout(_inputLayout);
  _deviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);
  _deviceContext->VSSetConstantBuffers(2, 1, &_lightBuffer);
  _worldMatrix = XMMatrixIdentity();
}

void Shader::setPS(PS ps) {
  switch (ps) {
    case PS::NORMAL:
      _deviceContext->PSSetShader(_pixelShader, nullptr, 0);
      break;
    case PS::BLUR:
      _deviceContext->PSSetShader(_blurShader, nullptr, 0);
      break;
    case PS::LUMINANCE:
      _deviceContext->PSSetShader(_luminanceShader, nullptr, 0);
      break;
  }
}

void Shader::setView(Float3 e, Float3 f) {
  XMVECTOR eye = XMVectorSet(e.x, e.y, e.z, 0.0f);
  XMVECTOR focus = XMVectorSet(f.x, f.y, f.z, 0.0f);
  XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  _viewMatrix = XMMatrixLookAtLH(eye, focus, up);
}

void Shader::setProjection(XMMATRIX projMatrix) {
  _projMatrix = projMatrix;
}

XMMATRIX Shader::getOrthoMatrix() {
  return _orthoMatrix;
}

XMMATRIX Shader::getPerspectiveMatrix() {
  return _perspectiveMatrix;
}

void Shader::setWorldMatrix(Float3 pos, Float3 rotate, Float3 scale) {
  _worldMatrix = XMMatrixIdentity();
  _worldMatrix *= XMMatrixScaling(scale.x, scale.y, scale.z);
  _worldMatrix *= XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
  _worldMatrix *= XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void Shader::setWorldMatrix(XMMATRIX& world) {
  _worldMatrix = world;
}

void Shader::set2DMatrix() {
  Transpose mat;
  mat.world = XMMatrixIdentity();
  mat.view = XMMatrixIdentity();
  mat.projection = getOrthoMatrix();
  setMatrix(mat);
}

void Shader::set3DMatrix(XMMATRIX view) {
  Transpose mat;
  mat.world = _worldMatrix;
  mat.view = view;
  mat.projection = getPerspectiveMatrix();
  setMatrix(mat);
}

void Shader::setMatrix() {
  XMFLOAT4X4 transpose[3];
  XMStoreFloat4x4(&transpose[0], XMMatrixTranspose(_worldMatrix));
  XMStoreFloat4x4(&transpose[1], XMMatrixTranspose(_viewMatrix));
  XMStoreFloat4x4(&transpose[2], XMMatrixTranspose(_projMatrix));
  _deviceContext->UpdateSubresource(_matrixBuffer, 0, nullptr, &transpose, 0, 0);
}

void Shader::setMatrix(Transpose& mat) {
  XMFLOAT4X4 transpose[3];
  XMStoreFloat4x4(&transpose[0], XMMatrixTranspose(mat.world));
  XMStoreFloat4x4(&transpose[1], XMMatrixTranspose(mat.view));
  XMStoreFloat4x4(&transpose[2], XMMatrixTranspose(mat.projection));
  _deviceContext->UpdateSubresource(_matrixBuffer, 0, nullptr, &transpose, 0, 0);
}

void Shader::setLight(const Light light) {
  _deviceContext->UpdateSubresource(_lightBuffer, 0, nullptr, &light, 0, 0);
}