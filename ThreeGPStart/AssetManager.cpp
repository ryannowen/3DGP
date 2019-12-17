#include "AssetManager.h"

std::shared_ptr<AssetManager> AssetManager::assetManager{ nullptr };

AssetManager::AssetManager()
{
	/// Loads missing texture
	std::shared_ptr<Helpers::ImageLoader> missingTex = std::make_shared<Helpers::ImageLoader>();
	if (!missingTex->Load("Data\\Textures\\MissingTexture.jpg"))
	{
		std::cout << "Could not load missing texture" << std::endl;
	}
	textures["MissingTexture.jpg"] = missingTex;
}

const Helpers::ImageLoader& AssetManager::LoadImage(const std::string argName)
{
	/// Returns texture if already in map
	if (textures.find(argName) != textures.end())
		return *textures.at(argName);

	/// Attemps to load texture, if fails returns missing texture
	std::shared_ptr<Helpers::ImageLoader> image = std::make_shared<Helpers::ImageLoader>();
	if (!image->Load("Data\\Textures\\" + argName))
	{
		std::cerr << argName << "does not exist Returning missing texture" << std::endl;
		return *textures.at("MissingTexture.jpg");
	}

	/// Adds texture to map and returns it
	textures[argName] = image;
	return *textures.at(argName);
}

