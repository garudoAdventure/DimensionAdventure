#pragma once

#include "MathStruct.h"

class MathTool {
	public:
		static bool checkCollision(Cube c1, Cube c2, bool is2DMode) {
			bool xCollide =
				c1.pos.x + c1.size.x / 2 > c2.pos.x - c2.size.x / 2 &&
				c2.pos.x + c2.size.x / 2 > c1.pos.x - c1.size.x / 2;
			bool yCollide =
				c1.pos.y + c1.size.y / 2 > c2.pos.y - c2.size.y / 2 &&
				c2.pos.y + c2.size.y / 2 > c1.pos.y - c1.size.y / 2;
			bool zCollide =
				c1.pos.z + c1.size.z / 2 > c2.pos.z - c2.size.z / 2 &&
				c2.pos.z + c2.size.z / 2 > c1.pos.z - c1.size.z / 2;

			return xCollide && yCollide && (is2DMode || zCollide);
		}

		template<class T>
		static T lerp(T start, T end, float step) {
			return start - (start - end) * step;
		}

		template<class T>
		static T easeInQuad(T start, T end, float step) {
			return (end - start) * step * step + start;
		}

		template<class T>
		static T easeOutQuad(T start, T end, float step) {
			return (end - start) * (1 - (1 - step) * (1 - step)) + start;
		}

		static Float3 getCoordPos(Float3 coord) {
			return {
				-22.5f + 3.0f * coord.x,
				-12.0f + 3.0f * coord.y,
				-15.0f + 3.0f * coord.z
			};
		}

		static float clamp(float num, float min, float max) {
			if (num < min) {
				return min;
			}
			if (num > max) {
				return max;
			}
			return num;
		}
};