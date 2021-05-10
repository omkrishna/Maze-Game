#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

class Dot
{
public:
    Dot();

    bool animFlag = false;
    int score = 0;
    int dir = 2;

    void handleEvent(SDL_Event &e, int n, Tile *tiles[]);
    void move(Tile *tiles[]);
    void render(int frame, int dir, int i, Tile *tiles[]);

    SDL_Rect getBox();

private:
    SDL_Rect mBox;

    int mType;
    int mVelX, mVelY;

    void renderGhosts(Tile *tiles[]);
    void setTarget(SDL_Rect ghost, Tile *tiles[], int i);
};
