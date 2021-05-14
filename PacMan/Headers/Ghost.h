#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

class Ghost
{
public:
    Ghost(int x, int y, int i);

    void render(int bin);
    bool isDead();

    SDL_Rect mBox;

private:
    int mFrame;
    Texture *mTexture;
};