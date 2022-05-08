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
+ Improved CSV writing by writing to an string stream 

### Behind the scenes
+ Improved unit testing
+ added optimization flag to CMakeLists
+ tried to adhere to const correctness