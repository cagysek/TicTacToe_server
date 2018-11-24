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
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

class ResponseManager
{
public:
    static void sendToClient(Player* pl, string msg);
};


#endif /* ResponseManager_hpp */
