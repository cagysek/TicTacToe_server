//
//  Game.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 12/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include "Game.hpp"

/**
 *  Constructor for game
 */
Game::Game(int id, Player *p1, Player *p2)
{

    this->id = id;
    this->p1 = p1;
    this->p2 = p2;
    
    p1->game_id = id;
    p1->game_indicator = 1;
    
    p2->game_id = id;
    p2->game_indicator = 2;
    
    
    this->gameLogic = new GameLogic();
    this->gameLogic->turn_indicator = p1->game_indicator;
    
    LogManager::log(__FILENAME__, __FUNCTION__, "New game with ID: " + to_string(id) + " for players " + p1->name + " and " + p2->name + " created");
}

/**
 *  Method to get player's opponent
 */
Player* Game::get_opponent(Player* pl)
{
    if (pl == get_player_1())
    {
        return get_player_2();
    }
    
    return get_player_1();
}

/**
 *  Game destructor
 */
Game::~Game()
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Game with ID:" + to_string(this->id) + " deleted");
}
