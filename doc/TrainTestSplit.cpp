#include <DataTable/DataTable.hpp>
#include <iostream>
#include <random>

int main()
{
    datatable::DataTable<double> beans("./data/dry_bean.csv", "Class");

    // Train/test split
    long train_size = 0.80 * beans.nrows();  // 80% of rows in training data
    long test_size = beans.nrows() - train_size;
    std::cout << train_size << "\t+\t" << test_size << "\t=\t" << beans.nrows() << std::endl;
    beans.shuffle_rows(500);                   // shuffle with 500 passes over data
    auto beans_train = beans.select_row_range(0, train_size);
    auto beans_test = beans.select_row_range(train_size, beans.nrows());

    // get DataTables of the train and test x and y variables, this could be done before the row selects too
    std::cout << beans_train.shape() << " " << beans_test.shape() << std::endl;
    auto beans_train_y = beans_train.select_columns({beans_train.get_response_column_name()});
    beans_train.drop_column(beans_train.get_response_column_name()); // explanatory variables

    auto beans_test_y = beans_test.select_columns({beans_test.get_response_column_name()});
    beans_test.drop_column(beans_test.get_response_column_name());  // explanatory variables

    std::cout << beans_test_y.shape() << " " << beans_test.shape() << std::endl;
    std::cout << beans_train_y.shape() << " " << beans_train.shape() << std::endl;

    return 0;
}
