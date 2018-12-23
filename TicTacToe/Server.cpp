//
//  Server.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 12/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include "Server.hpp"

#include <stdio.h>

#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>

// iotctl
#include <sys/ioctl.h>

#include <iostream>

fd_set Server::client_socks, Server::tests;
struct sockaddr_in Server::peer_addr, Server::client, Server::server;

/**
 * Server initilization
 */
void Server::setUp(Configuration *config)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "IP: " + config->ip + " Port: " + to_string(config->port) + " Max_games: " + to_string(config->max_games));
    
    //Create socket
    server_socket = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
    if (server_socket == -1)
    {
        LogManager::log(__FILENAME__, __FUNCTION__, "Could not create socket");
        return;
    }
    
    LogManager::log(__FILENAME__, __FUNCTION__, "Socket created");
    
    //set port release
    int enable = 1;
    if ( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0 )
    {
       LogManager::log(__FILENAME__, __FUNCTION__, "setsockopt(SO_REUSEADDR) failed");
    }
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
/*
    if (config->ip.compare("localhost") == 0)
    {
        server.sin_addr.s_addr = inet_addr("127.0.0.1");
    }
    else if (config->ip.compare("INADDR_ANY") == 0)
    {
        server.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        in_addr_t inAddr = inet_addr(config->ip.data());
        if (inAddr == -1)
        {
            LogManager::log(__FILENAME__, __FUNCTION__, "Invalid IP adddress");
            return;
        }
        else
        {
            server.sin_addr.s_addr = inAddr;
        }
    }
*/
    server.sin_port = htons(config->port);
 
   server.sin_addr.s_addr = INADDR_ANY;
  //  server.sin_port = htons(10000);
    
    //Bind
    if( ::bind(server_socket, (struct sockaddr *) &server, sizeof(server)) < 0 )
    {
        //print the error message
        LogManager::log(__FILENAME__, __FUNCTION__, "Bind failed. Error");
        return;
    }
    
    LogManager::log(__FILENAME__, __FUNCTION__, "Bind done");
    
    //Listen
    if(listen(server_socket , 3) < 0)
    {
        LogManager::log(__FILENAME__, __FUNCTION__, "Listen failed. Error");
        return;
    }
    
    GameManager::set_max_games(config->max_games);
    
    //Accept and incoming connection
    LogManager::log(__FILENAME__, __FUNCTION__, "Server is ready!");
}



void Server::listenConnections()
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Start listening");
    
    struct timeval client_timeout;
    client_timeout.tv_sec = 180;
    
    FD_ZERO(&client_socks);
    FD_SET(server_socket, &client_socks);
    
    int return_value, fd;
    
    while (1)
    {
        // zkopirujeme si fd_set do noveho, stary by byl znicen (select ho modifikuje)
        tests = client_socks;
        
        // sada deskriptoru je po kazdem volani select prepsana sadou deskriptoru kde se neco delo
        return_value = select(FD_SETSIZE, &tests, (fd_set*)NULL, (fd_set*)NULL, &client_timeout);
        
        int a2read;
        char cbuf[1024];
        // vynechavame stdin, stdout, stderr
        for (fd = 3; fd < FD_SETSIZE; fd++)
        {
            
            // je dany socket v sade fd ze kterych lze cist ?
            if (FD_ISSET(fd, &tests))
            {
                // je to server socket? prijmeme nove spojeni
                if (fd == server_socket)
                {
                    int c = sizeof(struct sockaddr_in);
                    client_socket = accept(server_socket, (struct sockaddr *)&client, (socklen_t*)&c);
                    FD_SET(client_socket, &client_socks);
                    
                    char *ip = inet_ntoa(client.sin_addr);
                    
                    GameManager::create_unlogged_player(ip, client_socket);
                    
                }
                else // je to klientsky socket? prijmem data
                {
                    // pocet bajtu co je pripraveno ke cteni
                    ioctl(fd, FIONREAD, &a2read);
                    
                    // mame co cist
                    if (a2read > 0)
                    {
                        
                        Player *pl = GameManager::get_logged_player_by_socket(fd);
                        
                        if (pl == NULL)
                        {
                            pl = GameManager::get_unlogged_player(fd);
                        }
                        
                        //clearbuffer
                        memset(cbuf, 0, 1024);
                        
                        int recv_result = (int)recv(pl->socket , &cbuf , 1024 , 0);
                        
                        // lost connection
                        if (recv_result <= 0)
                        {
                            disconnect(fd);
                        }
                        
                        //timeout
                        if (return_value == 0)
                        {
                            GameManager::exit(pl);
                        }

                        string msg(cbuf);
                        
                        RequestManager::resolve(pl, msg);
                        
                        msg.clear();
                        
                    }
                    // if gui is closed by ctrl+c
                    else if (a2read == 0)
                    {
                        disconnect(fd);
                    }
                   
                    else // na socketu se stalo neco spatneho
                    {
                        close(fd);
                        FD_CLR(fd, &client_socks);
                        
                        LogManager::log(__FILENAME__, __FUNCTION__, "Something bad happened on ioct");
                    }
                 
                }
            }
        }
    }
}

void Server::disconnect(int socket)
{
    Player *pl = GameManager::get_logged_player_by_socket(socket);
    
    if (pl != NULL)
    {
        if (pl->connected != -1)
        {
            LogManager::log(__FILENAME__, __FUNCTION__, "Player: " + pl->name + " socket ID: " + to_string(pl->socket) + " disconnecting...");
            
            GameManager::disconected_player(pl->socket);
        }
    }
    
    //if someone is disconnected remove socket
    closeSocket(socket);
}

/**
 * Method to close specific socket
 */
void Server::closeSocket(int socket)
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Closing socket with ID: " + to_string(socket));
    
    close(socket);
    FD_CLR(socket, &client_socks);
}




