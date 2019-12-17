#pragma once

#include <iostream>
#include <unordered_map>

#include "Helper.h"
#include "ImageLoader.h"
#include "Mesh.h"

class AssetManager
{
private:
	static std::shared_ptr<AssetManager> assetManager;

	std::unordered_map<std::string, std::shared_ptr<Helpers::ImageLoader>> textures;

	AssetManager();

public:
	static AssetManager& GetInstance()
	{
		if (assetManager == nullptr) 
		{
			assetManager.reset(new AssetManager());
		}

		return *assetManager;
	}

	const Helpers::ImageLoader& LoadImage(const std::string argName);
};

#define ASSETS AssetManager::GetInstance()
