#include "GameMain.h"
#include "GameEnd.h"
#include "Sound.h"
#include "./Player/Player.h"
#include "./Player/PlayerIdle.h"
#include "./Player/PlayerFreeze.h"
#include "./GameEvent/StartEvent.h"
#include "./GameEvent/ChangeFieldEvent.h"
#include "./GameEvent/TransformDimensionEvent.h"
#include "./GameEvent/TransformLayerEvent.h"

GameMain::GameMain() {
  Player::CreateInstance(this);
  camera = new Camera();
  fieldManager = new FieldManager(this);
  currentField = fieldManager->getField(0);
  PLAYER.setPos(MathTool::getCoordPos({ 1.0f, 1.1f, 5.0f }));

  layerScreen = new LayerScreen(this);
  offscreenTex = new RenderTexture(1280, 720);
  postProcess = new PostProcess(offscreenTex);
  mazeBg = new MazeBg();

  gameEventQueue.reserve(5);
  addEvent(new StartEvent(this));

  bgm = SOUND.loadSound("./assets/sound/bgm.wav");
}

GameMain::~GameMain() {
  delete mazeBg;
  delete postProcess;
  delete offscreenTex;
  delete layerScreen;
  delete fieldManager;
  delete camera;
  Player::DeleteInstance();
}

void GameMain::update() {
  if (gameEventQueue.size() > 0) {
    gameEventQueue.at(0)->update();
    return;
  }

  if (!isTriggerEntryField) {
    isTriggerEntryField = true;
    currentField->onEntryField();
  }

  PLAYER.update();
  updateField();

  moveCamera(PLAYER.getPos());

  statusUI.update();
}

void GameMain::draw() {
  camera->draw();

  drawOffscreen(PLAYER.getCurrentLayer());

  DX3D.setTargetView();
  DX3D.clear();
  SHADER.begin();
  DX3D.setViewport(1280.0f, 720.0f);

  drawGameScene(PLAYER.getCurrentLayer());
  PLAYER.getSpirit()->drawHint();

  // Game Event
  if (gameEventQueue.size() > 0) {
    gameEventQueue.at(0)->draw();
    if (gameEventQueue.at(0)->isEnd()) {
      delete gameEventQueue.at(0);
      gameEventQueue.erase(gameEventQueue.begin());
      if (gameEventQueue.size() == 0) {
        PLAYER.setState(new PlayerIdle());
      }
    }
  }

  // UI
  statusUI.draw();
}

void GameMain::addEvent(IGameEvent* gameEvent) {
  PLAYER.setState(new PlayerFreeze());
  PLAYER.changeState();
  gameEventQueue.emplace_back(gameEvent);
}

void GameMain::setNewField(int fieldID, Float3 doorPos, Float3 playerInitPos) {
  Field* field = fieldManager->getField(fieldID);
  if (!PLAYER.is2D()) {
    PLAYER.convertDimension();
  }
  PLAYER.setState(new PlayerFreeze());
  PLAYER.changeState();
  addEvent(new ChangeFieldEvent(camera, {
    doorPos.x - camera->get2DPos().x,
    doorPos.y - camera->get2DPos().y
    }, playerInitPos, [=]() {
      currentField = field;
      isTriggerEntryField = false;
    }
  ));
}

void GameMain::transformDimension() {
  addEvent(new TransformDimensionEvent(camera));
}

void GameMain::transformLayer() {
  layerScreen->drawScreen(camera, currentField);
  SOUND.setVolume(bgm, 0.2f);
  addEvent(new TransformLayerEvent(layerScreen));
}

void GameMain::moveCamera(Float3 targetPos) {
  camera->moveCamera(targetPos);
}

Float3& GameMain::getCameraPos() {
  return camera->getPos();
}

void GameMain::cameraVibration(bool isSet) {
  camera->vibration(isSet);
}

void GameMain::updateField() {
  mazeBg->update();
  currentField->update(PLAYER.getCurrentLayer());
  currentField->collisionCheck(camera->is2D());
}

RenderTexture* GameMain::getScreenTex() {
  return offscreenTex;
}

void GameMain::setGameEnd() {
  game->setState(new GameEnd());
}

void GameMain::setCheckpoint(CheckPoint checkPoint) {
  currentCheckpoint = checkPoint;
}

CheckPoint& GameMain::getCheckpoint() {
  return currentCheckpoint;
}

void GameMain::setSavePointPos(Float3& pos) {
  savePointPos = pos;
}

Float3& GameMain::getSavePointPos() {
  return savePointPos;
}

int GameMain::getBgmId() {
  return bgm;
}

void GameMain::drawGameScene(int layerIdx) {
  mazeBg->draw(Color::layerColor[layerIdx]);
  currentField->draw(layerIdx);
  postProcess->drawBloom(3);

  currentField->drawBillboard(layerIdx);
  PLAYER.draw();
}

void GameMain::drawOffscreen(int layerIdx) {
  offscreenTex->setTargetView();
  offscreenTex->clear();
  currentField->draw(layerIdx);
  PLAYER.getSpirit()->draw();
  postProcess->update(true);
}