//
//  Player.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 12/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include "Player.hpp"

/**
 *  Constructor for player
 */
Player::Player(char *ip, int socket)
{
    this->ip = ip;
    this->socket = socket;
    this->name = "UnKnow";
    this->want_rematch = false;
    this->connected = 0;
    this->score = 0;
    this->game_indicator = 0;
    this->state = "NEW";
    this->ping_status = true;
    this->is_exists = true;
    
    LogManager::log(__FILENAME__, __FUNCTION__, "New player created on socket: " + to_string(socket));
}

/**
 *  Setter to player's name
 */
void set_name(Player *pl, std::string name)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Player's name changed from " + pl->name + " to " + name);
    
    pl->name = name;
}

/**
 * Player destructor
 */
Player::~Player()
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Player on socket: " + to_string(this->socket) + " has been deleted");
}
