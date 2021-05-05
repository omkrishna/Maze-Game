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

bool isIITD(int i, int j)
{
    if (i == (i_max - 5) / 2 + 1)
    {
        if (j == ((j_max - 15) / 2 + 1) || j == ((j_max - 15) / 2 + 2) || j == ((j_max - 15) / 2 + 4) || j == ((j_max - 15) / 2 + 5) || j == ((j_max - 15) / 2 + 7) || j == ((j_max - 15) / 2 + 8) || j == ((j_max - 15) / 2 + 9) || j == ((j_max - 15) / 2 + 10) || j == ((j_max - 15) / 2 + 12) || j == ((j_max - 15) / 2 + 13) || j == ((j_max - 15) / 2 + 14))
            return true;
    }
    else if (i == (i_max - 5) / 2 + 2)
    {
        if (j == ((j_max - 14) / 2 + 1) || j == ((j_max - 15) / 2 + 2) || j == ((j_max - 15) / 2 + 4) || j == ((j_max - 15) / 2 + 5) || j == ((j_max - 15) / 2 + 7) || j == ((j_max - 15) / 2 + 8) || j == ((j_max - 15) / 2 + 9) || j == ((j_max - 15) / 2 + 10) || j == ((j_max - 15) / 2 + 12) || j == ((j_max - 15) / 2 + 13) || j == ((j_max - 15) / 2 + 15))
            return true;
    }
    else if (i == (i_max - 5) / 2 + 3)
    {
        if (j == ((j_max - 14) / 2 + 1) || j == ((j_max - 15) / 2 + 2) || j == ((j_max - 15) / 2 + 4) || j == ((j_max - 15) / 2 + 5) || j == ((j_max - 15) / 2 + 8) || j == ((j_max - 15) / 2 + 9) || j == ((j_max - 15) / 2 + 12) || j == ((j_max - 15) / 2 + 13) || j == ((j_max - 15) / 2 + 15))
            return true;
    }
    else if (i == (i_max - 5) / 2 + 4)
    {
        if (j == ((j_max - 14) / 2 + 1) || j == ((j_max - 15) / 2 + 2) || j == ((j_max - 15) / 2 + 4) || j == ((j_max - 15) / 2 + 5) || j == ((j_max - 15) / 2 + 8) || j == ((j_max - 15) / 2 + 9) || j == ((j_max - 15) / 2 + 12) || j == ((j_max - 15) / 2 + 13) || j == ((j_max - 15) / 2 + 15))
            return true;
    }
    else if (i == (i_max - 5) / 2 + 5)
    {
        if (j == ((j_max - 15) / 2 + 1) || j == ((j_max - 15) / 2 + 2) || j == ((j_max - 15) / 2 + 4) || j == ((j_max - 15) / 2 + 5) || j == ((j_max - 15) / 2 + 8) || j == ((j_max - 15) / 2 + 9) || j == ((j_max - 15) / 2 + 12) || j == ((j_max - 15) / 2 + 13) || j == ((j_max - 15) / 2 + 14))
            return true;
    }
    return false;
}

bool isBoundary(int i, int j)
{
    if (i == 1 or i == i_max || j == 1 or j == j_max)
        return true;
    return false;
}

bool randomSquare(int len, int wid, int wid_2, int i, int j)
{
    if ((j >= 3 && j < 3 + len) || (j <= j_max - 2 && j > j_max - len - 2))
    {
        if ((i >= 3 && i < 3 + wid) || (i > 3 + wid && i <= 3 + wid + wid_2))
            return true;
    }
    return false;
}

bool isCenterFlanks(int i, int j, int r, int r2)
{
    /*
     9,3   9,5              9,23    9,25



     13,3  13,5             13,23   13,25
    */
    if (r == 1)
    {
        //rectangles - 2,2 / 3,1 / 1,3
        if (r2 == 1)
        {
            if ((i >= 9 && i <= 10 && j >= 3 && j <= 5))
                return true;
            if ((i >= 12 && i <= 13 && j >= 3 && j <= 5))
                return true;

            if ((i >= 9 && i <= 10 && j >= 23 && j <= 25))
                return true;
            if ((i >= 12 && i <= 13 && j >= 23 && j <= 25))
                return true;
        }
        else if (r2 == 2)
        {
            if ((i >= 9 && i <= 11 && j >= 3 && j <= 5))
                return true;
            if ((i > 12 && i <= 13 && j >= 3 && j <= 5))
                return true;

            if ((i >= 9 && i <= 11 && j >= 23 && j <= 25))
                return true;
            if ((i > 12 && i <= 13 && j >= 23 && j <= 25))
                return true;
        }
        else
        {
            if ((i >= 9 && i < 10 && j >= 3 && j <= 5))
                return true;
            if ((i >= 11 && i <= 13 && j >= 3 && j <= 5))
                return true;

            if ((i >= 9 && i < 10 && j >= 23 && j <= 25))
                return true;
            if ((i >= 11 && i <= 13 && j >= 23 && j <= 25))
                return true;
        }
    }
    else if (r == 2)
    {
        /*
            |---
            |
                |
              --|
        */
        if (j == 3 && i >= 9 && i <= 11)
            return true;
        if (i == 9 && j >= 3 && j <= 5)
            return true;
        if (i == 13 && j >= 3 && j <= 5)
            return true;
        if (j == 5 && i >= 11 && i <= 13)
            return true;

        //
        if (j == 23 && i >= 11 && i <= 13)
            return true;
        if (i == 9 && j >= 23 && j <= 25)
            return true;
        if (i == 13 && j >= 23 && j <= 25)
            return true;
        if (j == 25 && i >= 9 && i <= 11)
            return true;
    }
    else
    {
        // inv L L
        if (j == 3 && i >= 11 && i <= 13)
            return true;
        if (i == 13 && j >= 3 && j <= 5)
            return true;
        if (i == 9 && j >= 3 && j <= 5)
            return true;
        if (j == 5 && i >= 9 && i <= 11)
            return true;

        //
        if (j == 23 && i >= 9 && i <= 11)
            return true;
        if (i == 9 && j >= 23 && j <= 25)
            return true;
        if (i == 13 && j >= 23 && j <= 25)
            return true;
        if (j == 25 && i >= 11 && i <= 13)
            return true;
    }
    return false;
}

bool centralCol(int r, int len_1, int wid_1, int wid_2, int i, int j)
{

    if (r == 0)
    {
        // central col in upper block
        if (j == 14 && i >= 3 && i <= 7)
            return true;

        cout << len_1 << "," << wid_1 << endl;
        if (wid_1 >= 2)
        {
            // _|_
            if (i >= 6 && i <= 7)
                if (j > 3 + len_1 && j < j_max - 2 - len_1)
                    return true;
        }
    }
    else
    {
        // upper block
        if ((j > len_1 + 3 && j < 14) || (j > 14 && j < j_max - len_1 - 2))
            if (i >= 3 && i <= 7)
                return true;
    }
    return false;
}

int main()
{
    ofstream fout("Map/mapfile.map");
    srand(time(0));

    int len_1 = 2 + rand() % 6;
    int wid_1, wid_2;

    int r = rand() % 2; // for Squares

    int r2 = rand() % 3; // for IITD Flanks
    int r3 = rand() % 3; // for IITD Flanks

    int r4 = rand() % 2; // include mid column

    if (r == 1)
    {
        wid_1 = 3;
        wid_2 = 1;
    }
    else
    {
        wid_1 = 2;
        wid_2 = 2;
    }

    for (int i = 1; i <= i_max; i++)
    {
        for (int j = 1; j <= j_max; j++)
        {
            if (isBoundary(i, j) or isIITD(i, j))
                fout << 0 << " ";
            else if (isCenterFlanks(i, j, r2, r3))
            {
                fout << 0 << " ";
            }
            else if (randomSquare(len_1, wid_1, wid_2, i, j) || centralCol(r4, len_1, wid_1, wid_2, i, j))
            {
                fout << 0 << " ";
            }
            else
            {
                fout << 1 << " ";
            }
        }
        fout << "\n";
    }

    fout.close();
    return 0;
}
