#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

using namespace std;

class ResourceManager
{
private:
    map<string, sf::Font*> fonts;

private:
    ResourceManager();
    ~ResourceManager();

public:
    static ResourceManager& GetInstance();

    bool LoadFont(string key, string path);
    sf::Font* GetFont(string key);
};