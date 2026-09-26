#include <filesystem>
#include <iostream>

#include "../include/sprites.h"

std::unordered_map<std::string,Texture2D> SpriteManager::sprites;

void SpriteManager::loadSprites(std::string path)
{
   for (const auto & entry : std::filesystem::directory_iterator(path))
   {
       if (!std::filesystem::is_directory(entry.path()))
       {
            Texture2D sprite = LoadTexture(entry.path().string().c_str());
            if (IsTextureValid(sprite))
            {
                sprites[entry.path().filename().string()] = sprite;
            }
            else
            {
                std::cerr << "ERROR unable to load sprite: " << entry.path().string() << "\n";
                return;
            }       
        }
   }
}

Texture2D SpriteManager::getSprite(std::string fileName)
{
    if (sprites.find(fileName) != sprites.end())
    {
        return sprites[fileName];
    }
    return {};
}