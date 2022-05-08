#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <iostream>
#include <string>
#include <DataTable/StringUtils.hpp>

datatable::StringUtils STRING_UTILS;

TEST_CASE ("rtrim", "right trim")
{
    std::string RIGHT_SPACES = "header1  ";
    std::string LEFT_SPACES = "  header1";
    std::string ALL_SPACES = "  header1  ";

    SECTION("rtrim", "rtrim right spaces")
    {
        STRING_UTILS.rtrim(RIGHT_SPACES);
        REQUIRE(RIGHT_SPACES == "header1");
    }

    SECTION("rtrim", "rtrim left spaces")
    {
        STRING_UTILS.rtrim(LEFT_SPACES);
        REQUIRE(LEFT_SPACES == "  header1");
    }

    SECTION("rtrim", "rtrim all spaces")
    {
        STRING_UTILS.rtrim(ALL_SPACES);
        REQUIRE(ALL_SPACES == "  header1");
    }
}

TEST_CASE ("ltrim", "left trim")
{
    std::string RIGHT_SPACES = "header1  ";
    std::string LEFT_SPACES = "  header1";
    std::string ALL_SPACES = "  header1  ";

    SECTION("ltrim", "ltrim right spaces")
    {
        STRING_UTILS.ltrim(RIGHT_SPACES);
        REQUIRE(RIGHT_SPACES == "header1  ");
    }

    SECTION("ltrim", "ltrim left spaces")
    {
        STRING_UTILS.ltrim(LEFT_SPACES);
        REQUIRE(LEFT_SPACES == "header1");
    }

    SECTION("ltrim", "ltrim all spaces")
    {
        STRING_UTILS.ltrim(ALL_SPACES);
        REQUIRE(ALL_SPACES == "header1  ");
    }
}

TEST_CASE ("trim", "trim")
{
    std::string RIGHT_SPACES = "header1  ";
    std::string LEFT_SPACES = "  header1";
    std::string ALL_SPACES = "  header1  ";

    SECTION("trim", "trim right spaces")
    {
        STRING_UTILS.trim(RIGHT_SPACES);
        REQUIRE(RIGHT_SPACES == "header1");
    }

    SECTION("trim", "trim left spaces")
    {
        STRING_UTILS.trim(LEFT_SPACES);
        REQUIRE(LEFT_SPACES == "header1");
    }

    SECTION("trim", "trim all spaces")
    {
        STRING_UTILS.trim(ALL_SPACES);
        REQUIRE(ALL_SPACES == "header1");
    }
}