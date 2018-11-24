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
    msg = msg += "\n";
    
    send(pl->socket, msg.data(), msg.length(), 0);
    
    cout << "Message: " << msg << "sent to " << pl->name << endl;
    
    msg.clear();
}
