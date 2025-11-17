#pragma once

#include "PlayerState.h"

class PlayerTeleport : public PlayerState {
	public:
		PlayerTeleport();
		void init() override;
		void update() override;
		void draw() override;

	private:
		Float3 _teleportPos = { 0.0f, 0.0f, 0.0f };
		bool _isTeleport = false;
		int _prepareTeleportCount = 0;
		int _teleportCount = 0;
		Float3 _teleportDistance = { 0.0f, 0.0f, 0.0f };
		const float _maxDistance = 8.0f;
		const float _minDistance = -8.0f;
};