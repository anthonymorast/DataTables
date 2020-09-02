#ifndef DATA_TABLE_DATA_TAB
#define DATA_TABLE_DATA_TAB

#include <DataTable/StringUtils.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <iostream>
#include <cstring>
#include <random>

#include <vector>
#include <iterator>

namespace datatable
{
    class DataTable
    {
        public:
            // constructors/destructors
            DataTable();
            DataTable(std::string csv_file_name, std::string response_column="", bool has_headers=true);
            DataTable(std::string* headers, std::string response_name, double** data, int nrows, int ncols, bool has_headers=true);
            DataTable(std::string* headers, int response_column, double** data, int nrows, int ncols, bool has_headers=true);
            ~DataTable();

            // file manip
            void from_csv(std::string filename, std::string response, bool has_headers=true);
            void to_file(std::string filename, char delimiter=',');

            // get data
            double** get_data();
            double* get_row(int row);
            double* get_column(int column);
            double* get_column(std::string column_name);
            double* get_response();
            double** get_all_explanatory();
            DataTable select_columns(int* column_numbers, int number_columns);
            DataTable select_columns(std::string* variables, int number_cols);
            DataTable select_rows(int* row_numbers, int number_rows);
            DataTable top_n_rows(int n);
            DataTable bottom_n_rows(int n);
            DataTable select_row_range(int start, int end);
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
            void drop_columns(int* columns, int count);
            void drop_columns(std::string* column_names, int count);
            void drop_rows(int* rows, int count);
            void shuffle_rows(int passes=100);

            // overridden operators
            double* operator[](int index) const;
            friend std::ostream& operator<<(std::ostream& os, const DataTable &table);

            // for other classes
            bool has_response();
            int nrows() { return _rows; }
            int ncols() { return _cols; }
            int* shape() { return new int[2] { _rows, _cols }; }
            int response_column_number() { return _response_column; }
        private:
            std::string* _headers = 0;
            double** _data = 0;
            int _cols = 0;
            int _rows = 0;
            std::string _response = "";
            int _response_column = 0;
            bool _data_loaded = false;
            bool _has_headers = false;
            StringUtils _str_utils;

            int get_column_from_header(std::string header);
    };
}

#endif
