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
    SECTION("col select", "select columns")
    {
        DataTable<float> dt(headers, "response", nullptr, 0, headers.size());

        // select column that doesn't exists
        REQUIRE_THROWS_WITH(dt["test"], Catch::Contains("Column 'test' was not found in the data table."));

        auto new_dt = dt["col1"];
        REQUIRE(new_dt.ncols() == 1);
        REQUIRE(new_dt.nrows() == 0);
    }
}
