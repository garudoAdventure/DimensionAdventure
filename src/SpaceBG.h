#pragma once

#include "Trail.h"
#include "PostProcess.h"
#include "./Render/Sprite.h"
#include "./Utils/MathTool.h"
#include <cmath>

struct Beam {
	Float3 initPos;
	Float3 pos;
	Float3 vel;
	int time = 0;
};

class SpaceBG {
	public:
		static constexpr int BEAM_NUM = 500;

		SpaceBG() {
			offscreenTex = new RenderTexture(1280.0f, 720.0f, { 0.0f, 0.0f, 0.0f, 1.0f });
			postProcess = new PostProcess(offscreenTex);
			for (int i = 0; i < BEAM_NUM; i++) {
				int seed = rand();
				beam[i].initPos = { cosf(seed), sinf(seed), (-(seed % 20)) - 10.0f };
				beam[i].pos = beam[i].initPos;
				beam[i].vel = { 0.0f, 0.0f, -0.1f };
				beam[i].time = seed % 300;
				trail[i] = new Trail(beam[i].initPos);
			}
		}
		void update() {
			for (int i = 0; i < BEAM_NUM; i++) {
				beam[i].pos += beam[i].vel;
				trail[i]->setPos(beam[i].pos);
				trail[i]->update();
				
				beam[i].time++;
				if (beam[i].time == 300) {
					beam[i].time = 0;
					beam[i].pos = beam[i].initPos;
					trail[i]->resetPos(beam[i].initPos);
				}
			}
			offscreenTex->setTargetView();
			offscreenTex->clear();
			for (int i = 0; i < BEAM_NUM; i++) {
				trail[i]->draw();
			}
			postProcess->update(false);
		}
		void draw() {
			DX3D.setTargetView();
			postProcess->drawBloom();
		}

	private:
		Beam beam[BEAM_NUM];
		Trail* trail[BEAM_NUM];
		PostProcess* postProcess;
		RenderTexture* offscreenTex;
};