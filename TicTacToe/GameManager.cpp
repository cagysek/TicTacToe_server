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
    
    printf("New player is registered. Socket ID: %d, Player name: %s and was moved from unlogged to logged players \n", pl->socket, pl->name.c_str());
    ResponseManager::sendToClient(pl, "LOGIN;0");
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

Player* GameManager::get_logged_player_by_name(string name)
{
    map<string, Player*>::iterator it = GameManager::logged_players.begin();
    while (it != GameManager::logged_players.end())
    {
        Player *pl = it->second;
        
        if (pl->name.compare(name) == 0)
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
        ResponseManager::sendState(pl, "WAITING;0");
        
    }
    else
    {
        
        ResponseManager::sendState(pl, "STARTING_GAME;0");
        ResponseManager::sendState(opponent, "STARTING_GAME;0");
        
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
        
        Player* new_player_on_turn = game->get_opponent(pl);
        game_logic->turn_indicator = new_player_on_turn->socket;
        
        
        ResponseManager::acceptMove(pl, row, column);
        ResponseManager::sentMoveToOpponent(game->get_opponent(pl), row, column);
        
        int result = game_logic->check_board();
        cout << "Game logic result: " << result << endl;
        
        if (result == -1)
        {
            printf("Tie");
            
            Player* opponent = game->get_opponent(pl);
            
            ResponseManager::sendResult(pl, "TIE;" + to_string(pl->score) + ";" + to_string(opponent->score));
            ResponseManager::sendResult(opponent, "TIE;" + to_string(opponent->score) + ";" + to_string(pl->score));
        }
        else if (result == 1)
        {
            cout << "Player " << pl->name << " is WINNER" << endl;
            
            pl->score += 1;
            
            Player* opponent = game->get_opponent(pl);
            
            ResponseManager::sendResult(pl, "WIN;" + to_string(pl->score) + ";" + to_string(opponent->score));
            ResponseManager::sendResult(opponent, "LOSE;" + to_string(opponent->score) + ";" + to_string(pl->score));
            
            
            game_logic->last_winner = pl;
        }
        
        
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
        
        ResponseManager::sendState(pl, "STARTING_GAME;0;");
        ResponseManager::sendState(game->get_opponent(pl), "STARTING_GAME;0;");
        
        pl->want_rematch = false;
        game->get_opponent(pl)->want_rematch = false;
    }
    else
    {
        ResponseManager::sendState(pl, "WAITING;0;");
    }
}

void GameManager::close_game(Player *pl)
{
    Game* game = get_running_game(pl->game_id);
    running_games.erase(game->id);
    
    pl->game_id = 0;
    pl->score = 0;
    
    game->get_opponent(pl)->game_id = 0;
    game->get_opponent(pl)->score = 0;
    
    ResponseManager::sendResult(pl, "CLOSE_GAME;0");
    ResponseManager::sendResult(game->get_opponent(pl), "CLOSE_GAME;0");
    
    delete game;
}


void GameManager::disconected_player(int pl_socket)
{
    Player* pl = get_logged_player_by_socket(pl_socket);
    pl->connected = -1;
    pl->socket = -1;
}

void GameManager::reconnected_player(Player *pl, int new_socket)
{
    pl->connected = 0;
    pl->socket = new_socket;
}



void GameManager::log_player_resolve(int client_socket, string name)
{
    Player* pl = get_logged_player_by_name(name);
    
    //check if player is logged
    if (pl != NULL)
    {
        if (pl->connected == 0)
        {
            ResponseManager::sendToSpecificSocket(client_socket, "NAME_IS_NOT_AVALIABLE");
        }
        else
        {
            pl->socket = client_socket;
            
            if (pl->game_id > 0)
            {
                //reconnect in game
                cout << "Reconnect" << endl;
            }
            else
            {
                //to lobby
                cout << "Lobby" << endl;
                ResponseManager::sendState(pl, "LOBBY");
            }
            
            cout << "Player with socket: " << pl->socket << " renamed to " << pl->name << endl;
        }
    }
    else
    {
        //get unloged a login
        GameManager::log_player(client_socket, name);
    }
}



void GameManager::print_games()
{
    map<int, Game*>::iterator it = running_games.begin();
    while (it != running_games.end())
    {
        Game *game = it->second;
    
        cout << game->id << endl;
        
        it++;
    }
}

void GameManager::exit(Player* pl)
{
    logged_players.erase(pl->name);
    
    ResponseManager::sendState(pl, "EXIT");
    
    delete pl;
}


