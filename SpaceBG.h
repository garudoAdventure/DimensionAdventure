#pragma once

#include "Sprite.h"
#include "Trail.h"
#include "MathTool.h"
#include "PostProcess.h"
#include <time.h>
#include <cmath>

#define TRAIL_NUM	500

struct Beam {
	Float3 initPos;
	Float3 pos;
	Float3 vel;
	int time = 0;
};

class SpaceBG {
	public:
		SpaceBG() {
			offscreenTex = new RenderTexture(1280.0f, 720.0f, { 0.0f, 0.0f, 0.0f, 1.0f });
			postProcess = new PostProcess(offscreenTex);
			for (int i = 0; i < TRAIL_NUM; i++) {
				int seed = rand();
				beam[i].initPos = {cosf(seed), sinf(seed), (-(seed % 20)) - 10.0f };
				beam[i].pos = beam[i].initPos;
				beam[i].vel = { 0.0f, 0.0f, -0.1f };
				beam[i].time = seed % 300;
				trail[i] = new Trail(beam[i].initPos);
			}
		}
		void update() {
			for (int i = 0; i < TRAIL_NUM; i++) {
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
			for (int i = 0; i < TRAIL_NUM; i++) {
				trail[i]->draw();
			}
			postProcess->update();
		}
		void draw() {
			DX3D.setTargetView();
			postProcess->draw();
		}

	private:
		Beam beam[TRAIL_NUM];
		Trail* trail[TRAIL_NUM];
		PostProcess* postProcess;
		RenderTexture* offscreenTex;


	private:
		const int permutation[512] = {
			151,160,137,91,90,15,
			131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
			190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
			88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
			77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
			102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
			135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
			5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
			223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
			129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
			251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
			49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
			138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,

			151,160,137,91,90,15,
			131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
			190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
			88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
			77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
			102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
			135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
			5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
			223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
			129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
			251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
			49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
			138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
		};
		Float2 grad(Float2 pos) {
			int p = permutation[((int)pos.x & 255) + permutation[(int)pos.y & 255]];
			return { cosf(p / 256.0f * 2 * PI), sinf(p / 256.0f * 2 * PI) };
		}
		float interpolate(float x, float y, float t) {
			return (y - x) * (3 - t * 2) * t * t + x;
		}
		float perlinNoise(Float2 pos) {
			Float2 frac = { pos.x - floor(pos.x), pos.y - floor(pos.y) };

			Float2 p00 = { floor(pos.x),		 floor(pos.y) };
			Float2 p01 = { floor(pos.x),		 floor(pos.y) + 1 };
			Float2 p10 = { floor(pos.x) + 1, floor(pos.y) };
			Float2 p11 = { floor(pos.x) + 1, floor(pos.y) + 1 };
			
			Float2 g00 = grad(p00);
			Float2 g01 = grad(p01);
			Float2 g10 = grad(p10);
			Float2 g11 = grad(p11);
			
			float val00 = dot(g00, pos - p00);
			float val01 = dot(g01, pos - p01);
			float val10 = dot(g10, pos - p10);
			float val11 = dot(g11, pos - p11);

			float intp0 = interpolate(val00, val01, frac.y);
			float intp1 = interpolate(val10, val11, frac.y);
			float intp = interpolate(intp0, intp1, frac.x);

			return intp;
		}
		float dot(Float2 vec1, Float2 vec2) {
			return vec1.x * vec2.x + vec1.y + vec2.y;
		}
};