#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

//a camera that moves when the mouse moves off screen and zooms in and out
//expected z to never be past 0
struct MouseMoveCamera : public Camera3D
{
    MouseMoveCamera(const Vector3& pos, const Vector2& bounds_);
    /**
     * @brief Moves camera if mouse is out of bounds
     * 
     */
    void handleControls();
    /**
     * @brief Updates position and target xy
     * 
     * @param pos 
     */
    void setPos(const Vector3& pos);
    void addPos(const Vector2& pos);
    void addPos(const Vector3& pos);
private:
    Vector2 bounds;
};

#endif // CAMERA_H