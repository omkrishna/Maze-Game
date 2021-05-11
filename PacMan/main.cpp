#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <bits/stdc++.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>

#include "Headers/Texture.h"
#include "Headers/Tile.h"
#include "Headers/Dot.h"
#include "Headers/Ghost.h"

//GamePlay Flags
bool quit = false;
bool ghosted = false;

//Screen and Window
const int sWidth = 1080;
const int sHeight = 840;
SDL_Window *Window = NULL;      //Window
SDL_Renderer *WRenderer = NULL; //Window Renderer

//Tile constants
const int tWidth = 40;
const int tHeight = 40;
const int tNumber = (sWidth / tWidth) * (sHeight / tHeight);
const int tSprites = 2;

//The different tile sprites
SDL_Rect tClips[tSprites];
const int S = 1; //Pellet
const int T = 0; //Tile
//const int P = 2;  //Power Pellet
const int N = 3; //No Tile

//Dot Dimensions and Velocity
const int dWidth = 40;
const int dHeight = 40;
const int dVel = 4;

//Dot Sprite Clips
const int dSprites = 8; //Number of Clips
const int uDir = 0;
const int rDir = 2;
const int dDir = 4;
const int lDir = 6;

//Dot Animation
SDL_Rect dClips[dSprites];
int dNumber = 1; //No of Players
int frame = 0;   //Animation Frame

//Ghost Count and Dimensions
const int ghNumber = 2;
const int ghWidth = 40;
const int ghHeight = 40;

//The Ghosts
Ghost *Ghosts[ghNumber];        //0:Blinky 1:Pinky
int px[ghNumber], py[ghNumber]; //Position
int ghDir[ghNumber] = {0};      //Direction

//Ghost Directions
const int ghU = 0;
const int ghD = 1;
const int ghL = 2;
const int ghR = 3;

//Textures
Texture dTexture;
Texture tTexture;
Texture ghTexture0;
Texture ghTexture1;

//Mixer
Mix_Music *gMusic = NULL; //Music
Mix_Chunk *gHigh = NULL;  //Sounds
Mix_Chunk *gLow = NULL;

bool init() //Starts up SDL and creates window
{
    bool success = true;

    if (SDL_Init((SDL_INIT_VIDEO | SDL_INIT_AUDIO)) < 0) //Initialize SDL
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

                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) //Mixer Loading
                {
                    printf("SDL_mixer Error: %s\n", Mix_GetError());
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
            int r = rand() % 4;
			//solid
			tClips[T].x = 0 + 40 * r;
			tClips[T].y = 0;
			tClips[T].w = tWidth;
			tClips[T].h = tHeight;

			//border
			tClips[N].x = 0 + 40 * r;
			tClips[N].y = 40;
			tClips[N].w = tWidth;
			tClips[N].h = tHeight;

			//dot
			tClips[S].x = 0 + 40 * r;
			tClips[S].y = 80;
			tClips[S].w = tWidth;
			tClips[S].h = tHeight;
        }
    }

    map.close();
    return tilesLoaded;
}

bool load(Tile *tiles[]) //Loads media
{
    bool success = true;

    if (!dTexture.loadFromFile("Images/dot.png")) //Load sprite sheet texture
    {
        printf("Failed to load PacMan Sprite Animation texture!\n");
        success = false;
    }
    else
    {
        //Set sprite clips
        for (int i = 0; i < dSprites; i++)
        {
            dClips[i].x = dWidth * i;
            dClips[i].y = 0;
            dClips[i].w = dWidth;
            dClips[i].h = dHeight;
        }
    }

    if (!tTexture.loadFromFile("Images/123.png")) //Load Tile Texture
    {
        printf("Failed to load tile set texture!\n");
        success = false;
    }

    if (!set(tiles)) //Load Tile Map
    {
        printf("Failed to load tile set!\n");
        success = false;
    }

    if (!ghTexture0.loadFromFile("Images/ghostw.png")) //Load Ghost Texture
    {
        printf("Failed to load ghost texture!\n");
        success = false;
    }

    if (!ghTexture1.loadFromFile("Images/ghost.png")) //Load Ghost Texture
    {
        printf("Failed to load ghost texture!\n");
        success = false;
    }

    gLow = Mix_LoadWAV("Sounds/low.wav"); //Load Music and Sound
    gHigh = Mix_LoadWAV("Sounds/high.wav");
    gMusic = Mix_LoadMUS("Sounds/beat.wav");

    if (gMusic == NULL || gHigh == NULL || gLow == NULL)
    {
        printf("SDL_mixer Error: %s\n", Mix_GetError());
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
    dTexture.free();
    ghTexture0.free();
    ghTexture1.free();

    //Free the sound effects
    Mix_FreeMusic(gMusic);
    Mix_FreeChunk(gHigh);
    Mix_FreeChunk(gLow);
    gMusic = NULL;
    gHigh = NULL;
    gLow = NULL;

    SDL_DestroyRenderer(WRenderer); //Destroy Renderer
    WRenderer = NULL;

    SDL_DestroyWindow(Window); //Destroy Window
    Window = NULL;

    IMG_Quit(); //Quit SDL
    Mix_Quit();
    SDL_Quit();
}

int dist(SDL_Rect a, SDL_Rect b) //Distance Between TopLeft-most Points of 2 Rects
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

bool collision(SDL_Rect a, SDL_Rect b) //Check if 2 Rects touch
{
    //Sides of Rectangles A and B
    int leftA, leftB, rightA, rightB, topA, topB, bottomA, bottomB;

    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if (bottomA <= topB)
        return false;
    if (topA >= bottomB)
        return false;
    if (rightA <= leftB)
        return false;
    if (leftA >= rightB)
        return false;
    return true;
}

bool wall(SDL_Rect box, Tile *tiles[]) //Check if a WallTile is touched
{
    for (int i = 0; i < tNumber; ++i)
        if (tiles[i]->getType() == T)
            if (collision(box, tiles[i]->getBox()))
                return true;
    return false;
}

int main(int argc, char *args[])
{
    srand(time(0));
    int exitAnim = 0;
    if (!init())
        printf("Failed to initialize!\n");
    else
    {
        Tile *tileSet[tNumber];
        if (!load(tileSet))
            printf("Failed to load media!\n");
        else
        {
            SDL_Event e;
            Dot dot[dNumber];

            Mix_PlayMusic(gMusic, -1);

            while (!quit)
            {
                if (ghosted == false)
                {
                    while (SDL_PollEvent(&e) != 0)
                    {
                        if (e.type == SDL_QUIT)
                            quit = true;

                        for (int i = 0; i < dNumber; i++)
                            dot[i].handleEvent(e, i, tileSet);
                    }

                    for (int i = 0; i < dNumber; i++)
                        dot[i].move(tileSet);

                    //Clear screen
                    SDL_SetRenderDrawColor(WRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderClear(WRenderer);

                    //Render current frame
                    for (int i = 0; i < tNumber; ++i)
                        tileSet[i]->render();
                    for (int i = 0; i < dNumber; i++)
                        dot[i].render(frame, dot[i].dir, i, tileSet);

                    //Update screen
                    SDL_RenderPresent(WRenderer);

                    //Go to next frame
                    ++frame;
                    if (frame / dSprites >= dSprites)
                        frame = 0;
                }

                else
                {
                    //Clear screen
                    SDL_SetRenderDrawColor(WRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderClear(WRenderer);

                    //Render current frame
                    for (int i = 0; i < tNumber; ++i)
                        tileSet[i]->render();
                    for (int i = 0; i < dNumber; i++)
                        dot[i].render(frame, exitAnim % 8, i, tileSet);

                    //Update screen
                    SDL_RenderPresent(WRenderer);

                    //Exit Animation
                    ++frame;
                    exitAnim++;
                    if (exitAnim > 100)
                        quit = true;
                }
            }

            std::cout << "\nGAME OVER\nStats:"
					  << "\nPlayer1 (Arrow Keys): \t" << dot[0].score - 1 //First Tile
					  << /* "\nPlayer2 (WSAD Keys): \t" << dot[1].score << */ "\n\n";
        }

        close(tileSet);
    }

    return 0;
}

//TEXTURE DEFINITIONS

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

//TILE DEFINITIONS

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

void Tile::changeType(int tileType) //Change tileType
{
    mType = tileType;
}

SDL_Rect Tile::getBox() //Get Collision Box
{
    return mBox;
}

//GHOST DEFINITIONS

Ghost::Ghost(int x, int y, int i) //Initializes Ghost
{
    mBox.x = x; //Collision box offset
    mBox.y = y;

    mBox.w = ghWidth; //Collision box dimensions
    mBox.h = ghHeight;

    mFrame = 1; //Initialize animation

    //Set type
    if (i == 0) //Blinky
        mTexture = &ghTexture0;
    else //Pinky
        mTexture = &ghTexture1;
}

void Ghost::render() //Shows Ghost
{
    mTexture->render(mBox.x, mBox.y); //Show image
    mFrame++;                         //Animate
}

bool Ghost::isDead()
{
    return mFrame > 20;
}

//DOT DEFINITIONS

Dot::Dot() //Initializes Variables
{
    mBox.x = tWidth; //Collision box offset
    mBox.y = tHeight;

    mBox.w = dWidth; //Collision box dimensions
    mBox.h = dHeight;

    mVelX = 0; //Dot velocity
    mVelY = 0;
    
    for (int i = 0; i < ghNumber; ++i) //Initialize Ghosts
        Ghosts[i] = new Ghost(sWidth - ghWidth - tWidth, sHeight - ghHeight - tHeight, i);
}

void Dot::move(Tile *tiles[]) //Move and Check sprite collision
{
    mBox.x += mVelX;       //Move left or right
    if (wall(mBox, tiles)) //Collision with wall
        mBox.x -= mVelX;

    if (mBox.x + dWidth > sWidth) //CrossOver
        mBox.x = 0;
    if (mBox.x < 0)
        mBox.x = sWidth - dWidth;

    mBox.y += mVelY; //Move up or down
    if (wall(mBox, tiles)) //Collision with wall
        mBox.y -= mVelY;

    if (mBox.y + dHeight > sHeight) //CrossOver
        mBox.y = 0;
    if (mBox.y < 0)
        mBox.y = sHeight - dHeight;

    if (!wall(mBox, tiles)) //Illusion of Eaten Pellets
		for (int i = 0; i < tNumber; ++i)
			if (tiles[i]->getType() == S /* || tiles[i]->getType() == P */)
				if (collision(mBox, tiles[i]->getBox()))
				{
					tiles[i]->changeType(N);
					score++;
				}
}

void Dot::render(int frame, int dir, int i, Tile *tiles[]) //Show Dot
{
    int bin = (frame % dSprites == 0) ? 0 : 1;
    SDL_Rect *currentClip;

    if (ghosted == false) //Show Ghosts
        renderGhosts(tiles);

    if (animFlag) //Animate Dot
    {
        currentClip = &dClips[dir + bin];

        if (i) //Sounds on Key Press
            Mix_PlayChannel(-1, gHigh, 0);
        else
            Mix_PlayChannel(-1, gLow, 0);
    }
    else
        currentClip = &dClips[dir];

    dTexture.render(mBox.x, mBox.y, currentClip); //Show Dot
}

void Dot::handleEvent(SDL_Event &e, int n, Tile *tiles[]) //Takes Key Presses
{
    switch (n)
    {
    case 0: //Player 1: UP DOWN LEFT RIGHT
    {
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_UP:
                dir = uDir;
                mVelY -= dVel;\
                animFlag = true;
                break;
            case SDLK_DOWN:
                dir = dDir;
                mVelY += dVel;
                animFlag = true;
                break;
            case SDLK_LEFT:
                dir = lDir;
                mVelX -= dVel;
                animFlag = true;
                break;
            case SDLK_RIGHT:
                dir = rDir;
                mVelX += dVel;
                animFlag = true;
                break;
            }
        }

        else if (e.type == SDL_KEYUP && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_UP:
                mVelY += dVel;
                animFlag = false;
                break;
            case SDLK_DOWN:
                mVelY -= dVel;
                animFlag = false;
                break;
            case SDLK_LEFT:
                mVelX += dVel;
                animFlag = false;
                break;
            case SDLK_RIGHT:
                mVelX -= dVel;
                animFlag = false;
                break;
            }
        }
    }
    break;

        /* case 1: //Player 2: W S A D
	{
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_w:
				dir = uDir;
				mVelY -= dVel;
				animFlag = true;
				break;
			case SDLK_s:
				dir = dDir;
				mVelY += dVel;
				animFlag = true;
				break;
			case SDLK_a:
				dir = lDir;
				mVelX -= dVel;
				animFlag = true;
				break;
			case SDLK_d:
				dir = rDir;
				mVelX += dVel;
				animFlag = true;
				break;
			}
		}

		else if (e.type == SDL_KEYUP && e.key.repeat == 0)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_w:
				mVelY += dVel;
				animFlag = false;
				break;
			case SDLK_s:
				mVelY -= dVel;
				animFlag = false;
				break;
			case SDLK_a:
				mVelX += dVel;
				animFlag = false;
				break;
			case SDLK_d:
				mVelX -= dVel;
				animFlag = false;
				break;
			}
		}
	}
	break; */

    default:
        break;
    }
}

void Dot::renderGhosts(Tile *tiles[])
{
    SDL_Rect Blinky = {mBox.x, mBox.y, mBox.w, mBox.h};
    SDL_Rect Pinky = {mBox.x + 2 * dWidth, mBox.y + 2 * dHeight, mBox.w, mBox.h}; //TODO: Improve Logic

    setTarget(Blinky, tiles, 0);
    setTarget(Pinky, tiles, 1);

    for (int i = 0; i < ghNumber; ++i)
        
        if (Ghosts[i]->isDead()) //Delete and Replace Dead Ghosts
        {
            delete Ghosts[i];
            Ghosts[i] = new Ghost(px[i], py[i], i);
        }

    for (int i = 0; i < ghNumber; ++i) //Show Ghosts
        Ghosts[i]->render();
}

void Dot::setTarget(SDL_Rect ghost, Tile *tiles[], int i)
{
    int gx = Ghosts[i]->mBox.x, gy = Ghosts[i]->mBox.y;

    SDL_Rect uBox, dBox, lBox, rBox;
    uBox = {gx, gy - ghWidth, ghWidth, ghHeight};
    dBox = {gx, gy + ghWidth, ghWidth, ghHeight};
    lBox = {gx - ghHeight, gy, ghWidth, ghHeight};
    rBox = {gx + ghHeight, gy, ghWidth, ghHeight};

    if (collision(Ghosts[i]->mBox, mBox))
        ghosted = true;

    int udist, ddist, ldist, rdist;
    udist = ddist = ldist = rdist = INT_MAX;
    if (!wall(uBox, tiles))
        if (ghDir[i] != ghD)
            udist = dist(uBox, ghost);
    if (!wall(dBox, tiles))
        if (ghDir[i] != ghU)
            ddist = dist(dBox, ghost);
    if (!wall(lBox, tiles))
        if (ghDir[i] != ghR)
            ldist = dist(lBox, ghost);
    if (!wall(rBox, tiles))
        if (ghDir[i] != ghL)
            rdist = dist(rBox, ghost);

    int mindist = std::min(std::min(udist, ddist), std::min(rdist, ldist));

    if (mindist == udist)
    {
        px[i] = uBox.x;
        py[i] = uBox.y;
        if (Ghosts[i]->isDead())
            ghDir[i] = ghU;
    }
    else if (mindist == ddist)
    {
        px[i] = dBox.x;
        py[i] = dBox.y;
        if (Ghosts[i]->isDead())
            ghDir[i] = ghD;
    }
    else if (mindist == ldist)
    {
        px[i] = lBox.x;
        py[i] = lBox.y;
        if (Ghosts[i]->isDead())
            ghDir[i] = ghL;
    }
    else
    {
        px[i] = rBox.x;
        py[i] = rBox.y;
        if (Ghosts[i]->isDead())
            ghDir[i] = ghR;
    }
}