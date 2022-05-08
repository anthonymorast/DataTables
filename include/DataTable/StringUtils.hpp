#ifndef DATA_TABLE_STR_UTILS
#define DATA_TABLE_STR_UTILS

#include <string>
#include <algorithm>

namespace datatable
{
    using string = std::string;
    class StringUtils
    {
        public:
            void ltrim(string& str);
            void rtrim(string& str);
            void trim(string& str);
            void trim_all(string* strings, int count);
    };
}

#endif
