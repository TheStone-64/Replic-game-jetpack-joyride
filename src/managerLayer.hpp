#pragma once
#include "Global.hpp"
#include "coin.hpp"
#include "laser.hpp"
#include "laserBeam.hpp"

class ManagerLayer {
    private:
        std::unique_ptr<CoinHandler> coin;
        std::unique_ptr<LaserHandler> laser;
        std::unique_ptr<LaserBeamHandler> laserBeam;

        std::map<EntityType, std::vector<sf::FloatRect>> areaTaken;
        std::vector<sf::FloatRect> getAllOccupiedAreas() const;
        
    public: 

        ManagerLayer() : 
            coin(std::make_unique<CoinHandler>()), 
            laser(std::make_unique<LaserHandler>()), 
            laserBeam(std::make_unique<LaserBeamHandler>()) {}

        template <typename EntityType, typename SpawnCallback>
        bool handlerGenericUpdate (
            std::unique_ptr<EntityType>& obj, 
            sf::Vector2f targetSize, 
            const std::vector<sf::FloatRect>& allOccupied, 
            const sf::View& view, 
            float speed, 
            int maxTry, 
            SpawnCallback onSpawn 
        );
        bool coin_handlerCallGenericUpdate(const sf::View& view, float speed, const std::vector<sf::FloatRect>& allOccupied);
        bool laser_handlerCallGenericUpdate(const sf::View& view, float speed, const std::vector<sf::FloatRect>& allOccupied);

        std::pair<int,bool> update(sf::Time time, const sf::View& view, const sf::FloatRect& player, float speed);
        void draw(sf::RenderWindow& window);
};