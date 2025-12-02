#include "GameMain.h"
#include "Player.h"
#include "GetItemEvent.h"
#include "FieldFadeInEvent.h"
#include "FieldFadeOutEvent.h"
#include "TransformDimensionEvent.h"
#include "TransformLayerEvent.h"

GameMain::GameMain() {
  Player::CreateInstance(this);
  camera = new Camera();
  fieldManager = new FieldManager(this);
  currentField = newField = fieldManager->getField(0);

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

  changeField();

  currentField->update(PLAYER.getCurrentLayer());
  PLAYER.update();

  currentField->collisionCheck(camera->is2D());

  camera->moveCamera();

  statusUI.update();
  itemList.update();
}

void GameMain::draw() {
  offscreenTex->setTargetView();
  offscreenTex->clear();
  camera->draw();
  currentField->draw(PLAYER.getCurrentLayer());
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

void GameMain::changeField() {
  if (currentField != newField) {
    currentField = newField;
    PLAYER.setPos(_playerInitPos);
    camera->set2DPos({ _playerInitPos.x, _playerInitPos.y });
    gameEventQueue.emplace_back(new FieldFadeInEvent({
      _playerInitPos.x - camera->get2DPos().x,
      _playerInitPos.y - camera->get2DPos().y
    }));
  }
}

void GameMain::addEvent(IGameEvent* gameEvent) {
  gameEventQueue.emplace_back(gameEvent);
}

void GameMain::setNewField(int fieldID, Float3 doorPos, Float3 playerInitPos) {
  Field* field = fieldManager->getField(fieldID);
  newField = field;
  _playerInitPos = playerInitPos;
  if (!PLAYER.is2D()) {
    PLAYER.convertDimension();
  }
  gameEventQueue.emplace_back(new FieldFadeOutEvent({
    doorPos.x - camera->get2DPos().x,
    doorPos.y - camera->get2DPos().y
  }));
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

void GameMain::setFourGodCorrect(int idx, bool correct) {
  fourGodCorrect[idx] = correct;
}