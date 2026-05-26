#include "laserBeam.hpp"

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

void LaserBeam::setUpSkin(sf::Texture textures[]) {
    // Per convenzione :
    // [0] Testa sx
    // [1] Corpo
    // [2] Testa dx

    textures[1].setRepeated(true);

    spriteHeadLeft .setTexture(textures[0], true);
    spriteBody     .setTexture(textures[1], true);
    spriteHeadRight.setTexture(textures[2], true);

    float scale = Config::LASER_BEAM_SCALE_TEXTURE;
    spriteHeadLeft .setScale({scale, scale});
    spriteBody     .setScale({scale, scale});
    spriteHeadRight.setScale({scale, scale});

    // Dato che la la skin del body è la skin che riempe lo schermo la imposto tale da riempirlo
    float headLeftWidthScaled  = textures[0].getSize().x * scale;
    float headRightWidthScaled = textures[2].getSize().x * scale;

    float bodySpaceOnScreen = Config::WINDOW_WIDTH - headLeftWidthScaled - headRightWidthScaled; // Spazio totale per il quale il body deve riempire lo schermo

    int requiredBodyPixels = static_cast<int>(bodySpaceOnScreen / scale);

    spriteBody.setTextureRect(
        sf::IntRect({0, 0},
        {requiredBodyPixels, static_cast<int>(textures[1].getSize().y)})
    );
}

// #########################################################################################################################################################################
LaserBeam::LaserBeam(sf::Vector2f startPos) : 
    spriteHeadLeft(texturesNonActive[0]),
    spriteBody(texturesNonActive[1]),
    spriteHeadRight(texturesNonActive[2]),
    state(LaserBeamState::DISABLE),
    stateTimer(0.0f)
{
    setUpSkin(texturesNonActive);
    setLaserPosition(startPos);
}

LaserBeam::LaserBeam() : 
    spriteHeadLeft(texturesNonActive[0]),
    spriteBody(texturesNonActive[1]),
    spriteHeadRight(texturesNonActive[2]),
    state(LaserBeamState::DISABLE),
    stateTimer(0.0f)
{
    setUpSkin(texturesNonActive);
}

// #########################################################################################################################################################################

sf::FloatRect LaserBeam::getBounds() const {
    return spriteBody.getGlobalBounds();
}

sf::Vector2f LaserBeam::getLaserSize() {
    return static_cast<sf::Vector2f>(spriteBody.getTexture().getSize()); // Dato che tutta la linea viene riempita dal body uso quello
}

void LaserBeam::setLaserPosition(sf::Vector2f pos) {
    // Setup testa sx
    float headLeftWidth = spriteHeadLeft.getGlobalBounds().size.x;
    spriteHeadLeft.setPosition({pos.x, pos.y});
    
    // Setup texta dx
    float headRightWidth = spriteHeadRight.getGlobalBounds().size.x;
    float rightHeadX = pos.x + Config::WINDOW_WIDTH - headRightWidth;
    spriteHeadRight.setPosition({rightHeadX, pos.y});

    // Setup body
    float headCenterY = pos.y + (spriteHeadLeft.getGlobalBounds().size.y / 2.f);
    float bodyCenterOffset = spriteBody.getGlobalBounds().size.y / 2.f;
    float bodyStartY = headCenterY - bodyCenterOffset;
    
    float bodyStartX = pos.x + headLeftWidth;

    spriteBody.setPosition({bodyStartX, bodyStartY});
}

void LaserBeam::setStateWarning() { 
    state = LaserBeamState::WARNING; 
};

void LaserBeam::setState(LaserBeamState state) {
    this->state = state;
}

sf::Vector2f LaserBeam::getPosition() const { 
    return spriteHeadLeft.getPosition(); 
}

bool LaserBeam::checkCollision(const sf::FloatRect& playerBounds){
    if (state != LaserBeamState::FIRING) return false;
    if (stateTimer < Config::LASER_BEAM_CHECK_COLLISION_OFFSET_TIMER) return false;

    // Calcolo un'altra area partendo dal corpo per "aiutare" il player riducendo la hitbox che sarebbe troppo grande
    sf::FloatRect bounds = spriteBody.getGlobalBounds();
  
    float centerX = bounds.getCenter().x;
    float centerY = bounds.getCenter().y;

    float newWidth  = bounds.size.x * Config::LASER_BEAM_CHECK_COLLISION_HITBOX_AREA_AMPLIFIE_ON_X;          
    float newHeight = bounds.size.y * Config::LASER_BEAM_CHECK_COLLISION_HITBOX_AREA_REDUCE_ON_Y;

    float newLeft = centerX - (newWidth  / 2.f);
    float newTop  = centerY - (newHeight / 2.f);

    sf::FloatRect lethalBox({newLeft, newTop}, {newWidth, newHeight});
    
    return playerBounds.findIntersection(lethalBox).has_value();
}

// #########################################################################################################################################################################


void LaserBeam::update(sf::Time dt) {
    if(state == LaserBeamState::DISABLE) return;

    // Tempo in comune // Tanto i singoli casi lo resettano 
    stateTimer += dt.asSeconds();

    if(state == LaserBeamState::WARNING) {
        if(stateTimer >= Config::LASER_BEAM_TIME_OF_WARNING) { 
            setUpSkin(texturesActive);
            state = LaserBeamState::FIRING;
            stateTimer = 0.f;
        }

    } else if (state == LaserBeamState::FIRING) {
        if(stateTimer >= Config::LASER_BEAM_TIME_OF_FIRING) {
            setUpSkin(texturesNonActive);
            state = LaserBeamState::FINISHED;
            stateTimer = 0.f;
            
        }

    } else if (state == LaserBeamState::FINISHED) {
        state = LaserBeamState::DISABLE;
        stateTimer = 0.f;
    }

}

void LaserBeam::draw(sf::RenderWindow& window) {
    if(state == LaserBeamState::DISABLE) return;

    window.draw(spriteBody);
    window.draw(spriteHeadLeft);
    window.draw(spriteHeadRight);
}
// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

LaserBeamHandler::LaserBeamHandler() 
    :   EventInView_BaseHandler(Config::LASER_BEAM_MIN_TIME_TO_GENERETE_LASER,Config::LASER_BEAM_MAX_TIME_TO_GENERETE_LASER),
        beamSound(beamBuffer),
        beamSoundDead(beamBufferDead),
        wait(false)
{
    beamSound.setVolume(Config::VOLUME_LASER_BEAM);
    beamSoundDead.setVolume(Config::VOLUME_LASER_BEAM_DEAD);
}

bool LaserBeamHandler::isActive() {
    return isEventActive;
}

void LaserBeamHandler::waitToGen(){
    wait = true;

    if (beamSound.getStatus() == sf::Sound::Status::Playing) {
        beamSound.pause(); 
    }
}

void LaserBeamHandler::goToGen(){
    wait = false;
}

bool LaserBeamHandler::getWaitStatus(){
    return wait;
}

void LaserBeamHandler::setWarningAll(){
    for(const auto& laser : activeBeams){
        laser->setStateWarning();
    }
}

bool LaserBeamHandler::checkCollisions(const sf::FloatRect& playerBounds){
    bool isThere = false;

    for(const auto& laser : activeBeams){
        if(laser->checkCollision(playerBounds)) {
            isThere = true;
        }
    }

    if(isThere){
        beamSound.setVolume(10);
        beamSoundDead.play();
        return true;
    }

    return false;
}


void LaserBeamHandler::triggerLaserEvent(const sf::View& view) {
    // Configurazione dei laser
    int numberLaser = std::uniform_int_distribution<int>(Config::LASER_BEAM_GENERETION_MIN_NUMBER,Config::LASER_BEAM_GENERETION_MAX_NUMBER)(generator);
    float startX = view.getCenter().x - (view.getSize().x / 2.f);
    
    // Configurazione corsie
    float laneHeight = Config::WINDOW_HEIGHT / static_cast<float>(Config::LASER_BEAM_GENERETION_MAX_LINE);
    
    // Configurazione lane disponibili e la loro genereazione
    std::vector<int> availableLanes(Config::LASER_BEAM_GENERETION_MAX_LINE);
    std::iota(availableLanes.begin(), availableLanes.end(), 0); // Riempo il vettore con LASER_BEAM_GENERETION_MAX_LINE numeri da 0 a LASER_BEAM_GENERETION_MAX_LINE

    std::shuffle(availableLanes.begin(), availableLanes.end(), generator); // Randomizzo 

    for(int i = 0; i < numberLaser; i++) {
        int selectedLane = availableLanes[i];
        float startY = (selectedLane * laneHeight) + (laneHeight / 2.f) - Config::LASER_BEAM_GENERETION_PADDING_Y; 
        
        activeBeams.push_back(std::make_unique<LaserBeam>(sf::Vector2f(startX, startY)));
        activeBeams.back()->setStateWarning();
    }

    isEventActive = true; 
}

void LaserBeamHandler::update(sf::Time dt, const sf::View& view) {
    if(wait == true) return;
    if (shouldTriggerEvent(dt.asSeconds())) {
        
        // Genero i laser
        triggerLaserEvent(view);
    }

    if(!isEventActive) return;

    if (beamSound.getStatus() != sf::Sound::Status::Playing) {
        beamSound.play();
    }

    // Inizio gestione laser
    float viewLeft = view.getCenter().x - (view.getSize().x / 2.f);
    bool allDisabled = true;

    // Posizionamento
    for(auto& laser : activeBeams){
        laser->setLaserPosition({viewLeft, laser->getPosition().y});
        laser->update(dt);
            
        if (laser->getState() != LaserBeamState::DISABLE) {
            allDisabled = false;
        }    
    }

    // Controllo finale prima della disattivazione di tutto
    if (allDisabled) {
        beamSound.stop(); // Per sicurezza 
        activeBeams.clear();
        setEventFinished(); 
        isEventActive = false;
    }
}

void LaserBeamHandler::draw(sf::RenderWindow& window) {
    if(wait == true) return;
    for(const auto& laser : activeBeams){
        laser->draw(window);
    }   
}