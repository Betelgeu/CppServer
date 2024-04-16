#include <stdio.h>
#include "Acceptor.h"
#include "Socket.h"
#include "Channel.h"
#include "Server.h"

Acceptor::Acceptor(EventLoop *_loop) : loop(_loop), sock(nullptr), acceptChannel(nullptr)
{
    sock = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 8888);
    sock->bind(addr);
    // acceptor使用阻塞式IO比较好，因为acceptor只负责接收新连接，不负责读写数据
    // sock->setnonblocking();
    sock->listen(); 
    acceptChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setReadCallback(cb);
    acceptChannel->enableRead();
    delete addr;
}

Acceptor::~Acceptor(){
    delete sock;
    delete acceptChannel;
}

void Acceptor::acceptConnection(){
    // 1. acceptor接收新连接，打印
    InetAddress *clnt_addr = new InetAddress();      
    Socket *clnt_sock = new Socket(sock->accept(clnt_addr));      
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->getAddr().sin_addr), ntohs(clnt_addr->getAddr().sin_port));
    clnt_sock->setnonblocking();
    // 2. 回调函数转回server，由server维护new Connection
    newConnectionCallback(clnt_sock);
    delete clnt_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _cb){
    newConnectionCallback = _cb;
}