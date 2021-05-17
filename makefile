build: main.cpp objects.cpp	client_udp.cpp server_udp.cpp network.cpp physic.cpp list.cpp font.cpp menu.cpp create_maze.cpp sound.cpp
	g++ -w -o game main.cpp objects.cpp client_udp.cpp server_udp.cpp network.cpp physic.cpp list.cpp font.cpp menu.cpp create_maze.cpp sound.cpp `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lSDL2_ttf -lpthread -I. -Wall
