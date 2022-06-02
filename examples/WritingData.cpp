#include <DataTable/DataTable.hpp>
#include <iostream>
#include <vector>

int main()
{
    /*
        Loading data sets from CSV files.
    */
    datatable::DataTable<double> bean("./data/dry_bean.csv", "Class");
    datatable::DataTable<float> iris("./data/iris.data", "class");
    datatable::DataTable<std::string> mpt("./data/mpt_data_tickers.csv", "");

    /*
        Write data from data tables to * delimited and tab delimited files.
    */
    bean.to_delimited("./output_data/dry_bean.data", '*');
    iris.to_delimited("./output_data/iris_tabs.data", '\t');
    mpt.to_delimited("./output_data/mpt_amps.amps", '&');

    /*
        Write data from data tables to comma delimited files.
    */
    bean.to_csv("./output_data/dry_bean_copy.csv");
    iris.to_csv("./output_data/iris_copy.data");
    mpt.to_csv("./output_data/mpt_copy.data");

    return 0;
}
