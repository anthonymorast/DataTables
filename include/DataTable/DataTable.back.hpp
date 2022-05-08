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

    // TODO: need to seriously test the DataTable class, there is a lot of stuff going on...
    template <typename TType> DataTable<TType>::DataTable(){}

    template <typename TType> DataTable<TType>::DataTable(std::string csv_filename, std::string response_name, bool has_headers)
    {
        _str_utils.trim(response_name);
        from_csv(csv_filename, response_name, has_headers);	// set data, ncols, nrows, response vars, and headers in here
        _has_headers = has_headers;
    }

    template <typename TType> DataTable<TType>::DataTable(std::string* headers, std::string response_name, TType** data, int nrows, int ncols, bool has_headers)
    {
        // TODO: should consider deep-copying the pointers in the constructors in case the user frees
        // memory we're using
        _str_utils.trim_all(headers, ncols);
        _cols = ncols;
        _rows = nrows;
        _data = data;
        _headers = headers;
        _response = response_name;
        if(has_response())
            _response_column = get_column_from_header(response_name);   // a blank ("") response can be used causing issues 
        _data_loaded = true;
        _has_headers = has_headers;
    }

    template <typename TType> DataTable<TType>::DataTable(const DataTable<TType>& old_table)
    {
        _cols = old_table._cols;
        _rows = old_table._rows;
        _headers = new std::string[_cols];
        for(int i = 0; i < _cols; i++)
        {
            _headers[i] = old_table._headers[i];
        }
        _data = new TType*[_rows];
        for(int i = 0; i < _rows; i++)
        {
            _data[i] = new TType[_cols];
            memcpy(_data[i], old_table._data[i], _cols * sizeof(TType));
        }
        _response = old_table._response;
        _response_column = old_table._response_column;
        _data_loaded = old_table._data_loaded;
        _has_headers = old_table._has_headers;
    }

    /*! DataTable<TType>::DataTable(string*, int, double**, int, int, bool)
     *  When the response column number is passed into this constructor, it's assumed the user will use
     *  0-based indexing. This is due to the fact that 0-based indexing is used in other get/print methods.
     */
    template <typename TType> DataTable<TType>::DataTable(std::string* headers, int response_column, TType** data, int nrows, int ncols, bool has_headers)
    {
        _str_utils.trim_all(headers, ncols);
        _headers = headers;
        _response_column = response_column;
        _response = headers[_response_column];
        _data = data;
        _rows = nrows;
        _cols = ncols;
        _has_headers = has_headers;
        _data_loaded = true;
    }

    template <typename TType> DataTable<TType>::~DataTable()
    {
        // TODO: ~DataTable(): should probably still free these... will need to pass back new arrays
        // from the getters
        //	delete[] _data;
    }

    template <typename TType> bool DataTable<TType>::has_response()
    {
        return std::strcmp(_response.c_str(), "");
    }

    template <typename TType> std::string DataTable<TType>::get_response_column_name()
    {
        if(has_response())
        {
            return _response;
        }
        return "";
    }

    template <typename TType> int DataTable<TType>::get_column_from_header(std::string header)
    {
        int column = _cols + 1;
        for(int i = 0; i < _cols; i++)
        {
            if(_headers[i] == header)
            {
                column = i;
                break;
            }
        }

        if(column == (_cols + 1))
        {
            std::cout << "ERROR: Header '" << header << "' not found." << std::endl;
            return -1;
        }
        return column;
    }

    template <typename TType> void DataTable<TType>::from_csv(std::string filename, std::string response, bool has_headers)
    {
        if(_data_loaded)
        {
            std::cout << "ERROR: Data has already been loaded for this table." << std::endl;
            return;
        }

        std::ifstream data_file(filename);
        if(!data_file.is_open())
        {
            std::cout << "ERROR: Unable to open file '" << filename << "', no data has been loaded." << std::endl;
            return;
        }

        std::vector<std::string> lines;
        std::string line;
        while(std::getline(data_file, line)) 
            lines.push_back(line);

        /* TREATS blank spaces as newlines

        std::copy(std::istream_iterator<std::string>(data_file),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(lines)); 
        */

        // read first line, regardless of headers, to get valuable data (ncols)
        line = lines.front();
        std::istringstream ss(line);
        std::string value;
        _cols = 0;
        while(std::getline(ss, value, ','))
        {
            _cols++;
        }

        if(has_headers)		// read the header line if there is one
        {
            _headers = new std::string[_cols];
            std::istringstream ss_headers(line);
            bool response_found = false;
            int count = 0;
            while(std::getline(ss_headers, value, ','))
            {
                _str_utils.trim(value);
                if(!response_found && !std::strcmp(value.c_str(), response.c_str()))
                {
                    response_found = true;
                    _response = value;
                    _response_column = count;
                }
                _headers[count] = value;
                count++;
            }
            if(!response_found)
            {
                std::cout << "ERROR: response variable not found." << std::endl;
                return;
            }
        }

        if(has_headers)
        {
            lines.erase(lines.begin());     // skip headers if need be
        }

        // process the remainder of the data file; first need to get rowcount to allocate memory
        _rows = lines.size();
        _data = new TType*[_rows];
        for(int i = 0; i < _rows; i++)
            _data[i] = new TType[_cols];

        int row_count = 0;
        for(auto it = lines.begin(); it != lines.end(); it++)
        {
            int col_count = 0;
            std::istringstream ss2((*it));
            while(std::getline(ss2, value, ','))
            {
                if(_date_utils.isDateTime(value))
                {
                    std::string format = _date_utils.getFormat(value);
                    _data[row_count][col_count] = (TType)_date_utils.getTimeFromString(value, format);
                    if(_date_columns_to_formats.find(col_count) == _date_columns_to_formats.end())
                        _date_columns_to_formats.insert(std::pair<int, std::string>(col_count, format));
                }
                else
                {
                    try
                    {
                        _data[row_count][col_count] = std::stod(value);
                    }
                    catch(std::exception &e)
                    {
                        std::cout << "ERROR: value '" << value << "' is not parsable as numeric." << std::endl;
                        return;
                    }
                }
                col_count++;
            }
            if(col_count != _cols)
            {
                std::cout << "ERROR: too many columns in row " << (row_count+1) << "." << std::endl;
            }
            row_count++;
        }

        data_file.close();
        _data_loaded = true;
    }

    template <typename TType> void DataTable<TType>::to_file(std::string filename, char delimiter)
    {
        std::ofstream out(filename);
        if(!out.is_open())
        {
            std::cout << "ERROR: could not open file '" << filename << "'." << std::endl;
            return;
        }

        if(!_data_loaded)
        {
            std::cout << "ERROR: writing empty data table." << std::endl;
            out.close();
            return;
        }

        if(_has_headers)
        {
            for(int i = 0; i < _cols; i++)
            {
                out << _headers[i];
                if(i < _cols-1)
                    out << delimiter;
            }
            out << std::endl;
        }

        for(int i = (_has_headers ? 1 : 0); i < _rows; i++)
        {
            for(int j = 0; j < _cols; j++)
            {
                out << _data[i][j];
                if(j < _cols-1)
                    out << delimiter;
            }
            out << std::endl;
        }
    }

    template <typename TType> TType** DataTable<TType>::get_data() { return _data; }
    template <typename TType> TType* DataTable<TType>::get_row(int row)
    {
        if(row >= _rows || row < 0)
        {
            std::cout << "ERROR: row index out of range; row " << row << " out of " << (_rows+1) << std::endl;
            std::cout << "NOTE: row indexing is 0-based." << std::endl;
            return 0;
        }
        return _data[row];
    }

    template <typename TType> TType* DataTable<TType>::get_column(int column)
    {
        if(column >= _cols || column < 0)
        {
            std::cout << "ERROR: column index out of range; column " << column << " out of " << (_cols+1) << std::endl;
            std::cout << "NOTE: column indexing is 0-based." << std::endl;
            return 0;
        }

        TType* col_data = new TType[_rows];
        for(int i = 0; i < _rows; i++)
        {
            col_data[i] = _data[i][column];
        }
        return col_data;
    }

    template <typename TType> TType* DataTable<TType>::get_column(std::string column_name)
    {
        int col = get_column_from_header(column_name);
        return (col < 0 ? 0 : get_column(col));
    }

    template <typename TType> TType* DataTable<TType>::get_response()
    {
        if(!has_response())
        {
            std::cout << "ERROR: response variable not set." << std::endl;
            return 0;
        }
        int col = get_column_from_header(_response);
        return (col < 0 ? 0 : get_column(_response));
    }

    /*
        Becomes necessary when the response column contains typenamees (represented by integers)
        rather than doubles. Libraries like mlpack only accept integers for methods
        such as logistic regression.
    */
    template <typename TType> size_t* DataTable<TType>::get_int_response()
    {
        size_t* data = new size_t[_rows];
        for(int i = 0; i < _rows; i++)
        {
            data[i] = (size_t)_data[i];
        }
        return data;
    }

    template <typename TType> TType** DataTable<TType>::get_all_explanatory()
    {
        TType** data = new TType*[_rows];
        for(int i = 0; i < _rows; i++)
            data[i] = new TType[_cols-1];

        for(int i = 0; i < _rows; i++)
        {
            for(int j = 0; j < _cols; j++)
            {
                // TODO: consider using the trick where the response is moved to the first column (or last)
                if(j < _response_column)
                    data[i][j] = _data[i][j];
                else if(j > _response_column)	// use previous index after response column
                    data[i][j-1] = _data[i][j];
            }
        }
        return data;
    }

    template <typename TType> TType* DataTable<TType>::get_flat_explanatory()
    {
        TType* data = new TType[_rows * _cols];
        for(int i = 0; i < _rows; i++)
        {
            for(int j = 0; j < _cols; j++)
            {
                 data[(i*_cols) + j] = _data[i][j];
            }
        }
        return data;
    }

    template <typename TType> DataTable<TType> DataTable<TType>::select_columns(int* column_numbers, int number_columns)
    {
        std::string* headers = new std::string[number_columns];
        TType** data = new TType*[_rows];
        for(int i = 0; i < _rows; i++)
            data[i] = new TType[number_columns];

        int col_count = -1;
        bool keep_response = false;
        for(int k = 0; k < number_columns; k++) 
        {
            int col = column_numbers[k];
            if(col == _response_column)
                keep_response = true;
           
            headers[k] = _headers[col];
            for(int i = 0; i < _rows; i++)
            {
                data[i][k] = _data[i][col];
            }
        }

        DataTable<TType> new_data(headers, (keep_response ? _response : ""), data, _rows, number_columns);
        return new_data;
    }

    template <typename TType> DataTable<TType> DataTable<TType>::select_columns(std::string* variables, int number_cols)
    {
        if(!_has_headers)
        {
            std::cout << "ERROR: attempting to select columns by header values on headerless data table." << std::endl;
            DataTable<TType> e;
            return e;
        }

        int* column_numbers = new int[number_cols];
        for(int i = 0; i < number_cols; i++)
        {
            column_numbers[i] = get_column_from_header(variables[i]);
        }

        return select_columns(column_numbers, number_cols);
    }

    template <typename TType> DataTable<TType> DataTable<TType>::select_rows(int* row_numbers, int number_rows)
    {
        TType** data = new TType*[number_rows];
        for(int i = 0; i < number_rows; i++)
            data[i] = new TType[_cols];

        for(int k = 0; k < number_rows; k++)
        {
            memcpy(data[k], _data[row_numbers[k]], _cols * sizeof(TType));
        }

        DataTable<TType> new_dt(_headers, _response, data, number_rows, _cols);
        return new_dt;
    }

    template <typename TType> DataTable<TType> DataTable<TType>::top_n_rows(int n)
    {
        return select_row_range(0, n);
    }

    template <typename TType> DataTable<TType> DataTable<TType>::bottom_n_rows(int n)
    {
        return select_row_range(_rows - n, _rows);
    }

    template <typename TType> DataTable<TType> DataTable<TType>::select_row_range(int start, int end)
    {
        if(end < start)
        {
            std::cout << "ERROR (DataTable): row range end is before range start." << std::endl;
            return DataTable();
        }

        int size = end - start;
        int* rows = new int[size];
        for(int i = 0; i < size; i++)
        {
            rows[i] = (i + start);
        }

        DataTable<TType> return_table = select_rows(rows, size);
        delete[] rows;
        return return_table;
    }

    template <typename TType> void DataTable<TType>::print(std::ostream& stream)
    {
        if(!_data_loaded)
        {
            std::cout << "ERROR: printing empty data table." << std::endl;
            return;
        }

        char delimiter = ',';
        if(_has_headers)
        {
            for(int i = 0; i < _cols; i++)
            {
                stream << _headers[i];
                if(i < _cols-1)
                    stream << delimiter;
            }
            stream << std::endl;
        }

        for(int i = 0; i < _rows; i++)
        {
            for(int j = 0; j < _cols; j++)
            {
                if(_date_columns_to_formats.find(j) != _date_columns_to_formats.end())
                    stream << _date_utils.getStringFromTime((time_t)_data[i][j], _date_columns_to_formats[j]);
                else 
                    stream << _data[i][j];

                if(j < _cols-1)
                    stream << delimiter;
            }
            stream << std::endl;
        }
    }

    template <typename TType> void DataTable<TType>::print_shape(std::ostream& stream)
    {
        int* s = shape();
        stream << "(" << s[0] << ", " << s[1] << ")" << std::endl;
    }

    template <typename TType> void DataTable<TType>::print_column(std::ostream& stream, int column)
    {
        if(column >= _cols || column < 0)
        {
            std::cout << "ERROR: column index out of range; column " << column << " out of " << (_cols+1) << std::endl;
            std::cout << "NOTE: column indexing is 0-based." << std::endl;
            return;
        }


        for(int i = 0; i < _rows; i++)
        {
            if(_date_columns_to_formats.find(column) != _date_columns_to_formats.end())
                stream << _date_utils.getStringFromTime((time_t)_data[i][column], _date_columns_to_formats[column]) << std::endl;
            else
                stream << _data[i][column] << std::endl;
        }
    }

    template <typename TType> void DataTable<TType>::print_column(std::ostream& stream, std::string column)
    {
        int	col = get_column_from_header(column);
        if(col < 0)
            return;
        print_column(stream, col);
    }

    template <typename TType> void DataTable<TType>::print_row(std::ostream& stream, int row)
    {
        if(row >= _rows || row < 0)
        {
            std::cout << "ERROR: row index out of range; row " << row << " out of " << (_rows+1) << std::endl;
            std::cout << "NOTE: row indexing is 0-based." << std::endl;
            return;
        }

        for(int i = 0; i < _cols; i++)
        {
            if(_date_columns_to_formats.find(i) != _date_columns_to_formats.end())
                    stream << _date_utils.getStringFromTime((time_t)_data[row][i], _date_columns_to_formats[i]) << ", ";
            else 
                stream << _data[row][i] << ", ";
        }
        stream << std::endl;
    }

    template <typename TType> void DataTable<TType>::print_headers(std::ostream& stream)
    {
        for(int i = 0; i < _cols; i++)
            stream << _headers[i] << (i < _cols-1 ? ", " : "");
        stream << std::endl;
    }

    template <typename TType> void DataTable<TType>::drop_column(int column)
    {
        drop_columns(new int[1]{column}, 1);
    }

    template <typename TType> void DataTable<TType>::drop_column(std::string column)
    {
        drop_columns(new std::string[1]{column}, 1);
    }

    template <typename TType> void DataTable<TType>::drop_columns(int* columns, int count)
    {
        std::map<int, std::string> new_date_columns_to_formats;
        std::string* new_headers = new std::string[_cols-count];
        TType** new_data = new TType*[_rows];
        for(int i = 0; i < _rows; i++)
            new_data[i] = new TType[_cols - count];

        int new_col_count = 0;
        for(int i = 0; i < _cols; i++)
        {
            auto it = _date_columns_to_formats.find(i);
            bool found = false;
            for(int k = 0; k < count; k++)
            {
                if(columns[k] == i)
                {
                    found = true;
                    break;
                }
            }

            if(!found)      // if the column is not in the drop cols, copy it
            {
                for(int j = 0; j < _rows; j++)
                {
                    new_data[j][new_col_count] = _data[j][i];
                }
                if(_has_headers)
                    new_headers[new_col_count] = _headers[i];
                
                if(it != _date_columns_to_formats.end())
                    new_date_columns_to_formats.insert(std::pair<int, std::string>(new_col_count, it->second));

                new_col_count++;
            }
        }

        delete[] _data;
        if(_has_headers)
        {
            _headers = new_headers;
            // in case this changed
            _response_column = get_column_from_header(_response);
        }
        _data = new_data;
       _cols -= count;
       _date_columns_to_formats = new_date_columns_to_formats;
    }

    template <typename TType> void DataTable<TType>::drop_columns(std::string* column_names, int count)
    {
        int* columns = new int[count];
        for(int i = 0; i < count; i++)
        {
            columns[i] = get_column_from_header(column_names[i]);
        }

        drop_columns(columns, count);
    }

    template <typename TType> void DataTable<TType>::drop_rows(int* rows, int count)
    {
        TType** new_data = new TType*[_rows - count];
        for(int i = 0; i < (_rows-count); i++)
            new_data[i] = new TType[_cols];

        int new_count = 0;
        for(int i = 0; i < _rows; i++)
        {
            bool found = false;
            for(int j = 0; j < count; j++)
            {
                if(rows[j] == i)
                {
                    found = true;
                    break;
                }
            }
            if(!found)
            {
                for(int j = 0; j < _cols; j++)
                {
                    new_data[new_count][j] = _data[i][j];
                }
                new_count++;
            }
        }

        delete[] _data;
        _data = new_data;
        _rows -= count;
    }

    template <typename TType> std::string DataTable<TType>::get_header_at(int col)
    {
        if(!_has_headers)
        {
            std::cout << "ERROR: the table does not have headers." << std::endl;
            return "";
        }

        if(col > _cols || col < 0)
        {
            std::cout << "ERROR get_header_at(col): column index out of range 0 to " << (_cols-1) << "." << std::endl;
            return "";
        }

        return _headers[col];
    }

    template <typename TType> std::string* DataTable<TType>::get_headers()
    {
        if(!_has_headers)
        {
            std::cout << "ERROR: the table does not have headers." << std::endl;
            return 0;
        }
        return _headers;
    }

    template <typename TType> std::string* DataTable<TType>::get_explanatory_headers()
    {
        if(!_has_headers)
        {
            std::cout << "ERROR: the table does not have headers." << std::endl;
            return 0;
        }

        std::string* return_values = new std::string[_cols-1];
        int idx = 0;
        for(int i = 0; i < _cols; i++)
        {
            if(i == _response_column)
                continue;
            return_values[idx] = _headers[i];
            idx++;
        }
        return return_values;
    }

    template <typename TType> void DataTable<TType>::shuffle_rows(int passes)
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<int> distribution(0, _rows-1);

        TType *temp;
        for(int i = 0; i < passes; i++)
        {
            for(int j = 0; j < _rows; j++)
            {
                int r1 = distribution(g);
                int r2 = distribution(g);

                temp = _data[r1];
                _data[r1] = _data[r2];
                _data[r2] = temp;
            }
        }

    }

    template <typename TType> TType* DataTable<TType>::pct_change(std::string column)
    {
        return pct_change(get_column_from_header(column));
    }

    template <typename TType> TType* DataTable<TType>::pct_change(int column)
    {
        TType* changes = new TType[_rows-1];
        for(int i = 1; i < _rows; i++)
        {
            changes[i-1] = (_data[i][column] - _data[i-1][column])/_data[i-1][column];
        }

        return changes;
    }

    template <typename TType> TType* DataTable<TType>::sma(std::string column, int periods)
    {
        return sma(get_column_from_header(column), periods);
    }

    template <typename TType> TType* DataTable<TType>::sma(int column, int periods)
    {
        TType* sma = new TType[_rows-periods+1];
        for(int i = periods; i <= _rows; i++)
        {
            TType sum = 0;
            for(int j = periods; j > 0; j--)
                sum += _data[i-j][column];
            sma[i-periods] = (sum/periods);
        }

        return sma;
    }

    template <typename TType> TType* DataTable<TType>::ema(std::string column, int periods)
    {
        return ema(get_column_from_header(column), periods);
    }

    template <typename TType> TType* DataTable<TType>::ema(int column, int periods)
    {
        TType* ema = new TType[_rows - periods + 1];
        TType smoothing = 2.0/(TType)(periods + 1);
        ema[0] = sma(column, periods)[0];

        for(int i = 1; i <= (_rows - periods); i++)
            ema[i] = (_data[i + periods - 1][column] * smoothing) + (ema[i-1] * (1-smoothing));

        return ema;
    }

    template <typename TType> TType* DataTable<TType>::rsi(std::string column, int periods)
    {
        return rsi(get_column_from_header(column), periods);
    }

    template <typename TType> TType* DataTable<TType>::rsi(int column, int periods)
    {
        // TODO: not working
        TType* rsi = new TType[_rows - periods];
        TType* changes = pct_change(column);
        TType avg_up = 0;
        TType avg_down = 0;

        for(int i = 0; i < periods; i++) 
        {
            if(changes[i] > 0)
                avg_up += changes[i];
            else 
                avg_down += changes[i];
        }
        // consider these the SMAs to start an EMA calculation
        avg_up /= periods;
        avg_down /= periods;
        rsi[0] = 100.0 - (100.0 / (1.0 + (avg_up/avg_down)));
        
        // use EMA to keep running average of ups and downs
        TType smoothing = 2.0/((TType)(periods + 1));
        for(int i = 1; i < (_rows-periods); i++) 
        {
            if(changes[i+periods-1] > 0)
                avg_up = (changes[i + periods - 1] * smoothing) + (avg_up * (1 - smoothing));
            else
                avg_down = (changes[i + periods - 1] * smoothing) + (avg_down * (1 - smoothing));
            rsi[i] = 100.0 - (100.0 / (1.0 + ((avg_up/avg_down))));
        }

        return rsi;
    }
}

#endif
