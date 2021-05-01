/* NON USABLE */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "Headers/Texture.h"

SDL_Window *Window = NULL; //Window
SDL_Renderer *WRenderer = NULL; //window Renderer

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