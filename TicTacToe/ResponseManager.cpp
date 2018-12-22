//
//  ResponseManager.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 22/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include "ResponseManager.hpp"

/**
 *  Method to send message to player
 */
void ResponseManager::sendToClient(Player *pl, string msg)
{
    pl->msg_out = msg;
    
    LogManager::log(__FILENAME__, __FUNCTION__, "Message: " + msg + " is sending to player " + pl->name);
    
    msg = msg += "\n";
    
    send(pl->socket, msg.data(), msg.length(), 0);
    
    msg.clear();
}

/**
 *  Accept player's move
 */
void ResponseManager::acceptMove(Player *pl, int row, int column)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Preparing message for player " + pl->name);
    
    string temp = "YOUR_TURN;" + to_string(row) +";" + to_string(column) + ";";
    ResponseManager::sendToClient(pl, temp);
}

/**
 *  Send player's move to opponent
 */
void ResponseManager::sentMoveToOpponent(Player *pl, int row, int column)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Preparing message for player " + pl->name);
    
    string temp = "OPPONENT_TURN;" + to_string(row) +";" + to_string(column) + ";";
    ResponseManager::sendToClient(pl, temp);
}

/**
 *  Send status text to player
 */
void ResponseManager::sendStatus(Player *pl, string msg)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Preparing message for player " + pl->name);
    
    string temp = "STATUS;" + msg;
    ResponseManager::sendToClient(pl, temp);
}

/**
 *  Send new state to player
 */
void ResponseManager::sendState(Player *pl, string state)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Preparing message for player " + pl->name);
    
    ResponseManager::sendToClient(pl, state + ";");
}

/**
 *  Send game result to player
 */
void ResponseManager::sendResult(Player *pl, string msg)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Preparing message for player " + pl->name);
    
    ResponseManager::sendToClient(pl, msg + ";");
}

/**
 * Send message to specific socket
 */
void ResponseManager::sendToSpecificSocket(int socket_id, string msg)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Message: " + msg + " is sending to socket " + to_string(socket_id));

    msg = msg += "\n";
    
    send(socket_id, msg.data(), msg.length(), 0);
    
    msg.clear();
}

/**
 *  Send whole game to player
 */
void ResponseManager::sendGameToClient(Player *pl, Game *game)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Preparing message for player " + pl->name);
    
    string msg = "RECONNECT;";
    for (int i = 0 ; i < game->size ; i++)
    {
    
        for (int j = 0 ; j < game->size ; j++)
        {
            int val = game->gameLogic->get_value(i, j);
            msg += to_string(val);
            if (j + 1 < game->size)
            {
                msg += ",";
            }
        }

        if (i + 1 < game->size)
        {
            msg += ",";
        }
    }
    
    msg += ";" + to_string(pl->game_indicator);
    msg += ";";
    
    sendToClient(pl, msg);
    
}
