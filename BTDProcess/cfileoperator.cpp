#include "cfileoperator.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#if defined(WIN32) || defined(_WIN32)
#include <io.h>
#else
#include <dirent.h>
#endif

using namespace std;

CfileOperator::CfileOperator()
{

}

void CfileOperator::readDirectory(const string& directoryName, vector<string>& filenames, bool addDirectoryName)
{
    filenames.clear();

#if defined(WIN32) | defined(_WIN32)
    struct _finddata_t s_file;
    string str = directoryName + "\\*.*";

    intptr_t h_file = _findfirst(str.c_str(), &s_file);
    if (h_file != static_cast<intptr_t>(-1.0))
    {
        do
        {
            if (addDirectoryName)
                filenames.push_back(directoryName + "\\" + s_file.name);
            else
                filenames.push_back((string)s_file.name);
        } while (_findnext(h_file, &s_file) == 0);
    }
    _findclose(h_file);
#else
    DIR* dir = opendir(directoryName.c_str());
    if (dir != NULL)
    {
        struct dirent* dent;
        while ((dent = readdir(dir)) != NULL)
        {
            if(dent->d_type == DT_REG)
            {
                if (addDirectoryName)

                    filenames.push_back(directoryName + "/" + string(dent->d_name));
                else
                    filenames.push_back(string(dent->d_name));
            }
        }

        closedir(dir);
    }
#endif

    sort(filenames.begin(), filenames.end());
}


