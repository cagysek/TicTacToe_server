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

// kvuli iotctl
#include <sys/ioctl.h>

#include <iostream>

fd_set Server::client_socks, Server::tests;
struct sockaddr_in Server::peer_addr, Server::client, Server::server;


void Server::setUp()
{
    //Create socket
    server_socket = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
    if (server_socket == -1)
    {
        printf("Could not create socket");
        return;
    }
    puts("Socket created.");
    
    //set port release
    int enable = 1;
    if ( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0 )
        perror("setsockopt(SO_REUSEADDR) failed");
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(10000);
    
    //Bind
    if( ::bind(server_socket, (struct sockaddr *) &server, sizeof(server)) < 0 )
    {
        //print the error message
        perror("Bind failed. Error");
        return;
    }
    puts("Bind done.");
    
    //Listen
    if(listen(server_socket , 3) < 0)
    {
        //print the error message
        perror("Listen failed. Error");
        return;
    }
    
    //Accept and incoming connection
    puts("Server ready!");
}



void Server::listenConnections()
{
    
    
    struct timeval client_timeout;
    client_timeout.tv_sec = 10;
    
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
                        
                        recv(pl->socket , &cbuf , 1024 , 0);

                        string msg(cbuf);
                        
                        cout << "Recv from " << pl->name << " message: " << msg.data() << endl;
                        
                        RequestManager::resolve(pl, msg);
                        
                        msg.clear();
                        
                    }
                    else if (a2read == 0)
                    {
                        Player *pl = GameManager::get_logged_player_by_socket(fd);
                        
                        if (pl != NULL)
                        {
                            if (pl->connected != -1)
                            {
                                cout << "Player: " << pl->name << " socket ID: " << pl->socket << " is disconnected" << endl;
                                GameManager::disconected_player(pl->socket);
                            }
                        }
                       
                        //if someone is disconnected remove socket
                        closeSocket(fd);
                        
                    }
                   
                    else // na socketu se stalo neco spatneho
                    {
                        close(fd);
                        FD_CLR(fd, &client_socks);
                        printf("Klient se odpojil a byl odebran ze sady socketu\n");
                    }
                 
                }
            }
        }
    }
}

void Server::closeSocket(int socket)
{
    cout << "Client exit game and was removed from sockets. Socket ID: " << socket << endl;
    close(socket);
    FD_CLR(socket, &client_socks);
}

