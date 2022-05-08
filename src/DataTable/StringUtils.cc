#include <DataTable/StringUtils.hpp>

namespace datatable
{
    void StringUtils::ltrim(string& str)
    {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), 
            [](unsigned char character) 
            {
                return !std::isspace(character);
            }
        ));
    }

    void StringUtils::rtrim(string& str)
    {
        str.erase(std::find_if(str.rbegin(), str.rend(), 
            [](unsigned char character)
            {   
                return !std::isspace(character);
            }
        ).base(), str.end());
    }

    void StringUtils::trim(string& str)
    {
        ltrim(str);
        rtrim(str);
    }

    void StringUtils::trim_all(string* strs, int n)
    {
        for(int i = 0; i < n; i++)
        {
            trim(strs[i]);
        }
    }
}
