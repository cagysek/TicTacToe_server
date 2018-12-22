//
//  main.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 12/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include <iostream>
#include "Server.hpp"
#include "Network.hpp"
#include "LogManager.hpp"

int main(int argc, const char * argv[])
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Starting server!");
    
    Server *server = new Server();
    server->setUp();
    
    server->listenConnections();
    
    return 0;
}
