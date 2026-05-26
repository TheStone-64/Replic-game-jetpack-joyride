#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <set>
#include <algorithm>
#include <random>
#include <map>
#include <memory>


// #########################################################################################################################################################################
// Seed di generazione 
static std::mt19937 generator(std::random_device{}());


// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

// Costanti Globali
namespace Config {

    // #########################################################################################
    // Schermo
    inline const char* WINDOW_TITLE = "JetPack Joyride";
    inline constexpr int WINDOW_WIDTH = 1280;
    inline constexpr int WINDOW_HEIGHT = 460;
    inline constexpr float MAXFRAMERATE = 75;
    inline constexpr float BACKGROUND_OFFSET_VIEW = 1000;
    // #########################################################################################
    // Menu
    inline constexpr int REACH_TO_EXPIRE_BACKGROUN_LOADING = 5;
    inline constexpr float MENU_LOADING_MIN_TIME_ROTATE_SKIN = 1.5;
    
    // #########################################################################################
    // Player
    inline constexpr float GRAVITY = 1.68f;                                                // Fattore moltiplicante per il tempo usato per jetpack up 
    inline constexpr float THRUST = 2.1f;                                                 // Fattore moltiplicante per il tempo usato per jetpack down
    inline constexpr float PLAYER_PADDING = static_cast<float>(WINDOW_HEIGHT * 3) / 100;  // OFFSET per garantire che lo sprite rimanga a schermo e non salga al di fuori
    inline constexpr float PLAYER_X = 100;                                                // Punto di spawn sulle x e punto di minimo sulle x
    inline constexpr float MAX_FALL_SPEED = 500.0f;                                       // Velocità limite in caduta
    inline constexpr float MAX_RISE_SPEED = -600.0f;                                      // Velocità limite in salita

    inline constexpr float MIN_TIME_ROTATE_SKIN = 0.15f;                                   // Tempo minimo per la rotazione della skin ( se si diminuisce la skin è troppo veloce )
    
    // #########################################################################################
    // Coin
    inline constexpr int COIN_MAX_SET_ON_SCREEN = 2;                                            // Massimo di coin sullo schermo nello stesso momento // Anche se è impossibile // Per ora con la configurazione attuale
    inline constexpr int COIN_MAX_MEMORIY = 4;                                                  // Massimo di coin su tutto il gioco contemporaneamente 
    inline constexpr float COIN_DELAY_SPAWN = 6;                                                //
    inline constexpr float COIN_OFFSET = 200.0f;                                                // Sommo un fattore di errore ai singoli coin prima della eliminazione per assicurarmi che siano fuori dallo schermo 
    inline constexpr float COIN_PROBABILITY_SPAWN = 10;                                         // Probabilità di inserire delle monete nel gioco
    inline constexpr float COIN_PADDING_SPAWN_VIEW = 300;                                       // Margine di spawn rispetto alla view In modo da generare le monete con un leggero margine sulla destra
    inline constexpr float COIN_MIN_DISTANCE_BETWEEN_SPAWNS = 700;                             // Distanza minima dallo scorso spawn per prendere in considerazione un nuovo spawn
    inline constexpr float COIN_PADDING_Y_SPAWN = static_cast<float>(WINDOW_HEIGHT * 10) / 100; // Padding per la generazione delle monete in modo tale che non sia attaccato
    inline constexpr int COIN_MAX_TRY_SEARCH_AREA_FREE = 3;                                   // Quando passa tutti i controlli per la generazione della moneta e devo verificare se l'area dove vorrei disegnare è libera, ho massimo 3 tentativi di posizionamento poi vado avanti e non genero nulla
    inline constexpr float COIN_SPACING = 40.0f;                                                // Usato negli algoritmi come spazio
    inline constexpr float COIN_DEFAULT_ANIMATION_TIMER = 0.f;                                  // Set e Reset della variabile animationTimer per il cambio della skin 
    
    inline constexpr float COIN_NUMBER_ALGORITHM = 6;                                           // Schemi di generazione di monete
    
    // Draw coin
    inline constexpr int COIN_FIGURE_SQUARE = 4;
    inline constexpr int COIN_FIGURE_SQUARE_NUMBER_LAYER = 4;

    inline constexpr int COIN_FIGURE_RECTANGLE_LITTLE_LINE = 4;
    inline constexpr int COIN_FIGURE_RECTANGLE_BIG_LINE = 6;
    inline constexpr int COIN_FIGURE_RECTANGLE_NUMBER_LAYER = 3;

    inline constexpr int COIN_FIGURE_SNAKE_SEGNMENT_LENGHT = 4;
    inline constexpr int COIN_FIGURE_SNAKE_NUMBER_LAYER = 2;

    inline constexpr int COIN_FIGURE_PYRAMID_LAYERS = 3;
    inline constexpr int COIN_FIGURE_PYRAMID_BASE = 5;

    inline constexpr int COIN_FIGURE_DIAMOND_SIZE = 5; // Griglia 5x5

    inline constexpr int COIN_FIGURE_RING_WIDTH = 5;
    inline constexpr int COIN_FIGURE_RING_HEIGHT = 4;

    inline constexpr int COIN_FIGURE_WAVE_LENGTH = 8;
    inline constexpr int COIN_FIGURE_WAVE_HEIGHT = 3;

    // #########################################################################################
    // Laser
    
    inline constexpr float LASER_MIN_DISTANCE_BETWEEN_SPAWNS = 400;
    inline constexpr float LASER_PROBABILITY_SPAWN = 5;
    inline constexpr float LASER_OFFSET = 300.0f;
    inline constexpr float LASER_PADDING_SPAWN_VIEW = 100;
    inline constexpr float LASER_PADDING_Y_SPAWN = static_cast<float>(WINDOW_HEIGHT * 10) / 100;
    inline constexpr float LASER_NUMBER_MODELS = 3;
    inline constexpr int LASER_MAX_MEMORIY = 6;
    inline constexpr int LASER_MAX_TRY_SEARCH_AREA_FREE = 4;

    // #########################################################################################
    // Laser Beam
    inline constexpr float LASER_BEAM_SCALE_TEXTURE = 0.7f;
    inline constexpr float LASER_BEAM_CHECK_COLLISION_OFFSET_TIMER = 0.15f;

    inline constexpr float LASER_BEAM_CHECK_COLLISION_HITBOX_AREA_AMPLIFIE_ON_X = 1.2f;
    inline constexpr float LASER_BEAM_CHECK_COLLISION_HITBOX_AREA_REDUCE_ON_Y = 0.3f;

    inline constexpr float LASER_BEAM_TIME_OF_WARNING = 1.5f;
    inline constexpr float LASER_BEAM_TIME_OF_FIRING = 3.5f;

    inline constexpr float LASER_BEAM_MIN_TIME_TO_GENERETE_LASER = 5.0f;
    inline constexpr float LASER_BEAM_MAX_TIME_TO_GENERETE_LASER = 10.0f;

    inline constexpr int LASER_BEAM_GENERETION_MIN_NUMBER = 2;
    inline constexpr int LASER_BEAM_GENERETION_MAX_NUMBER = 3;
    inline constexpr int LASER_BEAM_GENERETION_MAX_LINE = 4;
    inline constexpr float LASER_BEAM_GENERETION_PADDING_Y = 45.0f;
    

    
    // #########################################################################################
    // Generazione
    inline constexpr float WINDOW_MIN_GENERATE = static_cast<float>(WINDOW_HEIGHT * 10) / 100;
    inline constexpr float WINDOW_MAX_GENERATE = WINDOW_HEIGHT - WINDOW_MIN_GENERATE;
  
    // #########################################################################################
    // Sound 

    inline constexpr float VOLUME_MUSIC = 30.0f;
    inline constexpr float VOLUME_JETPACK = 55.0f;
    inline constexpr float VOLUME_COIN = 80.0f;
    inline constexpr float VOLUME_LASER = 100.0f;
    inline constexpr float VOLUME_LASER_BEAM = 90.0f;
    inline constexpr float VOLUME_LASER_BEAM_DEAD = 100.0f;
    // #########################################################################################
    // Velocità di gioco
    inline constexpr float SCROLL_SPEED = 450.0f;
    inline constexpr float PLAYER_MAX_VELOCITY = 1000.0f;
    inline constexpr float ACCELERATION_RATE = 5.0f;

    // #########################################################################################
    // Area
    inline constexpr float AREA_PADDING = 35.0f;

    // #########################################################################################
    // Utils per la hud
    
    inline constexpr float HUD_CHARACTER_SIZE = 24.0f; 
    inline constexpr sf::Color HUD_COIN_COLOR = sf::Color::Yellow;
    inline constexpr sf::Color HUD_METERS_COLOR = sf::Color::White;

    inline constexpr sf::Vector2f HUD_FIST_LINE = {20.0f, 20.0f};
    inline constexpr sf::Vector2f HUD_SECOND_LINE = {HUD_FIST_LINE.x, HUD_FIST_LINE.y + (HUD_CHARACTER_SIZE * 1.3)};
    
    // Parametro generico
    inline constexpr float PIXEL_FOR_METER = 100.f; 
}

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

namespace Utils {

    inline bool isAreaSafe(const sf::FloatRect& newArea, const std::vector<sf::FloatRect>& occupiedAreas) {        
        sf::FloatRect areaWithPadding = newArea;

        // Angolo in alto a sinistra
        areaWithPadding.position.x -= Config::AREA_PADDING;
        areaWithPadding.position.y -= Config::AREA_PADDING;

        // Larghezza e altezza
        areaWithPadding.size.x += Config::AREA_PADDING * 2.f;
        areaWithPadding.size.y += Config::AREA_PADDING * 2.f;

        for (const auto& area : occupiedAreas) {
            if (areaWithPadding.findIntersection(area).has_value()) {
                return false; 
            }
        }
        
        return true; 
    }

    inline bool shouldOrNot(float chance) {
        std::uniform_real_distribution<float> distribution(0.0f, 100.0f);       
        return distribution(generator) < chance;
    }

    inline float generateYPos(float patternSizeY) {
        float yMin = Config::WINDOW_MIN_GENERATE;   
        float yMax = Config::WINDOW_MAX_GENERATE;

        if (yMax < yMin) yMax = yMin; 
        
        float yPos = std::uniform_real_distribution<float>(yMin, yMax)(generator);
        yPos = std::clamp(yPos, yMin, yMax - patternSizeY);
        
        return yPos;
    }

}

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

namespace Debug {
    enum Level { INFO, DBG, WARN, ERR };
    enum Module { BACKGROUND, PLAYER, GAME, VIEW, COIN, LASER, ENTITY, MENU, LASER_BEAM };

    inline const char* LevelNames[] = { "INFO", "DEBUG", "WARN", "ERROR" };
    inline const char* ModuleNames[] = { "BACKGROUND", "PLAYER","GAME","VIEW", "COIN", "LASER", "ENTITY", "MENU", "LASER_BEAM" };

    // #########################################################################################
    // Da modificare i suoi elementi per visualizzare o meno le stampe per ogni modulo
    inline std::set<Module> active_modules = { Module::BACKGROUND, Module::PLAYER, Module::GAME, Module::COIN, Module::LASER, Module::ENTITY, Module::MENU, Module::LASER_BEAM};
    inline std::set<Level>  active_levels  = {Level::INFO, Level::DBG, Level::ERR, Level::WARN};
    const bool all_modules = false;
    const bool all_levels = true;
    const bool active = false;

    // #########################################################################################
    inline void print_args() { std::cout << std::endl; }

    template <typename T, typename... Args>
    void print_args(T first, Args... args) {
        std::cout << first << " ";
        print_args(args...);
    }   

    template <typename... Args>
    void log(Module m, Level l, Args... args) {
        if(!active) return;
        if (active_modules.count(m) || all_modules) {
            if(active_levels.count(l )  || all_levels)
                std::cout << "[" << ModuleNames[m] << "] [" << LevelNames[l] << "] ";
            print_args(args...);
        }
    }
}
