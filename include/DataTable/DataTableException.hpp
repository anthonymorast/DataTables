#ifndef DATA_TABLE_EXCEPTION
#define DATA_TABLE_EXCEPTION

#include <string>

namespace datatable 
{
    struct DataTableException : public std::runtime_error
    {
        ~DataTableException() {}
        DataTableException(const std::string error_msg) : runtime_error(error_msg) {}
    };
}

#endif