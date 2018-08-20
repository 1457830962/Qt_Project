#include "csolution.h"

CSolution::CSolution()
{


}

bool CSolution::Find(int target, vector<vector<int> > array)
{
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(array[i][j]==target)
                return true;
            else {
                return false;
            }
        }
    }

}

bool CSolution::Find_1(int target, vector<vector<int> > array)
{
    int row = array.size();
    int column = array[0].size();

    int i,j;
    for (i = row - 1, j = 0; i >= 0 && j<column;)
    {
        if (target == array[i][j])
            return true;

        if (target<array[i][j])
        {
            i--;
            continue;
        }

        if (target>array[i][j])
        {
            j++;
            continue;
        }
    }
    return false;

}


