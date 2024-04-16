#include <string.h>
#include <unistd.h>
#include <functional>
#include <iostream>
#include "Buffer.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

using namespace std;

void oneClient(int id, int msgs, int wait) {
  // 设置socket连接服务器
  Socket *sock = new Socket();
  InetAddress *addr = new InetAddress("127.0.0.1", 8888);
  sock->connect(addr);

  int sockfd = sock->getFd();

  Buffer *sendBuffer = new Buffer();
  Buffer *readBuffer = new Buffer();

  sleep(wait);
  int count = 0;
  while (count < msgs) {  // 一共发送msgs次消息
    // write to server
    sendBuffer->setBuf("I'm client!");
    ssize_t write_bytes = write(sockfd, sendBuffer->c_str(), sendBuffer->size());
    if (write_bytes == -1) {
      printf("socket already disconnected, can't write any more!\n");
      break;
    }
    // read from server
    int already_read = 0;  // 已经读取的字节数
    char buf[1024];        // 这个buf大小无所谓
    while (true) {
      bzero(&buf, sizeof(buf));
      ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
      // reading from server
      if (read_bytes > 0) {
        readBuffer->append(buf, read_bytes);
        already_read += read_bytes;
      }
      // EOF
      else if (read_bytes == 0) {
        printf("server disconnected!\n");
        exit(EXIT_SUCCESS);
      }
      // print readBuffer
      if (already_read >= sendBuffer->size()) {
        printf("client:%d count: %d, message from server: %s\n", id, count++, readBuffer->c_str());
        break;
      }
    }
    readBuffer->clear();
  }
  delete addr;
  delete sock;
}

int main(int argc, char *argv[]) {
  int threads = 100;
  int msgs = 100;
  int wait = 0;
  int o;
  const char *optstring = "t:m:w:";
  while ((o = getopt(argc, argv, optstring)) != -1) {
    switch (o) {
      case 't':
        threads = stoi(optarg);
        break;
      case 'm':
        msgs = stoi(optarg);
        break;
      case 'w':
        wait = stoi(optarg);
        break;
      case '?':
        printf("error optopt: %c\n", optopt);
        printf("error opterr: %d\n", opterr);
        break;
    }
  }

  ThreadPool *poll = new ThreadPool(threads);
  std::function<void(int)> func = std::bind(oneClient, std::placeholders::_1, msgs, wait);
  for (int i = 0; i < threads; ++i) {
    poll->add(func, i);
  }
  delete poll;
  return 0;
}
