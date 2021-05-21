#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <iostream>
#include <string>
#include <DataTable/DataTable.hpp>

TEST_CASE ("x_to_x_squared", "[data select]")
{
    datatable::DataTable<float> dt("x_to_x_squared.csv", "x2", true);
    SECTION("top n rows", "[data select]")
    {
        
    }
}