#include "Shader.h"
#include "./DirectX/Directx.h"
#include <fstream>

Shader::Shader(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
  _device = device;
  _deviceContext = deviceContext;

  std::ifstream ifs_vs("vertexShader.cso", std::ios::binary);
  ifs_vs.seekg(0, std::ios::end); // ファイルポインタを末尾に移動
  std::streamsize filesize = ifs_vs.tellg(); // ファイルポインタの位置を取得（つまりファイルサイズ）
  ifs_vs.seekg(0, std::ios::beg); // ファイルポインタを先頭に戻す

  // バイナリデータを格納するためのバッファを確保
  unsigned char* vsbinary_pointer = new unsigned char[filesize];
  ifs_vs.read((char*)vsbinary_pointer, filesize); // バイナリデータを読み込む
  ifs_vs.close(); // ファイルを閉じる

  _device->CreateVertexShader(vsbinary_pointer, filesize, nullptr, &_vertexShader);

  D3D11_INPUT_ELEMENT_DESC layout[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "BONE_IDX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
  };
  _device->CreateInputLayout(&layout[0], 6, vsbinary_pointer, filesize, &_inputLayout);
  _deviceContext->IASetInputLayout(_inputLayout);

  delete[] vsbinary_pointer; // バイナリデータのバッファを解放

  {
    std::ifstream ifs_ps("pixelShader.cso", std::ios::binary);
    ifs_ps.seekg(0, std::ios::end);
    filesize = ifs_ps.tellg();
    ifs_ps.seekg(0, std::ios::beg);
    unsigned char* psbinary_pointer = new unsigned char[filesize];
    ifs_ps.read((char*)psbinary_pointer, filesize);
    ifs_ps.close();

    _device->CreatePixelShader(psbinary_pointer, filesize, nullptr, &_pixelShader);
    delete[] psbinary_pointer;
  }
  {
    std::ifstream ifs_ps("noTexPS.cso", std::ios::binary);
    ifs_ps.seekg(0, std::ios::end);
    filesize = ifs_ps.tellg();
    ifs_ps.seekg(0, std::ios::beg);
    unsigned char* psbinary_pointer = new unsigned char[filesize];
    ifs_ps.read((char*)psbinary_pointer, filesize);
    ifs_ps.close();

    _device->CreatePixelShader(psbinary_pointer, filesize, nullptr, &_noTexShader);
    delete[] psbinary_pointer;
  }
  {
    std::ifstream ifs_ps("luminancePS.cso", std::ios::binary);
    ifs_ps.seekg(0, std::ios::end);
    filesize = ifs_ps.tellg();
    ifs_ps.seekg(0, std::ios::beg);
    unsigned char* psbinary_pointer = new unsigned char[filesize];
    ifs_ps.read((char*)psbinary_pointer, filesize);
    ifs_ps.close();

    _device->CreatePixelShader(psbinary_pointer, filesize, nullptr, &_luminanceShader);
    delete[] psbinary_pointer;
  }
  {
    std::ifstream ifs_ps("blurPS.cso", std::ios::binary);
    ifs_ps.seekg(0, std::ios::end);
    filesize = ifs_ps.tellg();
    ifs_ps.seekg(0, std::ios::beg);
    unsigned char* psbinary_pointer = new unsigned char[filesize];
    ifs_ps.read((char*)psbinary_pointer, filesize);
    ifs_ps.close();

    _device->CreatePixelShader(psbinary_pointer, filesize, nullptr, &_blurShader);
    delete[] psbinary_pointer;
  }
  {
    std::ifstream ifs_ps("bloomPS.cso", std::ios::binary);
    ifs_ps.seekg(0, std::ios::end);
    filesize = ifs_ps.tellg();
    ifs_ps.seekg(0, std::ios::beg);
    unsigned char* psbinary_pointer = new unsigned char[filesize];
    ifs_ps.read((char*)psbinary_pointer, filesize);
    ifs_ps.close();

    _device->CreatePixelShader(psbinary_pointer, filesize, nullptr, &_bloomShader);
    delete[] psbinary_pointer;
  }
  {
    std::ifstream ifs_ps("glitchPS.cso", std::ios::binary);
    ifs_ps.seekg(0, std::ios::end);
    filesize = ifs_ps.tellg();
    ifs_ps.seekg(0, std::ios::beg);
    unsigned char* psbinary_pointer = new unsigned char[filesize];
    ifs_ps.read((char*)psbinary_pointer, filesize);
    ifs_ps.close();

    _device->CreatePixelShader(psbinary_pointer, filesize, nullptr, &_glitchShader);
    delete[] psbinary_pointer;
  }
  {
    std::ifstream ifs_ps("noisePS.cso", std::ios::binary);
    ifs_ps.seekg(0, std::ios::end);
    filesize = ifs_ps.tellg();
    ifs_ps.seekg(0, std::ios::beg);
    unsigned char* psbinary_pointer = new unsigned char[filesize];
    ifs_ps.read((char*)psbinary_pointer, filesize);
    ifs_ps.close();

    _device->CreatePixelShader(psbinary_pointer, filesize, nullptr, &_noiseShader);
    delete[] psbinary_pointer;
  }
  {
    std::ifstream ifs_ps("sprialPS.cso", std::ios::binary);
    ifs_ps.seekg(0, std::ios::end);
    filesize = ifs_ps.tellg();
    ifs_ps.seekg(0, std::ios::beg);
    unsigned char* psbinary_pointer = new unsigned char[filesize];
    ifs_ps.read((char*)psbinary_pointer, filesize);
    ifs_ps.close();

    _device->CreatePixelShader(psbinary_pointer, filesize, nullptr, &_spiralShader);
    delete[] psbinary_pointer;
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
  SAFE_RELEASE(_blurShader);
  SAFE_RELEASE(_bloomShader);
  SAFE_RELEASE(_luminanceShader);
  SAFE_RELEASE(_glitchShader);
  SAFE_RELEASE(_noiseShader);
  SAFE_RELEASE(_spiralShader);
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

void Shader::setVS(VS vs) {
  switch (vs) {
  case VS::GENERAL:
    _deviceContext->VSSetShader(_vertexShader, NULL, 0);
    break;
  }
}

void Shader::setPS(PS ps) {
  switch (ps) {
    case PS::GENERAL:
      _deviceContext->PSSetShader(_pixelShader, nullptr, 0);
      break;
    case PS::NO_TEX:
      _deviceContext->PSSetShader(_noTexShader, nullptr, 0);
      break;
    case PS::LUMINANCE:
      _deviceContext->PSSetShader(_luminanceShader, nullptr, 0);
      break;
    case PS::BLUR:
      _deviceContext->PSSetShader(_blurShader, nullptr, 0);
      break;
    case PS::BLOOM:
      _deviceContext->PSSetShader(_bloomShader, nullptr, 0);
      break;
    case PS::GLITCH:
      _deviceContext->PSSetShader(_glitchShader, nullptr, 0);
      break;
    case PS::NOISE:
      _deviceContext->PSSetShader(_noiseShader, nullptr, 0);
      break;
    case PS::SPIRAL:
      _deviceContext->PSSetShader(_spiralShader, nullptr, 0);
      break;
  }
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

void Shader::setWorld(XMMATRIX& world) {
  _worldMatrix = world;
}

void Shader::setView(Float3 e, Float3 f) {
  XMVECTOR eye = XMVectorSet(e.x, e.y, e.z, 0.0f);
  XMVECTOR focus = XMVectorSet(f.x, f.y, f.z, 0.0f);
  XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  _viewMatrix = XMMatrixLookAtLH(eye, focus, up);
}

void Shader::set2DMatrix() {
  Transpose mat;
  mat.world = XMMatrixIdentity();
  mat.view = XMMatrixIdentity();
  mat.projection = getOrthoMatrix();
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

XMMATRIX Shader::getInverseView() {
  XMMATRIX inverseView;
  inverseView = XMMatrixInverse(nullptr, _viewMatrix);
  inverseView.r[3].m128_f32[0] = 0.0f;
  inverseView.r[3].m128_f32[1] = 0.0f;
  inverseView.r[3].m128_f32[2] = 0.0f;
  return inverseView;
}