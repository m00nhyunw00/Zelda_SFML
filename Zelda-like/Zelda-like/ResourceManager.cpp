#include "ResourceManager.h"
#include <iostream>

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    for (auto iterator = fonts.begin();
        iterator != fonts.end();
        iterator++)
    {
        delete iterator->second;
    }

    fonts.clear();
}

ResourceManager& ResourceManager::GetInstance()
{
    static ResourceManager instance;

    return instance;
}

bool ResourceManager::LoadFont(string key, string path)
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

sf::Font* ResourceManager::GetFont(string key)
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