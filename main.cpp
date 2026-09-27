
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

#define _USE_MATH_DEFINES
#include <math.h>

#include <raylib.h>
#include <raymath.h>

#include "include/camera.h"
#include "include/frames.h"

#include "include/player.h"
#include "include/cards.h"
#include "include/card_repo.h"
#include "include/sprites.h"
#include "include/actions.h"
#include "include/options.h"
#include "include/globals.h"

#define PLATFORM_DESKTOP

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const Vector2 screenDimen = {1000,1000};
    SetConfigFlags( FLAG_VSYNC_HINT);
    InitWindow(screenDimen.x, screenDimen.y, "raylib [core] example - basic window");
    InitAudioDevice();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    SpriteManager::loadSprites("./sprites");
    CardsLookup::loadCards();


    Player& player = Globals::player;

    for (int i = 0; i < 5; i++)
    {
        player.hand.emplace_back(new Card("BALLS",{},GRAY,{5,4}));
    }

    player.currentEnemy.reset(new EnemyCard("Evil man",{},{
        Option({Actions::AddCardsToHand({"Apple"})}),
        Option({Actions::AddCardsToDeck({"Apple"})},{1})
        }));

    player.addCardToDeck("Kind Grandma");

    Interface interface;

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        Frames::accumulate();

        do
        {
            //physics loop
        }
        while(Frames::hasFramesLeft());

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(WHITE);
                
            interface.update(player);

            DrawFPS(10, 10);

        EndDrawing();

        //----------------------------------------------------------------------------------
    }


    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
