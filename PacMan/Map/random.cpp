#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

const int sWidth = 1040;
const int sHeight = 960;

//Tile Constants
const int tWidth = 40;
const int tHeight = 40;

const int i_max = sHeight / tHeight;
const int j_max = sWidth / tWidth;
int main()
{
    ofstream fout("Map/mapfile.map");
    srand(time(0));

    for (int i = 0; i < i_max; i++)
    {
        for (int j = 0; j < j_max; j++)
            if (i == 0 || i == i_max - 1 || j == 0 || j == j_max - 1)
                fout << 0 << " ";
            else
            {
                switch (i)
                {
                case ((i_max - 4) / 2):
                    if (j == ((j_max - 14) / 2) || j == ((j_max - 14) / 2 + 2) || j == ((j_max - 14) / 2 + 4) || j == ((j_max - 14) / 2 + 5) || j == ((j_max - 14) / 2 + 6) || j == ((j_max - 14) / 2 + 11) || j == ((j_max - 14) / 2 + 12))
                        fout << 2 << " ";
                    else
                        fout << rand() % 2 + 0 << " ";
                    break;
                case ((i_max - 4) / 2 + 1):
                    if (j == ((j_max - 14) / 2 + 0) || j == ((j_max - 14) / 2 + 2) || j == ((j_max - 14) / 2 + 5) || j == ((j_max - 14) / 2 + 11) || j == ((j_max - 14) / 2 + 13))
                        fout << 2 << " ";
                    else
                        fout << rand() % 2 + 0 << " ";
                    break;
                case ((i_max - 4) / 2 + 2):
                    if (j == ((j_max - 14) / 2 + 0) || j == ((j_max - 14) / 2 + 2) || j == ((j_max - 14) / 2 + 5) || j == ((j_max - 14) / 2 + 8) || j == ((j_max - 14) / 2 + 9) | j == ((j_max - 14) / 2 + 11) || j == ((j_max - 14) / 2 + 13))
                        fout << 2 << " ";
                    else
                        fout << rand() % 2 + 0 << " ";
                    break;
                case ((i_max - 4) / 2 + 3):
                    if (j == ((j_max - 14) / 2 + 0) || j == ((j_max - 14) / 2 + 2) || j == ((j_max - 14) / 2 + 5) || j == ((j_max - 14) / 2 + 11) || j == ((j_max - 14) / 2 + 12))
                        fout << 2 << " ";
                    else
                        fout << rand() % 2 + 0 << " ";
                    break;
                default:
                    fout << rand() % 2 + 0 << " ";
                    break;
                }
            }
        fout << "\n";
    }

    fout.close();
    return 0;
}