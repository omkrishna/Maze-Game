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

bool randomLine(int row, int width, int i, int j)
{
    if (i == row && j > 2 && j <= width + 2)
    {
        return true;
    }
    if (i == row && j < j_max - 1 && j > (j_max - width - 2))
        return true;

    return false;
    //cout << x << endl;
}

int main()
{
    ofstream fout("Map/mapfile.map");
    srand(time(0));

    int r_row = 3 + rand() % 5;
    int r_width = rand() % j_max / 2;
    cout << r_row << "," << r_width << endl;
    for (int i = 1; i <= i_max; i++)
    {

        for (int j = 1; j <= j_max; j++)
        {
            if (isBoundary(i, j) or isIITD(i, j))
                fout << 0 << " ";
            else if (randomLine(r_row, r_width, i, j))
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
