#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "game.h"
#include "player.h"
#include "text.h"
#include "events.h"
#include "maps.h"
#include "gameObj.h"
#include "settings.h"
#include "client.h"
#include "menu.h"
#include "mainMenu.h"
#include "toast.h"

#ifdef _WIN32
#include <winsock.h>
#endif

extern SDL_Texture *playerTexture;
extern SDL_Texture *playerTextureCrouch;
extern Player MainPlayer;
extern struct OtherPlayer *players;
extern size_t numPlayers;
extern bool paused;
extern double dt;
extern bool playing;

extern TTF_Font *Font;
extern SDL_Color White;

extern SDL_Texture **loadedTextures;
extern size_t textureSize;

extern SDL_Texture **loadedInteractablesActive;
extern SDL_Texture **loadedInteractablesInactive;
extern size_t interactableSize;

extern size_t textArrSize;
extern struct Text *textArr;

extern SDL_Thread *getUpdate;

extern bool onServer;

void render();
void calculateDT();