#include "ResourceManager.h"
#include <iostream>

using namespace std;

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
    for (auto iterator = textures.begin();
        iterator != textures.end();
        iterator++)
    {
        delete iterator->second;
    }

    textures.clear();

    for (auto iterator = fonts.begin();
        iterator != fonts.end();
        iterator++)
    {
        delete iterator->second;
    }

    fonts.clear();

    for (auto iterator = sounds.begin();
        iterator != sounds.end();
        iterator++)
    {
        delete iterator->second;
    }

    sounds.clear();
}

ResourceManager& ResourceManager::GetInstance()
{
    static ResourceManager instance;

    return instance;
}

// Texture --------------------------------------------------------------------

bool ResourceManager::LoadTexture(const string& key, const string& path)
{
    if (textures.find(key) != textures.end())
    {
        return true;
    }

    sf::Texture* texture = new sf::Texture();

    if (!texture->loadFromFile(path))
    {
        cerr << "Failed to texture font: " << path << endl;

        delete texture;

        return false;
    }

    textures[key] = texture;

    return true;
}

sf::Texture* ResourceManager::GetTexture(const string& key)
{
    if (textures.find(key) == textures.end())
    {
        cerr << "Texture not found: "
            << key
            << endl;

        return NULL;
    }

    return textures[key];
}

// Sprite --------------------------------------------------------------------

//bool ResourceManager::LoadSprite(const std::string& key, const std::string& textureKey, const sf::IntRect& textureRect)
//{
//    if (sprites.find(key) != sprites.end())
//    {
//        return true;
//    }
//
//    sf::Texture* texture = GetTexture(textureKey);
//
//    if (texture == nullptr)
//    {
//        cerr << "Failed to find texture: " << textureKey << endl;
//
//        return false;
//    }
//
//    sf::Sprite* sprite = new sf::Sprite(*texture);
//
//    sprite->setTextureRect(textureRect);
//
//    sprites[key] = sprite;
//
//    return true;
//}
//
//sf::Sprite* ResourceManager::GetSprite(const std::string& key)
//{
//    if (sprites.find(key) == sprites.end())
//    {
//        cerr << "Sprite not found: "
//            << key
//            << endl;
//
//        return NULL;
//    }
//
//    return sprites[key];
//}


// Font --------------------------------------------------------------------

bool ResourceManager::LoadFont(const string& key, const string& path)
{
    if (fonts.find(key) != fonts.end())
    {
        return true;
    }

    sf::Font* font = new sf::Font();

    if (!font->openFromFile(path))
    {
        cerr << "Failed to load font: " << path << endl;

        delete font;

        return false;
    }

    fonts[key] = font;

    return true;
}

sf::Font* ResourceManager::GetFont(const string& key)
{
    if (fonts.find(key) == fonts.end())
    {
        cerr << "Font not found: "
            << key
            << endl;

        return NULL;
    }

    return fonts[key];
}

// Sound --------------------------------------------------------------------

bool ResourceManager::LoadSound(const string& key, const string& path)
{
    // TODO
}

sf::SoundBuffer* ResourceManager::GetSound(const string& key)
{
    // TODO
}