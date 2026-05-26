#pragma once
#include "Global.hpp"


// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

// Se modifichi le componenti modifica anche i vettori in player.cpp
const std::string PL_SKIN_PLAYER_WALK_1    = "../assets/pic/barry/barry.png";
const std::string PL_SKIN_PLAYER_WALK_2    = "../assets/pic/barry/barry2.png";
const int SKIN_WALK = 2;


// Se modifichi le componenti modifica anche i vettori in player.cpp
const std::string PL_SKIN_PLAYER_FLY_UP    = "../assets/pic/barry/barry3.png";
const std::string PL_SKIN_PLAYER_FLY_DOWN  = "../assets/pic/barry/barryst.png";
const int SKIN_FLY = 2;

// later
const std::string PL_EFFECT_FIRE  = "../";
const std::string PL_EFFECT_FLOOR = "../";

const std::string PL_SOUND = "../assets/sound/jetpack_se.wav";

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################
enum class PlayerState { ALIVE, DYING, DEAD };

class Player {
    private:
        inline static sf::Texture texturePlayerWalk[SKIN_WALK];
        inline static sf::Texture texturePlayerFly[SKIN_FLY];
        sf::Sprite playerSprite;
        
        inline static sf::SoundBuffer jetpackBuffer;
        sf::Sound jetpackSound;
        
        bool flipSkinWalk;
        float animationTimer = 0.f;
        float posSpriteY; // cordiante dello sprite quando non vola
        PlayerState state = PlayerState::ALIVE;
        struct PhysicsComponent {
            float velocity = 0.0f;
            bool isFlying = false;
        };

        PhysicsComponent physics;

    public:
        Player();
        void handleSpace_press();
        void handleSpace_release();
        sf::FloatRect getBounds();

        void kill();
        PlayerState getState() const { return state; }
        bool isDead() const { return state == PlayerState::DEAD; };
        
        void update(sf::Time dt, float scrollVelocity);
        void draw(sf::RenderWindow& window);
};