# v2.0 TODO

+ use smart pointers where necessary
+ implement actual runtime and compile time errors rather than printing messages
    + static_assert and DataTableExctpion
+ add general efficiency improvements
+ check issues in GitHub for other suggestions
+ set and update SHAPE_TYPE variable when _nrows or _ncols changes
    + this should get rid of the print_shape function

## Added Improvements

### User-facing
+ added late in v1.0:
    + support for SOME datetime formats.
        + date/times in these formats will be properly loaded, displayed, and written
+ DT_SHAPE_TYPE to make the datatable::shape() method act more like the pandas.DataFrame.shape() method
    + output and indexing
+ Improved CSV writing by writing to an string stream and avoiding file write system calls until the end
    + mnist: ~29 seconds -> ~16 seconds
+ Improved Exception handling by throwing informative runtime errors rather than just outpuutting messages
+ selecting column by data by column dt["colname"] which returns a DataTable<T>
+ provides a free() method to clear vectors and maps and free memory
+ get_column_names() return a vector of column names which can be printed, `ostream << dt.get_column_names() << std::endl;`
+ removed the `DataTable(vector<string> headers, int response_col, T** data, int nrows, int ncols, bool has_headers)` constructor
    + was this likely to be used?
+ added two new methods for viewing the data, `head()` and `tail()` which return the top and bottom 10 rows in a data table, respectively
+ removed the `print_row(std::ostream& stream, int row)` function in favor of `std::cout << dt.select_row_range(start, start) << std::endl;`
+ replaced `DataTable<TType> select_columns(std::string* coulumn_names, int number_cols) const` with `DataTable<TType> select_columns(std::vector<std::string> coulumn_names) const`
+ removed `void print_column(std::ostream& stream, int column);` and `void print_column(std::ostream& stream, std::string column_name);` in favor of `std::ostream << dt.select_columns(col_numbers);` or `std::ostream << dt.select_columns(col_names);`, respectively.
+ added from/to_delimited to specifier the delimiter in the file to be loaded. In the previous release only comma separated files were supported.
+ changed the method names `get_response()` and `get_int_response()` to `response()` and `int_response()`, respectively.
+ added `min()`, `max()`, `mean()`, and `sum()` functions which take either a column number for multi-column data tables or no parameter for single column data tables.


### Behind the scenes
+ Improved unit testing
+ added optimization flag to CMakeLists
+ tried to adhere to const correctness rules
+ passed values by const/reference where applicable to avoid copies
+ began added unit tests 
