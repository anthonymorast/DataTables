#ifndef DATA_TABLE_DATA_TAB
#define DATA_TABLE_DATA_TAB

#include <DataTable/StringUtils.hpp>
#include <DataTable/DateUtils.hpp>
#include <DataTable/DataTableException.hpp>
#include <DataTable/ShapeType.hpp>
#include <DataTable/ColumnNames.hpp>

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
#include <functional>
#include <limits>
#include <type_traits>

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
            int _get_column_from_header(std::string header, bool should_error = true) const;
    };
}

#include "DataTable.tpp"
#include "DataTableEco.tpp"

#endif
