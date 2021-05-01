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

private:    
    SDL_Rect mBox; //Tile Attributes
    int mType; //Tile Type
};