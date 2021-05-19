#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

class Dot
{
public:
    Dot();
    Dot(int i);
 
    int dir = 2;
    int time_req;
    int score = -1;
    int ghostedby;
    bool animFlag = false;
    bool ghosted = false;
    bool audit = false;
    bool por = false;
    SDL_Rect mBox;

    void handleEvent(SDL_Event &e, int n, Tile *tiles[]);
    void move(Tile *tiles[]);
    void render(int frame, int dir, int i, Tile *tiles[]);
    void renderFirst(int frame, int dir, int i, Tile *tiles[]);
    void renderSecond(int frame, int dir, int i, Tile *tiles[]);

    SDL_Rect getBox();

    void setTarget(SDL_Rect ghost, Tile *tiles[], int i);

private:
    int mType;
    int mVelX, mVelY;

    void renderGhosts(Tile *tiles[], int bin);
};
