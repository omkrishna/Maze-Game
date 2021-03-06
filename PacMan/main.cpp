#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <bits/stdc++.h>
#include <ctime>
#include <cmath>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>

#include "Headers/Texture.h"
#include "Headers/Tile.h"
#include "Headers/Dot.h"
#include "Headers/Ghost.h"

//Button constants
const int bWidth = 230;
const int bHeight = 60;
const int nButton = 5;

enum bTypes
{
	bMouse = 0,
	bMouseClick = 1,
	bSprites = 2
};
//Mouse button sprites
SDL_Rect bClips[bSprites];

//The mouse button
class Button
{
public:
	Button();

	void setPosition(int x, int y);
	void handleEvent(SDL_Event *e, int i);
	void render(int i, int x, int y);
	bool rules = false;

private:
	SDL_Point mPosition;   //Top left position
	bTypes mCurrentSprite; //Currently used global sprite
};

//Buttons objects
Button button1; //Single Player
Button button2; //Multi Player
Button beg;
Button med;
Button adv;

//GamePlay Flags
bool Won = false;
bool quit = false;
bool play = false;
bool replay = false;
bool Ghosted = false;

int level = 2; //Intermediate By Default

//Screen and Window
const int sWidth = 1080;
const int sHeight = 840;
SDL_Window *Window = NULL;		//Window
SDL_Renderer *WRenderer = NULL; //Window Renderer

//Tile constants
const int tWidth = 40;
const int tHeight = 40;
const int tNumber = (sWidth / tWidth) * (sHeight / tHeight);
const int tSprites = 6;

//The different tile sprites
SDL_Rect tClips[tSprites];
const int S = 1; //Pellet
const int T = 0; //Tile
const int P = 2; //Power Pellet: por
const int A = 3; //Audit Pellet: Audit
const int N = 4; //No Tile
const int I = 5; //IIT Delhi

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
SDL_Rect d1Clips[dSprites];
SDL_Rect d2Clips[dSprites];
int dNumber = 2; //No of Players
int frame = 0;	 //Animation Frame

//Ghost Count and Dimensions
const int ghNumber = 3;
const int ghSprites = 2;
const int ghWidth = 40;
const int ghHeight = 40;

//The Ghosts
Ghost *Ghosts[ghNumber];
int px[ghNumber], py[ghNumber]; //Position
int ghDir[ghNumber] = {0};		//Direction

//Ghost Animation
SDL_Rect g0Clips[ghSprites];
SDL_Rect g1Clips[ghSprites];
SDL_Rect g2Clips[ghSprites];

//Ghost Directions
const int ghU = 0;
const int ghD = 1;
const int ghL = 2;
const int ghR = 3;

//Ghost Corners
SDL_Rect Corner[3] = {{sWidth - ghWidth - tWidth, ghHeight - tHeight, tWidth, tHeight},
					  {sWidth - ghWidth - tWidth, sHeight - ghHeight - tHeight, tWidth, tHeight},
					  {ghWidth - tWidth, sHeight - ghHeight - tHeight, tWidth, tHeight}};

//Globally used font
TTF_Font *gFont = NULL;

//Textures
Texture sTexture1;
Texture sTexture2;
Texture dTexture;
Texture tTexture;
Texture bTexture1;
Texture bTexture2;
Texture begTexture;
Texture medTexture;
Texture advTexture;
Texture ghTexture0; //Ghosts[0] = COP
Texture ghTexture1; //Ghosts[1] = HUL
Texture ghTexture2; //Ghosts[2] = ELL
Texture gTextTexture;

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
		Window = SDL_CreateWindow("IITD x Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sWidth, sHeight, SDL_WINDOW_SHOWN);
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
				int imgFlags = IMG_INIT_PNG;							   //PNG loading
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

				if (TTF_Init() == -1) //Initialize SDL_ttf
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
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
			int r = 0; //rand() % 4;
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

			//power
			tClips[P].x = 160;
			tClips[P].y = 120;
			tClips[P].w = tWidth;
			tClips[P].h = tHeight;

			//audit
			tClips[A].x = 0;
			tClips[A].y = 120;
			tClips[A].w = tWidth;
			tClips[A].h = tHeight;

			//iit delhi
			tClips[I].x = 160;
			tClips[I].y = 80;
			tClips[I].w = tWidth;
			tClips[I].h = tHeight;
		}
	}

	map.close();
	return tilesLoaded;
}

bool load(Tile *tiles[]) //Loads media
{
	bool success = true;

	if (!sTexture1.loadFromFile("Images/screen1.png")) //Screen
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}

	if (!sTexture2.loadFromFile("Images/screen2.png")) //Screen
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}

	if (!bTexture1.loadFromFile("Images/1player(1).png")) //Single Player
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
		for (int i = 0; i < bSprites; ++i)
		{
			bClips[i].x = 0;
			bClips[i].y = 0;
			bClips[i].w = bWidth;
			bClips[i].h = bHeight;
		}

	if (!bTexture2.loadFromFile("Images/2player(2).png")) //Multi Player
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
		for (int i = 0; i < bSprites; ++i)
		{
			bClips[i].x = 0;
			bClips[i].y = 0;
			bClips[i].w = bWidth;
			bClips[i].h = bHeight;
		}

	if (!begTexture.loadFromFile("Images/beg.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
		for (int i = 0; i < bSprites; ++i)
		{
			bClips[i].x = 0;
			bClips[i].y = 0 + bHeight * i;
			bClips[i].w = bWidth;
			bClips[i].h = bHeight;
		}

	if (!medTexture.loadFromFile("Images/int.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
		for (int i = 0; i < bSprites; ++i)
		{
			bClips[i].x = 0;
			bClips[i].y = 0 + bHeight * i;
			bClips[i].w = bWidth;
			bClips[i].h = bHeight;
		}

	if (!advTexture.loadFromFile("Images/adv.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
		for (int i = 0; i < bSprites; ++i)
		{
			bClips[i].x = 0;
			bClips[i].y = 0 + bHeight * i;
			bClips[i].w = bWidth;
			bClips[i].h = bHeight;
		}

	if (!dTexture.loadFromFile("Images/players.png")) //Load sprite sheet texture
	{
		printf("Failed to load PacMan Sprite Animation texture!\n");
		success = false;
	}
	else
	{
		//Randomize Character : PacMan or PacWoman
		int r = rand() % 2;
		if (r == 0)
		{
			for (int i = 0; i < dSprites; i++)
			{
				d1Clips[i].x = dWidth * i;
				d1Clips[i].y = 0;
				d1Clips[i].w = dWidth;
				d1Clips[i].h = dHeight;
			}

			if (dNumber == 2)
				for (int i = 0; i < dSprites; i++)
				{
					d2Clips[i].x = dWidth * i;
					d2Clips[i].y = dHeight;
					d2Clips[i].w = dWidth;
					d2Clips[i].h = dHeight;
				}
		}
		else
		{
			for (int i = 0; i < dSprites; i++)
			{
				d1Clips[i].x = dWidth * i;
				d1Clips[i].y = dHeight;
				d1Clips[i].w = dWidth;
				d1Clips[i].h = dHeight;
			}

			if (dNumber == 2)
				for (int i = 0; i < dSprites; i++)
				{
					d2Clips[i].x = dWidth * i;
					d2Clips[i].y = 0;
					d2Clips[i].w = dWidth;
					d2Clips[i].h = dHeight;
				}
		}
	}

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

	if (!ghTexture0.loadFromFile("Images/cop.png")) //Load ghost texture
	{
		printf("Failed to load cop texture!\n");
		success = false;
	}
	else
	{
		//Set sprite clips
		for (int i = 0; i < 2; i++)
		{
			g0Clips[i].x = dWidth * i;
			g0Clips[i].y = 0;
			g0Clips[i].w = dWidth;
			g0Clips[i].h = dHeight;
		}
	}

	if (!ghTexture1.loadFromFile("Images/hul.png")) //Load ghost texture
	{
		printf("Failed to load col texture!\n");
		success = false;
	}
	else
	{
		//Set sprite clips
		for (int i = 0; i < 2; i++)
		{
			g1Clips[i].x = dWidth * i;
			g1Clips[i].y = 0;
			g1Clips[i].w = dWidth;
			g1Clips[i].h = dHeight;
		}
	}

	if (!ghTexture2.loadFromFile("Images/ell.png")) //Load ghost texture
	{
		printf("Failed to load ell texture!\n");
		success = false;
	}
	else
	{
		//Set sprite clips
		for (int i = 0; i < 2; i++)
		{
			g2Clips[i].x = dWidth * i;
			g2Clips[i].y = 0;
			g2Clips[i].w = dWidth;
			g2Clips[i].h = dHeight;
		}
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
		if (tiles[i] != NULL)
		{
			delete tiles[i];
			tiles[i] = NULL;
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
		if (tiles[i]->getType() == T || tiles[i]->getType() == I)
			if (collision(box, tiles[i]->getBox()))
				return true;
	return false;
}

bool loadMedia(std::string s, int n)
{
	bool success = true;
	gFont = TTF_OpenFont("Fonts/lazy.ttf", n);

	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		SDL_Color textColor = {255, 255, 255};
		if (!gTextTexture.loadFromRenderedText(s, textColor))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}

	return success;
}

void renderGhostsMultiplayer(Dot &dot1, Dot &dot2, Tile *tiles[], int frame)
{
	int bin = (frame % (2 * dSprites) > dSprites) ? 0 : 1;

	for (int i = 0; i < ghNumber; i++)
	{
		if (collision(Ghosts[i]->mBox, dot1.mBox))
		{
			if (!dot1.ghosted)
				dot1.ghostedby = i;
			dot1.ghosted = true;
		}
		if (collision(Ghosts[i]->mBox, dot2.mBox))
		{
			if (!dot2.ghosted)
				dot2.ghostedby = i;
			dot2.ghosted = true;
		}
	}

	if (Ghosted)
	{
		SDL_Rect Random = {rand() % sWidth, rand() % sHeight, dot1.mBox.w, dot1.mBox.h};
		for (int i = 0; i < ghNumber; i++)
			dot1.setTarget(Random, tiles, i);
	}
	else
	{
		//dot1 - ghost
		if (dot1.audit)
		{
			SDL_Rect Clyde_t;
			if (dist(Ghosts[0]->mBox, dot1.mBox) > 8 * tHeight)
				Clyde_t = {dot1.mBox.x, dot1.mBox.y, dot1.mBox.w, dot1.mBox.h};
			else
				Clyde_t = Corner[0];
			dot1.setTarget(Clyde_t, tiles, 0);
		}
		else if (dot1.por)
		{
			SDL_Rect Clyde_t = Corner[0];
			dot1.setTarget(Clyde_t, tiles, 0);
		}
		else
		{
			if (!dot1.ghosted)
			{
				SDL_Rect Blinky_t = {dot1.mBox.x, dot1.mBox.y, dot1.mBox.w, dot1.mBox.h};
				dot1.setTarget(Blinky_t, tiles, 0);
			}
			else
			{
				SDL_Rect Blinky_t = {dot2.mBox.x, dot2.mBox.y, dot2.mBox.w, dot2.mBox.h};
				dot2.setTarget(Blinky_t, tiles, 0);
			}
		}

		//dot2 - ghost 1 & 2
		if (dot2.audit)
		{
			SDL_Rect Clyde_t;
			if (dist(Ghosts[1]->mBox, dot2.mBox) > 8 * tHeight)
				Clyde_t = {dot2.mBox.x, dot2.mBox.y, dot2.mBox.w, dot2.mBox.h};
			else
				Clyde_t = Corner[1];
			dot2.setTarget(Clyde_t, tiles, 1);
		}
		else if (dot2.por)
		{
			SDL_Rect Clyde_t = Corner[1];
			dot2.setTarget(Clyde_t, tiles, 1);
		}
		else
		{
			if (!dot2.ghosted)
			{
				SDL_Rect Blinky_t = {dot2.mBox.x, dot2.mBox.y, dot2.mBox.w, dot2.mBox.h};
				dot2.setTarget(Blinky_t, tiles, 1);
				//ghost 0 behaves like pinky
			}
			else
			{
				SDL_Rect Blinky_t = {dot1.mBox.x, dot1.mBox.y, dot1.mBox.w, dot1.mBox.h};
				dot1.setTarget(Blinky_t, tiles, 1);
				//ghost 0 behaves like pinky
			}
		}

		SDL_Rect Inky_t = {2 * Ghosts[1]->mBox.x - Ghosts[0]->mBox.x, 2 * Ghosts[0]->mBox.y - Ghosts[0]->mBox.y, dot1.mBox.w, dot1.mBox.h}; //Inky: Ghosts[2]
		dot1.setTarget(Inky_t, tiles, 2);
	}

	for (int i = 0; i < ghNumber; ++i)

		if (Ghosts[i]->isDead()) //Delete and Replace Dead Ghosts
		{
			delete Ghosts[i];
			Ghosts[i] = new Ghost(px[i], py[i], i);
		}

	for (int i = 0; i < ghNumber; ++i) //Show Ghosts
		Ghosts[i]->render(bin);
}

int main(int argc, char *args[])
{
	std::stringstream s;
	std::srand(time(0));
	int exitAnim = 0;
	int indivAnim = 0;

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
			for (int i = 0; i < dNumber; i++)
				dot[i] = Dot(i);

			Mix_PlayMusic(gMusic, -1);

			while (!quit)
			{
				for (int i = 0; i < dNumber; i++)
				{
					++dot[i].time_req;
					if (dot[i].time_req > 300)
					{
						dot[i].audit = false;
						dot[i].por = false;
					}
				}

				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
						quit = true;

					for (int i = 0; i < dNumber; i++)
						dot[i].handleEvent(e, i, tileSet);

					//Handle button events
					button1.handleEvent(&e, 1);
					button2.handleEvent(&e, 2);
					beg.handleEvent(&e, 3);
					med.handleEvent(&e, 4);
					adv.handleEvent(&e, 5);
				}

				for (int i = 0; i < dNumber; i++)
					dot[i].move(tileSet);

				//Clear screen
				SDL_SetRenderDrawColor(WRenderer, 0, 0, 0, 0);
				SDL_RenderClear(WRenderer);

				if (play == false)
				{
					int h = 90;
					if (frame % 75)
						sTexture1.render(0, 0);
					else
						sTexture2.render(0, 0);

					{
						s << "IITD x Pacman";
						if (!loadMedia(s.str(), 80))
							printf("Failed to load media!\n");
						else
							gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h);
						s.str("");

						s << "Welcome to IITD (Pacman-ish Simulation)";
						if (!loadMedia(s.str(), 35))
							printf("Failed to load media!\n");
						else
							gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 80);
						s.str("");

						s << "Scoring:";
						if (!loadMedia(s.str(), 25))
							printf("Failed to load media!\n");
						else
							gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 260);
						s.str("");

						s << "-Collect Regular Pellets to increase score by 1";
						if (!loadMedia(s.str(), 25))
							printf("Failed to load media!\n");
						else
							gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 285);
						s.str("");

						s << "-Collect PORs to increase score by 10";
						if (!loadMedia(s.str(), 25))
							printf("Failed to load media!\n");
						else
							gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 310);
						s.str("");

						s << "-Audit courses to increase score by 20 and decrease chances of failing";
						if (!loadMedia(s.str(), 25))
							printf("Failed to load media!\n");
						else
							gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 335);
						s.str("");

						s << "Rules;";
						if (!loadMedia(s.str(), 25))
							printf("Failed to load media!\n");
						else
							gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 410);
						s.str("");
					}

					//Render buttons
					beg.render(3, (sWidth - bWidth) / 2 - 1.5 * bWidth, h + 160);
					med.render(4, (sWidth - bWidth) / 2, h + 160);
					adv.render(5, (sWidth - bWidth) / 2 + 1.5 * bWidth, h + 160);

					button1.render(1, (sWidth - bWidth) / 2 - bWidth, 650);
					button2.render(2, (sWidth - bWidth) / 2 + bWidth, 650);

					if (button1.rules)
					{
						s << "-Use Arrow Keys to move around the Campus Maze";
						if (!loadMedia(s.str(), 25))
							printf("Failed to load media!\n");
						else
							gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 435);
						s.str("");

						s << "-Avoid Course Ghosts or Fail";
						if (!loadMedia(s.str(), 25))
							printf("Failed to load media!\n");
						else
							gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 460);
						s.str("");
					}

					if (button2.rules)
					{
						s << "-Player1: Use Arrow Keys to move around the Campus Maze";
						if (!loadMedia(s.str(), 25))
							printf("Failed to load media!\n");
						else
							gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 435);
						s.str("");

						s << "-Player2: Use WSAD Keys to move around the Campus Maze";
						if (!loadMedia(s.str(), 25))
							printf("Failed to load media!\n");
						else
							gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 460);
						s.str("");

						s << "-Avoid Course Ghosts or Fail";
						if (!loadMedia(s.str(), 25))
							printf("Failed to load media!\n");
						else
							gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 485);
						s.str("");
					}
				}

				else
				{
					//Render current frame
					for (int i = 0; i < tNumber; ++i)
						tileSet[i]->render();

					for (int i = 0; i < dNumber; i++)
						dot[i].render(frame, dot[i].dir, i, tileSet);

					if (dNumber == 2)
						renderGhostsMultiplayer(dot[0], dot[1], tileSet, frame);

					s << "IITD x Pacman";
					if (!loadMedia(s.str(), 50))
						printf("Failed to load media!\n");
					else
						gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, 0);
					s.str("");

					if (dNumber == 1)
					{
						if (dot[0].ghosted)
							Ghosted = true;
					}
					else if (dNumber == 2)
					{
						if (dot[0].ghosted && dot[1].ghosted)
							Ghosted = true;
					}

					if (!Ghosted)
					{
						for (int k = 0; k < dNumber; k++)
						{
							s << "Score " << dot[k].score;
							if (!loadMedia(s.str(), 40))
								printf("Failed to load media!\n");
							else
								gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2 + pow(-1, k) * (dNumber - 1) * gTextTexture.getWidth(), sHeight - gTextTexture.getHeight() - 5);
							s.str("");

							if (dot[k].audit)
							{
								s << "Audit";
								if (!loadMedia(s.str(), 40))
									printf("Failed to load media!\n");
								else
									gTextTexture.render(5, sHeight - gTextTexture.getHeight() - 5);
								s.str("");
							}

							if (dot[k].por)
							{
								s << "PoR";
								if (!loadMedia(s.str(), 40))
									printf("Failed to load media!\n");
								else
									gTextTexture.render(sWidth - gTextTexture.getWidth() - 5, sHeight - gTextTexture.getHeight() - 5);
								s.str("");
							}
						}

						if (dNumber == 2)
						{
							indivAnim++;
							if (dot[0].ghosted)
								dot[0].render(frame, indivAnim % 8, 0, tileSet);
							if (dot[1].ghosted)
								dot[1].render(frame, indivAnim % 8, 1, tileSet);
						}
					}

					if (Ghosted || Won)
					{
						exitAnim++;

						for (int i = 0; i < tNumber; ++i)
							tileSet[i]->render();
						for (int i = 0; i < dNumber; i++)
							dot[i].render(frame, exitAnim % 8, i, tileSet);

						if (dNumber == 2)
							renderGhostsMultiplayer(dot[0], dot[1], tileSet, frame);

						if (exitAnim > 150)
						{
							//Clear screen
							SDL_SetRenderDrawColor(WRenderer, 0, 0, 0, 0);
							SDL_RenderClear(WRenderer);

							if (frame % 75)
								sTexture1.render(0, 0);
							else
								sTexture2.render(0, 0);

							int h = 120;
							s << "IITD x Pacman";
							if (!loadMedia(s.str(), 80))
								printf("Failed to load media!\n");
							else
								gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, 90);
							s.str("");

							if (!Won)
							{
								if (dNumber == 1)
								{
									s << "You Failed ";
									if (dot[0].ghostedby == 0)
										s << "COP!";
									else if (dot[0].ghostedby == 1)
										s << "HUL!";
									else if (dot[0].ghostedby == 2)
										s << "ELL!";

									if (!loadMedia(s.str(), 50))
										printf("Failed to load media!\n");
									else
										gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 80);
									s.str("");

									s << "Score: " << dot[0].score;
									if (!loadMedia(s.str(), 50))
										printf("Failed to load media!\n");
									else
										//Render current frame
										gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 160);
									s.str("");
								}
								if (dNumber == 2)
								{
									s << "Player 1 (Arrows) Failed ";
									if (dot[0].ghostedby == 0)
										s << "COP!";
									else if (dot[0].ghostedby == 1)
										s << "HUL!";
									else if (dot[0].ghostedby == 2)
										s << "ELL!";

									if (!loadMedia(s.str(), 50))
										printf("Failed to load media!\n");
									else
										gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 80);
									s.str("");

									s << "Score (Arrow): " << dot[0].score;
									if (!loadMedia(s.str(), 50))
										printf("Failed to load media!\n");
									else
										//Render current frame
										gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 160);
									s.str("");

									s << "Player 2 (WSAD) Failed ";
									if (dot[1].ghostedby == 0)
										s << "COP!";
									else if (dot[1].ghostedby == 1)
										s << "HUL!";
									else if (dot[1].ghostedby == 2)
										s << "ELL!";

									if (!loadMedia(s.str(), 50))
										printf("Failed to load media!\n");
									else
										gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 280);
									s.str("");

									s << "Score (WSAD): " << dot[1].score;
									if (!loadMedia(s.str(), 50))
										printf("Failed to load media!\n");
									else
										//Render current frame
										gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 360);
								}
							}
							else
							{
								if (dNumber == 1)
								{
									s << "YOU WON!!";
									if (!loadMedia(s.str(), 50))
										printf("Failed to load media!\n");
									else
										gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 80);
									s.str("");

									s << "Score: " << dot[0].score;
									if (!loadMedia(s.str(), 50))
										printf("Failed to load media!\n");
									else
										//Render current frame
										gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 160);
									s.str("");
								}
								if (dNumber == 2)
								{
									if (dot[0].ghosted)
									{
										s << "Player 1 (Arrows) Failed ";
										if (dot[0].ghostedby == 0)
											s << "COP!";
										else if (dot[0].ghostedby == 1)
											s << "HUL!";
										else if (dot[0].ghostedby == 2)
											s << "ELL!";
									}
									else
										s << "Player 1 (Arrows) WON!! ";

									if (!loadMedia(s.str(), 50))
										printf("Failed to load media!\n");
									else
										gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 80);
									s.str("");

									s << "Score (Arrow): " << dot[0].score;
									if (!loadMedia(s.str(), 50))
										printf("Failed to load media!\n");
									else
										//Render current frame
										gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 160);
									s.str("");

									if (dot[1].ghosted)
									{
										s << "Player 2 (WSAD) Failed ";
										if (dot[0].ghostedby == 0)
											s << "COP!";
										else if (dot[0].ghostedby == 1)
											s << "HUL!";
										else if (dot[0].ghostedby == 2)
											s << "ELL!";
									}
									else
										s << "Player 2 (WSAD) WON!! ";

									if (!loadMedia(s.str(), 50))
										printf("Failed to load media!\n");
									else
										gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 280);
									s.str("");

									s << "Score (WSAD): " << dot[1].score;
									if (!loadMedia(s.str(), 50))
										printf("Failed to load media!\n");
									else
										//Render current frame
										gTextTexture.render((sWidth - gTextTexture.getWidth()) / 2, h + 360);
								}
							}
						}
					}
				}

				//Update screen
				SDL_RenderPresent(WRenderer);

				//Go to next frame
				++frame;
				if (frame / dSprites >= dSprites)
					frame = 0;
			}
		}

		close(tileSet);
	}

	return 0;
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
	if (i == 0)
		mTexture = &ghTexture0;
	else if (i == 1)
		mTexture = &ghTexture1;
	else if (i == 2)
		mTexture = &ghTexture2;
}

void Ghost::render(int bin) //Shows Ghost
{
	SDL_Rect *currentClip; //Show image
	currentClip = &g0Clips[bin];
	mTexture->render(mBox.x, mBox.y, currentClip);

	mFrame++; //Animate
}

bool Ghost::isDead()
{
	if (!Ghosted)
		return mFrame > 10 * level;
	else
		return mFrame > 3;
}

//DOT DEFINITIONS

Dot::Dot()
{
}

Dot::Dot(int i) //Initializes Variables
{
	if (i == 0) //Collision box offset and Sprite direction
	{
		mBox.x = tWidth;
		mBox.y = tHeight;
		dir = 2;
	}
	else
	{
		if (dNumber == 2)
		{
			mBox.x = sWidth - ghWidth - tWidth;
			mBox.y = tHeight;
			dir = 6;
		}
	}

	mBox.w = dWidth; //Collision box dimensions
	mBox.h = dHeight;

	mVelX = 0; //Dot velocity
	mVelY = 0;

	for (int i = 0; i < ghNumber; ++i) //Initialize Ghosts
	{
		if (i == 0)
			Ghosts[i] = new Ghost(sWidth - ghWidth - tWidth, sHeight - ghHeight - tHeight, i);
		else if (i == 1)
			Ghosts[i] = new Ghost(sWidth - ghWidth - tWidth * 9, sHeight - ghHeight - tHeight, i);
		else
			Ghosts[i] = new Ghost(sWidth - ghWidth - tWidth * 18, sHeight - ghHeight - tHeight, i);
	}
}

void Dot::move(Tile *tiles[]) //Move and Check sprite collision
{
	mBox.x += mVelX;	   //Move left or right
	if (wall(mBox, tiles)) //Collision with wall
		mBox.x -= mVelX;

	if (mBox.x + dWidth > sWidth) //CrossOver
		mBox.x = 0;
	if (mBox.x < 0)
		mBox.x = sWidth - dWidth;

	mBox.y += mVelY;	   //Move up or down
	if (wall(mBox, tiles)) //Collision with wall
		mBox.y -= mVelY;

	if (mBox.y + dHeight > sHeight) //CrossOver
		mBox.y = 0;
	if (mBox.y < 0)
		mBox.y = sHeight - dHeight;

	if (!wall(mBox, tiles)) //Illusion of Eaten Pellets
	{
		bool TNI = true;
		for (int i = 0; i < tNumber; ++i)
		{
			if (tiles[i]->getType() == S)
			{
				TNI = false;
				if (collision(mBox, tiles[i]->getBox()))
				{
					tiles[i]->changeType(N);
					score++;
				}
			}

			if (tiles[i]->getType() == P)
			{
				TNI = false;
				if (collision(mBox, tiles[i]->getBox()))
				{
					por = true;
					time_req = 0; //Set Time to Zero
					tiles[i]->changeType(N);
					score += 20;
				}
			}

			if (tiles[i]->getType() == A)
			{
				TNI = false;
				if (collision(mBox, tiles[i]->getBox()))
				{
					audit = true;
					time_req = 0; //Set Time to Zero
					tiles[i]->changeType(N);
					score += 20;
				}
			}
		}

		Won = TNI;
	}
}

void Dot::render(int frame, int dir, int i, Tile *tiles[]) //Show Dot
{
	int bin = (frame % (2 * dSprites) > dSprites) ? 0 : 1;
	SDL_Rect *currentClip;

	if (dNumber == 1)
		renderGhosts(tiles, bin); //Show Ghosts

	if (animFlag) //Animate Dot
	{
		if (i)
			currentClip = &d1Clips[dir + bin];
		else
			currentClip = &d2Clips[dir + bin];

		if (!ghosted)
			if (i) //Sounds on Key Press
				Mix_PlayChannel(-1, gHigh, 0);
			else
				Mix_PlayChannel(-1, gLow, 0);
	}
	else
	{
		if (i)
			currentClip = &d1Clips[dir];
		else
			currentClip = &d2Clips[dir];
	}

	dTexture.render(mBox.x, mBox.y, currentClip); //Show Dot
}

void Dot::handleEvent(SDL_Event &e, int n, Tile *tiles[]) //Takes Key Presses
{
	if (play && !ghosted)
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
					mVelY -= dVel;
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

		case 1: //Player 2: W S A D
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
		break;

		default:
			break;
		}
	}

	else
	{
		mVelX = 0;
		mVelY = 0;
	};
}

void Dot::renderGhosts(Tile *tiles[], int bin)
{
	if (Ghosted)
	{
		SDL_Rect Random = {rand() % sWidth, rand() % sHeight, mBox.w, mBox.h};
		for (int i = 0; i < ghNumber; i++)
			setTarget(Random, tiles, i);
	}
	else
	{
		if (audit)
		{
			for (int i = 0; i < ghNumber; i++)
			{
				SDL_Rect Clyde_t; //Clyde: Ghosts[3]
				if (dist(Ghosts[i]->mBox, mBox) > 8 * tHeight)
					Clyde_t = {mBox.x, mBox.y, mBox.w, mBox.h};
				else
					Clyde_t = Corner[i];
				setTarget(Clyde_t, tiles, i);
			}
		}
		else if (por)
		{
			for (int i = 0; i < ghNumber; i++)
			{
				SDL_Rect Clyde_t = Corner[i];
				setTarget(Clyde_t, tiles, i);
			}
		}
		else
		{
			SDL_Rect Blinky_t = {mBox.x, mBox.y, mBox.w, mBox.h};
			setTarget(Blinky_t, tiles, 0);

			int pinky_x = 0, pinky_y = 0;
			if (dir == uDir)
				pinky_y = 2 * dHeight;
			if (dir == dDir)
				pinky_y = -3 * dHeight;
			if (dir == rDir)
				pinky_x = 3 * dHeight;
			if (dir == lDir)
				pinky_x = -2 * dHeight;

			SDL_Rect Pinky_t; //Pinky: Ghosts[1]
			Pinky_t = {mBox.x + pinky_x, mBox.y + pinky_y, mBox.w, mBox.h};
			setTarget(Pinky_t, tiles, 1);

			//TODO: Improve Logic
			SDL_Rect Inky_t = {2 * pinky_x - Ghosts[0]->mBox.x, 2 * pinky_y - Ghosts[0]->mBox.y, mBox.w, mBox.h}; //Inky: Ghosts[2]
			setTarget(Inky_t, tiles, 2);
		}
	}

	for (int i = 0; i < ghNumber; ++i)

		if (Ghosts[i]->isDead()) //Delete and Replace Dead Ghosts
		{
			delete Ghosts[i];
			Ghosts[i] = new Ghost(px[i], py[i], i);
		}

	for (int i = 0; i < ghNumber; ++i) //Show Ghosts
		Ghosts[i]->render(bin);
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
	{
		if (!ghosted)
			ghostedby = i;
		ghosted = true;
	}

	int udist, ddist, ldist, rdist;
	udist = ddist = ldist = rdist = INT_MAX;
	if (!wall(uBox, tiles) && uBox.y > 0)
		if (ghDir[i] != ghD)
			udist = dist(uBox, ghost);
	if (!wall(dBox, tiles) && dBox.y < sHeight)
		if (ghDir[i] != ghU)
			ddist = dist(dBox, ghost);
	if (!wall(lBox, tiles) && lBox.x > 0)
		if (ghDir[i] != ghR)
			ldist = dist(lBox, ghost);
	if (!wall(rBox, tiles) && rBox.x < sWidth)
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

bool Texture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	free();
	SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);

	if (textSurface == NULL)
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
	{
		mTexture = SDL_CreateTextureFromSurface(WRenderer, textSurface);

		if (mTexture == NULL)
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		else //Get image dimensions
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		SDL_FreeSurface(textSurface);
	}

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

int Texture::getWidth() //Return Texture Width
{
	return mWidth;
}

int Texture::getHeight() //Return Texture Height
{
	return mHeight;
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

//BUTTON DEFINITIONS

Button::Button() //Initializes Variables
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = bMouse;
}

void Button::setPosition(int x, int y) //Sets Top Left Position
{
	mPosition.x = x;
	mPosition.y = y;
}

void Button::handleEvent(SDL_Event *e, int i) //Takes Mouse Events
{
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		int x, y; //Get mouse position
		SDL_GetMouseState(&x, &y);
		//mCurrentSprite = bMouse;

		bool inside = true;

		if (i == 1 || i == 2)
		{
			if (x < mPosition.x || x > mPosition.x + bWidth || y < mPosition.y || y > mPosition.y + bHeight)
				inside = false;
		}
		else
		{
			if (x < mPosition.x || x > mPosition.x + bWidth || y < mPosition.y || y > mPosition.y + bHeight)
				inside = false;
		}

		if (!inside) //Mouse is outside button
			rules = false;
		else //Mouse is inside button
		{
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				rules = true;
				break;

			case SDL_MOUSEBUTTONDOWN:
				switch (i)
				{
				case 1:
					dNumber = 1;
					break;

				case 2:
					dNumber = 2;
					break;

				case 3:
					mCurrentSprite = bMouseClick;
					med.mCurrentSprite = bMouse;
					adv.mCurrentSprite = bMouse;
					level = 3;
					break;

				case 4:
					mCurrentSprite = bMouseClick;
					beg.mCurrentSprite = bMouse;
					adv.mCurrentSprite = bMouse;
					level = 2;
					break;

				case 5:
					mCurrentSprite = bMouseClick;
					beg.mCurrentSprite = bMouse;
					med.mCurrentSprite = bMouse;
					level = 1;
					break;
				default:
					break;
				}

			case SDL_MOUSEBUTTONUP:
				if (i == 1 || i == 2)
					play = true;
				else
					mCurrentSprite = bMouseClick;
				break;
			}
		}
	}
}

void Button::render(int i, int x, int y) //Show current button sprite
{
	switch (i)
	{
	case 1:
		bTexture1.render(mPosition.x, mPosition.y, &bClips[mCurrentSprite]);
		button1.setPosition(x, y);
		break;

	case 2:
		bTexture2.render(mPosition.x, mPosition.y, &bClips[mCurrentSprite]);
		button2.setPosition(x, y);
		break;

	case 3:
		begTexture.render(mPosition.x, mPosition.y, &bClips[mCurrentSprite]);
		beg.setPosition(x, y);
		break;

	case 4:
		medTexture.render(mPosition.x, mPosition.y, &bClips[mCurrentSprite]);
		med.setPosition(x, y);
		break;

	case 5:
		advTexture.render(mPosition.x, mPosition.y, &bClips[mCurrentSprite]);
		adv.setPosition(x, y);
		break;
	default:
		break;
	}
}
