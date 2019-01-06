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
    pl->msg_in = msg;
    
    vector<string> msg_parts = Utils::split(msg, ";");
    
    if(msg_parts.empty())
    {
        pl->invalid_message_counter++;
        
        LogManager::log(__FILENAME__, __FUNCTION__, "Invalid message to resolve. Player:" + pl->name + " socket:" + to_string(pl->socket) + ". Invalid message counter: " + to_string(pl->invalid_message_counter));
    }
    else
    {
        //type of message
        string type = msg_parts[0];
        LogManager::log(__FILENAME__, __FUNCTION__, "Resolving player: " + pl->name + " message:" + msg);
        
        if ( type.compare("NAME") == 0 )
        {
            pl->invalid_message_counter = 0;
            
            
            string name = "";
            if (msg_parts.size() > 1 && !msg_parts[1].empty())
            {
                name = msg_parts[1];
            }
            
            GameManager::log_player_resolve(pl->socket, name);
            
            
        }
        else if ( type.compare("FIND_GAME") == 0 )
        {
            pl->invalid_message_counter = 0;
            
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
            pl->invalid_message_counter = 0;
            
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
            pl->invalid_message_counter = 0;
            
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
            pl->invalid_message_counter = 0;
            
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
            pl->invalid_message_counter = 0;
            
            if (pl->state.compare("LOBBY") == 0 || pl->state.compare("WAITING") == 0)
            {
                GameManager::exit(pl);
            }
            else
            {
                LogManager::log(__FILENAME__, __FUNCTION__, "EXIT: Invalid operation. Player:" + pl->name + " is in state: " + pl->state + ". Can not do this operation");
            }
            
        }
        else if (type.compare("ACK") == 0)
        {
            //ack dont reset invalid_message_counter
            
            pl->ping_status = true;
        }
        else if (type.compare("PING") == 0)
        {
            // just for testing from client
        }
        else
        {
            pl->invalid_message_counter++;
            
            LogManager::log(__FILENAME__, __FUNCTION__, "Player: " + pl->name + " unknown message type recieved. Type:" + type + ". Invalie message counter: " + to_string(pl->invalid_message_counter));
        }
    }
}

void RequestManager::resolveInput(Player *pl, std::string msg)
{
    vector<string> msg_parts = Utils::split(msg, "|");
    vector<string> used_parts;
    bool is_used = false;
    
    for(int i = 0 ; i < msg_parts.size() ; i++)
    {
        is_used = false;
        
        if (used_parts.size() > 0)
        {
            for(int j = 0 ; j < used_parts.size() ; j++)
            {
                if (used_parts[j].compare(msg_parts[i]) == 0)
                {
                    is_used = true;
                }
            }
        }
        
        if(!is_used)
        {
            cout << msg_parts[i] << endl;
            used_parts.insert(used_parts.begin(), msg_parts[i]);
            RequestManager::resolve(pl, msg_parts[i]);
        }
    }
}
