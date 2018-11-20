//
//  GameManager.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 19/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include "GameManager.hpp"

map<int, Player*> GameManager::unlogged_players;
map<string, Player*> GameManager::logged_players;
stack<Player*> GameManager::players_queue;

void GameManager::log_player(int client_socket, string name)
{
    Player *pl = GameManager::get_unlogged_player(client_socket);
    
    GameManager::unlogged_players.erase(client_socket);
    
    pl->set_name(name.data());
    
    GameManager::logged_players.insert(make_pair(name, pl));
    
    printf("New player is registered. Socket ID: %d, Player name: %s", pl->socket, pl->name.c_str());
}


void GameManager::create_unlogged_player(char *ip, int client_socket)
{
    Player *pl = new Player(ip, client_socket);
    
    //add new connected player
    GameManager::unlogged_players.insert(make_pair(client_socket, pl));
    
    printf("Pripojen novy klient s Ip: ");
    std::cout << pl->ip << " socket: " << pl->socket << "\n";
}


Player* GameManager::get_unlogged_player(int id)
{
    auto search = GameManager::unlogged_players.find(id);
    if (search != GameManager::unlogged_players.end())
    {
        return search->second;
    }
    
    return NULL;
}

Player* GameManager::get_logged_player_by_socket(int id)
{
    std::map<string, Player*>::iterator it = GameManager::logged_players.begin();
    while (it != GameManager::logged_players.end())
    {
        Player *pl = it->second;
        
        if (pl->socket == id)
        {
            return pl;
        }
        
        // Increment the Iterator to point to next entry
        it++;
    }
    
    return NULL;
}
