#pragma once

class IGameEvent {
	public:
		virtual ~IGameEvent() = default;
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual bool isEnd() = 0;
};