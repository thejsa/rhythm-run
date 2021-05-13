#include "Geometry.hpp"

class MovingObject {
public:
    MovingObject() {};

    // positions
    Vec2D oldPosition;
    Vec2D position;

    // speeds
    Vec2D oldSpeed;
    Vec2D speed;

    // object scale
    // Vec2D scale;

    // Axis-Aligned BBox and offset
    AABB myAABB;
    Vec2D aabbOffset;

    // Walls logic
    bool pushedRightWall;
    bool pushesRightWall;

    bool pushedLeftWall;
    bool pushesLeftWall;

    bool wasOnGround;
    bool isOnGround;

    bool wasAtCeiling;
    bool isAtCeiling;

    virtual void update(float a_timeDelta) = 0;
    void updatePhysics(float a_timeDelta);
};