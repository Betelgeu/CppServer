#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#include "InetAddress.h"
#include "Socket.h"

#define BUFFER_SIZE 1024 

int main() {
    // set socket
    Socket *serv_sock = new Socket();
    // connect
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->connect(serv_addr);

    while(true){
        char buf[BUFFER_SIZE];  //buf大小必须大于或等于服务器端buf大小，否则无法一次接受完服务器发回的数据
        // write
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(serv_sock->getFd(), buf, sizeof(buf));
        if(write_bytes == -1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        // read
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(serv_sock->getFd(), buf, sizeof(buf));
        if(read_bytes > 0){
            printf("message from server: %s\n", buf);
        }else if(read_bytes == 0){
            printf("server socket disconnected!\n");
            break;
        }else if(read_bytes == -1){
            delete serv_addr;
            delete serv_sock;
            errif(true, "socket read error");
        }
    }
    delete serv_addr;
    delete serv_sock;
    return 0;
}
