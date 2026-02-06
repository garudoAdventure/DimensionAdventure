#pragma once

#include "Trail.h"
#include "./Render/Sprite.h"
#include "./PostProcess/Bloom.h"
#include "./Common/Color.h"
#include "./Utils/MathTool.h"
#include <cmath>

struct Beam {
	Float3 initPos;
	Float3 pos;
	Float3 vel;
	int time = 0;
};

class SwitchLayerBg {
	public:
		static constexpr int BEAM_NUM = 500;
		static constexpr int LOOP_TIME = 300;

		SwitchLayerBg() {
			offscreenTex = new RenderTexture(1280.0f, 720.0f, Color::black);
			bloomBeam = new Bloom(offscreenTex);
			for (int i = 0; i < BEAM_NUM; i++) {
				int r = rand();
				beam[i].initPos = { cosf(r), sinf(r), (-(r % 20)) - 10.0f };
				beam[i].pos = beam[i].initPos;
				beam[i].vel = { 0.0f, 0.0f, -0.1f };
				beam[i].time = r % LOOP_TIME;
				trail[i] = new Trail(beam[i].initPos);
			}
		}

		void update() {
			for (int i = 0; i < BEAM_NUM; i++) {
				beam[i].pos += beam[i].vel;
				trail[i]->setPos(beam[i].pos);
				trail[i]->update();
				
				beam[i].time++;
				if (beam[i].time == LOOP_TIME) {
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
			bloomBeam->setClipLuminance(false);
			bloomBeam->update();
		}

		void draw() {
			DX3D.setTargetView();
			bloomBeam->drawBloom();
		}

	private:
		Beam beam[BEAM_NUM];
		Trail* trail[BEAM_NUM];
		Bloom* bloomBeam;
		RenderTexture* offscreenTex;
};