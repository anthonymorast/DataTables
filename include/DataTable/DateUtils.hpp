#ifndef DATA_TABLE_DATE_UTILS
#define DATA_TABLE_DATE_UTILS

#include <string>
#include <ctime>
#include <regex>
#include <map>
#include <iostream>

namespace datatable
{
    class DateUtils
    {
        public:
            DateUtils();
            bool isDateTime(std::string value);
            std::string getFormat(std::string value);
            std::time_t getTimeFromString(std::string value, std::string format) const;
            std::string getStringFromTime(std::time_t timestamp, std::string format) const;
        private:
            std::map<std::string, std::regex> _regular_expressions;
    };
}

#endif 