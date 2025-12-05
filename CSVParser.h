#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "MathStruct.h"

class CSVParser {
	public:
		CSVParser() = default;
		std::vector<BlockInfo> loadCSV(const std::string& path) {
			std::vector<BlockInfo> array;
			std::ifstream file(path);
			std::string line;
			while (std::getline(file, line)) {
				std::istringstream iStream(line);
				std::string cell;
				std::vector<std::string> row;
				for (int i = 0; i < 5; i++) {
					std::getline(iStream, cell, ',');
					row.emplace_back(cell);
				}
				std::getline(iStream, cell);
				row.emplace_back(cell);

				int x = parseToInt(row[0]);
				int y = parseToInt(row[1]);
				int z = parseToInt(row[2]);
				int w = parseToInt(row[3]);
				int val = parseToInt(row[4]);
				Float3 size = parseToFloat3(row[5]);
				
				BlockInfo info;
				info.pos = { x, y, z, w };
				info.type = val;
				info.scale = size;
				array.emplace_back(info);
			}
			return array;
		}

	private:
		int parseToInt(std::string s) {
			std::stringstream ss(s);
			int num;
			ss >> num;
			return num;
		}

		Float3 parseToFloat3(std::string s) {
			Float3 vector;
			std::string coordStr = s.substr(2, s.length() - 4);
			std::istringstream iStream(coordStr);
			std::string coord;
			std::getline(iStream, coord, ',');
			vector.x = parseToInt(coord);

			std::getline(iStream, coord, ',');
			vector.y = parseToInt(coord);

			std::getline(iStream, coord, ',');
			vector.z = parseToInt(coord);

			return vector;
		}
};