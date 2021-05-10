#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

class Tile
{
public:
    Tile(int x, int y, int tileType);

    void render();
    int getType();
    void changeType(int tileType);

    SDL_Rect getBox();

private:
    SDL_Rect mBox;
    int mType;
};
