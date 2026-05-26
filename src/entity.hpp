#pragma once
#include "Global.hpp"

enum EntityType{
    TypeCoin, TypeLaser, TypeLaserBeam
};

class Entity {
    public:
        virtual ~Entity() = default;

        virtual void update(sf::Time dt) = 0;
        virtual void draw(sf::RenderWindow& window) = 0;

        virtual sf::Vector2f getPosition() const = 0;
};


class EventOutOfView_BaseHandler {
    protected:
        float lastSpawnX = 0; 
        
        float minDistance;
        float probability;
        float paddingView;
        float paddingSizeY;
        int maxMemory;

    public:
        EventOutOfView_BaseHandler(float dist, float prob, float padding, float paddingSizeY, int mem) 
            : minDistance(dist), probability(prob), paddingView(padding), paddingSizeY(paddingSizeY), maxMemory(mem) {}

        virtual ~EventOutOfView_BaseHandler() = default;

        bool canSpawn(const sf::View& view, float currentSpeed, int currentActiveCount) {
            float viewRight = view.getCenter().x + (view.getSize().x / 2.f);

            float dynamicMinDistance = ((minDistance * (currentSpeed /10)));

            //log(Debug::ENTITY, Debug::DBG, "dynamicMinDistance  = ",  dynamicMinDistance);
            
            if (viewRight - lastSpawnX > dynamicMinDistance) {
                if (Utils::shouldOrNot(probability)) {
                    if (currentActiveCount < maxMemory) {
                        return true;
                    }
                }
            }
            return false;
        }

        void resetSpawnDistance(float currentX) {
            lastSpawnX = currentX;
        }

        float getNextSpawnX(float viewRight) const { 
            return viewRight + paddingView; 
        }
};

class EventInView_BaseHandler {
    protected:
        float eventTimer;
        float timeToNextEvent;
        bool isEventActive;

        float minTimeSpawn;
        float maxTimeSpawn;
        
        // Reset dei parametri vitali per la generazione
        void resetEventTimer() {
            eventTimer = 0.0f;
            isEventActive = false;
            timeToNextEvent = std::uniform_real_distribution<float>(minTimeSpawn, maxTimeSpawn)(generator);
        }

    public:
        EventInView_BaseHandler(float minTime, float maxTime) 
            : eventTimer(0.0f), timeToNextEvent(0.0f), isEventActive(false), minTimeSpawn(minTime), maxTimeSpawn(maxTime) {
                resetEventTimer();
            }

        virtual ~EventInView_BaseHandler() = default;

        bool shouldTriggerEvent(float dt) {
            if (!isEventActive) {
                eventTimer += dt;
                if (eventTimer >= timeToNextEvent) {
                    isEventActive = true;
                    return true; 
                }
            }
            return false;
        }

        void setEventFinished() {
            resetEventTimer();
        }
};