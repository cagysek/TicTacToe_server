//
//  Server.hpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 12/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp

#include "Player.hpp"
#include "Game.hpp"
#include "RequestManager.hpp"
#include "LogManager.hpp"

#include <stdio.h>
#include <iostream>

class Server
{
    public:
        void setUp();
        void listenConnections();
        static void closeSocket(int socket);
    
        static fd_set client_socks, tests;
    
    private:
        int server_socket, client_socket, fd;
        static struct sockaddr_in server , client, peer_addr;
};

#endif /* Server_hpp */
