CC := g++
CFLAGS := -std=c++11 -Wall -g

all: server client

server: util.o server.o Epoll.o InetAddress.o Socket.o Channel.o
	@echo "Linking server executable..."
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Server executable linked successfully!"

client: util.o client.o Socket.o InetAddress.o
	@echo "Linking client executable..."
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Client executable linked successfully!"

%.o: %.cpp
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "$< compiled successfully!"

clean:
	@echo "Cleaning up..."
	rm -f *.o server client
	@echo "Clean up complete!"
