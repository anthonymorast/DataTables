#include <DataTable/StringUtils.hpp>

namespace datatable
{
    std::string StringUtils::trim(std::string str)
    {
        auto start = str.begin();
        while(start != str.end() && std::isspace(*start)) 
        {
            start++;
        }

        auto end = str.end();
        do 
        {
            end--;
        } while(std::distance(start, end) > 0 && std::isspace(*end));

        return std::string(start, end+1);
    }

    void StringUtils::trim_all(std::string* strs, int n)
    {
        for(int i = 0; i < n; i++)
        {
            strs[i] = trim(strs[i]);
        }
    }
}
