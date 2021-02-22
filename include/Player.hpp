/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2021
 */

#include "Entity.hpp"

/**
 * Player class
 */

class Player : public Entity
{
public:
    void jump();
private:
    float velocity_x;
};