//
//  net.hpp
//  AI
//
//  Created by Yinr on 2018/6/3.
//  Copyright © 2018年 Yinr. All rights reserved.
//

#ifndef NET_HPP
#define NET_HPP

#include <stdio.h>
#include "curl/curl.h"
#include <iostream>
#include "bitboard.hpp"
using namespace std;

extern string id;
extern string player;
extern string qipan;
//1 2 3
string tcurl(string url);

//4下棋
//ret = requests.post(SERVER_IP+"move/3/2/W")
void tcurl(char x, char y);

size_t myWrite( void *ptr, size_t size, size_t nmemb, void *stream);
void changeData(string netData,Bitboard b);

#endif /* net_hpp */
