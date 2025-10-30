#include "GameMain.h"
#include "GetItemEvent.h"
#include "FieldFadeInEvent.h"
#include "FieldFadeOutEvent.h"
#include "TransformDimensionEvent.h"
#include "TransformLayerEvent.h"

GameMain::GameMain() {
  camera = new Camera();
  player = new Player(this);
  currentField = newField = fieldManager.getField(0, this);
}

GameMain::~GameMain() {
  delete player;
  delete currentField;
  delete camera;
}

void GameMain::update() {  
  if (gameEventQueue.size() != 0) {
    gameEventQueue[0]->update();
    return;
  }

  changeField();

  currentField->update(player->getCurrentLayer());
  player->update();

  currentField->collide(player, camera->is2D());

  camera->moveCamera(player);

  layerScreen.update(camera, currentField, player);

  statusUI.update(player);
}

void GameMain::draw() {
  camera->draw();

  // 3D
  currentField->draw(player->getCurrentLayer());
  player->draw();

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
}

void GameMain::changeField() {
  if (currentField != newField) {
    delete currentField;
    currentField = newField;
    player->setPos(_playerInitPos);
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
  Field* field = fieldManager.getField(fieldID, this);
  newField = field;
  _playerInitPos = playerInitPos;
  if (!player->is2D()) {
    player->convertDimension();
  }
  gameEventQueue.emplace_back(new FieldFadeOutEvent({
    doorPos.x - camera->get2DPos().x,
    doorPos.y - camera->get2DPos().y
  }));
}

void GameMain::transformDimension() {
  gameEventQueue.emplace_back(new TransformDimensionEvent(camera, player));
}

void GameMain::transformLayer() {
  gameEventQueue.emplace_back(new TransformLayerEvent(&layerScreen, player));
}
