CC := g++
CFLAGS := -std=c++11 -Wall -g

all: server client

server: server.o src/Epoll.o src/InetAddress.o src/Socket.o src/Channel.o src/util.o src/EventLoop.o src/Server.o
	@echo "Linking server executable..."
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Server executable linked successfully!"

client: client.o src/Socket.o src/InetAddress.o src/util.o
	@echo "Linking client executable..."
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Client executable linked successfully!"

%.o: %.cpp
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "$< compiled successfully!"

clean:
	@echo "Cleaning up..."
	rm -f *.o src/*.o server client
	@echo "Clean up complete!"
