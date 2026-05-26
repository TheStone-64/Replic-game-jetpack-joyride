#pragma once
#include "Global.hpp"
#include "entity.hpp"

using namespace Debug;

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################
// Data la sua utilità in generale non la includo nel namespace
enum LaserType {
    DIAGONAL = 0, HORIZONTAL = 1, VERTICAL = 2 
};

namespace LASER_UTILS {
    inline const std::map<LaserType, std::vector<std::string>> LASER_SKINS = {
        { LaserType::DIAGONAL,   { "../assets/pic/zappers/d1.png", "../assets/pic/zappers/d2.png", "../assets/pic/zappers/d3.png", "../assets/pic/zappers/d4.png" } },
        { LaserType::HORIZONTAL, { "../assets/pic/zappers/h1.png", "../assets/pic/zappers/h2.png", "../assets/pic/zappers/h3.png", "../assets/pic/zappers/h4.png" } },
        { LaserType::VERTICAL,   { "../assets/pic/zappers/v1.png", "../assets/pic/zappers/v2.png", "../assets/pic/zappers/v3.png", "../assets/pic/zappers/v4.png" } }
    };

    inline const std::string LASER_SOUND = "../assets/sound/zapper.wav";
}

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################
class Laser : public Entity {
    private:

        inline static std::map<LaserType, std::vector<sf::Texture>> texturesMap;
        
        inline static sf::SoundBuffer laserBuffer;
        sf::Sound laserSound; 

        LaserType type;
        sf::Sprite sprite;
        
        int currentFrame = 0;

        int generareLaserModel_basedType (LaserType type);
    
    public:

        Laser(LaserType t, sf::Vector2f startPos) : laserSound(laserBuffer), type(t) , sprite(texturesMap[type][0]){
            if (texturesMap[type].empty()) {
                log(LASER, WARN, "Missing texture ! Fallback a DIAGONAL.");
                type = LaserType::DIAGONAL; 
            }

            int maxFrames = texturesMap[type].size();

            currentFrame = std::uniform_int_distribution<int>(0, maxFrames - 1)(generator);

            if(type == DIAGONAL) sprite.setScale({0.3f, 0.3f});
            sprite.setTexture(texturesMap[type][currentFrame]);
            sprite.setPosition(startPos);

            laserSound.setVolume(Config::VOLUME_LASER); 
        }    
        

        // #########################################################################################################################################################################
        // Logica di upload statico delle texture
        static void loadSpecificTextures(LaserType type, const std::vector<std::string>& paths) {
            std::vector<sf::Texture> texList(paths.size());
            for (size_t i = 0; i < paths.size(); ++i) {
                if (!texList[i].loadFromFile(paths[i])) {
                    log(LASER, WARN, "Texture not loaded:", paths[i]);
                }
            }
            Laser::texturesMap[type] = std::move(texList);
        }

        static void loadTextures(){
            log(LASER, INFO, "Loading Laser textures...");

            for (const auto& [type, paths] : LASER_UTILS::LASER_SKINS) {
                loadSpecificTextures(type, paths);
            }
            log(LASER, INFO, "... Laser textures loaded.");
        }
        
        static void loadSound() {
            log(Debug::LASER, Debug::INFO, "Loading pattern sound...");
            if (!laserBuffer.loadFromFile(LASER_UTILS::LASER_SOUND)) {
                Debug::log(Debug::LASER, Debug::ERR, "Impossible to load sound of laser.");
            }
        }
        
        // #########################################################################################################################################################################
        void playSound();
        sf::FloatRect getBounds() const;
        sf::Vector2f getLaserSize();
        void setLaserPosition(sf::Vector2f pos);

        sf::Vector2f getPosition() const override { return sprite.getPosition(); }
        bool checkCollision(const sf::FloatRect& playerBounds);

        void update(sf::Time dt) override;  
        void draw(sf::RenderWindow& window) override;
};

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################
class LaserHandler : public EventOutOfView_BaseHandler {
    private:
        std::vector<std::unique_ptr<Laser>> activeLaser;
        LaserType lastGen = DIAGONAL;
    public:
        LaserHandler() : EventOutOfView_BaseHandler(
           Config::LASER_MIN_DISTANCE_BETWEEN_SPAWNS, 
           Config::LASER_PROBABILITY_SPAWN, 
        Config::LASER_PADDING_SPAWN_VIEW,
   Config::LASER_PADDING_Y_SPAWN,
            Config::LASER_MAX_MEMORIY
        ) {}

        bool shouldCreate(const sf::View& view, float incrementDistance);
        
        LaserType generateLaserType(); 
        void addLaser(LaserType choice, sf::Vector2f spawnPos);
        std::vector<sf::FloatRect> getOccupiedAreas() const;
        sf::Vector2f getLaserSize();
        bool checkCollisions(const sf::FloatRect& playerBounds);
        int getNumberLaserActive(); 
        bool areThereActiveLaser();

        void update( sf::Time dt, const sf::View& view);
        void draw(sf::RenderWindow& window);
};