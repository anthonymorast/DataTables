#include <DataTable/DataTable.hpp>
#include <iostream>
#include <vector>

int main()
{
    // load the dry bean dataset from the UCI machine learning repo.
    datatable::DataTable<double> dry_bean("./data/dry_bean.csv", "Class");

    /*
        NOTE: most, if not all, of the methods below return another data table object.
            It is equally legitimate to create another data table from these operations.
            I am just outputting them here since that is the purpose of this example
            file.
    */
    std::cout << dry_bean.head();                       // print first 10 rows
    std::cout << dry_bean.get_column_names();           // print headers
    std::cout << dry_bean.top_n_rows(2);                // print first 2 rows
    std::cout << dry_bean.select_row_range(10, 11);     // print row 10
    std::cout << dry_bean.tail();                       // print bottom 10 rows
    // print columns class and area
    std::vector<std::string> columns{"Class", "Area"};
    std::cout << dry_bean.select_columns(columns);
    std::cout << dry_bean.shape() << std::endl;         // print shape of the data set
    // print shape of the dataset in another way
    std::cout << "ROWS: " << dry_bean.shape()[0] << "\tCOLS: " << dry_bean.shape()[1] << std::endl;
    std::cout << dry_bean.max(0) << std::endl;          // print max value of "Area" column
    std::cout << dry_bean["Area"].max() << std::endl;   // print max value of "Area" column
    std::cout << dry_bean["Area"].min() << std::endl;   // print min value of "Area" column
    std::cout << dry_bean["Compactness"].sum() << std::endl; // print sum of the "Compactness" column
    std::cout << dry_bean.head()["Compactness"] << std::endl;// print first 10 rows for "Compactness"
    std::cout << dry_bean.head()["MajorAxisLength"].max()
              << std::endl;                                  // print max of first 10 "MajorAxisLength" rows.
    std::cout << dry_bean.head()["ShapeFactor1"].sum()
              << std::endl;                                  // print sum of first 10 "ShapeFactor1" rows.

    /*
        As previously mentioned, the methods above almost all return data table objects.
        This means the methods could be mixed and matched and a variety of ways by
        chaining the operations to provide a lot of details about the data table.
    */

    return 0;
}
