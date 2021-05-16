#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

class Texture
{
public:
    Texture();
    ~Texture();

    bool loadFromFile(std::string path);
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
    void free();
    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    int getWidth();
    int getHeight();

private:
    SDL_Texture *mTexture;
    int mWidth;
    int mHeight;
};