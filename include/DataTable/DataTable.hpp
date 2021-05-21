#ifndef DATA_TABLE_DATA_TAB
#define DATA_TABLE_DATA_TAB

#include <DataTable/StringUtils.hpp>
#include <DataTable/DateUtils.hpp>

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
    template <typename TType> 
    class DataTable
    {
        public:
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
        private:
            std::string* _headers = 0;
            TType** _data = 0;
            int _cols = 0;
            int _rows = 0;
            std::string _response = "";
            int _response_column = 0;
            bool _data_loaded = false;
            bool _has_headers = false;
            std::vector<int> _datetime_columns;
            StringUtils _str_utils;
            DateUtils _date_utils;
            // date formats for the datetime columns
            std::map<int, std::string> _date_columns_to_formats;

            int get_column_from_header(std::string header);
    };

    template <typename TType> TType* DataTable<TType>::operator[](int row) const
    {
        if(row >= _rows || row < 0)
        {
            std::cout << "ERROR: row index out of range; row " << row << " out of " << (_rows+1) << std::endl;
            std::cout << "NOTE: row indexing is 0-based." << std::endl;
            return 0;
        }
        return _data[row];
    }

    template <typename TType> std::ostream& operator<<(std::ostream& os, const DataTable<TType>& table)
    {
        if(!table._data_loaded)
        {
            os << "ERROR: printing empty data table." << std::endl;
            return os;
        }

        char delimiter = ',';
        if(table._has_headers)
        {
            for(int i = 0; i < table._cols; i++)
            {
                os << table._headers[i];
                if(i < table._cols-1)
                    os << delimiter;
            }
            os << std::endl;
        }

        for(int i = 0; i < table._rows; i++)
        {
            for(int j = 0; j < table._cols; j++)
            {
                if(table._date_columns_to_formats.find(j) != table._date_columns_to_formats.end())
                    os << table._date_utils.getStringFromTime((time_t)table._data[i][j], table._date_columns_to_formats.at(j));
                else 
                    os << table._data[i][j];

                if(j < table._cols-1)
                    os << delimiter;
            }
            os << std::endl;
        }

        return os;
    }
}

#endif
