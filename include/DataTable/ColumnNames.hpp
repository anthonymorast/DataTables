#ifndef COLUMN_NAMES
#define COLUMN_NAMES

#include <vector>
#include <string>

namespace datatable 
{
    struct ColumnNames
    {
        ColumnNames(std::vector<std::string> names) : _names(names) {}
        friend ostream& operator<<(ostream& os, const ColumnNames cn) 
        {
            std::string output {"("};
            for(auto name : cn._names)
                output += name + ", ";
            output.pop_back();
            output.pop_back();
            output += ")";

            os << output << std::endl;
            return os;
        }

        private:
            std::vector<std::string> _names;
    };
}

#endif