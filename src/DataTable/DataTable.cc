#include <DataTable/DataTable.hpp>

namespace datatable
{
    // TODO: need to seriously test the DataTable class, there is a lot of stuff going on...

    DataTable::DataTable(){}

    DataTable::DataTable(std::string csv_filename, std::string response_name, bool has_headers)
    {
        response_name = _str_utils.trim(response_name);
        from_csv(csv_filename, response_name, has_headers);	// set data, ncols, nrows, response vars, and headers in here
        _has_headers = has_headers;
    }

    DataTable::DataTable(std::string* headers, std::string response_name, double** data, int nrows, int ncols, bool has_headers)
    {
        // TODO: should consider deep-copying the pointers in the constructors in case the user frees
        // memory we're using
        _str_utils.trim_all(headers, ncols);
        _cols = ncols;
        _rows = nrows;
        _data = data;
        _headers = headers;
        _response = response_name;
        _response_column = get_column_from_header(response_name);
        _data_loaded = true;
        _has_headers = has_headers;
    }

    /*! DataTable::DataTable(string*, int, double**, int, int, bool)
     *  When the response column number is passed into this constructor, it's assumed the user will use
     *  0-based indexing. This is due to the fact that 0-based indexing is used in other get/print methods.
     */
    DataTable::DataTable(std::string* headers, int response_column, double** data, int nrows, int ncols, bool has_headers)
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

    DataTable::~DataTable()
    {
        // TODO: ~DataTable(): should probably still free these... will need to pass back new arrays
        // from the getters
        //	delete[] _data;
    }

    bool DataTable::has_response()
    {
        return std::strcmp(_response.c_str(), "");
    }

    int DataTable::get_column_from_header(std::string header)
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

    void DataTable::from_csv(std::string filename, std::string response, bool has_headers)
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
        std::copy(std::istream_iterator<std::string>(data_file),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(lines));

        // read first line, regardless of headers, to get valuable data (ncols)
        std::string line = lines.front();
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
                value = _str_utils.trim(value);
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
            lines.erase(lines.begin());     // skip headers if need be

        // process the remainder of the data file; first need to get rowcount to allocate memory
        _rows = lines.size();
        _data = new double*[_rows];
        for(int i = 0; i < _rows; i++)
            _data[i] = new double[_cols];

        int row_count = 0;
        for(auto it = lines.begin(); it != lines.end(); it++)
        {
            int col_count = 0;
            std::istringstream ss2((*it));
            while(std::getline(ss2, value, ','))
            {
                try
                {
                    _data[row_count][col_count] = std::stod(value);
                }
                catch(std::exception &e)
                {
                    std::cout << "ERROR: value '" << value << "' is not parsable as a double." << std::endl;
                    return;
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

    void DataTable::to_file(std::string filename, char delimiter)
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

    double** DataTable::get_data() { return _data; }
    double* DataTable::get_row(int row)
    {
        if(row >= _rows || row < 0)
        {
            std::cout << "ERROR: row index out of range; row " << row << " out of " << (_rows+1) << std::endl;
            std::cout << "NOTE: row indexing is 0-based." << std::endl;
            return 0;
        }
        return _data[row];
    }

    double* DataTable::get_column(int column)
    {
        if(column >= _cols || column < 0)
        {
            std::cout << "ERROR: column index out of range; column " << column << " out of " << (_cols+1) << std::endl;
            std::cout << "NOTE: column indexing is 0-based." << std::endl;
            return 0;
        }

        double* col_data = new double[_rows];
        for(int i = 0; i < _rows; i++)
        {
            col_data[i] = _data[i][column];
        }
        return col_data;
    }

    double* DataTable::get_column(std::string column_name)
    {
        int col = get_column_from_header(column_name);
        return (col < 0 ? 0 : get_column(col));
    }

    double* DataTable::get_response()
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
        Becomes necessary when the response column contains classes (represented by integers)
        rather than doubles. Libraries like mlpack only accept integers for methods
        such as logistic regression.
    */
    size_t* DataTable::get_int_response()
    {
        size_t* data = new size_t[_rows];
        for(int i = 0; i < _rows; i++)
        {
            data[i] = (size_t)_data[i];
        }
        return data;
    }

    double** DataTable::get_all_explanatory()
    {
        double** data = new double*[_rows];
        for(int i = 0; i < _rows; i++)
            data[i] = new double[_cols-1];

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

    double* DataTable::get_flat_explanatory()
    {
        double* data = new double[_rows * _cols];
        for(int i = 0; i < _rows; i++)
        {
            for(int j = 0; j < _cols; j++)
            {
                 data[(i*_cols) + j] = _data[i][j];
            }
        }
        return data;
    }

    DataTable DataTable::select_columns(int* column_numbers, int number_columns)
    {
        double** data = new double*[_rows];
        for(int i = 0; i < _rows; i++)
            data[i] = new double[number_columns];

        int col_count = -1;
        bool keep_response = false;
        std::string* headers = new std::string[number_columns];
        for(int j = 0; j < _cols; j++)      // swap loops to minimize number of k loops.
        {
            bool use_col = false;
            for(int k = 0; k < _cols; k++)
            {
                if(j == column_numbers[k])
                {
                    use_col = true;
                    if(j == _response_column)
                        keep_response = true;
                    col_count++;
                    break;
                }
            }

            if(use_col)
            {
                for(int i = 0; i < _rows; i++)
                    data[i][col_count] = _data[i][j];
                headers[col_count] = _headers[j];
            }
        }

        DataTable new_data(headers, (keep_response ? _response : ""), data, _rows, number_columns);
        return new_data;
    }

    DataTable DataTable::select_columns(std::string* variables, int number_cols)
    {
        if(!_has_headers)
        {
            std::cout << "ERROR: attempting to select columns by header values on headerless data table." << std::endl;
            DataTable e;
            return e;
        }

        int* column_numbers = new int[number_cols];
        for(int i = 0; i < number_cols; i++)
            column_numbers[i] = get_column_from_header(variables[i]);

        return select_columns(column_numbers, number_cols);
    }

    DataTable DataTable::select_rows(int* row_numbers, int number_rows)
    {
        double** data = new double*[number_rows];
        for(int i = 0; i < number_rows; i++)
            data[i] = new double[_cols];

        for(int k = 0; k < number_rows; k++)
        {
            memcpy(data[k], _data[row_numbers[k]], _cols * sizeof(double));
        }

        DataTable new_dt(_headers, _response, data, number_rows, _cols);
        return new_dt;
    }

    DataTable DataTable::top_n_rows(int n)
    {
        return select_row_range(0, n);
    }

    DataTable DataTable::bottom_n_rows(int n)
    {
        return select_row_range(_rows - n, _rows);
    }

    DataTable DataTable::select_row_range(int start, int end)
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

        DataTable return_table = select_rows(rows, size);
        delete[] rows;
        return return_table;
    }

    void DataTable::print(std::ostream& stream)
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
                stream << _data[i][j];
                if(j < _cols-1)
                    stream << delimiter;
            }
            stream << std::endl;
        }
    }

    void DataTable::print_shape(std::ostream& stream)
    {
        int* s = shape();
        stream << "(" << s[0] << ", " << s[1] << ")" << std::endl;
    }

    void DataTable::print_column(std::ostream& stream, int column)
    {
        if(column >= _cols || column < 0)
        {
            std::cout << "ERROR: column index out of range; column " << column << " out of " << (_cols+1) << std::endl;
            std::cout << "NOTE: column indexing is 0-based." << std::endl;
            return;
        }

        for(int i = 0; i < _rows; i++)
            stream << _data[i][column] << std::endl;
    }

    void DataTable::print_column(std::ostream& stream, std::string column)
    {
        int	col = get_column_from_header(column);
        if(col < 0)
            return;
        print_column(stream, col);
    }

    void DataTable::print_row(std::ostream& stream, int row)
    {
        if(row >= _rows || row < 0)
        {
            std::cout << "ERROR: row index out of range; row " << row << " out of " << (_rows+1) << std::endl;
            std::cout << "NOTE: row indexing is 0-based." << std::endl;
            return;
        }

        for(int i = 0; i < _cols; i++)
            stream << _data[row][i] << ", ";
        stream << std::endl;
    }

    void DataTable::print_headers(std::ostream& stream)
    {
        for(int i = 0; i < _cols; i++)
            stream << _headers[i] << (i < _cols-1 ? ", " : "");
        stream << std::endl;
    }

    double* DataTable::operator[](int row) const
    {
        if(row >= _rows || row < 0)
        {
            std::cout << "ERROR: row index out of range; row " << row << " out of " << (_rows+1) << std::endl;
            std::cout << "NOTE: row indexing is 0-based." << std::endl;
            return 0;
        }
        return _data[row];
    }

    std::ostream& operator<<(std::ostream& os, const DataTable& table)
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
                os << table._data[i][j];
                if(j < table._cols-1)
                    os << delimiter;
            }
            os << std::endl;
        }

        return os;
    }


    void DataTable::drop_columns(int* columns, int count)
    {
        std::string* new_headers = new std::string[_cols-count];
        double** new_data = new double*[_rows];
        for(int i = 0; i < _rows; i++)
            new_data[i] = new double[_cols - count];

        int new_col_count = 0;
        for(int i = 0; i < _cols; i++)
        {
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
        std::cout << _cols << std::endl;
        _data = new_data;
       _cols -= count;
        std::cout << _cols << std::endl;
    }

    void DataTable::drop_columns(std::string* column_names, int count)
    {
        int* columns = new int[count];
        for(int i = 0; i < count; i++)
        {
            columns[i] = get_column_from_header(column_names[i]);
        }

        drop_columns(columns, count);
    }

    void DataTable::drop_rows(int* rows, int count)
    {
        double** new_data = new double*[_rows - count];
        for(int i = 0; i < (_rows-count); i++)
            new_data[i] = new double[_cols];

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

    std::string DataTable::get_header_at(int col)
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

    std::string* DataTable::get_headers()
    {
        if(!_has_headers)
        {
            std::cout << "ERROR: the table does not have headers." << std::endl;
            return 0;
        }
        return _headers;
    }

    std::string* DataTable::get_explanatory_headers()
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

    void DataTable::shuffle_rows(int passes)
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<int> distribution(0, _rows-1);

        double *temp;
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
}
