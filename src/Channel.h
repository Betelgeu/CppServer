#pragma once
#include <sys/epoll.h>
#include <functional>

class Socket;
class EventLoop;
class Channel
{
private:
    EventLoop *loop;
    int fd;

    uint32_t events; // events that we want to listen
    uint32_t ready;  // events that are ready
    bool inEpoll;
    bool useThreadPool;
    // callback depend on ready event type when events are ready
    std::function<void()> readCallback;
    std::function<void()> writeCallback;
public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();

    void handleEvent();
    void enableRead();

    int getFd();
    uint32_t getEvents();
    uint32_t getReady();
    bool getInEpoll();
    void setInEpoll(bool _in = true);
    void useET();

    void setReady(uint32_t);
    void setReadCallback(std::function<void()>);
    void setUseThreadPool(bool use = true);
};

