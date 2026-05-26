#include "menu.hpp"

Menu::Menu() : 
    loadingText(font),
    spriteLoadingBg(texturesLoadingBg),
    spriteButtonPlay(textureButtonPlay),
    spritesBg(texturesBg), 
    titleText(font), 
    recordText(font), 
    coinsText(font), 
    drawBackground(false), // Rimozione della schermata loading
    timeBeforeExpireBackground(0.0f), 
    totalTimeInMenu(0.0f),
    animationTimer(0.f)

{
    
    // Loading font-texture
    if (!font.openFromFile(MENU_FONT)) {
        Debug::log(Debug::MENU, Debug::ERR, "Impossible to load font: ", MENU_FONT);
    }
    if(!textureButtonPlay.loadFromFile(MENU_PLAY_BUTTON)){
        Debug::log(Debug::MENU, Debug::ERR, "Impossible to load texture: ", MENU_PLAY_BUTTON);
    }
    if(!texturesLoadingBg.loadFromFile(MENU_LOADING_BG)){
        Debug::log(Debug::MENU, Debug::ERR, "Impossible to load texture: ", MENU_LOADING_BG);
    }
    if (!texturesBg.loadFromFile(MENU_BG)) {
        Debug::log(Debug::MENU, Debug::ERR, "Impossible to load texture: ", MENU_BG);
    }

    // ##################################################################################
    // Sfondo del caricamento
    float scaleX = static_cast<float>(Config::WINDOW_WIDTH)  / texturesLoadingBg.getSize().x;
    float scaleY = static_cast<float>(Config::WINDOW_HEIGHT) / texturesLoadingBg.getSize().y;

    spriteLoadingBg.setTexture(texturesLoadingBg, true);
    spriteLoadingBg.setScale({scaleX, scaleY});
    spriteLoadingBg.setPosition({0.f,0.f});

    // ##################################################################################
    // Scritta del caricamento 
    loadingText.setFont(font);
    loadingText.setString("Loading...");
    loadingText.setCharacterSize(30);
    loadingText.setFillColor(sf::Color::White);
    loadingText.setOutlineColor(sf::Color::Black);
    loadingText.setOutlineThickness(3.f);

    loadingText.setPosition({
        (Config::WINDOW_WIDTH - loadingText.getGlobalBounds().size.x) / 2.f, 
        (Config::WINDOW_HEIGHT - loadingText.getGlobalBounds().size.y - 10)
    });


    // ##################################################################################
    // ##################################################################################
    // ##################################################################################
    // ##################################################################################
    // Sfondo menu laboratorio
    scaleX = static_cast<float>(Config::WINDOW_WIDTH)  / texturesBg.getSize().x;
    scaleY = static_cast<float>(Config::WINDOW_HEIGHT) / texturesBg.getSize().y;

    spritesBg.setTexture(texturesBg, true);
    spritesBg.setScale({scaleX, scaleY});
    spritesBg.setPosition({0.f, 0.f});

    // ##################################################################################
    // Pannello dietro le scritte secondarie
    statsPanel.setSize({400.f, 120.f});

    statsPanel.setFillColor(COLOR_PANEL_BEHIND_SECOND_WORDS); 
    statsPanel.setOutlineColor(COLOR_OUT_LINE_PANEL_BEHIND_WORDS);
    statsPanel.setOutlineThickness(2.f);

    statsPanel.setPosition({
        (Config::WINDOW_WIDTH - statsPanel.getSize().x) / 2.f,
        135.f
    });

    // #################################################################################
    // Configurazione del titolo 
    titleText.setFont(font);
    titleText.setString("JETPACK JOYRIDE");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::White);//(72,82,107));
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(4.f);

    titleText.setPosition({
        (Config::WINDOW_WIDTH - titleText.getGlobalBounds().size.x) / 2.f, 
        50.f
    });

    // ##################################################################################
    // Configurazione scritte secondarie
    recordText.setFont(font);
    recordText.setCharacterSize(30);
    recordText.setFillColor(COLOR_SECOND_WORDS);
    recordText.setOutlineColor(sf::Color::Black);
    recordText.setOutlineThickness(2.f);
    
    coinsText.setFont(font);
    coinsText.setCharacterSize(30);
    coinsText.setFillColor(COLOR_SECOND_WORDS);
    coinsText.setOutlineColor(sf::Color::Black);
    coinsText.setOutlineThickness(2.f);

    // Settaggio di base delle statistiche 
    updateStats(0,0);
    
    // ##################################################################################
    // ##################################################################################
    // Configurazione del bottone play
    spriteButtonPlay.setTexture(textureButtonPlay, true);
        
    spriteButtonPlay.setOrigin({
        spriteButtonPlay.getLocalBounds().size.x / 2.f,
        spriteButtonPlay.getLocalBounds().size.y / 2.f
    });

    spriteButtonPlay.setPosition({
        Config::WINDOW_WIDTH / 2.f, 
        Config::WINDOW_HEIGHT / 1.3f
    });
}

void Menu::update(sf::RenderWindow& window, sf::Time time) {

    if(drawBackground == true && timeBeforeExpireBackground <= Config::REACH_TO_EXPIRE_BACKGROUN_LOADING) {
        // Gestione schermata caricamento
        timeBeforeExpireBackground += time.asSeconds(); 
        
        // Animazione scritta "loading" 
        animationTimer += time.asSeconds();
        if (animationTimer >= Config::MENU_LOADING_MIN_TIME_ROTATE_SKIN){
            if(loadingText.getString() == "Loading..."){
                loadingText.setString("Loading.");
            } else loadingText.setString(loadingText.getString() + ".");
            
            animationTimer = 0.f;
        }
        
        return;
    } 

    drawBackground = false;
    
    // ##################################################################################
    // Effetto hover sul bottone play

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (spriteButtonPlay.getGlobalBounds().contains(mousePos)) {
        spriteButtonPlay.setScale({1.05f, 1.05f}); 
        spriteButtonPlay.setColor(sf::Color(230, 230, 230)); 
    } else {
        spriteButtonPlay.setScale({1.f, 1.f});
        spriteButtonPlay.setColor(sf::Color::White); 
    }

    // ##################################################################################
    // Effetto di oscillazione verticale tramite la funzione seno del bottone 

    totalTimeInMenu += time.asSeconds();

    float oscillation = std::sin(totalTimeInMenu * 3.f) * 5.f;

    spriteButtonPlay.setPosition({
        Config::WINDOW_WIDTH / 2.f, 
        (Config::WINDOW_HEIGHT / 1.3f) + oscillation
    });
}

// Aggiorna i dati utente
void Menu::updateStats(int totalCoins, int record) {
    coinsText.setString("Total Coins: " + std::to_string(totalCoins));
    recordText.setString("Personal Record: " + std::to_string(record) + " m");

    coinsText.setPosition({(Config::WINDOW_WIDTH - coinsText.getGlobalBounds().size.x) / 2.f, 200.f});
    recordText.setPosition({(Config::WINDOW_WIDTH - recordText.getGlobalBounds().size.x) / 2.f, 150.f});
}

// Se non sono in caricamento posso prendere gli input per iniziare la partita
bool Menu::canSwitch() {
    return !drawBackground;
}

// Checker per vedere se è stato premuto il bottone di inizio partita
bool Menu::isPlayClicked(sf::Vector2f pos) const {
    return spriteButtonPlay.getGlobalBounds().contains(pos); 
}

void Menu::draw(sf::RenderWindow& window) {
    if(drawBackground){
        // Schermata di caricmento
        window.draw(spriteLoadingBg);
        window.draw(loadingText);
    } else {
        // Menu
        window.draw(spritesBg);
        window.draw(titleText);
        window.draw(statsPanel);
        window.draw(recordText);
        window.draw(coinsText);
        window.draw(spriteButtonPlay);
    }
}