//
//  ResponseManager.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 22/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include "ResponseManager.hpp"


void ResponseManager::sendToClient(Player *pl, string msg)
{
    pl->msg_out = msg;
    cout << "Message: " << msg << " sending to " << pl->name << endl;
    msg = msg += "\n";
    
    send(pl->socket, msg.data(), msg.length(), 0);
    
    msg.clear();
}


void ResponseManager::acceptRequest(Player *pl, string action)
{
    ResponseManager::sendToClient(pl, action + ";0;");
}

void ResponseManager::acceptMove(Player *pl, int row, int column)
{
    string temp = "YOUR_TURN;" + to_string(row) +";" + to_string(column) + ";";
    ResponseManager::sendToClient(pl, temp);
}

void ResponseManager::sentMoveToOpponent(Player *pl, int row, int column)
{
    string temp = "OPPONENT_TURN;" + to_string(row) +";" + to_string(column) + ";";
    ResponseManager::sendToClient(pl, temp);
}

void ResponseManager::sendState(Player *pl, string state)
{
    ResponseManager::sendToClient(pl, state + ";");
}

void ResponseManager::sendResult(Player *pl, string msg)
{
    ResponseManager::sendToClient(pl, msg + ";");
}

void ResponseManager::sendToSpecificSocket(int socket_id, string msg)
{
    cout << "Message: " << msg << " sending to socket: " << socket_id << endl;
    msg = msg += "\n";
    
    send(socket_id, msg.data(), msg.length(), 0);
    
    msg.clear();
}
