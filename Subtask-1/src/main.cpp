#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include <ctime>
#include <bits/stdc++.h>
#include "header-files/create_maze.h"
#include "header-files/objects.h"
#include "header-files/client_udp.h"
#include "header-files/server_udp.h"
#include "header-files/network.h"
#include "header-files/physics.h"
#include "header-files/constants.h"
#include "header-files/font.h"
#include "header-files/menu.h"
#include "header-files/sound.h"

struct Player players[MAX_PLAYERS];
int number_of_players = 0;
int16_t my_id = -1;
int16_t bullets_client[256];
int bullets_number = 0;
int maze[15][20]; // 2X
int god = -1;
int maze_array[300];

SDL_Texture* load_texture(SDL_Renderer *renderer, char *file) {
    SDL_Surface *bitmap = NULL;
    SDL_Texture *texture = NULL;
    bitmap = SDL_LoadBMP(file);
    texture = SDL_CreateTextureFromSurface(renderer, bitmap);
    SDL_FreeSurface(bitmap);
    return texture;
}

void init_players() {
    int i;
    for (i = 0; i < MAX_PLAYERS; i++) {
        players[i].position.x = SPAWN_X;
        players[i].position.y = SPAWN_Y;
        players[i].position.w = PLAYER_WIDTH;
        players[i].position.h = PLAYER_HEIGHT;
        players[i].left_key = SDLK_LEFT;
        players[i].right_key = SDLK_RIGHT;
        players[i].up_key = SDLK_UP;
        players[i].down_key = SDLK_DOWN;
        players[i].attack_key = SDLK_z;
        players[i].face = 1;
        players[i].shoot = false;
        players[i].y_speed = 0;
        players[i].can_jump = false;
        players[i].reloading = false;
        players[i].kills = 0;
        players[i].deaths = 0;
        players[i].score = 0;
        players[i].immune_time = 0;
        players[i].prev_bullet_time = -3;
    }
}

void receive_new_id(int id) {
    my_id = id;
    number_of_players = id;
    printf("my_id is now: %d\n", my_id);
}

void check_if_its_new_player(int id){
    if (id > number_of_players) {
        number_of_players = id;
        printf("new max player, now %d\n", number_of_players + 1);
    }
}


void* client_loop(void *arg) {
    int socket = *((int *) arg);
    int16_t tab[BUF_MAX];
    int length;
    int id, bullets_in_array;
    while (1) {
        length = client_listen(socket, tab);
        id = tab[0];
        // if(tab[0] == 'h') {
        //     cout << "recieved map\n";
        // }
        // if(sizeof(tab) == 300*sizeof('1')) {
        //     cout << "recieved map\n";
        // }
        if (id == -1) {
            receive_new_id(tab[1]);
        }
        if (id >= 0) {
            check_if_its_new_player(id);
            players[id].position.x = tab[1];
            players[id].position.y = tab[2];
            players[id].score = tab[3];
            players[id].deaths = tab[4];
            players[id].immune_time = tab[5];// //
        }
        if (id == -2) {
            bullets_in_array = (length - sizeof(int16_t)) / (sizeof(int16_t) * 2);
            memcpy(bullets_client, tab + 1, sizeof(int16_t) * 2 * bullets_in_array);
            bullets_number = bullets_in_array;
        }
        // if(id == -3) {
        //     cout << "recieved map\n";
        //     // for(int i=0; i<300; i++) {
        //     //     int x, y;
        //     //     y = i%20;
        //     //     x = i/20;
        //     //     maze[x][y] = tab[i];
        //     // }
        //     memcpy(maze_array, tab+1, 300);
        // }
        usleep(50);
    }
}

int main(int argc, char** argv){

    struct sockaddr_in server_addr, client_addr;
    int sock_server, sock_client;
    char *server_ip_addr = NULL;
    // char *name = NULL;

    char menu = 's';
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Texture *tex = NULL;
    SDL_Texture *immune_tex = NULL;
    SDL_Texture *blink0 = NULL;
    SDL_Texture *blink1 = NULL;
    SDL_Texture *blink2 = NULL;
    SDL_Texture *tex2 = NULL;
    SDL_Texture *tex3 = NULL;
    SDL_Texture *bullet = NULL;
    SDL_Texture *map = NULL;
    TTF_Init();
    TTF_Font *font;
    font = TTF_OpenFont("resources/m5x7.ttf", 60);
    init_players();
    char* name = "game";
    if(argc) {
        name = argv[1];
    }
    window = SDL_CreateWindow(
            name,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            1280, // 2X
            960, // 2X
            0);

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    init_sound();
    play_sound(4);
    int i;

    name = (char*) malloc(16 * sizeof(char));
    // ask_for_name(renderer, font, name);
    server_or_client(renderer, &menu, font);

    // cout << "menu : " << menu << endl;
    bool create_new_maze = 1;
    if(menu == 'c') {
        create_new_maze = 0;
    }
    if(create_new_maze) {
        srand((unsigned int)time(NULL)); //seed random number generator with system time
    	initialize();      //initialize the maze
    	generate();        //generate the maze
        reflect();
    	savetxt();
        cout << "New map created.\n";
    }

    ifstream in_maze("map.txt");
    if(!in_maze.is_open()) {
        cout << "Error : Could not open map.txt \n";
        exit(0);
    }
    string line;
    int ylen = 20; // 2X
    int x=0;
    while(getline(in_maze, line)) {
        for(int y=0; y<ylen; y++) {
            if(line[y] == 'W') {
                maze[x][y] = 0;
            } else if(line[y] == 'B') {
                // maze[x][y] = 2;
                maze[x][y] = 2;
            }
        }
        x++;
    }


    map = get_map_texture(renderer);
    tex = load_texture(renderer, "resources/player.bmp");
    immune_tex = load_texture(renderer, "resources/yellow.bmp");
    tex2 = load_texture(renderer, "resources/dotTile.bmp");
    tex3 = load_texture(renderer, "resources/tile.bmp");
    blink0 = load_texture(renderer, "resources/pacman_green_0.bmp");
    blink1 = load_texture(renderer, "resources/pacman_green_1.bmp");
    blink2 = load_texture(renderer, "resources/pacman_green_2.bmp");
    bullet = load_texture(renderer, "resources/yellow.bmp");

    if (menu == 'c') {
        server_ip_addr = (char*) malloc(16 * sizeof(char));
        ask_for_ip(renderer, font, server_ip_addr);
    }
    pthread_t thread_id_server, thread_id_client, thread_id_server_send;
    server_addr = server_sock_addr(server_ip_addr);
    client_addr = client_sock_addr();
    if(menu == 's') {
        prepare_server(&sock_server, &server_addr);
        pthread_create(&thread_id_server, NULL, server_receive_loop, &sock_server);
        pthread_create(&thread_id_server_send, NULL, server_send_loop, &sock_server);
    }
    prepare_client(&sock_client, &client_addr);
    pthread_create(&thread_id_client, NULL, client_loop, &sock_client);

    while (my_id < 0) {
        send_to_server(sock_client, server_addr, my_id, 0);
        usleep(100);
    }

    // cout << "my_id is: "<<my_id<<endl;
    // cout << "player name: "<<name<<endl;
    // players[my_id].name = name;
    // cout << "name set: "<<players[my_id].name;

    SDL_Rect bullet_pos;
    bullet_pos.w = BULLET_HEIGHT;
    bullet_pos.h = BULLET_HEIGHT;


    SDL_Event e;
    int c=0;
    time_t dif = 0;
    int winner = -1;
    int blinker = 0;
    while (1) {
        c++;
        if(c == 100) {
            time_t now = time(0);
            long int v = static_cast<long int>(now);
            v = v % 30;
            int new_god = (int)v/(int)(30/(number_of_players+1));
            if(god != new_god) {
                play_sound(2);
                god = new_god;
            }
            c=0;
            // cout << "God: " << god << endl;
        }
        // map = get_map_texture(renderer);

        // if(SDL_PollEvent(&e)) {
        //     pause_resume_music(e);
        // }

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
            resolve_keyboard(e, &players[my_id]);
            pause_resume_music(e);
        }
        send_to_server(sock_client, server_addr, my_id, key_state_from_player(&players[my_id]));
        usleep(30);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, map, NULL, NULL);
        for (i = 0; i <= number_of_players; i++) {
            // time_t now = time(0);
            // now = now%1000;
            // long int c = now - players[i].spawn_time;
            // cout << players[i].immune_time << endl;
            // cout << c << endl;
            int c_immune = players[i].immune_time;
            if(c_immune > 0) {
                SDL_RenderCopy(renderer, immune_tex, NULL, &players[i].position);
            } else {
                if(blinker < 10) {
                    SDL_RenderCopy(renderer, blink0, NULL, &players[i].position);
                } else if(blinker < 20) {
                    SDL_RenderCopy(renderer, blink1, NULL, &players[i].position);
                } else if(blinker < 30) {
                    SDL_RenderCopy(renderer, blink2, NULL, &players[i].position);
                }
            }
        }
        blinker++;
        if(blinker == 30) blinker = 0;

        disp_text(renderer, "GOD: ", font, 600, 10);
        char god_c[10] = {};
        sprintf(god_c, "%d", god);
        disp_text(renderer, god_c, font, 680, 10);

        if(number_of_players > 1) {
            disp_text(renderer, "player", font, 800, 10);
            for (i = 0; i <= number_of_players; i++) {
                char id[2] = {};
                sprintf(id, "%d", i);
                disp_text(renderer, id, font, 800, 40 + i * 40);
            }

            disp_text(renderer, "score", font, 980, 10);
            for (i = 0; i <= number_of_players; i++) {
                char score[10] = {};
                sprintf(score, "%d", players[i].score);
                disp_text(renderer, score, font, 980, 40 + i * 40);
            }

            // disp_text(renderer, "deaths", font, 1080, 10);
            // for (i = 0; i <= number_of_players; i++) {
            //     char deaths[10] = {};
            //     sprintf(deaths, "%d", players[i].deaths);
            //     disp_text(renderer, deaths, font, 1080, 40 + i * 40);
            // }
        } else {
            disp_text(renderer, "player", font, 800, 10);
            for (i = 0; i <= number_of_players; i++) {
                char id[2] = {};
                sprintf(id, "%d", i);
                disp_text(renderer, id, font, 800, 40 + i * 40);
            }

            disp_text(renderer, "score", font, 940, 10);
            for (i = 0; i <= number_of_players; i++) {
                char score[10] = {};
                sprintf(score, "%d", players[i].score);
                disp_text(renderer, score, font, 940, 40 + i * 40);
            }
        }


        for (i = 0; i < bullets_number; i++) {
            bullet_pos.x = bullets_client[i*2];
            bullet_pos.y = bullets_client[i*2 + 1];
            SDL_RenderCopy(renderer, bullet, NULL, &bullet_pos);
        }
        SDL_RenderPresent(renderer);

        for(i=0; i <= number_of_players; i++) {
            if(players[i].score > 11) {
                winner = i;
                break;
            }
        }
        if(winner!=-1) break;
    }

    if(winner != -1) {
        // while(1) {
            SDL_RenderClear(renderer);

            disp_text(renderer, "Player  wins!", font, 480, 400);
            char win[10] = {};
            sprintf(win, "%d", winner);
            disp_text(renderer, win, font, 610, 400);

            SDL_RenderPresent(renderer);
            usleep(5000000);
        // }
    }

    close(sock_client);
    close(sock_server);
    pthread_cancel(thread_id_client);
    pthread_cancel(thread_id_server);
    pthread_cancel(thread_id_server_send);
    close_sound();
    SDL_DestroyTexture(tex);
    SDL_DestroyTexture(bullet);
    SDL_DestroyTexture(map);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
