#pragma once
#include "Global.hpp"
#include "entity.hpp"

using namespace Debug;

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

namespace COIN_UTILS{
    const std::string COIN_SKIN_1 = "../assets/pic/coin/1.png";
    const std::string COIN_SKIN_2 = "../assets/pic/coin/2.png";
    const std::string COIN_SKIN_3 = "../assets/pic/coin/3.png";
    const std::string COIN_SKIN_4 = "../assets/pic/coin/4.png";
    const std::string COIN_SKIN_5 = "../assets/pic/coin/5.png";
    const std::string COIN_SKIN_6 = "../assets/pic/coin/6.png";
    const std::vector<std::string> COIN_SKIN_VECTOR = {COIN_SKIN_1, COIN_SKIN_2, COIN_SKIN_3, COIN_SKIN_4, COIN_SKIN_5, COIN_SKIN_6};
    const int COIN_SKIN = 6;

    const std::string COIN_SOUND = "../assets/sound/coin.wav";
}
// Data la sua itilità in generale non lo metto nel namespace // Tanto dal nome si capisce
enum CoinType {
    RECTANGLE = 0, SNAKE = 1, SQUARE = 2, PYRAMID = 3, DIAMOND = 4, RING = 5, WAVE = 6
};

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

class SingleCoin : public Entity {
    private:
        
        inline static sf::Texture textures [COIN_UTILS::COIN_SKIN];
        sf::Sprite sprite;

        bool collected = false;
        
        // Gestione rotazione skin del coin
        float animationTimer = Config::COIN_DEFAULT_ANIMATION_TIMER;
        int currentFrame = 0;

    public:

        SingleCoin(sf::Vector2f startPos) : sprite(textures[0]){
            sprite.setTexture(textures[0], true);
            sprite.setPosition(startPos);
        }
        
        // #########################################################################################################################################################################
        static void loadTextures() {                
            log(COIN,INFO, "Loading texture...");
            
            bool success = true;
            for(int i = 0; i < COIN_UTILS::COIN_SKIN; i++){
                if(!textures[i].loadFromFile(COIN_UTILS::COIN_SKIN_VECTOR[i])){
                    log(COIN, WARN, "Texture number",i , "not loaded");
                    success = false;
                }
            }

            log(COIN, success ? INFO : WARN, success ? "... all texture are loaded." : "... not all texture are loaded.");
        }

        static sf::Vector2f getCoinSize() {
            if (textures[0].getSize().x > 0) {
                return sf::Vector2f(textures[0].getSize().x, textures[0].getSize().y);
            }

            return {40.f, 40.f}; // Fallback 
        }
        
        // #########################################################################################################################################################################

        
        void collect();
        bool isCollected() const;
        void move(sf::Vector2f offset);
        sf::FloatRect getBounds() const;
        
        sf::Vector2f getPosition() const override;
        void update(sf::Time dt) override;
        void draw(sf::RenderWindow& window) override;

};

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

class CoinPattern  {
    private:
        std::vector<std::unique_ptr<SingleCoin>> coins;

        inline static sf::SoundBuffer coinBuffer;
        sf::Sound coinSound; 
        
    public:
        CoinPattern() : coinSound(coinBuffer) { 
            coinSound.setVolume(Config::VOLUME_COIN); 
        };
        
        // #########################################################################################################################################################################
        static void loadSound() {
            log(Debug::COIN, Debug::INFO, "Loading pattern sound...");
            if (!coinBuffer.loadFromFile(COIN_UTILS::COIN_SOUND)) {
                Debug::log(Debug::COIN, Debug::ERR, "Impossible to load sound of coin.");
            }
        }
        
        static sf::Vector2f getPatternSize(CoinType choice) {
            sf::Vector2f coinSize = SingleCoin::getCoinSize();
            float spacing = Config::COIN_SPACING;

            switch (choice) {
                case CoinType::SQUARE:
                    return sf::Vector2f(((Config::COIN_FIGURE_SQUARE-1) * spacing) + coinSize.x, 
                                        ((Config::COIN_FIGURE_SQUARE_NUMBER_LAYER-1) * spacing) + coinSize.y);
                    
                case CoinType::RECTANGLE:
                    return sf::Vector2f(((Config::COIN_FIGURE_RECTANGLE_BIG_LINE-1) * spacing) + coinSize.x, 
                                        ((Config::COIN_FIGURE_RECTANGLE_NUMBER_LAYER-1) * spacing) + coinSize.y); 

                case CoinType::SNAKE:
                    return sf::Vector2f(((Config::COIN_FIGURE_SNAKE_SEGNMENT_LENGHT - 1) * spacing) + coinSize.x, 
                                        ((Config::COIN_FIGURE_SNAKE_NUMBER_LAYER-1) * spacing) + coinSize.y); 

                case CoinType::PYRAMID:
                    return sf::Vector2f(((Config::COIN_FIGURE_PYRAMID_BASE - 1) * spacing) + coinSize.x, 
                                        ((Config::COIN_FIGURE_PYRAMID_LAYERS - 1) * spacing) + coinSize.y); 

                case CoinType::DIAMOND:
                    return sf::Vector2f(((Config::COIN_FIGURE_DIAMOND_SIZE - 1) * spacing) + coinSize.x, 
                                        ((Config::COIN_FIGURE_DIAMOND_SIZE - 1) * spacing) + coinSize.y); 

                case CoinType::RING:
                    return sf::Vector2f(((Config::COIN_FIGURE_RING_WIDTH - 1) * spacing) + coinSize.x, 
                                        ((Config::COIN_FIGURE_RING_HEIGHT - 1) * spacing) + coinSize.y); 

                case CoinType::WAVE:
                    return sf::Vector2f(((Config::COIN_FIGURE_WAVE_LENGTH - 1) * spacing) + coinSize.x, 
                                        ((Config::COIN_FIGURE_WAVE_HEIGHT - 1) * spacing) + coinSize.y);
                                        
                default: // Fallback
                    return sf::Vector2f(((Config::COIN_FIGURE_SQUARE-1) * spacing) + coinSize.x, 
                                        ((Config::COIN_FIGURE_SQUARE_NUMBER_LAYER-1) * spacing) + coinSize.y);
                    
            }
        }

        // #########################################################################################################################################################################

        void createSquare (sf::Vector2f offset);
        void createSpecialRect (sf::Vector2f offset);
        void createSnake (sf::Vector2f offset);
        
        void createPyramid(sf::Vector2f offset);
        void createDiamond(sf::Vector2f offset);
        void createRing(sf::Vector2f offset);
        void createWave(sf::Vector2f offset);

        int getCoins () const;

        int checkCollisions(const sf::FloatRect& playerBounds);
        
        void update(const sf::View& view, sf::Time dt);
        void draw(sf::RenderWindow& window);

        std::vector<sf::FloatRect> getOccupiedAreas() const;
};

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################
class CoinHandler : public EventOutOfView_BaseHandler {
    private:
        std::vector<std::unique_ptr<CoinPattern>> activePatterns;
        void clearElementActivePattern();

    public:
        CoinHandler() : EventOutOfView_BaseHandler(
            Config::COIN_MIN_DISTANCE_BETWEEN_SPAWNS, 
            Config::COIN_PROBABILITY_SPAWN, 
         Config::COIN_PADDING_SPAWN_VIEW,
    Config::COIN_PADDING_Y_SPAWN,
             Config::COIN_MAX_MEMORIY
        ) {}
    
        bool shouldCreate(const sf::View& view, float incrementDistance);

        std::vector<sf::FloatRect> getOccupiedAreas();
        
        CoinType generateCoinType();   
        void addCoinSequence(CoinType choice, sf::Vector2f spawnPos);
        int checkCollisions(const sf::FloatRect& playerBounds);
        void update(sf::Time dt, const sf::View& view);
        void draw(sf::RenderWindow& window);
};