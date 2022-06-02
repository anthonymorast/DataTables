#include <DataTable/DataTable.hpp>
#include <iostream>
#include <vector>

int main()
{
    datatable::DataTable<float> dt;  // empty data table
    dt.from_csv("./data/iris.data", "class");
    std::cout << dt << std::endl;       // print data from rows and columns of the datatable
    std::cout << dt.shape() << std::endl;   // print number of rows and columns
    std::cout << dt.get_column_names() << std::endl;    // print headers

    auto shape = dt.shape();
    std::cout << "# ROWS: " << shape[0] << std::endl;
    std::cout << "# COLS: " << shape[1] << std::endl;

    /*
        Alternatively, the code below will load the same Iris dataset.
    */
    datatable::DataTable<double> dt2("./data/iris.data", "");
    datatable::DataTable<float> dt3(
        dt.get_headers(), dt.get_response_column_name(), dt.get_data(),
        dt.shape()[0], dt.shape()[1]
    );
    datatable::DataTable<float> dt4(
        {"sepal_l","sepal_w","petal_l","petal_w","class"},      // headers (implicit vector conversin)
        "class",                                                // response
        dt.get_data(),                                          // data
        150,                                                    // rows
        5                                                       // cols
    );
    datatable::DataTable dt5 = dt4;

    /*
        Loading a data set without headers from a CSV file.
    */
    datatable::DataTable<double> mnist("./data/mnist_train.csv", "", false);

    return 0;
}
