#include "managerAllGame.hpp"

ManagerAllGame::ManagerAllGame() :

    state(State::MENU),
    window(sf::VideoMode({Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}), Config::WINDOW_TITLE),
    view(sf::View(sf::FloatRect({0.f, 0.f}, {Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}))),
    currentSpeed(Config::SCROLL_SPEED),
    speed(currentSpeed),
    waitActionAfterDead(false),
    menu(std::make_unique<Menu>()),
    game(std::make_unique<Game>()),
    traceCoinScore(0,0)
    
{
    
    window.setFramerateLimit(Config::MAXFRAMERATE);
}

void ManagerAllGame::run() {
    while (window.isOpen()) {
        sf::Time time = clock.restart();

        processEvents(); 
        update(time);    
        render();        
    }
}


// Tutto quello che riguarda il caricamento di una nuova partita deve essere inserito qua
// Reset completo dei parametri della classe
void ManagerAllGame::reload(){
    state = State::MENU;
    waitActionAfterDead = false;

    // Lo score se > lo sostituisco mentre per le monete tengo traccia per un futuro update per un negozio per le skin
    std::pair<int,int> thisRun = game->getHudInfo(); // second score // first coin
    if(thisRun.second > traceCoinScore.second) traceCoinScore.second = thisRun.second;
    traceCoinScore.first += thisRun.first;

    game = std::make_unique<Game>();

    currentSpeed = Config::SCROLL_SPEED;
    speed = 0.f; 
    
    view = sf::View(sf::FloatRect({0.f, 0.f}, {Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}));
    window.setView(view);
}


// Handler di tutti gli eventi 
void ManagerAllGame::processEvents() {
    window.handleEvents(
        
        [this](const sf::Event::Closed&) { window.close(); },
        [this](const sf::Event::MouseButtonPressed& event) {
            if(state == State::MENU) {
                
                sf::Vector2f mousePos = window.mapPixelToCoords(event.position);

                if (menu->isPlayClicked(mousePos)) {
                    state = State::GAME; 
                    game->playMusic();
                }
            } 
          },
   

        [this](const sf::Event::KeyPressed& event) {

            // Gestione menu
            if (state == State::MENU) {
                if (event.code == sf::Keyboard::Key::Enter) {
                    if(!menu->canSwitch()) return;
                    state = State::GAME;
                    game->playMusic();
                }
            } // Gestione gioco
            else if (state == State::GAME) {
                if (waitActionAfterDead) {
                    // Nel caso sia morto e prema spazio
                    if (event.code == sf::Keyboard::Key::Space) {
                        reload();
                    }
                } else { 
                    // Evento in un constesto normale 
                    game->keyPressed(event);
                }
            }
        },
        
        [this](const sf::Event::KeyReleased& event) { 
            if (state == State::GAME && !waitActionAfterDead) {
                game->KeyReleased(event); 
            }
        }
    );
}


// Update di game che si bassa sul tempo la view e la speed di conseguenza aggiorno tutti questi dati se mi ritrovono nella casistica game non morto
//  nel menu mi occupo di aggiornare per l'effetto loading iniziale + il meno in generale con i suoi effetti sul pulsante, mentre la updateStats serve a 
//  aggiornare i dati di coin e score nel menu
void ManagerAllGame::update(sf::Time time) {
    if (state == State::GAME) {
        
        if (waitActionAfterDead) return; //

        if(currentSpeed <= Config::PLAYER_MAX_VELOCITY) {
            currentSpeed += Config::ACCELERATION_RATE * time.asSeconds();
        }

        speed = currentSpeed * time.asSeconds();
        view.move({speed, 0});

        game->update(time, view, speed);

        if (game->isDead()) {
            waitActionAfterDead = true;
        }

    } else if (state == State::MENU) {
        menu->update(window, time);
        menu->updateStats(traceCoinScore.first, traceCoinScore.second);
    }
}


// Disegna game / menu
void ManagerAllGame::render() {
    window.clear(); 
    
    if (state == State::GAME) {
        
        window.setView(view);   
        game->draw(window);
        window.setView(window.getDefaultView());

    } else if (state == State::MENU) {
        menu->draw(window);
    }
    
    window.display();
}
