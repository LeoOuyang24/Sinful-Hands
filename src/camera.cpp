
#include <iostream>

#include "../include/camera.h"

#include "raymath.h"


MouseMoveCamera::MouseMoveCamera(const Vector3& pos, const Vector2& bounds_) : bounds(bounds_)
{
    position = pos; 
    target = {pos.x,pos.y,0};
    up = {0,-1,0};
    fovy = 45; 
    projection = CAMERA_PERSPECTIVE;
}

void MouseMoveCamera::setPos(const Vector3& pos)
{
    position = pos;

    target.x = pos.x;
    target.y = pos.y;
}

void MouseMoveCamera::addPos(const Vector2& pos)
{
    setPos(position + Vector3{pos.x,pos.y,position.z});
}

void MouseMoveCamera::addPos(const Vector3& pos)
{
    setPos(position + pos);
}

void MouseMoveCamera::handleControls()
{
    Vector2 mousePos = GetMousePosition();

    Vector2 disp = {};

    const float speed = 15;

    if (mousePos.x >= 0.9*bounds.x && mousePos.x <= bounds.x)
    {
        disp += Vector2{speed*GetFrameTime(),0};
    }
    else if (mousePos.x <= 0.1*bounds.x && mousePos.x >= 0)
    {
        disp += Vector2{-speed*GetFrameTime(),0};
    }

    if (mousePos.y >= 0.9*bounds.y && mousePos.y <= bounds.y)
    {
        disp += Vector2{0,speed*GetFrameTime()};
    }
    else if (mousePos.y <= 0.1*bounds.y && mousePos.y >= 0)
    {
        disp += Vector2{0,-speed*GetFrameTime()};
    }

    float scrollAmount = GetMouseWheelMove();

    addPos({disp.x,disp.y,scrollAmount*100*GetFrameTime()});
}