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

void printHelp();


int main(int argc, const char * argv[])
{
    LogManager::log(__FILENAME__, __FUNCTION__, "Starting server!");
    
    if (argc == 4) {
        
        // PORT
        if(std::stoi(argv[2]) < 0 || std::stoi(argv[2]) > 65535){
            LogManager::log(__FILENAME__, __FUNCTION__, "Port out of range! Allowed value from <0; 65535>");
            printHelp();
            return 1;
        }
        
        // MAX GAMES
        if(std::stoi(argv[3]) < 0){
            LogManager::log(__FILENAME__, __FUNCTION__, "Value of max games should be > 0");
            printHelp();
            return 1;
        }
        
        Configuration *configuration = new Configuration(argv[1], atoi(argv[2]),atoi(argv[3]));
    
        Server *server = new Server();
        server->setUp(configuration);
        
        server->listenConnections();
        
        delete(server);
        delete(configuration);
        
        return 0;
    } else {
        printHelp();
    }

}

void printHelp(){
    std::cout << "Using: ./server <IP_ADDR> <PORT> <MAX_GAMES>\n" << std::endl;
    std::cout << "IP_ADDR \t- Server IP adress" << std::endl;
    std::cout << "PORT \t\t- Port to listening" << std::endl;
    std::cout << "MAX_GAMES \t- Maximum games on server" << std::endl;
}

