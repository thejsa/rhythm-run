/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2021
 */

#pragma once

#include "Drawable.hpp"
#include "Entity.hpp"
#include "MovingObject.hpp"

#include <memory>

class Beat : public virtual Drawable, public MovingObject {
public:
    enum State {
        Normal,
        Collected
    };

    std::unique_ptr<Entity> myEntity;
    State currentState;
    long activationTime;

    Beat(long a_activationTime, std::unique_ptr<Entity>& a_entity);

    void update(float a_timeDelta);
    bool draw();
};