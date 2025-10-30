#include "DirectX.h"
#include "CubeRenderer.h"
#include "Shader.h"
#include "Texture.h"

CubeRenderer::CubeRenderer() {
  D3D11_BUFFER_DESC vbDesc = {};
  vbDesc.Usage = D3D11_USAGE_DEFAULT;
  vbDesc.ByteWidth = sizeof(Vertex) * 24;
  vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vbDesc.CPUAccessFlags = 0;
  DX3D.getDevice()->CreateBuffer(&vbDesc, NULL, &_vertexBuffer);

  D3D11_BUFFER_DESC ibDesc = {};
  ibDesc.Usage = D3D11_USAGE_DEFAULT;
  ibDesc.ByteWidth = sizeof(WORD) * 36;
  ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  ibDesc.CPUAccessFlags = 0;

  for (int i = 0; i < 6; i++) {
    _indexData[0 + i * 6] = 4 * i;
    _indexData[1 + i * 6] = 4 * i + 1;
    _indexData[2 + i * 6] = 4 * i + 2;
    _indexData[3 + i * 6] = 4 * i + 1;
    _indexData[4 + i * 6] = 4 * i + 3;
    _indexData[5 + i * 6] = 4 * i + 2;
  }

  D3D11_SUBRESOURCE_DATA ibData;
  ibData.pSysMem = _indexData;
  ibData.SysMemPitch = 0;
  ibData.SysMemSlicePitch = 0;

  DX3D.getDevice()->CreateBuffer(&ibDesc, &ibData, &_indexBuffer);
}

CubeRenderer::~CubeRenderer() {
  SAFE_RELEASE(_vertexBuffer);
}

void CubeRenderer::drawCube(Cube cube, Float4 color, int texID) {
  const float cubePosX = cube.pos.x;
  const float cubePosY = cube.pos.y;
  const float cubePosZ = cube.pos.z;
  _vertexData[0].postion = { cubePosX - cube.size.x / 2, cubePosY + cube.size.y / 2, cubePosZ + cube.size.z / 2 };
  _vertexData[1].postion = { cubePosX + cube.size.x / 2, cubePosY + cube.size.y / 2, cubePosZ + cube.size.z / 2 };
  _vertexData[2].postion = { cubePosX - cube.size.x / 2, cubePosY + cube.size.y / 2, cubePosZ - cube.size.z / 2 };
  _vertexData[3].postion = { cubePosX + cube.size.x / 2, cubePosY + cube.size.y / 2, cubePosZ - cube.size.z / 2 };
  
  _vertexData[4].postion = { cubePosX - cube.size.x / 2, cubePosY + cube.size.y / 2, cubePosZ - cube.size.z / 2 };
  _vertexData[5].postion = { cubePosX + cube.size.x / 2, cubePosY + cube.size.y / 2, cubePosZ - cube.size.z / 2 };
  _vertexData[6].postion = { cubePosX - cube.size.x / 2, cubePosY - cube.size.y / 2, cubePosZ - cube.size.z / 2 };
  _vertexData[7].postion = { cubePosX + cube.size.x / 2, cubePosY - cube.size.y / 2, cubePosZ - cube.size.z / 2 };

  _vertexData[8].postion = { cubePosX + cube.size.x / 2, cubePosY + cube.size.y / 2, cubePosZ - cube.size.z / 2 };
  _vertexData[9].postion = { cubePosX + cube.size.x / 2, cubePosY + cube.size.y / 2, cubePosZ + cube.size.z / 2 };
  _vertexData[10].postion = { cubePosX + cube.size.x / 2, cubePosY - cube.size.y / 2, cubePosZ - cube.size.z / 2 };
  _vertexData[11].postion = { cubePosX + cube.size.x / 2, cubePosY - cube.size.y / 2, cubePosZ + cube.size.z / 2 };

  _vertexData[12].postion = { cubePosX + cube.size.x / 2, cubePosY + cube.size.y / 2, cubePosZ + cube.size.z / 2 };;
  _vertexData[13].postion = { cubePosX - cube.size.x / 2, cubePosY + cube.size.y / 2, cubePosZ + cube.size.z / 2 };;
  _vertexData[14].postion = { cubePosX + cube.size.x / 2, cubePosY - cube.size.y / 2, cubePosZ + cube.size.z / 2 };;
  _vertexData[15].postion = { cubePosX - cube.size.x / 2, cubePosY - cube.size.y / 2, cubePosZ + cube.size.z / 2 };;

  _vertexData[16].postion = { cubePosX - cube.size.x / 2, cubePosY + cube.size.y / 2, cubePosZ + cube.size.z / 2 };
  _vertexData[17].postion = { cubePosX - cube.size.x / 2, cubePosY + cube.size.y / 2, cubePosZ - cube.size.z / 2 };
  _vertexData[18].postion = { cubePosX - cube.size.x / 2, cubePosY - cube.size.y / 2, cubePosZ + cube.size.z / 2 };
  _vertexData[19].postion = { cubePosX - cube.size.x / 2, cubePosY - cube.size.y / 2, cubePosZ - cube.size.z / 2 };

  _vertexData[20].postion = { cubePosX - cube.size.x / 2, cubePosY - cube.size.y / 2, cubePosZ - cube.size.z / 2 };
  _vertexData[21].postion = { cubePosX + cube.size.x / 2, cubePosY - cube.size.y / 2, cubePosZ - cube.size.z / 2 };
  _vertexData[22].postion = { cubePosX - cube.size.x / 2, cubePosY - cube.size.y / 2, cubePosZ + cube.size.z / 2 };
  _vertexData[23].postion = { cubePosX + cube.size.x / 2, cubePosY - cube.size.y / 2, cubePosZ + cube.size.z / 2 };

  for (int i = 0; i < 4; i++) {
	  _vertexData[i].normal = { 0.0f, 1.0f, 0.0f };
  }
  for (int i = 4; i < 8; i++) {
	  _vertexData[i].normal = { 0.0f, 0.0f, -1.0f };
  }
  for (int i = 8; i < 12; i++) {
	  _vertexData[i].normal = { 1.0f, 0.0f, 0.0f };
  }
  for (int i = 12; i < 16; i++) {
	  _vertexData[i].normal = { 0.0f, 0.0f, 1.0f };
  }
  for (int i = 16; i < 20; i++) {
	  _vertexData[i].normal = { -1.0f, 0.0f, 0.0f };
  }
  for (int i = 20; i < 24; i++) {
	  _vertexData[i].normal = { 0.0f, -1.0f, 0.0f };
  }

  for (int i = 0; i < 24; i++) {
    _vertexData[i].color = XMFLOAT4(color.r, color.g, color.b, color.a);
  }

  for (int i = 0; i < 6; i++) {
    _vertexData[0 + i * 4].texCoord = { 0.0f, 0.0f };
    _vertexData[1 + i * 4].texCoord = { cube.size.x / cube.size.y, 0.0f };
    _vertexData[2 + i * 4].texCoord = { 0.0f, 1.0f };
    _vertexData[3 + i * 4].texCoord = { cube.size.x / cube.size.y, 1.0f };
  }
  _vertexData[0].texCoord = { 0.0f, 0.0f };
  _vertexData[1].texCoord = { 1.0f, 0.0f };
  _vertexData[2].texCoord = { 0.0f, 0.1f };
  _vertexData[3].texCoord = { 1.0f, 0.1f };

  DX3D.getDeviceContext()->UpdateSubresource(_vertexBuffer, 0, NULL, &_vertexData[0], 0, 0);

  DX3D.setDepthEnable(true);
  SHADER.begin();
  SHADER.setMatrix();
  TEXTURE.setTexture(texID);

  UINT stride = sizeof(Vertex);
  UINT offset = 0;
  DX3D.getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
  DX3D.getDeviceContext()->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
  DX3D.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  DX3D.getDeviceContext()->DrawIndexed(36, 0, 0);
}