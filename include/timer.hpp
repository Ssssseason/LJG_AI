#ifndef TIMER_HPP
#define TIMER_HPP
#include <chrono>
#include <iostream>
#include <ctime>
using namespace std;

class Timer
{
    chrono::system_clock::time_point start = chrono::system_clock::now();
    int seconds;
  public:
    Timer(int seconds):seconds(seconds){}
    double getTimeLeft(){
        chrono::system_clock::time_point now = chrono::system_clock::now();
        return seconds - (double)chrono::duration_cast<chrono::milliseconds>(now - start).count() * chrono::milliseconds::period::num / chrono::milliseconds::period::den;
    }
};

#endif