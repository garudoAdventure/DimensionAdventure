#pragma once

#include "Field.h"
#include "IGameEventHandler.h"

class FieldManager {
	public:
		FieldManager(IGameEventHandler* gameEvent) {
			_fields.reserve(15);
			_fields.emplace_back(new Field00(gameEvent));
			_fields.emplace_back(new Field01(gameEvent));
			_fields.emplace_back(new Field02(gameEvent));
			_fields.emplace_back(new Field03(gameEvent));
			_fields.emplace_back(new Field04(gameEvent));
			_fields.emplace_back(new Field05(gameEvent));
			_fields.emplace_back(new Field06(gameEvent));
		}
		~FieldManager() {
			for (Field* field : _fields) {
				delete field;
			}
		}
		Field* getField(int id) {
			return _fields[id];
		}

	private:
		std::vector<Field*> _fields;
};