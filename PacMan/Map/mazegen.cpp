#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

const int sWidth = 1080;
const int sHeight = 840;

//Tile Constants
const int tWidth = 40;
const int tHeight = 40;

const int i_max = sHeight / tHeight;
const int j_max = sWidth / tWidth;
const int mid_thick = 5;

int maze[i_max][j_max];
ofstream fout("Map/mapfile.map");

void createMaze()
{

    for (int i = 0; i < i_max; i++)
    {
        for (int j = 0; j < j_max; j++)
            if (maze[i][j] == 0)
                fout << 0 << " ";
            else
                fout << 1 << " ";
        fout << endl;
    }
}

void genMidBlocks(int start, int end)
{
    // IITD in the middle
    for (int i = start - 1; i < end; i++)
    {
        for (int j = 0; j < j_max; j++)
        {
            if (i == start - 1)
            {
                if (j == ((j_max - 15) / 2) || j == ((j_max - 15) / 2 + 1) || j == ((j_max - 15) / 2 + 3) || j == ((j_max - 15) / 2 + 4) || j == ((j_max - 15) / 2 + 6) || j == ((j_max - 15) / 2 + 7) || j == ((j_max - 15) / 2 + 8) || j == ((j_max - 15) / 2 + 9) || j == ((j_max - 15) / 2 + 11) || j == ((j_max - 15) / 2 + 12) || j == ((j_max - 15) / 2 + 13))
                {
                    maze[i][j] = 0;
                }
            }
            else if (i == start)
            {
                if (j == ((j_max - 14) / 2) || j == ((j_max - 15) / 2 + 1) || j == ((j_max - 15) / 2 + 3) || j == ((j_max - 15) / 2 + 4) || j == ((j_max - 15) / 2 + 6) || j == ((j_max - 15) / 2 + 7) || j == ((j_max - 15) / 2 + 8) || j == ((j_max - 15) / 2 + 9) || j == ((j_max - 15) / 2 + 11) || j == ((j_max - 15) / 2 + 12) || j == ((j_max - 15) / 2 + 14))
                    maze[i][j] = 0;
            }
            else if (i == start + 1)
            {
                if (j == ((j_max - 14) / 2) || j == ((j_max - 15) / 2 + 1) || j == ((j_max - 15) / 2 + 3) || j == ((j_max - 15) / 2 + 4) || j == ((j_max - 15) / 2 + 7) || j == ((j_max - 15) / 2 + 8) || j == ((j_max - 15) / 2 + 11) || j == ((j_max - 15) / 2 + 12) || j == ((j_max - 15) / 2 + 14))
                    maze[i][j] = 0;
            }
            else if (i == start + 2)
            {
                if (j == ((j_max - 14) / 2) || j == ((j_max - 15) / 2 + 1) || j == ((j_max - 15) / 2 + 3) || j == ((j_max - 15) / 2 + 4) || j == ((j_max - 15) / 2 + 7) || j == ((j_max - 15) / 2 + 8) || j == ((j_max - 15) / 2 + 11) || j == ((j_max - 15) / 2 + 12) || j == ((j_max - 15) / 2 + 14))
                    maze[i][j] = 0;
            }
            else if (i == start + 3)
            {
                if (j == ((j_max - 15) / 2) || j == ((j_max - 15) / 2 + 1) || j == ((j_max - 15) / 2 + 3) || j == ((j_max - 15) / 2 + 4) || j == ((j_max - 15) / 2 + 7) || j == ((j_max - 15) / 2 + 8) || j == ((j_max - 15) / 2 + 11) || j == ((j_max - 15) / 2 + 12) || j == ((j_max - 15) / 2 + 13))
                    maze[i][j] = 0;
            }
        }
    }

    //side flanks
    int r1 = rand() % 3;
    int r2 = rand() % 2;

    for (int i = start - 1; i < end; i++)
    {
        for (int j = 0; j < j_max; j++)
        {
            if (r1 == 1)
            {
                //rectangles
                if (r2 == 1)
                {
                    if ((i >= 8 && i <= 9 && j >= 2 && j <= 4))
                        maze[i][j] = 0;
                    if ((i >= 11 && i <= 12 && j >= 2 && j <= 4))
                        maze[i][j] = 0;

                    if ((i >= 8 && i <= 9 && j >= 22 && j <= 24))
                        maze[i][j] = 0;
                    if ((i >= 11 && i <= 12 && j >= 22 && j <= 24))
                        maze[i][j] = 0;
                }
                else if (r2 == 2)
                {
                    if ((i >= 8 && i <= 10 && j >= 2 && j <= 4))
                        maze[i][j] = 0;
                    if ((i > 11 && i <= 12 && j >= 2 && j <= 4))
                        maze[i][j] = 0;

                    if ((i >= 8 && i <= 10 && j >= 22 && j <= 24))
                        maze[i][j] = 0;
                    if ((i > 11 && i <= 12 && j >= 22 && j <= 24))
                        maze[i][j] = 0;
                }
                else
                {
                    if ((i >= 8 && i < 9 && j >= 2 && j <= 4))
                        maze[i][j] = 0;
                    if ((i >= 10 && i <= 12 && j >= 2 && j <= 4))
                        maze[i][j] = 0;

                    if ((i >= 8 && i < 9 && j >= 22 && j <= 24))
                        maze[i][j] = 0;
                    if ((i >= 10 && i <= 12 && j >= 22 && j <= 24))
                        maze[i][j] = 0;
                }
            }
            else if (r1 == 2)
            {
                /*
            |---
            |
                |
              --|
             */
                if (j == 2 && i >= 8 && i <= 10)
                    maze[i][j] = 0;
                if (i == 8 && j >= 2 && j <= 4)
                    maze[i][j] = 0;
                if (i == 12 && j >= 2 && j <= 4)
                    maze[i][j] = 0;
                if (j == 4 && i >= 10 && i <= 12)
                    maze[i][j] = 0;

                //
                if (j == 22 && i >= 10 && i <= 12)
                    maze[i][j] = 0;
                if (i == 8 && j >= 22 && j <= 24)
                    maze[i][j] = 0;
                if (i == 12 && j >= 22 && j <= 24)
                    maze[i][j] = 0;
                if (j == 24 && i >= 8 && i <= 10)
                    maze[i][j] = 0;
            }
            else
            {
                // inv L L
                if (j == 2 && i >= 10 && i <= 12)
                    maze[i][j] = 0;
                if (i == 12 && j >= 2 && j <= 4)
                    maze[i][j] = 0;
                if (i == 8 && j >= 2 && j <= 4)
                    maze[i][j] = 0;
                if (j == 4 && i >= 8 && i <= 10)
                    maze[i][j] = 0;

                //
                if (j == 22 && i >= 8 && i <= 10)
                    maze[i][j] = 0;
                if (i == 8 && j >= 22 && j <= 24)
                    maze[i][j] = 0;
                if (i == 12 && j >= 22 && j <= 24)
                    maze[i][j] = 0;
                if (j == 24 && i >= 10 && i <= 12)
                    maze[i][j] = 0;
            }
        }
    }
}

void genTopBlocks(int start, int end)
{
    // generate random blocks for first half and then mirror
    // start row is always blocked and 2nd row is always path
    // end is always path

    int r = rand() % 2; // to decide if mid col is filled
    int r3 = rand() % 2;

    int r2 = 3 + rand() % 8;

    for (int i = start + 2; i < end - 1; i++)
    {
        for (int j = 2; j < j_max / 2; j++)
        {
            if (r == 0)
            {
                maze[i][j_max / 2] = 0;
                if (r3 == 0)
                {
                    // T shaped
                    if (i <= start + 3 && j >= j_max / 2 - 4 && j <= j_max / 2 + 4)
                        maze[i][j] = 0;

                    if (i > start + 4)
                    {
                        if ((j < j_max / 2 - 1 && j > j_max / 2 - 7) || (j > j_max / 2 + 1 && j < j_max / 2 + 7))
                            maze[i][j] = 0;
                    }
                    if (j <= j_max / 2 - 6 && j >= j_max / 2 - 7)
                        maze[i][j] = 0;

                    if (j >= 2 && j < 5)
                        maze[i][j] = 0;
                }
                else
                {
                    // _|_ shaped
                    if (i > start + 4 && j >= j_max / 2 - 4 && j <= j_max / 2 + 4)
                        maze[i][j] = 0;

                    // [_|_]
                    if (i <= start + 3)
                    {
                        if ((j < j_max / 2 - 1 && j > j_max / 2 - 7) || (j > j_max / 2 + 1 && j < j_max / 2 + 7))
                            maze[i][j] = 0;
                    }
                    if (j <= j_max / 2 - 6 && j >= j_max / 2 - 7)
                        maze[i][j] = 0;

                    if (j >= 2 && j < 5)
                        maze[i][j] = 0;
                }
            }
            else
            {
                maze[i][j] = 0;

                maze[i][r2] = 1;
                maze[(start + end) / 2][j] = 1;
            }
        }
    }

    //mirror code
    for (int i = start + 2; i < end - 1; i++)
    {
        for (int j = j_max / 2 + 1; j < j_max - 1; j++)
        {
            maze[i][j] = maze[i][j_max - j - 1];
        }
    }
}

void genBottomBlocks(int start, int end)
{
    // generate random blocks for first half and then mirror
    // start row is always path
    // end-2 is always path

    int r = rand() % 2; // to decide if mid col is filled
    int r3 = rand() % 2;

    int r2 = 3 + rand() % 8;

    for (int i = start; i < end - 2; i++)
    {
        for (int j = 2; j < j_max / 2; j++)
        {
            if (r == 0)
            {
                maze[i][j_max / 2] = 0;
                if (r3 == 0)
                {
                    // T shaped
                    if (i < start + 2 && j >= j_max / 2 - 4 && j <= j_max / 2 + 4)
                        maze[i][j] = 0;

                    if (i > start + 2)
                    {
                        if ((j < j_max / 2 - 1 && j > j_max / 2 - 7) || (j > j_max / 2 + 1 && j < j_max / 2 + 7))
                            maze[i][j] = 0;
                    }
                    if (j <= j_max / 2 - 6 && j >= j_max / 2 - 7)
                        maze[i][j] = 0;

                    if (j >= 2 && j < 5)
                        maze[i][j] = 0;
                }
                else
                {
                    // _|_ shaped
                    if (i > start + 2 && j >= j_max / 2 - 4 && j <= j_max / 2 + 4)
                        maze[i][j] = 0;

                    // [_|_]
                    if (i <= start + 1)
                    {
                        if ((j < j_max / 2 - 1 && j > j_max / 2 - 7) || (j > j_max / 2 + 1 && j < j_max / 2 + 7))
                            maze[i][j] = 0;
                    }
                    if (j <= j_max / 2 - 6 && j >= j_max / 2 - 7)
                        maze[i][j] = 0;

                    if (j >= 2 && j < 5)
                        maze[i][j] = 0;
                }
            }
            else
            {
                maze[i][j] = 0;

                maze[i][r2] = 1;
                maze[(start + end) / 2][j] = 1;
            }
        }
    }

    //mirror code
    for (int i = start; i < end - 2; i++)
    {
        for (int j = j_max / 2 + 1; j < j_max - 1; j++)
        {
            maze[i][j] = maze[i][j_max - j - 1];
        }
    }
}

void genTunnels()
{
    int r = 1 + rand() % i_max / 2;
    int r2 = i_max / 2 + rand() % i_max / 2;
    //cout << r << endl;
    //cout << r2 << endl;
    maze[r][0] = 1;
    maze[r][j_max - 1] = 1;

    maze[r2][0] = 1;
    maze[r2][j_max - 1] = 1;
}

int main()
{

    srand(time(0));

    /*
    genTopBlocks();
    genMidBlocks();
    genBottomBlocks();
    createMaze();

    store rows in top and bottom blocks as arrays, to mirror
    */

    // initialize everything with blocks
    for (int i = 0; i < i_max; i++)
        for (int j = 0; j < j_max; j++)
            maze[i][j] = 1;

    int top_start = 0;
    int top_end = (i_max - mid_thick) / 2;
    int mid_start = (i_max - mid_thick) / 2 + 1;
    int mid_end = mid_start + mid_thick - 1;
    int bottom_start = mid_end + 1;
    int bottom_end = i_max;

    // set boundaries as blocked
    for (int i = 0; i < i_max; i++)
        for (int j = 0; j < j_max; j++)
        {
            if (i == 0 || i == i_max - 1)
                maze[i][j] = 0;
            if (j == 0 || j == j_max - 1)
                maze[i][j] = 0;
        }

    genMidBlocks(mid_start, mid_end);
    genTopBlocks(top_start, top_end);
    genBottomBlocks(bottom_start, bottom_end);
    genTunnels();
    createMaze();

    fout.close();
    return 0;
}
