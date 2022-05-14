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
+ DT_SHAPE_TYPE to make the datatable::shape() method act more like the pandas.DataFrame.shape() method
    + output and indexing
+ Improved CSV writing by writing to an string stream and avoiding file write system calls until the end
    + mnist: ~29 seconds -> ~16 seconds
+ Improved Exception handling by throwing informative runtime errors rather than just outpuutting messages
+ selecting column by data by column dt["colname"] which returns a DataTable<T>
+ provides a free() method to clear vectors and maps and free memory
+ get_column_names() return a vector of column names which can be printed, `ostream << dt.get_column_names() << std::endl;`

### Behind the scenes
+ Improved unit testing
+ added optimization flag to CMakeLists
+ tried to adhere to const correctness
+ passed values by const/reference where applicable to avoid copies