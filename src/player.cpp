#include "../include/player.h"
#include "../include/options.h"
#include "../include/card_repo.h"
#include "../include/sprites.h"
#include "../include/sequencer.h"

#include "raylib_helper.h"

#include "raymath.h"


void Player::addCard(CardPtr& ptr, bool addToHand)
{
    if (ptr && hand.size() + board.size() < maxHandSize)
    {
        if (addToHand)
        {
            ptr->onEnterHand(*this);
            hand.addCard(std::move(ptr));
        }
        else if (!addToHand)
        {
            ptr->onEnterBoard(*this);
            board.addCard(std::move(ptr));
        }
    }
}

void Player::addCard(Card::CardIdentifier id, bool hand)
{
    CardPtr ptr = CardsLookup::getCard(id);
    addCard(ptr,hand);
}

void Player::changeHandSize(int amount)
{
    maxHandSize = std::max(maxHandSize + amount,0);
    if (hand.size() >= maxHandSize)
    {
        hand.resize(maxHandSize);
    }
}

void Player::addCardToDeck(Card::CardIdentifier id, int index)
{
    CardPtr ptr = CardsLookup::getCard(id);
    if (ptr)
    {
        if (deck.size() > 1)
        {
            int random = (index == -1) ? rand()%(deck.size()+1) : index;
            if (random > deck.size()) //new last card
            {
                deck.push_back(std::move(ptr));
            }
            else //insert somewhere before the last card
            {
                //TODO: Need to add an option so cards can't be added as the next card when inserted randomly
                deck.insert(deck.begin() + random,std::move(ptr));
            }
        }
        else
        {
            deck.emplace_back(std::move(ptr));
        }
    }       
}

void Player::draw()
{
    if (deck.size() > 0)
    {
        currentEnemy = deck.removeCard(0);//.reset(static_cast<EnemyCard*>(topdeck.release()));
    }
    else
    {
        currentEnemy.reset();
    }
}

void Player::addEffect(EffectCard& effectCard)
{
    for (int i = 0; i < board.size(); i ++)
    {
        if (board[i].get() == &effectCard)
        {
            effects.push_front(i); //only add if the card is in the board. If this function somehow gets called from a card not on the board, ignore it
        }
    }
}

void Player::removeEffect(EffectCard& effectCard)
{
    for (auto it = effects.begin(); it != effects.end(); ++it)
    {
        int index = *it;
        if (index < board.size() && index > 0)
        {
            if (board[index].get() != &effectCard)
            {
                std::cerr << "Warning: Removing effect for card #" << index << " is not the card we were expecting\n";
            }
            effects.erase(it);
            break;
        }
    }
}

EffectCard* Player::getEffect()
{
    return effects.size() > 0 && *effects.begin() < board.size() && *effects.begin() >= 0 ? static_cast<EffectCard*>(board[*effects.begin()].get()) : nullptr;
}

Resources Player::getBoardResources() const
{
    Resources total{};
    for (auto& ptr : board)
    {
        if (ptr.get())
        {
            for (int i = 0; i < Resources::RESOURCE_NAME_SIZE; i ++)
            {
                total[i] += ptr->resources[i];
            }
        }
    }
    return total;
}

void Player::takeDamage(int damage)
{
    health -= damage;
    if (health <= 0)
    {
        gameOver();
    }
}

void Player::addTool(Tool::Type type)
{
    if (type != Tool::NONE)
    {
        tools.push_back(type);
    }
}

void Player::setCurrentTool(int index)
{
    currentTool = index;
}

void Player::removeCurrentTool()
{
    tools.erase(tools.begin() + currentTool);
    currentTool = -1;
}

Tool::Type Player::getCurrentTool() const
{
    return currentTool >= 0 && currentTool < tools.size() ? tools[currentTool] : Tool::NONE;
}


void Player::gameOver()
{
    dead = true;
}

void Player::win()
{
    dead = true;
    won = true;
}

Rectangle Interface::getRegion(const Vector2& topLeft, const Vector2& section)
{
    Vector2 screenDimen = {GetScreenWidth(),GetScreenHeight()};
    return {screenDimen.x*topLeft.x,screenDimen.y*topLeft.y,screenDimen.x*section.x,screenDimen.y*section.y};

}

Rectangle Interface::getIthHandCardRect(int i, const Rectangle& handRect)
{
    return {handRect.x + (i+1)*(Card::CARD_DIMEN.x + HAND_CARD_SEPARATION*handRect.width), handRect.y + handRect.height/2 - Card::CARD_DIMEN.y/2,Card::CARD_DIMEN.x,Card::CARD_DIMEN.y};
}

void Interface::handleRegion(CardList& lst, const Rectangle& rect)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        for (int i = 0; i < lst.size(); i ++)
        {
            Rectangle cardRect = getIthHandCardRect(i,rect);

            if (lst[i].get() && CheckCollisionPointRec(GetMousePosition(),cardRect))
            {
                heldCard = lst.removeCard(i);
                break;
            }
        }
    }  
}

void Interface::handleMouse(Player& player)
{
    Rectangle handRect = getRegion(HAND_TOP_LEFT,HAND_SECTION);
    Rectangle boardRect = getRegion(BOARD_TOP_LEFT,BOARD_SECTION);
    Rectangle deckRect = getRegion(DECK_TOP_LEFT,DECK_SECTION);

    handleRegion(player.hand,handRect);
    handleRegion(player.board,boardRect);

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && heldCard)
    {
        if (CheckCollisionPointRec(GetMousePosition(),handRect))
        {
            player.addCard(heldCard,true);
        }
        else
        {        
            player.addCard(heldCard,false);
        }
    }

    if (player.currentEnemy)
    {
        /*EnemyCard* enemy = static_cast<EnemyCard*>(player.currentEnemy.get());
        Vector2 enemyPos = {deckRect.x + deckRect.width/2,deckRect.y + deckRect.height/2};
        if (enemy->handleInput(GetMousePosition(),player,enemyPos,ENEMY_SCALE))
        {
            Sequences::addSequence([start=GetTime(),enemyName = enemy->name,startPos = enemyPos]()
            {   
                if (GetTime() - start > 1)
                {
                    return true;
                }
                else
                {
                    //fetch a temporary copy of an enemy card for rendering
                    //super turbo ass solution
                    CardPtr tempRender = CardsLookup::getCard(enemyName);
                    if (tempRender)
                    {
                        tempRender->renderCentered(startPos + (Vector2(GetScreenWidth(),startPos.y) - startPos)*(GetTime() - start)/1.0f,0,ENEMY_SCALE);
                    }
                }
                return false;
            });
            player.draw();

        }*/
    }
}

void Interface::renderCardList(const CardList& lst, const Rectangle& rect)
{
    for (int i = 0; i < lst.size(); i ++)
    {
        if (lst[i].get())
        {
            Rectangle cardRect = getIthHandCardRect(i,rect);
            if (CheckCollisionPointRec(GetMousePosition(),cardRect))
            {
                lst[i]->render({cardRect.x - 2*Card::CARD_DIMEN.x,cardRect.y- 2*Card::CARD_DIMEN.y},0,4);
            }
            else
            {
                lst[i]->render({cardRect.x,cardRect.y},0,1);
            }
        }
    }
}

void Interface::update( Player& player)
{
    if (foresight > 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        //TODO: This sucks lol
        foresight = 0;
        return;
    }
    else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (player.getCurrentTool() == Tool::GLASSES || player.getCurrentTool() == Tool::BINOCULARS))
    {
        foresight = 3 + (2)*(player.getCurrentTool() == Tool::BINOCULARS);
        player.removeCurrentTool();
    }

    Vector2 screenDimen = {GetScreenWidth(),GetScreenHeight()};

    Vector2 screenCenter = screenDimen*0.5;

    Rectangle playerHandRect = getRegion(HAND_TOP_LEFT,HAND_SECTION);
    Rectangle playerBoardRect = getRegion(BOARD_TOP_LEFT,BOARD_SECTION);
    Rectangle playerDeckRect = getRegion(DECK_TOP_LEFT,DECK_SECTION);
    Rectangle playerStatsRect = getRegion(HEALTH_AND_HUNGER_TOP_LEFT,HEALTH_AND_HUNGER_SECTION);
    Rectangle playerToolsRect = getRegion({TOOLBOX.x,TOOLBOX.y},{TOOLBOX.z,TOOLBOX.w});

    //DrawRectangleLines(playerDeckRect.x,playerDeckRect.y,playerDeckRect.width,playerDeckRect.height,RED);

    //DrawRectangle(playerStatsRect.x,playerStatsRect.y,playerStatsRect.width,playerStatsRect.height,DARKGREEN);

    //DrawRectangleLines(playerHandRect.x,playerHandRect.y,playerHandRect.width,playerHandRect.height,BLUE); 

    int total = 0;
    for (int i =0 ; i < player.deck.size(); i ++)
    {
        if (player.deck[i] &&  player.deck[i]->name.size() >= 6 && player.deck[i]->name.substr(player.deck[i]->name.size() - 6,6) == "Spirit")
        {
            total ++;
        }
    }


    std::string message = "Cards Left In Deck: " + std::to_string(player.deck.size()) + (total > 0 ? ". Vengeful Spirits: " + std::to_string(total) : "");
    DrawRectangleLines(playerDeckRect.x,playerDeckRect.y,playerDeckRect.width,playerDeckRect.height,RED);
    DrawRectangle(playerDeckRect.x,playerDeckRect.y,playerDeckRect.height,playerDeckRect.height, {169,175,0,255});
    bool hasTool = player.getCurrentTool() == Tool::AXE || player.getCurrentTool() == Tool::WAND;
    if (EnemyCard* ptr = static_cast<EnemyCard*>(player.currentEnemy.get()))
    {
        bool tooled = CheckCollisionPointRec(GetMousePosition(),playerDeckRect) && hasTool ;

        DrawTexturePro(player.currentEnemy->cardArt,
                    {0,0,player.currentEnemy->cardArt.width,player.currentEnemy->cardArt.height},
                    {playerDeckRect.x,playerDeckRect.y,playerDeckRect.height,playerDeckRect.height},{0.5,0.5},0,tooled ? GRAY : WHITE);
        DrawText(player.currentEnemy->name.c_str(),playerDeckRect.x,playerDeckRect.y - .1*playerDeckRect.height,.1*playerDeckRect.height,BLACK);
        if (tooled && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            //TODO: put wand, glove and amybe axe functionality in a function elsewhere to not bloat this file
            switch (player.getCurrentTool())
            {
                case Tool::AXE:
                    player.draw();
                    break;
                case Tool::WAND:
                {
                    const int animalChoices = 5;
                    int animal = rand()%animalChoices;
                    Card::CardIdentifier polymorphs[animalChoices] = {"Sheep", "Frog", "Bear", "Sentient Corn", "Hungry Dragon"};
                    player.currentEnemy = CardsLookup::getCard(polymorphs[animal]);
                    break;
                }
            }
            player.removeCurrentTool();
        }
        else
        {
            for (int i = 0; i < static_cast<EnemyCard*>(player.currentEnemy.get())->options.size(); i ++)
            {
                float optionHeight = playerDeckRect.height*0.3333;
                Rectangle optionRect = {playerDeckRect.x + playerDeckRect.height*1.5, 
                                    playerDeckRect.y + optionHeight*i,
                                    playerDeckRect.width - playerDeckRect.height,
                                    optionHeight};
                bool hammered = player.getCurrentTool() == Tool::HAMMER;
                bool gloved = player.getCurrentTool() == Tool::GLOVE;
                if (ptr->options[i].valid(player) || hammered)
                {
                    DrawRectangle(optionRect.x,optionRect.y,optionRect.width,optionRect.height,LIGHTGRAY);
                    if (CheckCollisionPointRec(GetMousePosition(),optionRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        //option was hammered
                        if ((!ptr->options[i].valid(player) && hammered) || gloved)
                        {
                            player.removeCurrentTool();
                        }
                        
                        if (gloved)
                        {
                            for (int j = 0; j < static_cast<EnemyCard*>(player.currentEnemy.get())->options.size(); j ++)
                            {
                                if (ptr->options[j].valid(player))
                                {
                                    ptr->options[j].effect(player);
                                }
                            }                        
                        }
                        else
                        {
                            ptr->options[i].effect(player);
                        }

                        player.draw();
                        break;
                    }
                }
                else
                {
                    DrawRectangle(optionRect.x,optionRect.y,optionRect.width,optionRect.height,DARKGRAY);
                }
                DrawRectangleLines(optionRect.x,optionRect.y,optionRect.width,optionRect.height,BLACK);
                ptr->options[i].render(optionRect);
            }
        }
    }

    //DrawText(message.c_str(),playerDeckRect.x,playerDeckRect.y,30,WHITE);

    Texture2D boardSprite = SpriteManager::getSprite("board.png");
    /*DrawTexturePro(boardSprite,
        {0,0,boardSprite.width,boardSprite.height},
        playerBoardRect,
        {0.5,0.5},
        0,
        WHITE
    );*/
    //DrawRectangleLines(playerBoardRect.x, playerBoardRect.y, playerBoardRect.width, playerBoardRect.height,PURPLE);


    int x = playerBoardRect.x + playerBoardRect.width*.01f;
    int y = playerBoardRect.y + playerBoardRect.height*.9f - .2*playerStatsRect.width;
    Texture2D heartIcon = SpriteManager::getSprite("health_icon.png");
    Texture2D hungerIcon = SpriteManager::getSprite("hunger_icon.png");
    for (int i = 0; i < player.health; i ++)
    {
        DrawTexturePro(heartIcon,
            {0,0,heartIcon.width,heartIcon.height},
            {x + i*.2*playerStatsRect.width,y, .2*playerStatsRect.width, .2*playerStatsRect.width},{0.5,0.5},0,WHITE);
    }

    /*for (int i = 0; i < player.hunger; i ++)
    {
        DrawTexturePro(hungerIcon,
            {0,0,hungerIcon.width,hungerIcon.height},
            {x + i*.2*playerStatsRect.width, y + .3*playerStatsRect.height, .2*playerStatsRect.width, .2*playerStatsRect.width},{0.5,0.5},0,WHITE);        
    }*/

    if (heldCard)
    {
        heldCard->renderCentered(GetMousePosition(),0,1);
    }


    DrawRectangleLines(playerToolsRect.x,playerToolsRect.y,playerToolsRect.width,playerToolsRect.height,RED);
    const float spacing = playerToolsRect.width/2;
    const float iconDimen = playerToolsRect.width/2;
    for (int i = 0; i < player.tools.size(); i ++)
    {
        Rectangle toolRect = {playerToolsRect.x + playerToolsRect.width/2 - iconDimen/2,playerToolsRect.y + spacing*(i+1) - spacing/2 - iconDimen/2,iconDimen,iconDimen};
        DrawSprite(Tool::getSprite(player.tools[i]),toolRect,0, i == player.currentTool ? GRAY : WHITE);
        if (CheckCollisionPointRec(GetMousePosition(),toolRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            player.setCurrentTool(i);
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        player.setCurrentTool(-1);
    }
    if (player.getCurrentTool() != Tool::NONE)
    {
        DrawSpriteCentered(Tool::getSprite(player.getCurrentTool()),{GetMousePosition().x,GetMousePosition().y,30,30});
    }

    message = std::to_string(player.hand.size() + player.board.size()) + "/" + std::to_string(player.maxHandSize);
    DrawText(message.c_str(),playerHandRect.x + playerHandRect.width/2 - 10,playerHandRect.y,20,WHITE);

    renderCardList(player.hand,playerHandRect);
    renderCardList(player.board,playerBoardRect);
    handleMouse(player);

    if (foresight > 0)
    {
        DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(),Color{100,100,100,120});
        for (int i = 0; i < foresight; i ++)
        {
            const Vector2 dimen = {200,200};
            if (player.deck[i])
            DrawSprite(player.deck[i]->cardArt,{100 + dimen.x*i,GetScreenHeight()/2 - dimen.y/2,dimen.x,dimen.y});
        }
    }


    if (player.dead)
    {
        Rectangle gameOver = {0.1*screenDimen.x,0.1*screenDimen.y,0.7*screenDimen.x,0.7*screenDimen.y};
        DrawRectangle(gameOver.x,gameOver.y,gameOver.width,gameOver.height,GRAY);
        if (player.won)
        {
            DrawText("You lived...",gameOver.x + .1*screenDimen.x,gameOver.y + .1*screenDimen.y,100,GREEN);
        }
        else
        {
            DrawText("YOU DIED",gameOver.x + .1*screenDimen.x,gameOver.y + .1*screenDimen.y,100,RED);
        }
        Rectangle quitButton = {gameOver.x + gameOver.width/2 - 50, gameOver.y + gameOver.height*.75 - 50, 100, 100};
        if (CheckCollisionPointRec(GetMousePosition(),quitButton))
        {
            DrawRectangle(quitButton.x,quitButton.y,quitButton.width,quitButton.height,RED);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                exit(0);
            }
        }
        else
        {
            DrawRectangle(quitButton.x,quitButton.y,quitButton.width,quitButton.height,LIGHTGRAY);
        }
         DrawText("Quit",quitButton.x + 10,quitButton.y + quitButton.height/2 - 10,20,BLACK);
    }



}