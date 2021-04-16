# DataTables
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

```c++
// constructors/destructors
DataTable();
DataTable(std::string csv_file_name, std::string response_column="", bool has_headers=true);
DataTable(std::string* headers, std::string response_name, TType** data, int nrows, int ncols, bool has_headers=true);
DataTable(std::string* headers, int response_column, TType** data, int nrows, int ncols, bool has_headers=true);
DataTable(const DataTable<TType>&); // copy constructor
~DataTable();

// file manip
void from_csv(std::string filename, std::string response, bool has_headers=true);
void to_file(std::string filename, char delimiter=',');

// get data
TType** get_data();
TType* get_row(int row);
TType* get_column(int column);
TType* get_column(std::string column_name);
TType* get_response();
size_t* get_int_response();
TType** get_all_explanatory();
TType* get_flat_explanatory();
DataTable<TType> select_columns(int* column_numbers, int number_columns);
DataTable<TType> select_columns(std::string* variables, int number_cols);
DataTable<TType> select_rows(int* row_numbers, int number_rows);
DataTable<TType> top_n_rows(int n);
DataTable<TType> bottom_n_rows(int n);
DataTable<TType> select_row_range(int start, int end);
std::string get_header_at(int col);
std::string* get_headers();
std::string* get_explanatory_headers();
std::string get_response_column_name();

// visualization
void print(std::ostream& stream);
void print_column(std::ostream& stream, int column);
void print_column(std::ostream& stream, std::string column_name);
void print_row(std::ostream& stream, int row);
void print_headers(std::ostream& stream);
void print_shape(std::ostream& stream);

// table operations
void drop_columns(int* columns, int count);
void drop_columns(std::string* column_names, int count);
void drop_column(int column);
void drop_column(std::string column);
void drop_rows(int* rows, int count);
void shuffle_rows(int passes=100);

// data alteration 
TType* pct_change(std::string column);
TType* pct_change(int column);
TType* sma(std::string column, int periods);
TType* sma(int column, int periods);
TType* ema(std::string column, int periods);
TType* ema(int column, int periods);

// financial operations
TType* rsi(std::string column, int periods=14);
TType* rsi(int column, int periods=14);

// overridden operators
TType* operator[](int index) const;
template <typename U>
friend std::ostream& operator<<(std::ostream& os, const DataTable<U> &table);

// for other classes
bool has_response();
int nrows() { return _rows; }
int ncols() { return _cols; }
int* shape() { return new int[2] { _rows, _cols }; }
int response_column_number() { return _response_column; }
```
