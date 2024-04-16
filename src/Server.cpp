#include <functional>
#include <unistd.h>
#include "Server.h"
#include "Socket.h"
#include "Channel.h"
#include "Acceptor.h"
#include "Connection.h"
#include "ThreadPool.h"
#include "EventLoop.h"

Server::Server(EventLoop *_loop) : mainReactor(_loop), acceptor(nullptr){
    // set Acceptor
    acceptor = new Acceptor(mainReactor);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
    // set ThreadPool
    int size = std::thread::hardware_concurrency(); // 获取CPU核心最大并发数量
    thpool = new ThreadPool(size);
    // set subReactors
    for(int i = 0; i < size; ++i){
        subReactors.push_back(new EventLoop());
    }
    for(int i = 0; i < size; ++i){
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactors[i]);
        thpool->add(sub_loop);
    }
}

Server::~Server()
{
    delete acceptor;
    delete thpool;
}


void Server::newConnection(Socket *sock){
    if(sock->getFd() != -1){
        // 新连接随机选择一个subReactor
        int random = sock->getFd() % subReactors.size();
        Connection *conn = new Connection(subReactors[random], sock);
        std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connections[sock->getFd()] = conn;
    }
}

void Server::deleteConnection(int sockfd){
    if(sockfd != -1){
        auto it = connections.find(sockfd);
        if(it != connections.end()){
            Connection *conn = connections[sockfd];
            connections.erase(sockfd);
            close(sockfd);       //正常
            // delete conn;         //会Segmant fault
        }
    }
}