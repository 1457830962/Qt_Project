#ifndef CSOLUTION_H
#define CSOLUTION_H
#include<vector>
using namespace std;


class CSolution
{
public:
    CSolution();
public:
    bool Find(int target, vector<vector<int> > array);
    bool Find_1(int target, vector<vector<int> > array);
};

#endif // CSOLUTION_H
