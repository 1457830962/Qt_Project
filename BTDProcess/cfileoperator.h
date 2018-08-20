#ifndef CFILEOPERATOR_H
#define CFILEOPERATOR_H

#include <string>
#include <vector>
using namespace std;

class CfileOperator
{
private:
    CfileOperator();
public:
    static void readDirectory(const string& directoryName, vector<string>& filenames, bool addDirectoryName = true);

};

#endif // CFILEOPERATOR_H
