#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <DataTable/DataTable.hpp>

using namespace std;
using namespace datatable;

TEST_CASE ("headers", "header select")
{
    vector<string> headers {"col1", "col2", "col3", "col4", "response"};
    SECTION("rtrim", "rtrim right spaces")
    {
        DataTable<float> dt(headers, "response", nullptr, 0, 0);
        dt["col1"];
        dt["test"];
    }
}