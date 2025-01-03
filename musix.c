#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>


#define PROGRESS_BAR_LENGTH 50

// display progress bar
void displayProgressBar(int position, int total) {
    int filled = (position * PROGRESS_BAR_LENGTH) / stotal;
      printf("\r#");
    for (int i = 0; i < PROGRESS_BAR_LENGTH; i++) {
        if (i < filled)
            printf("=");
        else
            printf(" ");
    }
    printf("] %d%%", (position * 100) / total);
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file.wav|file.mp3>\n", argv[0]);
        return 1;
    }

    //  FUCK UP DETECTOR
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Mix_OpenAudio Error: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    // Load audio 
    Mix_Music *music = Mix_LoadMUS(argv[1]);
    if (!music) {
        fprintf(stderr, "Mix_LoadMUS Error: %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    printf("Playing: %s\n", argv[1]);
    printf("Press SPACE to pause/resume, 'q' to quit.\n");

    // Play 
    if (Mix_PlayMusic(music, 1) == -1) {  // Play once
        fprintf(stderr, "Mix_PlayMusic Error: %s\n", Mix_GetError());
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    bool running = true;
    bool paused = false;

    // Event loop for play/pause and quit
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
                    running = false;  // Quit
                } else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                    paused = !paused;
                    if (paused) {
                        Mix_PauseMusic();
                        printf("\nPaused. Press SPACE to resume.\n");
                    } else {
                        Mix_ResumeMusic();
                        printf("\nResumed. Press SPACE to pause.\n");
                    }
                }
            }
        }

        //  progress bar if not paused
        if (!paused) {
            int position = Mix_PlayingMusic() ? Mix_GetMusicPosition(NULL) : 0;  // Current position in ms
            int totalTime = 1000;  // Replace with total duration calculation if available
            displayProgressBar(position, totalTime);
        }

        SDL_Delay(90);  //optimze(I SAW TAHT THIS HELPS WITH CPU UTILIZATION)
    }

    // holdup
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
//NOW DONE
    printf("\nPlayback finished.\n");
    return 0;
}
