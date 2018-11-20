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
map<int, Game*> GameManager::running_games;

int GameManager::game_id_generator = 1;

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
    std::cout << pl->ip << " socket: " << pl->socket << endl;
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

void GameManager::want_play(Player* pl)
{
    cout << "Player: " << pl->name << " is seeking for opponent" << endl;
    Player *opponent = find_opponent();
    
    if (opponent == NULL)
    {
        cout << "Opponent not found. Player: " << pl->name << " added to queue" << endl;
        players_queue.push(pl);
    }
    else
    {
        cout << "Opponent found with name: " << opponent->name << endl;
        create_game(pl, opponent);
    }

}

Player* GameManager::find_opponent()
{
    if (!players_queue.empty())
    {
        Player *pl = players_queue.top();
        players_queue.pop();
        
        return pl;
    }
    
    return NULL;
}

void GameManager::create_game(Player* pl1, Player *pl2)
{
    cout << "Creating new game for players " << pl1->name << " and " << pl2->name << endl;
    Game *game = new Game(game_id_generator, pl1, pl2);
    
    running_games.insert(make_pair(game_id_generator, game));
    
    GameManager::game_id_generator += 1;
    
    cout << "New game with ID: " << game->id << " created." << endl;
}



