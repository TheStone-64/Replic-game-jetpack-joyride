#pragma once
#include "Global.hpp"
#include "entity.hpp"

using namespace Debug;

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

namespace LASER_BEAM_UTILS {

    inline const std::string PATH_LASER_ACTIVE_PHASE_ONE_LEFT_HEAD = "../assets/pic/lazer/laser_active_head_left.png";   
    inline const std::string PATH_LASER_ACTIVE_PHASE_ONE_RIGHT_HEAD = "../assets/pic/lazer/laser_active_head_right.png"; 
    inline const std::string PATH_LASER_ACTIVE_PHASE_ONE_BODY = "../assets/pic/lazer/laser_active_body.png";             

    inline const std::string PATH_LASER_NON_ACTIVE_PHASE_ONE_LEFT_HEAD  = "../assets/pic/lazer/laser_nonactive_head_left.png";
    inline const std::string PATH_LASER_NON_ACTIVE_PHASE_ONE_RIGHT_HEAD = "../assets/pic/lazer/laser_nonactive_head_right.png";
    inline const std::string PATH_LASER_NON_ACTIVE_PHASE_ONE_BODY  = "../assets/pic/lazer/laser_nonactive_body.png";
    inline const int NUMBER_LASER_SKIN = 3;

    inline const std::vector<std::string> PATH_LASER_NON_ACTIVE_SKINS = {PATH_LASER_NON_ACTIVE_PHASE_ONE_LEFT_HEAD, PATH_LASER_NON_ACTIVE_PHASE_ONE_BODY, PATH_LASER_NON_ACTIVE_PHASE_ONE_RIGHT_HEAD};
    inline const std::vector<std::string> PATH_LASER_ACTIVE_SKINS = {PATH_LASER_ACTIVE_PHASE_ONE_LEFT_HEAD, PATH_LASER_ACTIVE_PHASE_ONE_BODY, PATH_LASER_ACTIVE_PHASE_ONE_RIGHT_HEAD};

    inline const std::string PATH_LASER_SOUND = "../assets/sound/laser_beam.wav";
    inline const std::string PATH_LASER_SOUND_DEAD = "../assets/sound/zapper.wav";

}

enum LaserBeamState {
    WARNING, 
    FIRING,
    FINISHED,
    DISABLE
};

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

class LaserBeam : public Entity {
    private:
        inline static sf::Texture texturesActive   [LASER_BEAM_UTILS::NUMBER_LASER_SKIN];
        inline static sf::Texture texturesNonActive[LASER_BEAM_UTILS::NUMBER_LASER_SKIN];
        
        sf::Sprite spriteHeadLeft;
        sf::Sprite spriteBody;
        sf::Sprite spriteHeadRight;
        
        LaserBeamState state;
        float stateTimer;      // Tiene traccia del tempo trascorso nella fase attuale (Warning o Firing)
        
        void setUpSkin(sf::Texture texture[]);
    
    public:
        
        LaserBeam(sf::Vector2f startPos);
        LaserBeam();

        // #########################################################################################################################################################################
        
        static void loadTextures() {
            log(LASER_BEAM,INFO, "Loading texture...");
            
            bool success = true;
            for(int i = 0; i < LASER_BEAM_UTILS::NUMBER_LASER_SKIN; i++){
                if(!texturesActive[i].loadFromFile(LASER_BEAM_UTILS::PATH_LASER_ACTIVE_SKINS[i])){
                    log(LASER_BEAM, WARN, "Texture number",i , " active laser not loaded");
                    success = false;
                }
                if(!texturesNonActive[i].loadFromFile(LASER_BEAM_UTILS::PATH_LASER_NON_ACTIVE_SKINS[i])){
                    log(LASER_BEAM, WARN, "Texture number",i , "non  acrive laser not loaded");
                    success = false;
                }
            }

            log(LASER_BEAM, success ? INFO : WARN, success ? "... all texture are loaded." : "... not all texture are loaded.");
        }
        
        // #########################################################################################################################################################################
        
        sf::FloatRect getBounds() const;
        sf::Vector2f getLaserSize();
        void setLaserPosition(sf::Vector2f pos);
        
        void setStateWarning();
        void setState(LaserBeamState state);
        LaserBeamState getState() const { return state; }
        bool isFinished() const { return state == LaserBeamState::FINISHED; }

        sf::Vector2f getPosition() const override; 
        bool checkCollision(const sf::FloatRect& playerBounds);

        void update(sf::Time dt) override;
        void draw(sf::RenderWindow& window) override;
};

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

class LaserBeamHandler : public EventInView_BaseHandler{
    private:
        std::vector<std::unique_ptr<LaserBeam>> activeBeams;
        
        
        inline static sf::SoundBuffer beamBuffer;
        sf::Sound beamSound; 

        inline static sf::SoundBuffer beamBufferDead;
        sf::Sound beamSoundDead; 

        bool wait;

    public:
        LaserBeamHandler();

        static void loadSound(){
            log(Debug::LASER_BEAM, Debug::INFO, "Loading pattern sound...");
            if (!beamBuffer.loadFromFile(LASER_BEAM_UTILS::PATH_LASER_SOUND)) {
                Debug::log(Debug::LASER_BEAM, Debug::ERR, "Impossible to load sound of laser beam.");
            }
            
            if (!beamBufferDead.loadFromFile(LASER_BEAM_UTILS::PATH_LASER_SOUND_DEAD)) {
                Debug::log(Debug::LASER_BEAM, Debug::ERR, "Impossible to load sound of laser beam dead.");
            }


            log(Debug::LASER_BEAM, Debug::INFO, "... ending loading pattern sound. ");
        }


        bool isActive();
        void waitToGen();
        void goToGen();
        bool getWaitStatus();

        void setWarningAll();
        bool checkCollisions(const sf::FloatRect& playerBounds);
        void triggerLaserEvent(const sf::View& view);

        void update(sf::Time dt, const sf::View& view);
        void draw(sf::RenderWindow& window);
};