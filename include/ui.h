#ifndef UI_H
#define UI_H

#include "raylib.h"

#include "cards.h"

struct UIRenderInfo
{
    Rectangle rect;
};

struct UIElement
{
    virtual void render(const UIRenderInfo& info) = 0;
    virtual void update(const UIRenderInfo& info) = 0;
};

struct CardUI : public UIElement
{
    void render(const UIRenderInfo& info);
    void update(const UIRenderInfo& info);
private:
    bool hovered = false;
    Card::CardIdentifier id;
};

#endif // UI_H