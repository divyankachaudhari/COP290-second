#include "sound.h"

Mix_Music *music = NULL;
Mix_Chunk *kill_chunk = NULL; // 3
Mix_Chunk *type_chunk = NULL; // 0
Mix_Chunk *shoot_chunk = NULL; // 1
Mix_Chunk *god_chunk = NULL; // 2


void init_sound() {
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
    }

    music = Mix_LoadMUS( "resources/sounds/StarWars60.wav" );
    if( music == NULL )
	{
		cout << "Failed to load beat music! SDL_mixer Error: "<<  Mix_GetError() << endl;
	}

    kill_chunk = Mix_LoadWAV( "resources/sounds/death.wav" );
    if( kill_chunk == NULL )
	{
		cout << "Failed to load beat music! SDL_mixer Error: "<<  Mix_GetError() << endl;
	}

    type_chunk = Mix_LoadWAV( "resources/sounds/typing.wav" );
    if( type_chunk == NULL )
	{
		cout << "Failed to load beat music! SDL_mixer Error: "<<  Mix_GetError() << endl;
	}

    shoot_chunk = Mix_LoadWAV( "resources/sounds/shoot.wav" );
    if( shoot_chunk == NULL )
	{
		cout << "Failed to load beat music! SDL_mixer Error: "<<  Mix_GetError() << endl;
	}

    god_chunk = Mix_LoadWAV( "resources/sounds/god_change.wav" );
    if( god_chunk == NULL )
	{
		cout << "Failed to load beat music! SDL_mixer Error: "<<  Mix_GetError() << endl;
	}
}

void play_sound(int v) {
    switch (v) {
        case 0:
        Mix_PlayChannel( -1, type_chunk, 0 );
        break;

        case 1:
        Mix_PlayChannel( -1, shoot_chunk, 0 );
        break;

        case 2:
        Mix_PlayChannel( -1, god_chunk, 0 );
        break;

        case 3:
        Mix_PlayChannel( -1, kill_chunk, 0 );
        break;

        case 4:
        Mix_PlayMusic(music, -1);
    }
}

void pause_resume_music(SDL_Event e) {
    if(e.key.keysym.sym == SDLK_m) {
        // if(!Mix_PlayingMusic()) {
            play_sound(4);
        // }
    } else if(e.key.keysym.sym == SDLK_p) {
        if(Mix_PausedMusic()) {
            // Mix_ResumeMusic();
        } else {
            Mix_PauseMusic();
        }
    }
}

void close_sound() {
    Mix_FreeChunk( kill_chunk );
	Mix_FreeChunk( type_chunk );
	Mix_FreeChunk( shoot_chunk );
	Mix_FreeChunk( god_chunk );
	kill_chunk = NULL;
	type_chunk = NULL;
	shoot_chunk = NULL;
	god_chunk = NULL;

	//Free the music
	Mix_FreeMusic( music );
	music = NULL;

    Mix_Quit();
}
