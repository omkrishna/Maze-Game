#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "Headers/Texture.h"
#include "Headers/Tile.h"

/* #include "Texture.cpp"
#include "Tile.cpp" */

//Screen Dimensions
const int sWidth = 1080;
const int sHeight = 840;

//Tile Constants
const int tWidth = 40;
const int tHeight = 40;
const int tNumber = (sWidth / tWidth) * (sHeight / tHeight); // = 16 * 12
const int tSprites = 3;

//Tile Sprites
const int N = 0;  // white/no tile: 0
const int T1 = 1; // grey tile: 1
const int T2 = 2; // black tile: 2

//Scene Textures
Texture tTexture;
SDL_Rect tClips[tSprites];

SDL_Window *Window = NULL;      //Window
SDL_Renderer *WRenderer = NULL; //window Renderer

bool init() //Starts up SDL and creates window
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) //Initialize SDL
    {
        printf("SDL Initialization Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Window
        Window = SDL_CreateWindow("Random Maze", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sWidth, sHeight, SDL_WINDOW_SHOWN);
        if (Window == NULL)
        {
            printf("Window Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Window Renderer
            WRenderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (WRenderer == NULL)
            {
                printf("Renderer Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(WRenderer, 0xFF, 0xFF, 0xFF, 0xFF); //Renderer color
                int imgFlags = IMG_INIT_PNG;                               //PNG loading
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool set(Tile *tiles[]) //Sets Tiles from Tile Map
{
    bool tilesLoaded = true;
    int x = 0, y = 0; //Tile Offsets

    std::ifstream map("Map/mapfile.map"); //Open mapfile
    if (map.fail())
    {
        printf("Mapfile not loaded!\n"); //If mapfile cannot be loaded
        tilesLoaded = false;
    }
    else
    {
        for (int i = 0; i < tNumber; ++i) //Initialize the tiles
        {
            int tileType = -1;
            map >> tileType;

            if (map.fail()) //Insufficient tileTypes in mapfile
            {
                printf("Error loading map: Unexpected end of file!\n");
                tilesLoaded = false;
                break;
            }

            if ((tileType >= 0) && (tileType < tSprites)) //Valid tileType
                tiles[i] = new Tile(x, y, tileType);
            else
            {
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }

            //Move to next tile spot
            x += tWidth;
            if (x >= sWidth)
            {
                x = 0;
                y += tHeight;
            }
        }

        //Sprite sheet
        if (tilesLoaded)
        {
            //black
            tClips[T2].x = 0;
            tClips[T2].y = 0;
            tClips[T2].w = tWidth;
            tClips[T2].h = tHeight;
            //white
            tClips[N].x = 80;
            tClips[N].y = 0;
            tClips[N].w = tWidth;
            tClips[N].h = tHeight;
            //grey
            tClips[T1].x = 160;
            tClips[T1].y = 0;
            tClips[T1].w = tWidth;
            tClips[T1].h = tHeight;
        }
    }

    map.close();
    return tilesLoaded;
}

bool load(Tile *tiles[]) //Loads media
{
    bool success = true;

    if (!tTexture.loadFromFile("Images/tiles.png")) //Load Tile Texture
    {
        printf("Failed to load tile set texture!\n");
        success = false;
    }

    if (!set(tiles)) //Load Tile Map
    {
        printf("Failed to load tile set!\n");
        success = false;
    }

    return success;
}

void close(Tile *tiles[]) //Frees media and shuts down SDL
{
    for (int i = 0; i < tNumber; ++i) //Deallocate tiles
    {
        if (tiles[i] != NULL)
        {
            delete tiles[i];
            tiles[i] = NULL;
        }
    }

    tTexture.free(); //Free loaded images

    SDL_DestroyRenderer(WRenderer); //Destroy Renderer
    WRenderer = NULL;

    SDL_DestroyWindow(Window); //Destroy Window
    Window = NULL;

    IMG_Quit(); //Quit SDL
    SDL_Quit();
}

int main(int argc, char *args[])
{
    if (!init()) //Start up SDL and create window
        printf("Failed to initialize!\n");
    else
    {
        Tile *tileSet[tNumber]; //The level tiles

        if (!load(tileSet)) //Load media
            printf("Failed to load media!\n");
        else
        {
            bool quit = false;
            SDL_Event e; //Event handler

            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0) //User requests quit
                    if (e.type == SDL_QUIT)
                        quit = true;

                //Screen
                SDL_SetRenderDrawColor(WRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(WRenderer);
                for (int i = 0; i < tNumber; ++i) //Render level
                    tileSet[i]->render();
                SDL_RenderPresent(WRenderer);
            }
        }

        close(tileSet);
    }

    return 0;
}

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

//TEXTURE CLASS DEFINITIONS

Texture::Texture() //Initializes
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

Texture::~Texture() //Deallocates
{
    free();
}

bool Texture::loadFromFile(std::string path) //Loads image at specified path
{
    free();
    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
        printf("SDL_image Error: %s\n", IMG_GetError());
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(WRenderer, loadedSurface);
        if (newTexture == NULL)
            printf("Texture Error: %s\n", SDL_GetError());
        else
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        SDL_FreeSurface(loadedSurface);
    }

    mTexture = newTexture;
    return mTexture != NULL;
}

void Texture::free() //Frees texture if it exists
{
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) //Modulates texture rgb
{
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

//Renders texture at given point
void Texture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(WRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}