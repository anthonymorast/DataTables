#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <iostream>
#include <string>
#include <DataTable/ShapeType.hpp>

using DT_SHAPE_TYPE = datatable::DT_SHAPE_TYPE;

TEST_CASE ("[]", "square bracket operations")
{
    const int ROWS = 100;
    const int COLUMNS = 300;
    DT_SHAPE_TYPE shape(ROWS, COLUMNS);
    SECTION("[1]", "columns")
    {
        REQUIRE(shape[1] == COLUMNS);
    }

    SECTION("[0]", "rows")
    {
        REQUIRE(shape[0] == ROWS);
    }

    SECTION("[!= 0 && != 1]", "invalid")
    {
        REQUIRE_THROWS_WITH(shape[3], Catch::Contains("Invalid shape index"));
        REQUIRE_THROWS_WITH(shape[-1], Catch::Contains("Invalid shape index"));
        REQUIRE_THROWS_WITH(shape[10202], Catch::Contains("Invalid shape index"));
    }
}

TEST_CASE ("setters", "test the updating methods")
{
    SECTION("setters", "set rows")
    {
        DT_SHAPE_TYPE shape(400, 2000);
        REQUIRE(shape[0] == 400);
        REQUIRE(shape[1] == 2000);
        shape.set_rows(200);
        REQUIRE(shape[0] == 200);
        REQUIRE(shape[1] == 2000);
    }

    SECTION("setters", "set columns")
    {
        DT_SHAPE_TYPE shape(400, 2000);
        REQUIRE(shape[0] == 400);
        REQUIRE(shape[1] == 2000);
        shape.set_cols(1000);
        REQUIRE(shape[0] == 400);
        REQUIRE(shape[1] == 1000);
    }

    SECTION("setters", "update all")
    {
        DT_SHAPE_TYPE shape(400, 2000);
        REQUIRE(shape[0] == 400);
        REQUIRE(shape[1] == 2000);
        shape.update(200, 500);
        REQUIRE(shape[0] == 200);
        REQUIRE(shape[1] == 500);
    }
}

TEST_CASE ("constructor", "size provided constructor") 
{
    DT_SHAPE_TYPE shape(100, 200);
    SECTION("constructor", "correct rows") 
    {
        REQUIRE(shape[0] == 100);
    }

    SECTION("constructor", "correct columns")
    {
        REQUIRE(shape[1] == 200);
    }
}

TEST_CASE ("empty shape type", "empty constructor")
{
    DT_SHAPE_TYPE shape;
    SECTION("empty shape type", "correct rows") 
    {
        REQUIRE(shape.get_rows() == 0);
        REQUIRE(shape[0] == 0);
    }

    SECTION("empty shape type", "correct columns") 
    {
        REQUIRE(shape.get_columns() == 0);
        REQUIRE(shape[1] == 0);
    }
}