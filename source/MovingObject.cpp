#include "MovingObject.hpp"
#include "debug.hpp"

void MovingObject::updatePhysics(float a_timeDelta)
{
    // store old position and speed
    oldPosition = position;
    oldSpeed = speed;

    // store previous walls state
    wasOnGround = isOnGround;
    wasAtCeiling = isAtCeiling;
    pushedRightWall = pushesRightWall;
    pushedLeftWall = pushesLeftWall;

    // update speed based on acceleration
    speed.x += accel.x * a_timeDelta;
    speed.y += accel.y * a_timeDelta;

    // update position based on speed and delta
    position.x += speed.x * a_timeDelta;
    // y is flipped (top left @ 0,0)
    position.y -= speed.y * a_timeDelta;

    // check if object is on floor
    if (position.y >= 240.0f) {
        isOnGround = true;
        position.y = 240.0f;
    } else {
        isOnGround = false;
    }

    myAABB.center.x = position.x + aabbOffset.x;
    myAABB.center.y = position.y + aabbOffset.y;
}