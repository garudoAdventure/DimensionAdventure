#pragma once

#include "Model.h"

class ModelManager {
  public:
		Model* loadModel(const std::string path) {
			if (modelMap.find(path) != modelMap.end()) {
				return modelMap[path];
			}
			Model* model = new Model(path);
			modelMap[path] = model;
			return model;
		}

	private:
		std::unordered_map<std::string, Model*> modelMap;

	public:
		static void CreateInstance() {
			DeleteInstance();
			s_instance = new ModelManager();
		}
		static void DeleteInstance() {
			delete s_instance;
			s_instance = nullptr;
		}
		static ModelManager& GetInstance() {
			return *s_instance;
		}
	private:
		ModelManager() = default;
		static inline ModelManager* s_instance;
};

#define MODEL ModelManager::GetInstance()