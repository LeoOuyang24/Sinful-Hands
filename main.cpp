
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <time.h>

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
#include "include/sequencer.h"

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

    srand(time(NULL));
    CardsLookup::loadCards();

    Player& player = Globals::player;

    for (int i = 0; i < 2; i++)
    {
        player.addCard("Potato", true);
        player.addCard("Dagger",true);
    }
    player.addCard("Allowance",true);
    player.addCard("Single Coin",true);

    for (int i = 0; i < CardsLookup::startingDeck.size(); i ++)
    {
        player.addCardToDeck(CardsLookup::startingDeck[i]);
    }

    player.currentEnemy = CardsLookup::getCard("Kind Vendor"); 
    player.deck.insert(player.deck.begin(),CardsLookup::getCard("Castle Gates"));

    Interface interface;

    Texture2D bg = SpriteManager::getSprite("background.png");

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        Frames::accumulate();

        do
        {
            //physics loop
        }
        while(Frames::hasFramesLeft());

        if (IsKeyPressed(KEY_SPACE))
        {
            for (int i = 0; i < player.deck.size(); i ++)
            {
                std::cout << player.deck[i]->name << "\n";
            }
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(WHITE);
            DrawTexturePro(bg,{0,0,bg.width,bg.height},{0,0,screenDimen.x,screenDimen.y},{0.5,0.5},0,WHITE);
                
            interface.update(player);

            Sequences::update();

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
