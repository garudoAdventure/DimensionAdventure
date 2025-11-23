#pragma once

#include "Field.h"
#include "IGameEventHandler.h"

class FieldManager {
	public:
		FieldManager() = default;
		Field* getField(int id, IGameEventHandler* gameEvent) {
			switch (id) {
				case 0:
					return new Field00(gameEvent);
				case 1:
					return new Field01(gameEvent);
				case 2:
					return new Field02(gameEvent);
				case 3:
					return new Field03(gameEvent);
				case 4:
					return new Field04(gameEvent);
				case 5:
					return new Field05(gameEvent);
			}
		}
};