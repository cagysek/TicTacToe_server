//
//  Utils.cpp
//  TicTacToe
//
//  Created by Jan Čarnogurský on 29/11/2018.
//  Copyright © 2018 Jan Čarnogurský. All rights reserved.
//

#include "Utils.hpp"

/**
 *  Split message by specific delimeter
 */
vector<string> Utils::split(const string str, const string delim)
{
        vector<string> tokens;
        size_t prev = 0, pos = 0;
    
        size_t valid = str.find(delim, prev);
    
        int max_size_of_first_part = 20;
    
        if(valid > max_size_of_first_part || valid <= 0)
        {
            return tokens;
        }
    
        do {
            pos = str.find(delim, prev);
            
            if (pos == string::npos) pos = str.length();
            
            string token = str.substr(prev, pos - prev);
            
            if (!token.empty()) tokens.push_back(token);
            
            prev = pos + delim.length();
        } while (pos < str.length() && prev < str.length());
    
        return tokens;
}
