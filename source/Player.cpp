#include "Player.hpp"
#include "Constants.hpp"

#include "debug.hpp"
#include <3ds.h>

Player::Player(std::unique_ptr<Entity> a_entity)
    : weight(20.0f)
{
    // take ownership of entity
    myEntity = std::move(a_entity);
    position = {
        myEntity->getCenterXRaw(),
        myEntity->getCenterYRaw()
    };
    oldPosition = position;

    speed = { 0, 0 };
    oldSpeed = speed;

    jumpSpeed = Constants::JUMP_SPEED;
    walkSpeed = Constants::WALK_SPEED;

    currentState = State::Jump;
}

void Player::update(float a_timeDelta)
{
    // handle keypad input
    u32 kHeld = hidKeysHeld();

    switch (currentState) {
    case State::Stand:
        // eprintf("State::Stand\n");
        speed = { 0, 0 };

        // if the character is not on the ground, it must be jumping
        if (!isOnGround) {
            currentState = State::Jump;
            break;
        }

        // if left XOR right is held, must be walking
        if (kHeld & KEY_A) {
            speed.y = jumpSpeed * weight;
            currentState = State::Jump;
            break;
        } else if ((kHeld & KEY_LEFT) != (kHeld & KEY_RIGHT)) {
            currentState = State::Walk;
        }
        break;
    case State::Walk:
        // eprintf("State::Walk\n");
        // walk animation not implemented

        // if right and left are both held or released, stand still
        if ((kHeld & KEY_RIGHT) == (kHeld & KEY_LEFT)) {
            currentState = State::Stand;
            speed = { 0, 0 };
            // break;
        } else if (kHeld & KEY_RIGHT) {
            if (pushesRightWall)
                speed.x = 0.0f;
            else
                speed.x = walkSpeed * weight;
        } else if (kHeld & KEY_LEFT) {
            if (pushesLeftWall)
                speed.x = 0.0f;
            else
                speed.x = -walkSpeed * weight;
        }

        // handle jump
        if (kHeld & KEY_A) {
            speed.y = jumpSpeed * weight;
            currentState = State::Jump;
        } else if (!isOnGround) {
            // falling, so jump state but don't add Y speed
            currentState = State::Jump;
        }
        break;
    case State::Jump:
        // eprintf("State::Jump\n");
        // jump animation not implemented

        // apply gravity, but cap falling speed at terminal velocity
        speed.y += -Constants::GRAVITY * weight * a_timeDelta;
        speed.y = std::min(speed.y, Constants::TERMINAL_VELOCITY);

        // if jump button not held, reduce speed
        // if (!(kHeld & KEY_A) && speed.y > 0.0f)
        // speed.y = std::max(speed.y, Constants::MIN_JUMP_SPEED);

        // if right and left are both held or released, stand still
        if ((kHeld & KEY_RIGHT) == (kHeld & KEY_LEFT)) {
            // currentState = State::Stand;
            speed.x = 0;
            // break;
        } else if (kHeld & KEY_RIGHT) {
            if (pushesRightWall)
                speed.x = 0.0f;
            else
                speed.x = walkSpeed * weight;
        } else if (kHeld & KEY_LEFT) {
            if (pushesLeftWall)
                speed.x = 0.0f;
            else
                speed.x = -walkSpeed * weight;
        }

        // if we hit the ground
        if (isOnGround) {
            // if standing still
            if ((kHeld & KEY_LEFT) == (kHeld & KEY_RIGHT)) {
                currentState = State::Stand;
                speed = { 0, 0 };
            } else {
                // walk state
                currentState = State::Walk;
                speed.y = 0.0f;
            }
            // hit floor sfx?
        }
        break;

    case State::GrabLedge:
        // eprintf("State::GrabLedge\n");
        // not implemented
        eprintf("** TODO implement GrabLedge**\n");
        break;
    }

    updatePhysics(a_timeDelta);

    myEntity->setPosition(position.x, position.y);

    if ((!wasOnGround && isOnGround)
        || (!wasAtCeiling && isAtCeiling)
        || (!pushedLeftWall && pushesLeftWall)
        || (!pushedRightWall && pushesRightWall)) {
        // player has hit the walls
        eprintf("hit walls\n");
    }
};

bool Player::draw()
{
    return myEntity->draw();
}