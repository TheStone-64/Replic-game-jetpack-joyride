#include "background.hpp"

using namespace Debug;

Background::Background() {
    log(BACKGROUND, INFO, "Loading costructor for texture...");
    
    std::string paths[BG_ALL] = {BG_START_LAB, BG_LAB_FIRST, BG_LAB_SECOND};

    for (int i = 0; i < BG_ALL; i++) {
        if (textures[i].loadFromFile(paths[i])) {

            sprites.emplace_back(textures[i]); 
            sprites[i].setPosition({dimTotal, 0.0f});

            // Misure necessarie per la update in quanto riesco a determinare in che sezione del vettore sprite mi trovo
            dimSprite_x[i] = textures[i].getSize().x;
            dimTotal += dimSprite_x[i];

            log(BACKGROUND, DBG, "For i=", i, "position x =", dimTotal);
        
        } else {
            log(BACKGROUND, WARN, "Img not loaded for i=", i);
        }
        
    }

    log(BACKGROUND, INFO,"... all texture are loaded!");
}

void Background::update(const sf::View& view) {
    float viewLeft = view.getCenter().x - (view.getSize().x / 2.f);

    for (int i = 0; i < BG_ALL; i++) {

        // Se la fine dello sprite è a sinistra dell'inizio della view
        if (sprites[i].getPosition().x + dimSprite_x[i] < viewLeft) {

            float maxRight = 0;
            for (const sf::Sprite& s : sprites) {
                float rightEdge = s.getPosition().x + s.getGlobalBounds().size.x;
                if (rightEdge > maxRight) maxRight = rightEdge;
            }
            
            // 0 è da escludere in quanto contiene lo start 
            log(Debug::BACKGROUND, Debug::DBG, "maxRight=", maxRight);    
            sprites[i].setTexture(textures[(i == 0) ? 1 : i]); 
            sprites[i].setPosition({ maxRight, 0 });
        }
    }
}

void Background::draw(sf::RenderWindow& window) {
    for (int i = 0; i < BG_ALL; i++) {
        window.draw(sprites[i]);
    }
}