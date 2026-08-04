#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <SFML/Audio.hpp>


class ResourceManager
{
private:
    std::map<std::string, sf::Texture*> textures;
    std::map<std::string, sf::Font*> fonts;
    std::map<std::string, sf::SoundBuffer*> sounds;


private:
    ResourceManager();
    ~ResourceManager();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

public:
    static ResourceManager& GetInstance();

    bool LoadTexture(std::string key, std::string path);
    sf::Texture* GetTexture(std::string key);

    bool LoadFont(std::string key, std::string path);
    sf::Font* GetFont(std::string key);

    bool LoadSound(std::string key, std::string path);
    sf::SoundBuffer* GetSound(std::string key);
};