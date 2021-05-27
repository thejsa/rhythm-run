#include "Beat.hpp"
#include "Constants.hpp"

#include "debug.hpp"
#include <3ds.h>

Beat::Beat(long a_activationTime, std::unique_ptr<Entity>& a_entity)
    : activationTime(a_activationTime)
{
    // take ownership of entity
    myEntity = std::move(a_entity);
    position = {
        myEntity->getX(),
        myEntity->getY()
    };
    printf("t=%ld posX=%.02f posY=%.02f\n", activationTime, position.x, position.y);
    oldPosition = position;

    speed = { 0, 0 };
    accel = { 0, 0 };
    oldSpeed = speed;

    currentState = State::Normal;
}

void Beat::update(float a_timeDelta)
{
    // handle keypad input
    // u32 kHeld = hidKeysHeld();

    switch (currentState) {
    case State::Collected:
        // eprintf("State::Stand\n");
        speed = { 0, 0 };
        break;
    case State::Normal:
        break;
    }

    updatePhysics(a_timeDelta);

    myEntity->setPosition(position.x, position.y);

    // if ((!wasOnGround && isOnGround)
    //     || (!wasAtCeiling && isAtCeiling)
    //     || (!pushedLeftWall && pushesLeftWall)
    //     || (!pushedRightWall && pushesRightWall)) {
    //     // Beat has hit the walls
    //     eprintf("hit walls\n");
    // }
};

bool Beat::draw()
{
    return myEntity->draw();
}