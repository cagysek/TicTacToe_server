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
    
    printf("New player is registered. Socket ID: %d, Player name: %s and was moved from unlogged to logged players", pl->socket, pl->name.c_str());
}


void GameManager::create_unlogged_player(char *ip, int client_socket)
{
    cout << "Connected new player with IP: " << ip << " on socket: " << client_socket << ".";
    
    Player *pl = new Player(ip, client_socket);
    
    //add new connected player
    GameManager::unlogged_players.insert(make_pair(client_socket, pl));
    
    cout << " and was added into unlogged players." << endl;
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
    map<string, Player*>::iterator it = GameManager::logged_players.begin();
    while (it != GameManager::logged_players.end())
    {
        Player *pl = it->second;
        
        if (pl->socket == id)
        {
            return pl;
        }
        
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


void GameManager::turn(Player* pl, int row, int column)
{
    Game* game = get_running_game(pl->game_id);
    GameLogic* game_logic = game->gameLogic;
    
    int status = game_logic->turn(row, column, pl);
    
    if (status == 0)
    {
        int result = game_logic->check_board();
        
        if ( result == 0 )
        {
            Player* new_player_on_turn = game->get_opponent(pl);
            game_logic->turn_indicator = new_player_on_turn->socket;
        }
        else if (result == -1)
        {
            printf("Tie");
        }
        else
        {
            cout << "Player " << pl->name << " is WINNER";
            game_logic->last_winner = pl;
            //resolve_result(pl, result);
        }
    }
    
}

void GameManager::resolve_result(Player *pl, int result)
{
    if (result == -1)
    {
        printf("Tie");
    }
    else
    {
        cout << "Player " << pl->name << " is WINNER";
    }
}

Game* GameManager::get_running_game(int id)
{
    map<int, Game*>::iterator it = running_games.begin();
    while (it != running_games.end())
    {
        Game *game = it->second;
        
        if (game->id == id)
        {
            return game;
        }
        
        it++;
    }
    
    return NULL;
}

void GameManager::rematch(Player *pl)
{
    cout << "Player " << pl->name << " wants rematch." << endl;
    pl->want_rematch = true;
    Game* game = get_running_game(pl->game_id);
    
    if ( game->get_player_1()->want_rematch && game->get_player_2()->want_rematch)
    {
        cout << "Both players want rematch. Cleaning old board..." << endl;
        GameLogic* game_logic = game->gameLogic;
        game_logic->reset_board();
        
        //starts player which lose
        game_logic->turn_indicator = game->get_opponent(game_logic->last_winner)->socket;
    }
    
}

