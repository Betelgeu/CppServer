CC := g++
CFLAGS := -std=c++11 -pthread -Wall -g

all: server client

server: server.o src/Epoll.o src/InetAddress.o src/Socket.o src/Channel.o src/util.o src/EventLoop.o src/Server.o src/Acceptor.o src/Connection.o src/Buffer.o src/ThreadPool.o
	@echo "Linking server executable..."
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Server executable linked successfully!"

client: client.o src/Socket.o src/InetAddress.o src/util.o src/Buffer.o
	@echo "Linking client executable..."
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Client executable linked successfully!"

thread_test: ThreadPoolTest.o src/ThreadPool.o
	@echo "Linking thread_test executable..."
	$(CC) $(CFLAGS) $^ -o $@
	@echo "thread_test executable linked successfully!"

test: test.o src/Epoll.o src/InetAddress.o src/Socket.o src/Channel.o src/util.o src/EventLoop.o src/Server.o src/Acceptor.o src/Connection.o src/Buffer.o src/ThreadPool.o
	@echo "Linking test executable..."
	$(CC) $(CFLAGS) $^ -o $@
	@echo "test executable linked successfully!"
%.o: %.cpp
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "$< compiled successfully!"

clean:
	@echo "Cleaning up..."
	rm -f *.o src/*.o server client
	@echo "Clean up complete!"
