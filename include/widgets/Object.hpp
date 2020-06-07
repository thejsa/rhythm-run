/**
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#include <3ds.h>

namespace widgets {

/**
 * @class Object
 * @brief an abstract Object class for widgets to inherit from
 */ 
class Object {
    /// @brief Which screen is this Object on?
    /// @note e.g GFX_TOP, GFX_BOTTOM
    gfxScreen_t screen;
    /// @brief Width
    float width;
    /// @brief Height
    float height;
    /// @brief Center X coordinate
    float center_x;
    /// @brief Center Y coordinate
    float center_y;
    /// @brief X coordinate
    float x;
    /// @brief Y coordinate
    float y;

    public:
    /// @brief Object constructor
    /// @param screen Which screen is this Object on?
    /// @param width,height Dimensions of the Object.
    /// @param center_x,center_y Coordinates of the Object's center, within itself.
    /// @param x,y Coordinates of the position in 2D space.
    Object(gfxScreen_t screen, float width, float height, float center_x, float center_y, float x, float y);
    
    /// @brief Object destructor
    ~Object();

    /// @brief Get width.
    /// @returns Width of the Object.
    float getWidth(void);
};

}