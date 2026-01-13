#include "DirectX.h"
#include "Sprite.h"
#include "Shader.h"
#include "Texture.h"

Sprite::Sprite() {
  D3D11_BUFFER_DESC vbDesc = {};
  vbDesc.Usage = D3D11_USAGE_DEFAULT;
  vbDesc.ByteWidth = sizeof(Vertex) * 4;
  vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vbDesc.CPUAccessFlags = 0;
  DX3D.getDevice()->CreateBuffer(&vbDesc, NULL, &_vertexBuffer);
}

Sprite::~Sprite() {
  SAFE_RELEASE(_vertexBuffer);
}

void Sprite::drawSprite2D(Float2 pos, Float2 size, Float4 color) {
  Vertex vertexData[4];

  const Float2 repos = {
    pos.x / 1280.0f * 16.0f * 3.0f,
    pos.y / 720.0f * 9.0f * 3.0f,
  };

  const Float2 resize = {
    size.x / 1280.0f * 16.0f * 3.0f,
    size.y / 720.0f * 9.0f * 3.0f,
  };
  
  vertexData[0].postion = { repos.x - resize.x / 2, repos.y + resize.y / 2, 0.1f };
  vertexData[1].postion = { repos.x + resize.x / 2, repos.y + resize.y / 2, 0.1f };
  vertexData[2].postion = { repos.x - resize.x / 2, repos.y - resize.y / 2, 0.1f };
  vertexData[3].postion = { repos.x + resize.x / 2, repos.y - resize.y / 2, 0.1f };
  
  vertexData[0].texCoord = { 0.0f, 0.0f };
  vertexData[1].texCoord = { 1.0f, 0.0f };
  vertexData[2].texCoord = { 0.0f, 1.0f };
  vertexData[3].texCoord = { 1.0f, 1.0f };

  for (int i = 0; i < 4; i++) {
    vertexData[i].color = { color.r, color.g, color.b, color.a };
  }
  for (int i = 0; i < 4; i++) {
	  vertexData[i].normal = { 0.0f, 0.0f, -1.0f };
  }

  DX3D.getDeviceContext()->UpdateSubresource(_vertexBuffer, 0, NULL, &vertexData[0], 0, 0);

  DX3D.setDepthEnable(false);
  SHADER.set2DMatrix();

  draw();
}

void Sprite::drawSprite2D(Float2 pos, Float2 size, ID3D11ShaderResourceView* tex, Float4 color, float width, float height) {
  Vertex vertexData[4];

  const Float2 repos = {
    pos.x / width * 16.0f * 3.0f,
    pos.y / height * 9.0f * 3.0f,
  };

  const Float2 resize = {
    size.x / width * 16.0f * 3.0f,
    size.y / height * 9.0f * 3.0f,
  };

  vertexData[0].postion = { repos.x - resize.x / 2, repos.y + resize.y / 2, 0.1f };
  vertexData[1].postion = { repos.x + resize.x / 2, repos.y + resize.y / 2, 0.1f };
  vertexData[2].postion = { repos.x - resize.x / 2, repos.y - resize.y / 2, 0.1f };
  vertexData[3].postion = { repos.x + resize.x / 2, repos.y - resize.y / 2, 0.1f };

  vertexData[0].texCoord = { 0.0f, 0.0f };
  vertexData[1].texCoord = { 1.0f, 0.0f };
  vertexData[2].texCoord = { 0.0f, 1.0f };
  vertexData[3].texCoord = { 1.0f, 1.0f };

  for (int i = 0; i < 4; i++) {
    vertexData[i].color = { color.r, color.g, color.b, color.a };
  }
  for (int i = 0; i < 4; i++) {
    vertexData[i].normal = { 0.0f, 0.0f, -1.0f };
  }

  DX3D.getDeviceContext()->UpdateSubresource(_vertexBuffer, 0, NULL, &vertexData[0], 0, 0);

  DX3D.setDepthEnable(false);
  SHADER.set2DMatrix();
  TEXTURE.setTexture(tex);

  draw();
}

void Sprite::drawSprite2DUV(Float2 pos, Float2 size, ID3D11ShaderResourceView* tex, Float4 color, Float2 uv, Float2 uvSize) {
  Vertex vertexData[4];

  const Float2 repos = {
    pos.x / 1280.0f * 16.0f * 3.0f,
    pos.y / 720.0f * 9.0f * 3.0f,
  };

  const Float2 resize = {
    size.x / 1280.0f * 16.0f * 3.0f,
    size.y / 720.0f * 9.0f * 3.0f,
  };

  vertexData[0].postion = { repos.x - resize.x / 2, repos.y + resize.y / 2, 0.1f };
  vertexData[1].postion = { repos.x + resize.x / 2, repos.y + resize.y / 2, 0.1f };
  vertexData[2].postion = { repos.x - resize.x / 2, repos.y - resize.y / 2, 0.1f };
  vertexData[3].postion = { repos.x + resize.x / 2, repos.y - resize.y / 2, 0.1f };

  vertexData[0].texCoord = { uv.x,            uv.y };
  vertexData[1].texCoord = { uv.x + uvSize.x, uv.y };
  vertexData[2].texCoord = { uv.x,            uv.y + uvSize.y };
  vertexData[3].texCoord = { uv.x + uvSize.x, uv.y + uvSize.y };

  for (int i = 0; i < 4; i++) {
    vertexData[i].color = { color.r, color.g, color.b, color.a };
  }
  for (int i = 0; i < 4; i++) {
    vertexData[i].normal = { 0.0f, 0.0f, 1.0f };
  }

  DX3D.getDeviceContext()->UpdateSubresource(_vertexBuffer, 0, NULL, &vertexData[0], 0, 0);

  DX3D.setDepthEnable(false);
  SHADER.set2DMatrix();
  TEXTURE.setTexture(tex);
  SHADER.setPS(PS::GENERAL);

  draw();
}

void Sprite::drawSprite2DRotate(Float2 pos, Float2 size, ID3D11ShaderResourceView* tex, float radian, Float2 center, Float4 color) {
  Vertex vertexData[4];

  const Float2 repos = {
    pos.x / 1280.0f * 16.0f * 3.0f,
    pos.y / 720.0f * 9.0f * 3.0f,
  };

  const Float2 resize = {
    size.x / 1280.0f * 16.0f * 3.0f,
    size.y / 720.0f * 9.0f * 3.0f,
  };

  vertexData[0].postion = { repos.x - resize.x / 2, repos.y + resize.y / 2, 0.1f };
  vertexData[1].postion = { repos.x + resize.x / 2, repos.y + resize.y / 2, 0.1f };
  vertexData[2].postion = { repos.x - resize.x / 2, repos.y - resize.y / 2, 0.1f };
  vertexData[3].postion = { repos.x + resize.x / 2, repos.y - resize.y / 2, 0.1f };

  for (int i = 0; i < 4; i++) {
    const Float2 resizeCenter = {
      center.x / 1280.0f * 16.0f * 3.0f,
      center.y / 720.0f * 9.0f * 3.0f,
    };
    XMFLOAT3 pos = vertexData[i].postion;
    float newX = (pos.x - resizeCenter.x) * cosf(radian) - (pos.y - resizeCenter.y) * sinf(radian);
    float newY = (pos.x - resizeCenter.x) * sinf(radian) + (pos.y - resizeCenter.y) * cosf(radian);
    vertexData[i].postion.x = newX + resizeCenter.x;
    vertexData[i].postion.y = newY + resizeCenter.y;
  }

  vertexData[0].texCoord = { 0.0f, 0.0f };
  vertexData[1].texCoord = { 1.0f, 0.0f };
  vertexData[2].texCoord = { 0.0f, 1.0f };
  vertexData[3].texCoord = { 1.0f, 1.0f };

  for (int i = 0; i < 4; i++) {
    vertexData[i].color = { color.r, color.g, color.b, color.a };
  }
  for (int i = 0; i < 4; i++) {
    vertexData[i].normal = { 0.0f, 0.0f, -1.0f };
  }

  DX3D.getDeviceContext()->UpdateSubresource(_vertexBuffer, 0, NULL, &vertexData[0], 0, 0);

  DX3D.setDepthEnable(false);
  SHADER.set2DMatrix();
  TEXTURE.setTexture(tex);
  SHADER.setPS(PS::GENERAL);

  draw();
}

void Sprite::drawSceneCover(Float2 pos, Float2 size, ID3D11ShaderResourceView* tex, float scale) {
  Vertex vertexData[4];

  const Float2 resize = {
    size.x / 1280.0f * 16.0f * 3.0f,
    size.y / 720.0f * 9.0f * 3.0f,
  };

  vertexData[0].postion = { -resize.x / 2,  resize.y / 2, 0.1f };
  vertexData[1].postion = { resize.x / 2,   resize.y / 2, 0.1f };
  vertexData[2].postion = { -resize.x / 2, -resize.y / 2, 0.1f };
  vertexData[3].postion = { resize.x / 2,  -resize.y / 2, 0.1f };

  const Float2 repos = {
    pos.x * 1280.0f / 16.0f / 3.0f,
    pos.y * 720.0f / 9.0f / 3.0f,
  };

  Float2 texCenter = {
    (repos.x + 640.0f) / 1280.0f,
    (640.0f - repos.y) / 1280.0f
  };

  float minX = 0.0f - std::max(0.0f, (texCenter.x - 0.5f) * 4);
  float maxX = 1.0f + std::max(0.0f, (0.5f - texCenter.x) * 4);
  float minY = 0.0f - std::max(0.0f, (texCenter.y - 0.5f) * 4);
  float maxY = 1.0f + std::max(0.0f, (0.5f - texCenter.y) * 4);
  
  float diffX = maxX - minX;
  float diffY = maxY - minY;
  if (diffX > diffY) {
    minY -= (diffX - diffY) * 0.5f;
    maxY += (diffX - diffY) * 0.5f;
  }
  if (diffX < diffY) {
    minX -= (diffY - diffX) * 0.5f;
    maxX += (diffY - diffX) * 0.5f;
  }
  
  vertexData[0].texCoord = { minX - (scale * texCenter.x),       minY - (scale * texCenter.y)      };
  vertexData[1].texCoord = { maxX + (scale * (1 - texCenter.x)), minY - (scale * texCenter.y)      };
  vertexData[2].texCoord = { minX - (scale * texCenter.x),       maxY + (scale * (1 - texCenter.y))};
  vertexData[3].texCoord = { maxX + (scale * (1 - texCenter.x)), maxY + (scale * (1 - texCenter.y))};

  for (int i = 0; i < 4; i++) {
    vertexData[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
  }
  for (int i = 0; i < 4; i++) {
    vertexData[i].normal = { 0.0f, 0.0f, -1.0f };
  }

  DX3D.getDeviceContext()->UpdateSubresource(_vertexBuffer, 0, NULL, &vertexData[0], 0, 0);

  TEXTURE.setTexture(tex);
  SHADER.setPS(PS::GENERAL);

  DX3D.setDepthEnable(false);
  SHADER.set2DMatrix();

  draw();
}

void Sprite::drawSprite3D(Float2 size, ID3D11ShaderResourceView* tex, Float4 color) {
  Vertex vertexData[4];

  const Float2 resize = {
    size.x / 1280.0f * 16.0f * 3.0f,
    size.y / 720.0f * 9.0f * 3.0f,
  };

  vertexData[0].postion = { -resize.x / 2,  resize.y / 2, 0.0f };
  vertexData[1].postion = {  resize.x / 2,  resize.y / 2, 0.0f };
  vertexData[2].postion = { -resize.x / 2, -resize.y / 2, 0.0f };
  vertexData[3].postion = {  resize.x / 2, -resize.y / 2, 0.0f };

  vertexData[0].texCoord = { 0.0f, 0.0f };
  vertexData[1].texCoord = { 1.0f, 0.0f };
  vertexData[2].texCoord = { 0.0f, 1.0f };
  vertexData[3].texCoord = { 1.0f, 1.0f };

  for (int i = 0; i < 4; i++) {
    vertexData[i].color = { color.r, color.g, color.b, color.a };
  }

  for (int i = 0; i < 4; i++) {
    vertexData[i].normal = { 0.0f, 0.0f, -1.0f };
  }

  DX3D.getDeviceContext()->UpdateSubresource(_vertexBuffer, 0, NULL, &vertexData[0], 0, 0);
  DX3D.setDepthEnable(true);
  TEXTURE.setTexture(tex);
  SHADER.setPS(PS::GENERAL);
  draw();
}

void Sprite::draw() {
  Light light;
  light.enable = false;
  SHADER.setLight(light);

  UINT stride = sizeof(Vertex);
  UINT offset = 0;
  DX3D.getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
  DX3D.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  DX3D.getDeviceContext()->Draw(4, 0);
}