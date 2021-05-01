/* NON USABLE */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "Headers/Texture.h"
#include "Headers/Tile.h"

//Tile Constants
const int tWidth = 80;
const int tHeight = 80;
const int tNumber = 192; // = 16 * 12
const int tSprites = 4;

//Scene Textures
Texture tTexture;
SDL_Rect tClips[tSprites];

//TILE CLASS DEFINITIONS

Tile::Tile(int x, int y, int tileType) //Tile Position and Type
{    
    mBox.x = x; //Get the offsets
    mBox.y = y;
    
    mBox.w = tWidth; //Set the collision box
    mBox.h = tHeight;
    
    mType = tileType; //Get the tile type
}

void Tile::render() //Show Tile
{
    tTexture.render(mBox.x, mBox.y, &tClips[mType]);
}

int Tile::getType() //Get tileType
{
    return mType;
}

