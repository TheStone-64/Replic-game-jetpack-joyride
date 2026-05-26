#include "coin.hpp"

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

void SingleCoin::collect() { 
    collected = true; 
}

bool SingleCoin::isCollected() const { 
    return collected; 
}

void SingleCoin::move(sf::Vector2f offset) { 
    sprite.move(offset);
}

sf::Vector2f SingleCoin::getPosition() const  { 
    return sprite.getPosition(); 
}

sf::FloatRect SingleCoin::getBounds() const { 
    return sprite.getGlobalBounds(); 
}
        
// Rotazione della skin della singola moneta
void SingleCoin::update(sf::Time dt) {
    animationTimer += dt.asSeconds();
    if (animationTimer >= Config::MIN_TIME_ROTATE_SKIN) {
        currentFrame = (currentFrame + 1) % COIN_UTILS::COIN_SKIN;
        sprite.setTexture(textures[currentFrame]);
        animationTimer = Config::COIN_DEFAULT_ANIMATION_TIMER;
    }
}

void SingleCoin::draw(sf::RenderWindow& window)  {
    if (!collected) window.draw(sprite);
}

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################
// Metodi di disegno

void CoinPattern::createSquare(sf::Vector2f offset) {
    for (int x = 0; x < Config::COIN_FIGURE_SQUARE; ++x) {
        for (int y = 0; y < Config::COIN_FIGURE_SQUARE; ++y) {
            coins.push_back(std::make_unique<SingleCoin>(
                sf::Vector2f(offset.x + x * Config::COIN_SPACING, offset.y + y * Config::COIN_SPACING)
            ));
        }
    }
}

void CoinPattern::createSpecialRect(sf::Vector2f offset) {
    // Riga 1: ---- (4 monete centrate)
    for(int i = 1; i <= Config::COIN_FIGURE_RECTANGLE_LITTLE_LINE; ++i) coins.push_back(std::make_unique<SingleCoin>(sf::Vector2f(offset.x + i* Config::COIN_SPACING, offset.y)));
    // Riga 2: ------ (6 monete)
    for(int i = 0; i < Config::COIN_FIGURE_RECTANGLE_BIG_LINE; ++i)  coins.push_back(std::make_unique<SingleCoin>(sf::Vector2f(offset.x + i* Config::COIN_SPACING, offset.y + Config::COIN_SPACING)));
    // Riga 3: ---- (4 monete centrate)
    for(int i = 1; i <= Config::COIN_FIGURE_RECTANGLE_LITTLE_LINE; ++i) coins.push_back(std::make_unique<SingleCoin>(sf::Vector2f(offset.x + i* Config::COIN_SPACING, offset.y + 2 * Config::COIN_SPACING)));
}

void CoinPattern::createSnake(sf::Vector2f offset) {
    int segmentLength = Config::COIN_FIGURE_SNAKE_SEGNMENT_LENGHT;
    
    // Primo pezzo ALTO (----)
    for(int c = 0; c < segmentLength; ++c) {
        coins.push_back(std::make_unique<SingleCoin>(sf::Vector2f(offset.x + c * Config::COIN_SPACING, offset.y)));
    }

    // Secondo pezzo BASSO (____) - Parte da dove finisce il primo
    for(int c = 0; c < segmentLength; ++c) {
        coins.push_back(std::make_unique<SingleCoin>(sf::Vector2f(offset.x + (c + segmentLength) * Config::COIN_SPACING, offset.y + Config::COIN_SPACING)));
    }

    // Terzo pezzo ALTO (----) - Parte da dove finisce il secondo
    for(int c = 0; c < segmentLength; ++c) {
        coins.push_back(std::make_unique<SingleCoin>(sf::Vector2f(offset.x + (c + segmentLength * 2) * Config::COIN_SPACING, offset.y)));
    }
}

void CoinPattern::createPyramid(sf::Vector2f offset) {
    int maxCols = Config::COIN_FIGURE_PYRAMID_BASE; // 5
    int rows = Config::COIN_FIGURE_PYRAMID_LAYERS;  // 3
    int mid = maxCols / 2; // Centro della piramide 2
    
    for (int y = 0; y < rows; ++y) {
        int colsInRow = 1 + y * 2; // 1, 3, 5...
        int startX = mid - y;      
        
        for (int i = 0; i < colsInRow; ++i) {
            coins.push_back(std::make_unique<SingleCoin>(
                sf::Vector2f(offset.x + (startX + i) * Config::COIN_SPACING, offset.y + y * Config::COIN_SPACING)
            ));
        }
    }
}


void CoinPattern::createDiamond(sf::Vector2f offset) {
    int size = Config::COIN_FIGURE_DIAMOND_SIZE; // 5
    int mid = size / 2; // 2
    
    for (int y = 0; y < size; ++y) {
        int distFromMid = std::abs(mid - y);
        int colsInRow = size - (distFromMid * 2);
        int startX = distFromMid;
        
        for (int i = 0; i < colsInRow; ++i) {
            coins.push_back(std::make_unique<SingleCoin>(
                sf::Vector2f(offset.x + (startX + i) * Config::COIN_SPACING, offset.y + y * Config::COIN_SPACING)
            ));
        }
    }
}

// Crea un rettangolo vuoto al centro 
void CoinPattern::createRing(sf::Vector2f offset) {
    int w = Config::COIN_FIGURE_RING_WIDTH;  // 5
    int h = Config::COIN_FIGURE_RING_HEIGHT; // 4
    
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {

            // Disegna solo sui bordi esterni
            if (x == 0 || x == w - 1 || y == 0 || y == h - 1) {
                coins.push_back(std::make_unique<SingleCoin>(
                    sf::Vector2f(offset.x + x * Config::COIN_SPACING, offset.y + y * Config::COIN_SPACING)
                ));
            }
        }
    }
}

void CoinPattern::createWave(sf::Vector2f offset) {
    // Schema dell'onda sull'asse Y (1, 0, 1, 2, 1, 0, 1, 2)
    int yOffsets[] = {1, 0, 1, 2, 1, 0, 1, 2}; 
    int actualLen = Config::COIN_FIGURE_WAVE_LENGTH; // 8
    
    for (int x = 0; x < actualLen; ++x) {
        coins.push_back(std::make_unique<SingleCoin>(
            sf::Vector2f(offset.x + x * Config::COIN_SPACING, offset.y + yOffsets[x] * Config::COIN_SPACING)
        ));
    }
}

// #########################################################################################################################################################################
int CoinPattern::getCoins () const {
    return coins.size();
} 

// #########################################################################################################################################################################

int CoinPattern::checkCollisions(const sf::FloatRect& playerBounds){
    int numberCoinHit = 0;
    for(auto& coin : coins){
        if(!coin->isCollected() && playerBounds.findIntersection(coin->getBounds())){
            coin->collect();
            numberCoinHit++;
        }
    }
    
    if(numberCoinHit != 0)
        coinSound.play();
    
    return numberCoinHit;
}
        

// Aggiorno i singoli coin, inoltre tolgo i coin out of view
void CoinPattern::update(const sf::View& view, sf::Time dt) {
    
    for (auto& coin : coins) {
        coin->update(dt);
    }

    // Se gli elementi sono al di fuori della view li tolgo
    coins.erase(
    std::remove_if(
            coins.begin(),
            coins.end(), 
            [&view](const auto& coin) 
            {
                return (coin->getPosition().x + Config::COIN_OFFSET < view.getCenter().x - (view.getSize().x / 2.f));
            }
        ), 
        coins.end()
    );
}

void CoinPattern::draw(sf::RenderWindow& window) {
    for (auto& coin : coins) {
        coin->draw(window);
    }
}

// Funzione speciale
std::vector<sf::FloatRect> CoinPattern::getOccupiedAreas() const {
    std::vector<sf::FloatRect> areas;
    areas.reserve(coins.size());
    
    for (const auto& coin : coins) {
        if (!coin->isCollected()) { 
            areas.push_back(coin->getBounds());
        }
    }
    return areas;
}

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

bool CoinHandler::shouldCreate(const sf::View& view, float incrementDistance) {
    return canSpawn(view, incrementDistance, activePatterns.size());
}

// Elimino dalla memoria tutti gli insemi di moneti vuoti 
void CoinHandler::clearElementActivePattern () {
    activePatterns.erase(
        std::remove_if(
            activePatterns.begin(), 
             activePatterns.end(), 
             [](const auto& p) {
                    return p->getCoins() == 0;
                }
        ), 
        activePatterns.end()
    );
}

CoinType CoinHandler::generateCoinType() {
    return static_cast<CoinType>(
        std::uniform_int_distribution<int>(0, Config::COIN_NUMBER_ALGORITHM)(generator)
    );
}

// Data una sequenza generata solitamente da CoinHandler::generateCoinType() lo aggiungo al vettore di pattern
void CoinHandler::addCoinSequence(CoinType choice, sf::Vector2f spawnPos) {
    auto newPattern = std::make_unique<CoinPattern>();

    switch (choice) {
        case CoinType::SQUARE:     
            newPattern->createSquare(spawnPos);
            break;
        case CoinType::RECTANGLE:     
            newPattern->createSpecialRect(spawnPos);
            break;
        case CoinType::SNAKE:
            newPattern->createSnake(spawnPos);
            break;
        case CoinType::PYRAMID:
            newPattern->createPyramid(spawnPos);
            break;
        case CoinType::DIAMOND:
            newPattern->createDiamond(spawnPos);
            break;
        case CoinType::RING:
            newPattern->createRing(spawnPos);
            break;
        case CoinType::WAVE:
            newPattern->createWave(spawnPos);
            break;
        default:
            newPattern->createSquare(spawnPos);
            break;
    }

    activePatterns.push_back(std::move(newPattern)); 
}

int CoinHandler::checkCollisions(const sf::FloatRect& playerBounds) {
    int numberCoin = 0;
    for(auto& seqCoin : activePatterns){
        numberCoin  += seqCoin->checkCollisions(playerBounds);
    }

    return numberCoin;
}

// Aggiorno i blocchi di monete che aggiorneranno a loro volta le singole monete e cancello le monete dalla memoria sia le singole che i blocchi 
void CoinHandler::update(sf::Time dt, const sf::View& view) {
    for (auto& p : activePatterns) { 
        p->update(view, dt);
    }

    clearElementActivePattern();
}

void CoinHandler::draw(sf::RenderWindow& window) {
    for(const auto& seqcoin : activePatterns){
        seqcoin->draw(window);
    }
}

std::vector<sf::FloatRect> CoinHandler::getOccupiedAreas() {
    std::vector<sf::FloatRect> totalAreas;

    for (const auto& pattern : activePatterns) {
        std::vector<sf::FloatRect> patternAreas = pattern->getOccupiedAreas();

        totalAreas.insert(totalAreas.end(), patternAreas.begin(), patternAreas.end());
    }
    
    return totalAreas;
}