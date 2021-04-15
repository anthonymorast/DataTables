#include <DataTable/DateUtils.hpp>

namespace datatable
{
    DateUtils::DateUtils() 
    {
        // yyyy-mm-dd
        _regular_expressions.insert(std::pair<std::string, std::regex>("%Y-%m-%d", std::regex("^\\d{4}\\-(0[1-9]|1[012])\\-(0[1-9]|[12][0-9]|3[01])$")));
        // yyyy/mm/dd
        _regular_expressions.insert(std::pair<std::string, std::regex>("%Y-%m-%d", std::regex("^\\d{4}/(0[1-9]|1[012])/(0[1-9]|[12][0-9]|3[01])$")));
        // mm/dd/yyyy
        _regular_expressions.insert(std::pair<std::string, std::regex>("%m/%d/%Y", std::regex("^(1[0-2]|0[1-9])/(3[01]|[12][0-9]|0[1-9])/[0-9]{4}$")));
        // mm-dd-yyyy
        _regular_expressions.insert(std::pair<std::string, std::regex>("%m/%d/%Y", std::regex("^(1[0-2]|0[1-9])\\-(3[01]|[12][0-9]|0[1-9])\\-[0-9]{4}$")));
        // yyyy/mm/dd hh:mm:ss
        _regular_expressions.insert(std::pair<std::string, std::regex>("%Y/%m/%d %H:%i:%s", std::regex("\\d{4}/\\d{2}/\\d{2}\\s+\\d{2}:\\d{2}:\\d{2}")));
        // yyyy-mm-dd hh:mm:ss
        _regular_expressions.insert(std::pair<std::string, std::regex>("%Y/%m/%d %H:%i:%s", std::regex("\\d{4}\\-\\d{2}\\-\\d{2}\\s+\\d{2}:\\d{2}:\\d{2}")));

        // TODO: cont.
    }

    bool DateUtils::isDateTime(std::string value)
    {
        for (auto it = _regular_expressions.begin(); it != _regular_expressions.end(); it++) 
        {
            if(std::regex_match(value, it->second))
                return true;
        }
        return false;
    }

    std::string DateUtils::getFormat(std::string value)
    {
        for (auto it = _regular_expressions.begin(); it != _regular_expressions.end(); it++) 
        {
            if(std::regex_match(value, it->second))
                return it->first;
        }
        return "";
    }

    std::time_t DateUtils::getTimeFromString(std::string value, std::string format) const 
    {
        struct std::tm tm = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        strptime(value.c_str(), format.c_str(), &tm);
        return std::mktime(&tm);
    }

    std::string DateUtils::getStringFromTime(std::time_t timestamp, std::string format) const 
    {
        std::tm* tm = std::localtime(&timestamp);
        char buffer[32];
        std::strftime(buffer, 32, format.c_str(), tm);
        std::string time_string(buffer);
        return time_string;
    }
}