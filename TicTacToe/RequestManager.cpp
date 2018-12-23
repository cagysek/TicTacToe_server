//
//  MessageManager.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 19/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include "RequestManager.hpp"


/**
 *  Resolve incoming message
 */
void RequestManager::resolve(Player *pl, std::string msg)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Recv from " + pl->name + " message: " + msg.data());
    
    pl->msg_in = msg;
    
    vector<string> msg_parts = Utils::split(msg, ";");
    
    if(msg_parts.empty())
    {
        LogManager::log(__FILENAME__, __FUNCTION__, "Invalid message from player:" + pl->name + " socket:" + to_string(pl->socket));
    }
    else
    {
        //type of message
        string type = msg_parts[0];
        LogManager::log(__FILENAME__, __FUNCTION__, "Resolving message type:" + type);
        
        if ( type.compare("NAME") == 0 )
        {
            if (pl->state.compare("NEW") == 0)
            {
                string name = "";
                if (msg_parts.size() > 1 && !msg_parts[1].empty())
                {
                    name = msg_parts[1];
                }
                
                GameManager::log_player_resolve(pl->socket, name);
                
                pl->state = "LOBBY";
            }
            else
            {
                LogManager::log(__FILENAME__, __FUNCTION__, "NAME: Invalid operation. Player:" + pl->name + " is in state: " + pl->state + ". Can not do this operation");
            }
        }
        else if ( type.compare("FIND_GAME") == 0 )
        {
            if (pl->state.compare("LOBBY") == 0)
            {
                GameManager::want_play(pl);
            }
            else
            {
                LogManager::log(__FILENAME__, __FUNCTION__, "FIND_GAME: Invalid operation. Player:" + pl->name + " is in state: " + pl->state + ". Can not do this operation");
            }
        }
        else if ( type.compare("TURN") == 0 )
        {
            if (pl->state.compare("IN_GAME") == 0)
            {
                try {
                    int row = stoi(msg_parts[1]);
                    int column = stoi(msg_parts[2]);
                    
                    GameManager::turn(pl, row, column);
                    
                } catch (const exception &exc) {
                    LogManager::log(__FILENAME__, __FUNCTION__, "Invalid input from player: " + pl->name + ". Msg: " + msg + ". Err: " + exc.what());
                }
            }
            else
            {
                LogManager::log(__FILENAME__, __FUNCTION__, "TURN: Invalid operation. Player:" + pl->name + " is in state: " + pl->state + ". Can not do this operation");
            }
        }
        else if ( type.compare("REMATCH") == 0 )
        {
            if (pl->state.compare("RESULT") == 0)
            {
                GameManager::rematch(pl);
            }
            else
            {
                LogManager::log(__FILENAME__, __FUNCTION__, "REMATCH: Invalid operation. Player:" + pl->name + " is in state: " + pl->state + ". Can not do this operation");
            }
        }
        else if ( type.compare("CLOSE_GAME") == 0 )
        {
            if (pl->state.compare("RESULT") == 0)
            {
                GameManager::close_game(pl);
            }
            else
            {
                LogManager::log(__FILENAME__, __FUNCTION__, "CLOSE_GAME: Invalid operation. Player:" + pl->name + " is in state: " + pl->state + ". Can not do this operation");
            }
            
        }
        else if ( type.compare("EXIT") == 0)
        {
            if (pl->state.compare("LOBBY") == 0 || pl->state.compare("WAITING") == 0)
            {
                GameManager::exit(pl);
            }
            else
            {
                LogManager::log(__FILENAME__, __FUNCTION__, "EXIT: Invalid operation. Player:" + pl->name + " is in state: " + pl->state + ". Can not do this operation");
            }
            
        }
        else
        {
            LogManager::log(__FILENAME__, __FUNCTION__, "Player: " + pl->name + " unknown message type recieved. Type:" + type);
        }
    }
}
