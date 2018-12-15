//
//  ResponseManager.hpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 22/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#ifndef ResponseManager_hpp
#define ResponseManager_hpp

#include "Player.hpp"
#include "Game.hpp"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

class ResponseManager
{
public:
    static void sendToClient(Player* pl, string msg);
    static void sendToSpecificSocket(int socket_id, string msg);
    static void acceptRequest(Player* pl, string action);
    static void dennyRequest(Player* pl, string action);
    static void acceptMove(Player *pl, int row, int column);
    static void sentMoveToOpponent(Player *pl, int row, int column);
    static void sendState(Player* pl, string state);
    static void sendResult(Player* pl, string msg);
    static void sendGameToClient(Player* pl, Game* game);
    
    static void sendStatus(Player* pl, string msg);
};


#endif /* ResponseManager_hpp */
