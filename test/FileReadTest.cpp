#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <iostream>
#include <string>
#include <DataTable/DataTable.hpp>

TEST_CASE ("x_to_x_squared", "[fileread]")
{
    datatable::DataTable<float> dt("x_to_x_squared.csv", "x2", true);
    SECTION("shape", "[fileread]")
    {
        int* shape = dt.shape();
        REQUIRE(shape[0] == 100);
        REQUIRE(shape[1] == 2);
    }

    SECTION("headers", "[fileread]")
    {
        std::string* headers = dt.get_headers();
        REQUIRE(headers[0] == "x");
        REQUIRE(headers[1] == "x2");
    }

    SECTION("num rows", "[fileread]")
    {
        REQUIRE(100 == dt.nrows());
    }

    SECTION("num cols", "[fileread]")
    {
        REQUIRE(2 == dt.ncols());
    }

    SECTION("response", "[fileread]")
    {
        REQUIRE("x2" == dt.get_response_column_name());
    }
}