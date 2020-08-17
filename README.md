# DataTables
Data tables for C++. The goal for these tables is to encompass all of the functionality of other data frame objects,
e.g. R and Pandas DataFrames, while remaining computationally and memory efficient.

# Dependencies 
+ CMAKE

# Installation
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
datatable::DataTable dt(<params>);
```

or 

```c
using namespace datatable;
...
DataTable dt(<params>);
```

# Constructors
There are a few different constructors available for DataTables. The function declarations are shown below (the parameter names are somewhat straightforward).

```c++
DataTable();
DataTable(std::string csv_file_name, std::string response_column="", bool has_headers=true);
DataTable(std::string* headers, std::string response_name, double** data, int nrows, int ncols, bool has_headers=true);
DataTable(std::string* headers, int response_column, double** data, int nrows, int ncols, bool has_headers=true);
```

Depending on how the DataTable is created some functionality may not be available. For example, if no response column name was specified it's not possible to get the response or explanatory columns since they will not be defined.

# Functionality
Below are the function declarations for the functionality that is currently implemented. In future releases I plan to use some documentation generating software, e.g. doxygen, to generate better documentation. For the time being the function names and parameters are pretty self-explanatory.

```c++
// load and dump data
void from_csv(std::string filename, std::string response, bool has_headers=true);
void to_file(std::string filename, char delimiter=',');

// get data
double** get_data();					
double* get_row(int row);		
double* get_column(int column);
double* get_column(std::string column_name);
double* get_response();
double** get_all_explanatory();
// START GET SPECIFIC ROWS AND COLUMNS
DataTable select_columns(int* column_numbers, int number_columns);
DataTable select_columns(std::string* variables, int number_cols);
DataTable select_rows(int* row_numbers, int number_rows);
DataTable top_n_rows(int n);
DataTable bottom_n_rows(int n);
DataTable select_row_range(int start, int end);
// END GET SPECIFIC ROWS AND COLUMNS
std::string get_header_at(int col);
std::string* get_headers();
std::string* get_explanatory_headers();

// visualization
void print(std::ostream& stream);
void print_column(std::ostream& stream, int column);
void print_column(std::ostream& stream, std::string column_name);
void print_row(std::ostream& stream, int row);
void print_headers(std::ostream& stream);
void print_shape(std::ostream& stream);   

// table operations
void drop_columns(int* columns, int count);   // removes columnes from the data set by index 
void drop_columns(std::string* column_names, int count);  // removes columns from the data set by name
void drop_rows(int* rows, int count); // removes rows from the data set
void shuffle_rows(int passes=100);    // shuffles the data set

// overridden operators
double* operator[](int index) const;  // get row of DataTable via dt[row_number]
friend std::ostream& operator<<(std::ostream& os, const DataTable &table);  // prints some information about the data table

// etc.
bool has_response();            // true if response has been specified and is available in data set
int nrows() { return _rows; }   // return number of rows
int ncols() { return _cols; }   // return number of columns 
int* shape() { return new int[2] { _rows, _cols }; }    // numpy.shape
int response_column_number() { return _response_column; } // column number for response column
```
