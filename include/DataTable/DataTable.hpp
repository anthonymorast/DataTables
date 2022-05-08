#ifndef DATA_TABLE_DATA_TAB
#define DATA_TABLE_DATA_TAB

#include <DataTable/StringUtils.hpp>
#include <DataTable/DateUtils.hpp>
#include <DataTable/DataTableException.hpp>
#include <DataTable/ShapeType.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <iostream>
#include <cstring>
#include <random>
#include <vector>
#include <iterator>
#include <map>

namespace datatable
{
    template <typename T> 
    class DataTable
    {
        public:
            // constructors/destructors
            DataTable() {}
            DataTable(
                const std::string& csv_file_name, 
                std::string response_column="", 
                bool has_headers=true);
            DataTable(
                const std::string* headers, 
                const std::string& response_name, 
                const T** data, 
                int nrows, 
                int ncols, 
                bool has_headers=true);
            DataTable(
                const std::string* headers, 
                int response_column, 
                const T** data, 
                int nrows, 
                int ncols, 
                bool has_headers=true);
            DataTable(const DataTable<T>&); // copy constructor
            ~DataTable();

            // file manip
            void from_csv(
                const std::string& filename, 
                const std::string& response, 
                bool has_headers=true);
            void to_csv(const std::string& filename, char delimiter=',') const;

            // // get data
            T** get_data() const { return _data; }
            // TType* get_row(int row);
            // TType* get_column(int column);
            // TType* get_column(std::string column_name);
            // TType* get_response();
            // size_t* get_int_response();
            // TType** get_all_explanatory();
            // TType* get_flat_explanatory();
            // DataTable<TType> select_columns(int* column_numbers, int number_columns);
            // DataTable<TType> select_columns(std::string* variables, int number_cols);
            // DataTable<TType> select_rows(int* row_numbers, int number_rows);
            // DataTable<TType> top_n_rows(int n);
            // DataTable<TType> bottom_n_rows(int n);
            // DataTable<TType> select_row_range(int start, int end);
            // std::string get_header_at(int col);
            // std::string* get_headers();
            // std::string* get_explanatory_headers();
            // std::string get_response_column_name();

            // // visualization
            // void print(std::ostream& stream);
            // void print_column(std::ostream& stream, int column);
            // void print_column(std::ostream& stream, std::string column_name);
            // void print_row(std::ostream& stream, int row);
            // void print_headers(std::ostream& stream);

            // // table operations
            // void drop_columns(int* columns, int count);
            // void drop_columns(std::string* column_names, int count);
            // void drop_column(int column);
            // void drop_column(std::string column);
            // void drop_rows(int* rows, int count);
            // void shuffle_rows(int passes=100);

            // // data alteration 
            // TType* pct_change(std::string column);
            // TType* pct_change(int column);
            // TType* sma(std::string column, int periods);
            // TType* sma(int column, int periods);
            // TType* ema(std::string column, int periods);
            // TType* ema(int column, int periods);

            // // financial operations
            // TType* rsi(std::string column, int periods=14);
            // TType* rsi(int column, int periods=14);

            // // overridden operators
            T* operator[](int index) const;     // select row
            DataTable<T> operator[](std::string column) const;  // select column into DT
            // template <typename U>
            // friend std::ostream& operator<<(std::ostream& os, const DataTable<U> &table);

            // // for other classes
            bool has_response() { return std::strcmp(_response.c_str(), ""); }
            int nrows() { return _datatable_shape[0]; }
            int ncols() { return _datatable_shape[1]; }
            DT_SHAPE_TYPE shape() { return _datatable_shape; }
            int response_column_number() { return _response_column; }

        private:
            std::vector<std::string> _headers;
            T** _data {0};
            std::string _response {""};
            int _response_column {0};
            bool _data_loaded {false};
            bool _has_headers {false};
            bool _allocated_data {false};
            std::vector<int> _datetime_columns;
            StringUtils _str_utils;
            DateUtils _date_utils;
            DT_SHAPE_TYPE _datatable_shape;

            // date formats for the datetime columns
            std::map<int, std::string> _date_columns_to_formats;
            int _get_column_from_header(std::string header);
            T* _process_row(std::string row);
    };
}

#include "DataTable.tpp"

#endif
