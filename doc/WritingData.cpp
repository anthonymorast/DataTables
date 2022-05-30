#include <DataTable/DataTable.hpp>
#include <iostream>
#include <vector>

int main()
{
    /*
        Loading a data set with and without headers from CSV files.
    */
    datatable::DataTable<double> mnist("./data/mnist_train.csv", "", false);
    datatable::DataTable<float> iris("./data/iris.data", "class");

    /*
        Write data from data tables to * delimited and tab delimited files.
    */
    mnist.to_delimited("./output_data/mnist_stars.data", '*');
    iris.to_delimited("./output_data/iris_tabs.data", '\t');

    /*
        Write data from data tables to comma delimited files.
    */
    mnist.to_csv("./output_data/mnist_copy.csv");
    iris.to_csv("./output_data/iris_copy.data");

    return 0;
}
