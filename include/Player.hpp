#include "Drawable.hpp"
#include "Entity.hpp"
#include "MovingObject.hpp"

#include <memory>

class Player : public virtual Drawable, public MovingObject {
public:
    enum State {
        Stand,
        Walk,
        Jump,
        DoubleJump,
        GrabLedge
    };

    std::unique_ptr<Entity> myEntity;
    State currentState;
    float jumpSpeed;
    float walkSpeed;
    float weight;

    Player(std::unique_ptr<Entity> a_entity);

    void update(float a_timeDelta);
    bool draw();
};