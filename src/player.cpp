#include "../include/player.h"
#include "../include/options.h"
#include "../include/card_repo.h"
#include "../include/sprites.h"

#include "raymath.h"


void Player::addCard(CardPtr& ptr, bool addToHand)
{
    if (ptr)
    {
        if (addToHand && hand.size() < MAX_HAND_SIZE)
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

void Player::addCardToDeck(Card::CardIdentifier id)
{
    CardPtr ptr = CardsLookup::getCard(id);
    if (ptr)
    {
        if (deck.size() > 1)
        {
            deck.insert(deck.begin() + rand()%(deck.size()-1) + 1,std::move(ptr));
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

void Player::gameOver()
{
    dead = true;
}

Rectangle Interface::getRegion(const Vector2& topLeft, const Vector2& section)
{
    Vector2 screenDimen = {GetScreenWidth(),GetScreenHeight()};
    return {screenDimen.x*topLeft.x,screenDimen.y*topLeft.y,screenDimen.x*section.x,screenDimen.y*section.y};

}

Rectangle Interface::getIthHandCardRect(int i, const Rectangle& handRect)
{
    return {handRect.x + i*(Card::CARD_DIMEN.x + HAND_CARD_SEPARATION*handRect.width), handRect.y + handRect.height/2 - Card::CARD_DIMEN.y/2,Card::CARD_DIMEN.x,Card::CARD_DIMEN.y};
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
        EnemyCard* enemy = static_cast<EnemyCard*>(player.currentEnemy.get());
        Vector2 enemyPos = {deckRect.x + deckRect.width/2,deckRect.y + deckRect.height/2};
        if (enemy->handleInput(GetMousePosition(),player,enemyPos,ENEMY_SCALE))
        {
            player.draw();
        }
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
    Vector2 screenDimen = {GetScreenWidth(),GetScreenHeight()};

    Vector2 screenCenter = screenDimen*0.5;

    Rectangle playerHandRect = getRegion(HAND_TOP_LEFT,HAND_SECTION);
    Rectangle playerBoardRect = getRegion(BOARD_TOP_LEFT,BOARD_SECTION);
    Rectangle playerDeckRect = getRegion(DECK_TOP_LEFT,DECK_SECTION);
    Rectangle playerStatsRect = getRegion(HEALTH_AND_HUNGER_TOP_LEFT,HEALTH_AND_HUNGER_SECTION);

    DrawRectangleLines(playerDeckRect.x,playerDeckRect.y,playerDeckRect.width,playerDeckRect.height,RED);

    DrawRectangle(playerStatsRect.x,playerStatsRect.y,playerStatsRect.width,playerStatsRect.height,DARKGREEN);

    DrawRectangleLines(playerHandRect.x,playerHandRect.y,playerHandRect.width,playerHandRect.height,BLUE); 

    DrawRectangleLines(playerBoardRect.x, playerBoardRect.y, playerBoardRect.width, playerBoardRect.height,PURPLE);

    renderCardList(player.hand,playerHandRect);
    renderCardList(player.board,playerBoardRect);

    int x = playerStatsRect.x + playerStatsRect.width*.1f;
    int y = playerStatsRect.y + playerStatsRect.height*.1f;
    Texture2D heartIcon = SpriteManager::getSprite("health_icon.png");
    Texture2D hungerIcon = SpriteManager::getSprite("hunger_icon.png");
    for (int i = 0; i < player.health; i ++)
    {
        DrawTexturePro(heartIcon,
            {0,0,heartIcon.width,heartIcon.height},
            {x + i*.2*playerStatsRect.width,y, .2*playerStatsRect.width, .2*playerStatsRect.width},{0.5,0.5},0,WHITE);
    }

    for (int i = 0; i < player.hunger; i ++)
    {
        DrawTexturePro(hungerIcon,
            {0,0,hungerIcon.width,hungerIcon.height},
            {x + i*.2*playerStatsRect.width, y + .3*playerStatsRect.height, .2*playerStatsRect.width, .2*playerStatsRect.width},{0.5,0.5},0,WHITE);        
    }

    if (heldCard)
    {
        heldCard->renderCentered(GetMousePosition(),0,1);
    }

    if (player.currentEnemy > 0)
    {
        player.currentEnemy->renderCentered({playerDeckRect.x + playerDeckRect.width/2, playerDeckRect.y + playerDeckRect.height/2},0,ENEMY_SCALE);
    }
    
    if (player.dead)
    {
        Rectangle gameOver = {0.1*screenDimen.x,0.1*screenDimen.y,0.7*screenDimen.x,0.7*screenDimen.y};
        DrawRectangle(gameOver.x,gameOver.y,gameOver.width,gameOver.height,GRAY);
        DrawText("YOU DIED",gameOver.x + .1*screenDimen.x,gameOver.y + .1*screenDimen.y,100,RED);
    }

    handleMouse(player);


}