#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

const int i_max = 12;
const int j_max = 16;
int main()
{
    ofstream fout("Map/mapfile.map");
    srand(time(0));

    for (int i = 0; i < i_max; i++)
    {
        for (int j = 0; j < j_max; j++)
        if (i==0 || i == i_max - 1 || j == 0 || j == j_max - 1)
            fout << 0 << " ";
        else
        {
            switch (i)
            {
            case 4:
                if (j == 1 || j == 3 || j == 5 || j == 6 || j == 7 || j == 12 || j == 13)
                    fout << 2 << " ";
                else
                    fout << rand() % 2 + 0 << " ";
                break;
            case 5:
                if (j == 1 || j == 3 || j == 6 || j == 12 || j == 14)
                    fout << 2 << " ";
                else
                    fout << rand() % 2 + 0 << " ";
                break;
            case 6:
                if (j == 1 || j == 3 || j == 6 || j == 9 || j == 10 | j == 12 || j == 14)
                    fout << 2 << " ";
                else
                    fout << rand() % 2 + 0 << " ";
                break;
            case 7:
                if (j == 1 || j == 3 || j == 6 || j == 12 || j == 13)
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