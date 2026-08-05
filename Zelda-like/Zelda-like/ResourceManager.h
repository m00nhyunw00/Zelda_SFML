#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <string>

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

    bool LoadTexture(const std::string& key, const std::string& path);
    sf::Texture* GetTexture(const std::string& key);

    bool LoadFont(const std::string& key, const std::string& path);
    sf::Font* GetFont(const std::string& key);

    bool LoadSound(const std::string& key, const std::string& path);
    sf::SoundBuffer* GetSound(const std::string& key);
};