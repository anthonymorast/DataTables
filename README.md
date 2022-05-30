# DataTables

[![Build and Test](https://github.com/anthonymorast/DataTables/actions/workflows/cmake.yml/badge.svg)](https://github.com/anthonymorast/DataTables/actions/workflows/cmake.yml)

Data tables for C++. The goal for these tables is to encompass all of the functionality of other data frame objects,
e.g. R and Pandas DataFrames, while remaining computationally and memory efficient.

# Dependencies
+ CMAKE

# Installation
After downloading/cloning the source and navigating to the root folder of the source code, the project can be installed via CMake as is done below.

```
mkdir build
cd build
cmake ..
```
## Linux
```
make && sudo make install
```
## Windows
CMAKE should create a Visual Studio project file to use the project in VS. This project was only tested in a Linux environment and I have little to no experience using CMake in Windows.

# Usage
After installation the DataTable structure can be imported into other C++ projects. The project must be linked in *g++* via the *-l* flag, i.e. *g++ <project_file>.cc -lDataTable*. Some times there will be errors when linking projects installed via CMake. A quick Google search reveals this is caused by ldconfig not being run after installation, running the command *ldconfig* in the Linux terminal should fix the issue assuming the project was installed correctly.

Once installed, the DataTable stucture can be imported via

```c
#include <DataTable/DataTable.hpp>
```

The DataTable object lives in the *datatable* namespace so a DataTable can be created via

```c
datatable::DataTable<type> dt(<params>);
```

or

```c
using namespace datatable;
...
DataTable<type> dt(<params>);
```

# Constructors
There are a few different constructors available for DataTables. The function declarations are shown below (the parameter names are somewhat straightforward).

```c++
DataTable<type>();
DataTable<type>(std::string csv_file_name, std::string response_column="", bool has_headers=true);
DataTable<type>(std::string* headers, std::string response_name, double** data, int nrows, int ncols, bool has_headers=true);
DataTable<type>(std::string* headers, int response_column, double** data, int nrows, int ncols, bool has_headers=true);
```

Depending on how the DataTable is created some functionality may not be available. For example, if no response column name was specified it's not possible to get the response or explanatory columns since they will not be defined.

# Functionality
Below are the function declarations for the functionality that is currently implemented. In future releases I plan to use some documentation generating software, e.g. doxygen, to generate better documentation. For the time being the function names and parameters are pretty self-explanatory.

Note that TType is the template type of the class. For example, if you wanted a data table that stores integers the instantiation could look like this:

```c++
datatable::DataTable<int> dt;
```

If you're table needs to store floats the instantiation with the blank constructor could be:

```c++
datatable::DataTable<float> dt;
```

and so on for any data type. **NOTE** that the DataTable class has only been tested with numeric data types.

As of version 2.0, the shape of a data table can easily be determined by sending the DT_SHAPE_TYPE to an ostream. For example,

```c++
datatable::DataTable<double> dt(...);
auto shape = dt.shape();
std::cout << shape << std::endl;
// or
std::cout << dt.shape() << std::endl;
```

the [] operator is also overloaded for this class

```c++
datatable::DataTable<double> dt(...);
auto shape = dt.shape();
std::cout << shape[0] << ", " << shape[1] << std::endl; // outputs # rows and # cols
```


```c++
DataTable() {}
DataTable(
    const std::string& csv_file_name,
    std::string response_column="",
    bool has_headers=true);
DataTable(
    std::vector<std::string> headers,
    std::string response_name,
    T** data,
    int nrows,
    int ncols,
    bool has_headers=true);
DataTable(const DataTable<T>&); // copy constructor
~DataTable();
void free();

// file manip
void from_csv(
    const std::string& filename,
    const std::string& response,
    bool has_headers=true);
void from_delimited(
  const std::string& filename,
  const std::string& response,
  char delimiter,
  bool has_headers=true
);
void to_csv(const std::string& filename) const;
void to_delimited(const std::string &filename, char delimiter) const;

// select data
T** get_data() const { return _data; }
T* get_row(int row) const;
T* get_column(int column) const;
T* get_column(std::string column_name) const;
T* response() const;
size_t* int_response() const;
T** get_all_explanatory() const;
T* get_flat_explanatory() const;
DataTable<T> select_columns(std::vector<int> column_numbers) const;
DataTable<T> select_columns(std::vector<std::string> column_names) const;
DataTable<T> select_rows(std::vector<int> row_numbers) const;
DataTable<T> select_row_range(int start, int end) const;
DataTable<T> top_n_rows(int n) const { return select_row_range(0, n); }
DataTable<T> bottom_n_rows(int n) const { return select_row_range(_datatable_shape[0] - n, _datatable_shape[0]); }
DataTable<T> head() const { return top_n_rows(10); }
DataTable<T> tail() const { return bottom_n_rows(10); }
std::string get_header_at(int col) const;
const std::vector<std::string>& get_headers() const { return _headers; };
const std::vector<std::string>& get_explanatory_headers() const;
ColumnNames get_column_names() const
{
    ColumnNames names(_headers);
    return names;
};
std::string get_response_column_name() const { return _response; };

// table operations
void drop_columns(std::vector<int>& columns);
void drop_columns(std::vector<std::string>& column_names);
void drop_column(int column);
void drop_column(const std::string& column);
void shuffle_rows(int passes=100);

// data analysis
T* pct_change(std::string column) const;
T* pct_change(int column) const;
T* sma(std::string column, int periods) const;
T* sma(int column, int periods) const;
T* ema(std::string column, int periods) const;
T* ema(int column, int periods) const;
T* rsi(std::string column, int periods=14) const;
T* rsi(int column, int periods=14) const;
T min(int col) const;
T max(int col) const;
T min() const;
T max() const;
T sum(int col) const;
T sum() const;
T mean(int col) const;
T mean() const;

// overridden operators
T* operator[](int index) const;     // select row
DataTable<T> operator[](std::string column) const;  // select column into DT
template <typename U>
friend std::ostream& operator<<(std::ostream& os, const DataTable<U> &table);

// for other classes
bool has_response() const { return _response != ""; }
int nrows() const { return _datatable_shape[0]; }
int ncols() const { return _datatable_shape[1]; }
DT_SHAPE_TYPE shape() const { return _datatable_shape; }
int response_column_number() const { return _response_column; }
```
