#pragma once

#include "Trail.h"
#include "./Render/Sprite.h"
#include "./PostProcess/Bloom.h"
#include "./Common/Color.h"
#include "./Utils/MathTool.h"
#include <cmath>
#include <array>

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
				beam.at(i).initPos = { cosf(r), sinf(r), (-(r % 20)) - 10.0f };
				beam.at(i).pos = beam[i].initPos;
				beam.at(i).vel = { 0.0f, 0.0f, -0.1f };
				beam.at(i).time = r % LOOP_TIME;
				trail.at(i) = new Trail(beam.at(i).initPos);
			}
		}

		void update() {
			for (int i = 0; i < BEAM_NUM; i++) {
				beam.at(i).pos += beam[i].vel;
				trail.at(i)->setPos(beam[i].pos);
				trail.at(i)->update();
				
				beam.at(i).time++;
				if (beam.at(i).time == LOOP_TIME) {
					beam.at(i).time = 0;
					beam.at(i).pos = beam.at(i).initPos;
					trail.at(i)->resetPos(beam.at(i).initPos);
				}
			}
			offscreenTex->setTargetView();
			offscreenTex->clear();
			for (int i = 0; i < BEAM_NUM; i++) {
				trail.at(i)->draw();
			}
			bloomBeam->setClipLuminance(false);
			bloomBeam->update();
		}

		void draw() {
			DX3D.setTargetView();
			bloomBeam->drawBloom();
		}

	private:
		std::array<Beam, BEAM_NUM> beam;
		std::array<Trail*, BEAM_NUM> trail;
		Bloom* bloomBeam;
		RenderTexture* offscreenTex;
};