#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <iostream>
#include <string>
#include <DataTable/DataTable.hpp>

TEST_CASE ("Economic functions", "[eco funcs]")
{
    using T = float;
    datatable::DataTable<T> dt("x_to_x_squared.csv", "x2", true);
    SECTION("min", "[eco funcs]")
    {
        T min = dt["x"].min();
        REQUIRE(min == 0);

        min = dt.min(1);
        REQUIRE(min == 0);

        REQUIRE_THROWS_WITH(dt.min(), Catch::Contains("Column number is required for data tables with multiple columns."));
    }

    SECTION("max", "[eco funcs]")
    {
        T max = dt["x"].max();
        REQUIRE(max == 10);

        max = dt.max(1);
        REQUIRE(max == 100);

        REQUIRE_THROWS_WITH(dt.max(), Catch::Contains("Column number is required for data tables with multiple columns."));
    }

    SECTION("mean", "[eco funcs]")
    {
        T mean = dt["x"].mean();
        REQUIRE(mean == 5);

        mean = dt.mean(1);
        REQUIRE((int)mean == 33);

        REQUIRE_THROWS_WITH(dt.mean(), Catch::Contains("Column number is required for data tables with multiple columns."));
    }

    SECTION("sum", "[eco funcs]")
    {
        T s = dt["x"].sum();
        REQUIRE(s == 500);

        s = dt.sum(1);
        REQUIRE((int)s == 3350);

        REQUIRE_THROWS_WITH(dt.sum(), Catch::Contains("Column number is required for data tables with multiple columns."));
    }

    SECTION("sma", "[sma]")
    {

    }

    SECTION("ema", "[ema]")
    {

    }

    SECTION("rsi", "[rsi]")
    {

    }

    SECTION("pct_change", "[pct_change]")
    {

    }
}
