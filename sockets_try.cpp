#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/socket.in>
#include<netinet/in.h>

int main() {
    struct sockaddr_in server_addr, client_addr;
    char *server_ip_addr = NULL;
    int sock_server, sock_client;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);

    window = SDL_CreateWindow("My game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 540, 0);
    if(window == NULL) {
        cout << "Error: Could not create window: " << SDL_GetError() << endl;
        SDL_Quit();
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        SDL_DestryWindow(window);
        cout << "Error: Could not create renderer: " << SDL_GetError() << endl;
        SDL_Quit();
        return 0;
    }

    // load resoucres here

    // main menu here
        // if client
        // if server
        // create and bind sockets etc


    // main game loop

    // close connection
    // close threads
    // destroy windows, renderers 
}
