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

int GameManager::MAX_GAMES;

int TIMEOUT = 180;
int PING_INTERVAL = 5;

/**
 *  Method to create unloged player
 */
void GameManager::create_unlogged_player(char *ip, int client_socket)
{
    string string_ip = ip;
    LogManager::log(__FILENAME__, __FUNCTION__, "Connected new player on server with IP: " + string_ip + " on socker: " + to_string(client_socket));
    
    Player *pl = new Player(ip, client_socket);
    
    //add new connected player to unlloged players
    GameManager::unlogged_players.insert(make_pair(client_socket, pl));
    
}

/**
 *  Method to log player
 */
void GameManager::log_player(int client_socket, string name)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Creating new player with name:" + name + " on socket: " + to_string(client_socket));
    
    auto *pl = GameManager::get_unlogged_player(client_socket);
    
    GameManager::unlogged_players.erase(client_socket);
    
    pl->set_name(name.data());
    
    pl->state = "LOBBY";
    
    GameManager::logged_players.insert(make_pair(name, pl));
    
    pl->ping_thread = thread(GameManager::ping_player, pl);
    pl->is_thread_running = true;
//    pl->ping_thread.join();
  //  pl->thread t1(GameManager::ping_player, pl);
 //   t1.detach();
    ResponseManager::sendToClient(pl, "LOGIN");
}

/**
 *  Get unlogged player from unloged players by socket
 */
Player* GameManager::get_unlogged_player(int id)
{
    auto search = GameManager::unlogged_players.find(id);
    if (search != GameManager::unlogged_players.end())
    {
        return search->second;
    }
    
    return NULL;
}

/**
 *  Get logged player from logged players by socket
 */
Player* GameManager::get_logged_player_by_socket(int id)
{
    map<string, Player*>::iterator it = GameManager::logged_players.begin();
    while (it != GameManager::logged_players.end())
    {
        auto *pl = it->second;
        
        if (pl->socket == id)
        {
            return pl;
        }
        
        it++;
    }
    
    return NULL;
}

/**
 * Get logged player from logged players by name
 */
Player* GameManager::get_logged_player_by_name(string name)
{
    map<string, Player*>::iterator it = GameManager::logged_players.begin();
    while (it != GameManager::logged_players.end())
    {
        auto *pl = it->second;
        
        if (pl->name.compare(name) == 0)
        {
            return pl;
        }
        
        it++;
    }
    
    return NULL;
}

/**
 *  Finding opponent for player.
 *  If opponent is NOT found, player is added to queue
 *  If opponent in found, player and opponent are moved to game
 */
void GameManager::want_play(Player* pl)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Player: " + pl->name + " is seeking for opponent");
    
    if (running_games.size() < GameManager::MAX_GAMES)
    {
        auto *opponent = find_opponent();
        
        if (opponent == NULL)
        {
            LogManager::log(__FILENAME__, __FUNCTION__, "Opponent not found. Player: " + pl->name + " has beed added to queue");
            
            players_queue.push(pl);
            ResponseManager::sendState(pl, "WAITING;0");
            
            pl->state = "WAITING";
            
        }
        else
        {
            LogManager::log(__FILENAME__, __FUNCTION__, "Opponent found. Opponent name: " + opponent->name);
            
            ResponseManager::sendState(pl, "STARTING_GAME;0");
            ResponseManager::sendState(opponent, "STARTING_GAME;0");
            
            pl->state = "IN_GAME";
            opponent->state = "IN_GAME";
            
            create_game(pl, opponent);
            
        }
    }
    else
    {
        LogManager::log(__FILENAME__, __FUNCTION__, "Player: " + pl->name + " can not play, because maximum limit of games is reached");
        ResponseManager::sendState(pl, "MAXIMUM_GAMES_REACHED");
    }

}

/**
 *  Method check game queue.
 *  If queue is empty, method return NULL, else return player on top of stack
 */
Player* GameManager::find_opponent()
{
    if (!players_queue.empty())
    {
        auto *pl = players_queue.top();
        players_queue.pop();
        
        //check if player is disconnected
        if (pl->connected == -1)
        {
            return NULL;
        }
        
        return pl;
    }
    
    return NULL;
}

/**
 *  Creating of new game
 */
void GameManager::create_game(Player* pl1, Player *pl2)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Creating new game for players " + pl1->name + " and " + pl2->name);
    
    auto *game = new Game(game_id_generator, pl1, pl2);
    
    running_games.insert(make_pair(game_id_generator, game));
    
    GameManager::game_id_generator += 1; // dump game id generator
    
    //notify players who is playing
    ResponseManager::sendStatus(pl1, "Your are on Turn;");
    ResponseManager::sendStatus(pl2, "Opponent is on Turn;");
}

/**
 *  Resolving player's turn
*/
void GameManager::turn(Player* pl, int row, int column)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Resolving turn from player: " + pl->name + ". Row: " + to_string(row) + " Col: " + to_string(column));
    
    Game* game = get_running_game(pl->game_id);
    GameLogic* game_logic = game->gameLogic;
    
    //trying mark cell to player
    int status = game_logic->turn(row, column, pl);
    
    //turn accepted
    if (status == 0)
    {
        LogManager::log(__FILENAME__, __FUNCTION__, "Game ID: " + to_string(game->id) + " Row: " + to_string(row) + " Col: " + to_string(column) + " marked for player: " + pl->name);
        
        Player* new_player_on_turn = game->get_opponent(pl);
        game_logic->turn_indicator = new_player_on_turn->game_indicator;
        
        ResponseManager::sendStatus(game->get_opponent(pl), "Your are on Turn;");
        ResponseManager::sendStatus(pl, "Opponent is on Turn;");
        
        ResponseManager::acceptMove(pl, row, column);
        ResponseManager::sentMoveToOpponent(game->get_opponent(pl), row, column);
        
        int result = game_logic->check_board();
        
        if (result == -1)
        {
            LogManager::log(__FILENAME__, __FUNCTION__, "Game ID: " + to_string(game->id) + " result: TIE");
            
            Player* opponent = game->get_opponent(pl);
            
            pl->state = "RESULT";
            opponent->state = "RESULT";
            
            ResponseManager::sendResult(pl, "TIE;" + to_string(pl->score) + ";" + to_string(opponent->score));
            ResponseManager::sendResult(opponent, "TIE;" + to_string(opponent->score) + ";" + to_string(pl->score));
        }
        else if (result == 1)
        {
            LogManager::log(__FILENAME__, __FUNCTION__, "Game ID: " + to_string(game->id) + " result: " + pl->name + " is WINNER");
            
            pl->score += 1;
            
            Player* opponent = game->get_opponent(pl);
            
            pl->state = "RESULT";
            opponent->state = "RESULT";
            
            ResponseManager::sendResult(pl, "WIN;" + to_string(pl->score) + ";" + to_string(opponent->score));
            ResponseManager::sendResult(opponent, "LOSE;" + to_string(opponent->score) + ";" + to_string(pl->score));
            
            
            game_logic->last_winner = pl;
        }
    }
    //already marked
    else if (status == 1)
    {
        LogManager::log(__FILENAME__, __FUNCTION__, "Game ID: " + to_string(game->id) + " Player:" + pl->name + " Error: Cell is already marked");
    }
    //player is not on turn
    else if (status == 2)
    {
        LogManager::log(__FILENAME__, __FUNCTION__, "Game ID: " + to_string(game->id) + " Player:" + pl->name + " Error: Player is not on turn");
    }
    //out of range
    else if (status == -1)
    {
        LogManager::log(__FILENAME__, __FUNCTION__, "Game ID: " + to_string(game->id) + " Player:" + pl->name + " Error: Try mark cell which is out of range");
    }
    
}

/**
 *  Get running game by id
 */
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

/**
 *  Rematch option
 */
void GameManager::rematch(Player *pl)
{
    pl->want_rematch = true;
    Game* game = get_running_game(pl->game_id);
    
    LogManager::log(__FILENAME__, __FUNCTION__, "Game ID: " + to_string(game->id) + " Player:" + pl->name + " wants rematch");
    
    if ( game->get_player_1()->want_rematch && game->get_player_2()->want_rematch)
    {
        LogManager::log(__FILENAME__, __FUNCTION__, "Game ID: " + to_string(game->id) + " Both players want rematch. Starting new game...");
        
        GameLogic* game_logic = game->gameLogic;
        game_logic->reset_board();
        
        //starts player which lose
        game_logic->turn_indicator = game->get_opponent(game_logic->last_winner)->game_indicator;
        
        ResponseManager::sendState(pl, "STARTING_GAME;0;");
        ResponseManager::sendState(game->get_opponent(pl), "STARTING_GAME;0;");
        
        ResponseManager::sendStatus(game->get_opponent(game_logic->last_winner), "Your are on Turn;");
        ResponseManager::sendStatus(game_logic->last_winner, "Opponent is on Turn;");
        
        pl->want_rematch = false;
        game->get_opponent(pl)->want_rematch = false;
        
        pl->state = "IN_GAME";
        game->get_opponent(pl)->state = "IN_GAME";
    }
    else
    {
        LogManager::log(__FILENAME__, __FUNCTION__, "Game ID: " + to_string(game->id) + " Player:" + pl->name + " waiting for opponent decision");
        
        
        ResponseManager::sendState(pl, "WAITING;0;");
        
        ResponseManager::sendStatus(game->get_opponent(pl), "Opponent wants rematch!");
        
    }
}

/**
 *  Closing existing game, if player dont want play next game
 */
void GameManager::close_game(Player *pl)
{
    if (pl->game_id > 0)
    {
        Game* game = get_running_game(pl->game_id);
        
        LogManager::log(__FILENAME__, __FUNCTION__, "Game ID: " + to_string(game->id) + " Player:" + pl->name + " exit game");
        
        running_games.erase(game->id);
        
        //reseting players statistics
        pl->game_id = 0;
        pl->score = 0;
        pl->game_indicator = 0;
        
        auto *opponent = game->get_opponent(pl);
        
        opponent->game_id = 0;
        opponent->score = 0;
        opponent->game_indicator = 0;
        
        ResponseManager::sendResult(pl, "CLOSE_GAME;0");
        ResponseManager::sendResult(opponent, "CLOSE_GAME;0");
        
        ResponseManager::sendStatus(opponent, "Opponent left game");
        
        pl->state = "LOBBY";
        opponent->state = "LOBBY";
        
        delete game;
    }
    else
    {
        LogManager::log(__FILENAME__, __FUNCTION__, " Player:" + pl->name + " is not in game");
    }
    
}

/**
 *  Handle if player is dosconnected
 */
void GameManager::disconect_player(int pl_socket)
{

    Player *pl = get_logged_player_by_socket(pl_socket);
    
    if (pl != NULL)
    {
        pl->connected = -1;
        pl->socket = -1;
        
        if (pl->state.compare("IN_GAME") == 0)
        {
            notifyOpponent(pl, "Opponent is disconnected");
        }
        else if (pl->state.compare("RESULT") == 0)
        {
            close_game(pl);
        }
        
        LogManager::log(__FILENAME__, __FUNCTION__, "Player: " + pl->name + " has been disconnected");
        
    }
    
    //if someone is disconnected remove socket
    Server::closeSocket(pl_socket);
}

/**
 *  Method to notify player's opponent about something
 */
void GameManager::notifyOpponent(Player *pl, string msg)
{
    if (pl->game_id > 0)
    {
        auto game = get_running_game(pl->game_id);
        
        auto opponent = game->get_opponent(pl);
        
        LogManager::log(__FILENAME__, __FUNCTION__, "Game ID: " + to_string(game->id) + " notify " + pl->name + "'s" + " opponent " + opponent->name + " with message: " + msg);
        
        ResponseManager::sendStatus(opponent, msg);
    }
}

/**
 *  Method to reconnect player to game
 */
void GameManager::reconnected_player(Player *pl, int new_socket)
{
    pl->connected = 0;
    pl->socket = new_socket;
    
    Game *game = get_running_game(pl->game_id);
    
    if (game != NULL)
    {
        LogManager::log(__FILENAME__, __FUNCTION__, "Reconnecting player: " + pl->name + " to game: " + to_string(pl->game_id));
        
        pl->state = "IN_GAME";
        
        ResponseManager::sendGameToClient(pl, game);
        
        if (game->gameLogic->turn_indicator == pl->game_indicator)
        {
            ResponseManager::sendStatus(pl, "You are on Turn");
            notifyOpponent(pl, "Opponent is on Turn");
        }
        else
        {
            ResponseManager::sendStatus(pl, "Opponent is on Turn");
            notifyOpponent(pl, "Your are on Turn");
        }
    }
    //if game is not exists anymore
    else
    {
        LogManager::log(__FILENAME__, __FUNCTION__, "Reconnecting player: " + pl->name + " to lobby");
        
        pl->state = "LOBBY";
        
        ResponseManager::sendToClient(pl, "LOGIN");
    }
}

/**
 *  Method to log player into the game. If player was disconnected, will be reconnected to game.
 */
void GameManager::log_player_resolve(int client_socket, string name)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Log resolve for player on socket: " + to_string(client_socket) + " with name: \"" + name + "\"");
    
    if (!name.empty())
    {
        Player* pl = get_logged_player_by_name(name);
        
        //check if player is logged
        if (pl != NULL)
        {
            //if player is logged and playing
            //name UnKnow is used as default name, for this reason is not avaliable
            if (pl->connected == 0 || name.compare("UnKnow") == 0)
            {
                LogManager::log(__FILENAME__, __FUNCTION__, "Player on socket: " + to_string(client_socket) + " send name: " + name + ", but this name is not avaliable");
                
                ResponseManager::sendToSpecificSocket(client_socket, "NAME_IS_NOT_AVALIABLE");
            }
            else
            {
               
                GameManager::reconnected_player(pl, client_socket);
                    
                LogManager::log(__FILENAME__, __FUNCTION__, "Reconnecting player: " + pl->name + "to state: " + pl->state + " SUCCESS");
                
            }
        }
        else
        {
            //register new player
            GameManager::log_player(client_socket, name);
            
            
        }
    }
    else
    {
        LogManager::log(__FILENAME__, __FUNCTION__, "Socket: " + to_string(client_socket) + " invalid name: \"" + name + "\"");
        ResponseManager::sendToSpecificSocket(client_socket, "INVALID_NAME");
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

/**
 *  Method to delete player from game.
 */
void GameManager::exit(Player* pl)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Closing application for player: " + pl->name);
    
    pl->is_exists = false;
    
    int queue_size = static_cast<int>(players_queue.size());
    
    logged_players.erase(pl->name);
    
    delete_player_from_queue(pl, queue_size, 0);
    
    ResponseManager::sendState(pl, "EXIT");
    
    if (pl->game_id > 0)
    {
        GameManager::timeout_game_exit(pl);
    }
    
    LogManager::log(__FILENAME__, __FUNCTION__, "Closing ping thread for player: " + pl->name);
    while(pl->is_thread_running)
    {
        sleep(2);
    }
    
    if (pl->socket != -1)
    {
        Server::closeSocket(pl->socket);
    }
    
    
    //delete pl;
}

/**
 *  Remove specific player from game queue
 */
void GameManager::delete_player_from_queue(Player *pl, int n, int curr)
{
    // If stack is empty or all items
    // are traversed
    if (players_queue.empty() || curr == n)
        return;
    
    // Remove current item
    Player *tmp = players_queue.top();
    players_queue.pop();
    
    // Remove other items
    delete_player_from_queue(pl, n, curr+1);
    
    // Put all items back except middle
    if (pl != tmp)
    {
        players_queue.push(tmp);
    }
    else
    {
        LogManager::log(__FILENAME__, __FUNCTION__, "Player: " + pl->name + " has been removed from game queue");
    }
}

void GameManager::set_max_games(int max_games)
{
    GameManager::MAX_GAMES = max_games;
}

void GameManager::timeout_game_exit(Player *pl)
{
    Game* game = get_running_game(pl->game_id);
    
    LogManager::log(__FILENAME__, __FUNCTION__, "Game ID: " + to_string(game->id) + " Player:" + pl->name + " closing game");
    
    running_games.erase(game->id);
    
    auto *opponent = game->get_opponent(pl);
    
    //reseting opponent statistics
    opponent->game_id = 0;
    opponent->score = 0;
    opponent->game_indicator = 0;
    opponent->state = "LOBBY";
    
    LogManager::log(__FILENAME__, __FUNCTION__, "Game ID: " + to_string(game->id) + " sending information to opponent " + opponent->name);
    
    ResponseManager::sendState(opponent, "LOBBY");
    ResponseManager::sendStatus(opponent, "Opponent did not return. NOOB");
    
    delete game;
}

void GameManager::ping_player(Player* pl)
{
    
    int i = 0;
    while(1)
    {
        if (pl->is_exists == false)
        {
            break;
        }
        
        sleep(PING_INTERVAL);
        
    
        if (pl->ping_status == true)
        {
            i = 0;
            pl->ping_status = false;
            
            if (pl->is_exists == true)
            {
                ResponseManager::ping(pl);
            }
            else
            {
                break;
            }
        }
        else
        {
            GameManager::disconect_player(pl->socket);
            while(1)
            {
                i++;
                if (i == TIMEOUT)
                {
                    pl->is_thread_running = false;
                    
                    GameManager::exit(pl);
                    LogManager::log(__FILENAME__, __FUNCTION__, "Ping thread for player:" + pl->name + " has been closed");
                    pthread_exit(0);
                }
                
                if (pl->connected >= 0)
                {
                    pl->ping_status = true;
                    break;
                }
                
                sleep(1);
            }
        }
    }
    
    //for closing thread with button exit game
    pl->is_thread_running = false;
    LogManager::log(__FILENAME__, __FUNCTION__, "Ping thread for player:" + pl->name + " has been closed");
    pthread_exit(0);
}


