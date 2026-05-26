#pragma once
#include "Global.hpp"

inline const std::string MENU_FONT        = "../assets/fonts/rimouski.otf";
inline const std::string MENU_PLAY_BUTTON = "../assets/pic/menu/play.png";
inline const std::string MENU_LOADING_BG  = "../assets/pic/menu/loadingBackground4.jpg";
inline const std::string MENU_BG          = "../assets/pic/menu/backgroundMenu.jpg";

inline constexpr sf::Color COLOR_PANEL_BEHIND_SECOND_WORDS(20, 25, 35, 180);
inline constexpr sf::Color COLOR_OUT_LINE_PANEL_BEHIND_WORDS (93, 103, 121, 200);
inline constexpr sf::Color COLOR_SECOND_WORDS(20, 255, 50);


class Menu {
    private:
        
        inline static sf::Font font;
        sf::Text loadingText;
        // #########################################
    
        inline static sf::Texture texturesLoadingBg;
        sf::Sprite spriteLoadingBg;
        
        // #########################################
        inline static sf::Texture textureButtonPlay;
        inline static sf::Texture texturesBg;

        sf::Sprite spriteButtonPlay;
        sf::Sprite spritesBg;


        sf::Text titleText;
        sf::Text recordText;
        sf::Text coinsText;

        sf::RectangleShape statsPanel;

        bool drawBackground;
        float timeBeforeExpireBackground;
        float totalTimeInMenu;

        
        float animationTimer;
    
    public:
        Menu();
        
        void update(sf::RenderWindow& window, sf::Time time);
        void updateStats(int record, int totalCoins);
 
        bool canSwitch();
        bool isPlayClicked(sf::Vector2f pos) const;
        void draw(sf::RenderWindow& window);
};