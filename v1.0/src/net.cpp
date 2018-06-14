//
//  net.cpp
//  AI
//
//  Created by Yinr on 2018/6/3.
//  Copyright © 2018年 Yinr. All rights reserved.
//

#include "net.hpp"

string tcurl(string url){
    CURL *curl;
    curl = curl_easy_init();
    string res="";
    
    //抓取http数据
    curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&res);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, myWrite);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);
    //超时，不接受其他信号
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
    //执行
    curl_easy_perform(curl);
    //关闭
    curl_easy_cleanup(curl);
    //cout << version << endl;
    //cout << res << endl;
    return res;
}
string tcurl(char x, char y){
    //char *version = curl_version();
    CURL *curl;
    curl = curl_easy_init();
    string res="";
    string SERVER_IP="http://10.180.58.5:5000/";
    string url=SERVER_IP+"move/"+id+"/"+x+"/"+y+"/"+player;
    
    //抓取http数据
    //curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str()); // params
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&res);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, myWrite);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);
    //超时，不接受其他信号
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
    //执行
    curl_easy_perform(curl);
    //关闭
    curl_easy_cleanup(curl);
    //cout << version << endl;
    //cout << res << endl;
    return res;
}
//数据的获取
size_t myWrite( void *ptr, size_t size, size_t nmemb, void *stream){
    string *str = dynamic_cast<string*>((string*)stream);
    if( 0 == size ||   NULL == ptr){
        return -1;
    }
    
    char* pData = (char*)ptr;
    str->append(pData,nmemb*size);
    return nmemb*size;
}
void changeData(string netData,Bitboard* bp){
    (*bp).white=0;
    (*bp).black=0;
    for(int i=7;i>=0;i--){
        for(int j=7;j>=0;j--){
            if(netData[i*16+j*2]=='W'){
                (*bp).white=(*bp).white*2+1;
                (*bp).black*=2;
            }
            else if(netData[i*16+j*2]=='B'){
                (*bp).white*=2;
                (*bp).black=(*bp).black*2+1;
            }
            else {
                (*bp).white*=2;
                (*bp).black*=2;
            }
        }
    }
}
