#pragma once

#include "Field.h"
#include "./Game/IGameEventHandler.h"
#include <unordered_map>

class FieldManager {
	public:
		FieldManager(IGameEventHandler* gameEvent) {
			_fields[0] = new Field00(gameEvent);
			_fields[1] = new Field01(gameEvent);
			_fields[2] = new Field02(gameEvent);
			_fields[3] = new Field03(gameEvent);
			_fields[4] = new Field04(gameEvent);
			_fields[5] = new Field05(gameEvent);
			_fields[51] = new Field51(gameEvent);
			_fields[52] = new Field52(gameEvent);
			_fields[53] = new Field53(gameEvent);
			_fields[54] = new Field54(gameEvent);
			_fields[6] = new Field06(gameEvent);
		}
		~FieldManager() {
			for (auto it = _fields.begin(); it != _fields.end(); it++) {
				delete it->second;
			}
			_fields.clear();
		}
		Field* getField(int id) {
			return _fields[id];
		}

	private:
		std::unordered_map<int, Field*> _fields;
};