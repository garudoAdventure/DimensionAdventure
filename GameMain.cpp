#include "GameMain.h"
#include "Player.h"
#include "GetItemEvent.h"
#include "ChangeFieldEvent.h"
#include "TransformDimensionEvent.h"
#include "TransformLayerEvent.h"

GameMain::GameMain() {
  Player::CreateInstance(this);
  camera = new Camera();
  fieldManager = new FieldManager(this);
  currentField = fieldManager->getField(54);

  offscreenTex = new RenderTexture(1280, 720);
  postProcess = new PostProcess(offscreenTex);
}

GameMain::~GameMain() {
  delete fieldManager;
  delete camera;
  Player::DeleteInstance();
}

void GameMain::update() {  
  if (gameEventQueue.size() != 0) {
    gameEventQueue[0]->update();
    return;
  }

  updatePlayerAct();

  camera->moveCamera();

  statusUI.update();
  itemList.update();
}

void GameMain::draw() {
  offscreenTex->setTargetView();
  offscreenTex->clear();
  DX3D.setBlendMode(BlendMode::REND_TEX);
  camera->draw();
  currentField->draw(PLAYER.getCurrentLayer());
  PLAYER.getSpirit()->draw();
  postProcess->update();

  DX3D.setTargetView();
  DX3D.clear();
  SHADER.begin();
  DX3D.setViewport(1280.0f, 720.0f);

  postProcess->draw();
  currentField->draw(PLAYER.getCurrentLayer());
  PLAYER.draw();

  // Game Event
  if (gameEventQueue.size() != 0) {
    gameEventQueue[0]->draw();

    if (gameEventQueue[0]->isEnd()) {
      delete gameEventQueue[0];
      gameEventQueue.erase(gameEventQueue.begin());
    }
  }

  // UI
  statusUI.draw();
  // itemList.draw();
}

void GameMain::addEvent(IGameEvent* gameEvent) {
  gameEventQueue.emplace_back(gameEvent);
}

void GameMain::setNewField(int fieldID, Float3 doorPos, Float3 playerInitPos) {
  Field* field = fieldManager->getField(fieldID);
  if (!PLAYER.is2D()) {
    PLAYER.convertDimension();
  }
  gameEventQueue.emplace_back(new ChangeFieldEvent(camera, {
    doorPos.x - camera->get2DPos().x,
    doorPos.y - camera->get2DPos().y
    }, playerInitPos, [=]() {
      currentField = field;
    }
  ));
}

void GameMain::transformDimension() {
  gameEventQueue.emplace_back(new TransformDimensionEvent(camera));
}

void GameMain::transformLayer() {
  layerScreen.update(camera, currentField);
  gameEventQueue.emplace_back(new TransformLayerEvent(&layerScreen));
}

Float3& GameMain::getCameraPos() {
  return camera->getPos();
}

void GameMain::setCameraVibration(bool isSet) {
  camera->setVibration(isSet);
}

void GameMain::updatePlayerAct() {
  currentField->update(PLAYER.getCurrentLayer());
  PLAYER.update();
  currentField->collisionCheck(camera->is2D());
}