#ifndef DATA_TABLE_STR_UTILS
#define DATA_TABLE_STR_UTILS

#include <string>
#include <algorithm>

namespace datatable
{
    
    class StringUtils
    {
        public:
            std::string trim(std::string str);
            void trim_all(std::string* strings, int count);
    };

}

#endif
