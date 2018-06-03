#include <chrono>

using namespace std::chrono;
class Timer
{
public:
    Timer(): m_begin(high_resolution_clock::now()) {};
    
    void reset() {m_begin = high_resolution_clock::now();}
    
    int64_t elapsed() const;
    
    int64_t elapsed_micro() const;
    
    int64_t elapsed_nano() const;
    
    int64_t elapsed_minutes() const;
    
    int64_t elapsed_hours() const;
    
    int64_t elapsed_seconds() const;
    
private:
    time_point<high_resolution_clock> m_begin;
};

int64_t Timer::elapsed() const
{
    return duration_cast<milliseconds>(high_resolution_clock::now() - m_begin).count();
}

int64_t Timer::elapsed_hours() const
{
    return duration_cast<hours>(high_resolution_clock::now() - m_begin).count();
}

int64_t Timer::elapsed_micro() const
{
    return duration_cast<microseconds>(high_resolution_clock::now() - m_begin).count();
}

int64_t Timer::elapsed_nano() const
{
    return duration_cast<nanoseconds>(high_resolution_clock::now() - m_begin).count();
}

int64_t Timer::elapsed_seconds() const
{
    return duration_cast<seconds>(high_resolution_clock::now() - m_begin).count();
}

int64_t Timer::elapsed_minutes() const
{
    return duration_cast<minutes>(high_resolution_clock::now() - m_begin).count();
}

