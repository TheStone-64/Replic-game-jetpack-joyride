#include "player.hpp"

using  namespace Debug;

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

// Necessaria la pre dichiarazione degli elementi // Sprite e Sound non hanno un costruttore vuoto
Player::Player() : playerSprite(texturePlayerWalk[0]), jetpackSound(jetpackBuffer) {

    // Control skin wakl => 
    log(Debug::PLAYER, INFO, "Loading walk texture...");
    std::string pathsWalk[] = { PL_SKIN_PLAYER_WALK_1, PL_SKIN_PLAYER_WALK_2 };
    
    for (int i = 0; i < SKIN_WALK; i++) {
        if (!texturePlayerWalk[i].loadFromFile(pathsWalk[i])) {
            log(Debug::PLAYER, ERR, "Not find walk texture n.", i);
        }
    }

    flipSkinWalk = true;

    // Control skin fly =>
    log(Debug::PLAYER, INFO, "Loading fly texture...");
    std::string pathsFly[] = { PL_SKIN_PLAYER_FLY_UP, PL_SKIN_PLAYER_FLY_DOWN };
    
    for (int i = 0; i < SKIN_FLY; i++) {
        if (!texturePlayerFly[i].loadFromFile(pathsFly[i])) {
            log(Debug::PLAYER, ERR, "Not find fly texture n.", i);
        }
    }

    // Set skin and pos => 
    posSpriteY = Config::WINDOW_HEIGHT - (Config::PLAYER_PADDING + playerSprite.getTexture().getSize().y);

    playerSprite.setTexture(texturePlayerWalk[1], true);
    playerSprite.setPosition({ Config::PLAYER_X, posSpriteY }); 
    log(Debug::PLAYER, Debug::DBG, "sprite.y=", playerSprite.getPosition().y);

    // Control sound => 
    log(Debug::PLAYER, INFO, "Loading sound texture...");
    if (!jetpackBuffer.loadFromFile(PL_SOUND)) {
        Debug::log(PLAYER, ERR, "Impossible to load sound of jetpack");
    }

    jetpackSound.setBuffer(jetpackBuffer);

    jetpackSound.setVolume(Config::VOLUME_JETPACK);
    jetpackSound.setLooping(true);

    log(Debug::PLAYER, INFO, "... all texture and sound are loaded!");
}   


// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

void Player::handleSpace_press(){
    if (state == PlayerState::ALIVE) {
        physics.isFlying = true;
    }
}


void Player::handleSpace_release(){
    physics.isFlying = false;
}

sf::FloatRect Player::getBounds(){
    return playerSprite.getGlobalBounds();
}

void Player::kill() {
    if (state != PlayerState::ALIVE) return; 

    state = PlayerState::DYING;
    physics.isFlying = false; 
    
    // Rimbalzo all'indietro/alto 
    physics.velocity = -300.0f; 
    
    if (jetpackSound.getStatus() == sf::Sound::Status::Playing) {
        jetpackSound.stop();
    }
    
    playerSprite.setTexture(texturePlayerFly[1]); // Impsota una skin di caduta // da cercare e inserire 
}

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

void Player::update(sf::Time dt, float scrollVelocity) {;
    //log(PLAYER, DBG, "scroolVelocity=", scrollVelocity);
    float verticalSpeed = scrollVelocity;

    playerSprite.move({scrollVelocity, 0.f});

    if (state == PlayerState::DEAD) return;
    if (state == PlayerState::DYING) {
        
        // Aumento esponenzialmente la gravità per farlo cadere molto più velocemente
        physics.velocity += verticalSpeed * Config::GRAVITY;
        playerSprite.move({0.f, physics.velocity * dt.asSeconds()});

        // Se sta toccando il pavimento il pl è morto
        if (playerSprite.getPosition().y >= posSpriteY) {
            playerSprite.setPosition({playerSprite.getPosition().x, posSpriteY});
            state = PlayerState::DEAD;
        }
    
        return; 
    }
    
    if (physics.isFlying) {

        // Se lo sprite non supera il minimo delle y allora aumento la sua y
        if(playerSprite.getPosition().y - Config::PLAYER_PADDING > 0){
            //playerSprite.move({0.0f,-verticalSpeed });//* Config::JETPACK_POWER_UP});
            physics.velocity += -verticalSpeed * Config::THRUST;
        }

        // Control sound => 
        // Se il suono è off lo faccio partire 
        if (jetpackSound.getStatus() != sf::Sound::Status::Playing) {
            jetpackSound.play();
        }
        
        playerSprite.setTexture(texturePlayerFly[0]);

    } else {
        // Control sound => 
        if (jetpackSound.getStatus() == sf::Sound::Status::Playing) {
            jetpackSound.stop();
        }

        // Se lo sprite non è a nella posizione di default non vado a inserire la skin base gestisco ancora la caduta del volo cambiando skin e diminuendo la y
        if (playerSprite.getPosition().y < posSpriteY) {
            physics.velocity += verticalSpeed * Config::GRAVITY;
            //playerSprite.move({0.0f, verticalSpeed * Config::GRAVITY});
            playerSprite.setTexture(texturePlayerFly[1]);

        } else {        
            // Control skin => 
            animationTimer += dt.asSeconds();
            if (animationTimer >= Config::MIN_TIME_ROTATE_SKIN) { 
                flipSkinWalk = !flipSkinWalk;
                playerSprite.setTexture(texturePlayerWalk[flipSkinWalk]);
                animationTimer = 0.f;
            }
        }
    }

    // Se supera i limiti li reimposto e poi muovo
    if (physics.velocity > Config::MAX_FALL_SPEED) physics.velocity= Config::MAX_FALL_SPEED;
    if (physics.velocity < Config::MAX_RISE_SPEED) physics.velocity = Config::MAX_RISE_SPEED;

    playerSprite.move({0.f, physics.velocity * dt.asSeconds()});
    
    // Controllo atterraggio
    if (playerSprite.getPosition().y > posSpriteY) {
        playerSprite.setPosition({playerSprite.getPosition().x, posSpriteY});
        physics.velocity = 0.f;
    }

    // Controllo soffitto
    if (playerSprite.getPosition().y < Config::PLAYER_PADDING) {
        playerSprite.setPosition({playerSprite.getPosition().x, Config::PLAYER_PADDING});
        physics.velocity *= 0.8f;
        if(physics.velocity < 0) physics.velocity = 1.0f;
    }
    
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(playerSprite);
}
