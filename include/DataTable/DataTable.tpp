namespace datatable
{
    template <typename T>
    DataTable<T>::DataTable(
        const std::string& csv_filename,
        std::string response_name,
        bool has_headers)
    {
        _str_utils.trim(response_name);
        from_csv(csv_filename, response_name, has_headers);	// set data, ncols, nrows, response vars, and headers in here
        _has_headers = has_headers;
    }

    template <typename T>
    DataTable<T>::DataTable(
        std::vector<std::string> headers,
        std::string response_name,
        T** data,
        int nrows,
        int ncols,
        bool has_headers)
    {
        _headers = headers;
        _response = response_name;
        _datatable_shape.update(nrows, ncols);
        _has_headers = has_headers;
        /* seg faults with heap corruption when deep-copying more than once
        _data = new T*[nrows];
        for(size_t i = 0; i < nrows; i++)
            _data[i] = new T[ncols];
        std::copy(&data[0][0], &data[0][0] + (nrows*ncols), &_data[0][0]);*/
        _data = data;
        if(has_response())
            _response_column = _get_column_from_header(_response);
        _data_loaded = true;
    }

    template <typename T>
    DataTable<T>::DataTable(const DataTable<T>& rhs_table)
    {
        _datatable_shape.update(rhs_table._datatable_shape[0], rhs_table._datatable_shape[1]);
        _headers = rhs_table._headers;
        _data = rhs_table._data;
        _response = rhs_table._response;
        _response_column = rhs_table._response_column;
        _data_loaded = rhs_table._data_loaded;
        _has_headers = rhs_table._has_headers;
        _allocated_data = rhs_table._allocated_data;
    }

    template <typename T>
    DataTable<T>::~DataTable()
    {
        // copy/double free problems -- does this need to be done before the program exits?
        // maybe provide a clear() or free() method to do this and clear other objects
        // if(_allocated_data)
        //     delete []_data;
    }

    template <typename T>
    void DataTable<T>::free()
    {
        if(_allocated_data)
            delete []_data;
        _headers.clear();
        _datetime_columns.clear();
        _date_columns_to_formats.clear();
    }

    template <typename T>
    void DataTable<T>::from_csv(
      const std::string& filename,
      const std::string& response,
      bool has_headers
    )
    {
      from_delimited(filename, response, ',', has_headers);
    }

    template <typename T>
    void DataTable<T>::from_delimited(
        const std::string& filename,
        const std::string& response,
        char delimiter,
        bool has_headers)
    {
        if(_data_loaded)
            throw DataTableException("ERROR (from_delimited): Data has already been loaded for this table.");

        std::ifstream data_file(filename);
        if(!data_file.is_open())
            throw DataTableException("ERROR (from_delimited): Unable to open file '" + filename + "'.");

        std::vector<std::string> lines;
        std::string line;
        while(std::getline(data_file, line))
            lines.push_back(line);

        // // read first line, regardless of headers, to get valuable data (ncols)
        line = lines.front();
        const size_t cols = std::count(line.begin(), line.end(), delimiter) + 1;
        std::string value;
        if(has_headers)		// read the header line if there is one
        {
            _headers.resize(cols);
            std::istringstream ss_headers(line);
            bool response_found = false;
            size_t count = 0;
            while(std::getline(ss_headers, value, delimiter))
            {
                _str_utils.trim(value);
                if(!response_found && value == response)
                {
                    response_found = true;
                    _response = value;
                    _response_column = count;
                }
                _headers.at(count) = value;
                count++;
            }
            if(!response_found)
                throw DataTableException("ERROR (from_csv): Response variable not found.");
            lines.erase(lines.begin());         // remove headers from further processing
        }
        else
            _headers.resize(0);  // free memory

        _data = new T*[lines.size()];
        for(int i = 0; i < lines.size(); i++)
            _data[i] = new T[cols];

        line = lines.front();       // grab second line to find other useful data (datetime columns)
        std::istringstream ss(line);
        int col = 0;
        while(std::getline(ss, value, delimiter))
        {
            if(_date_utils.isDateTime(value))
                _date_columns_to_formats.insert(std::pair<int, std::string>(col, _date_utils.getFormat(value)));
            col++;
        }

        // copy data into 2-d array
        int row = 0;
        for(const auto it : lines)
        {
            col = 0;
            std::istringstream ss(it);
            while(std::getline(ss, value, delimiter))
            {
                auto date_map_it = _date_columns_to_formats.find(col);
                if(date_map_it != _date_columns_to_formats.end())
                    _data[row][col] = (T)_date_utils.getTimeFromString(value, date_map_it->second);
                else
                    try
                    {
                        _data[row][col] = std::stod(value);
                    }
                    catch(...)
                    {
                        throw DataTableException("ERROR (from_csv): Value '" + value + "' is not parsable as numeric.");
                    }
                col++;
            }
            row++;
        }

        data_file.close();
        _datatable_shape.update(lines.size(), cols);
        _allocated_data = true;
        _data_loaded = true;
    }

    template <typename T>
    void DataTable<T>::to_csv(const std::string& filename) const
    {
      to_delimited(filename, ',');
    }

    template <typename T>
    void DataTable<T>::to_delimited(
        const std::string& filename,
        const char delimiter) const
    {
        std::ofstream out(filename);
        if(!out.is_open())
            throw DataTableException("ERROR (to_csv): Could not open file '" + filename + "'.");

        if(!_data_loaded)
        {
            out.close();    // clean up and do nothing
            return;
        }

        std::stringstream write_stream("", std::ios_base::app | std::ios_base::out);
        if(_has_headers)
        {
            std::string headers {""};
            std::for_each(_headers.begin(), _headers.end(),
                [&headers, &delimiter](std::string header){headers += (header + delimiter);}
            );
            headers.pop_back();       // remove last comma
            write_stream << headers << "\n";
        }

        for(int i = 0; i < _datatable_shape[0]; i++)
        {
            std::string output_string {""};
            for(int j = 0; j < _datatable_shape[1]; j++)
			{
                if(_date_columns_to_formats.find(j) != _date_columns_to_formats.end())
                    output_string += _date_utils.getStringFromTime((time_t)_data[i][j], _date_columns_to_formats.at(j));
                else
                    output_string += std::to_string(_data[i][j]);
                output_string += delimiter;
			}
            output_string.pop_back();
            write_stream << output_string << "\n";
        }
        out << write_stream.rdbuf();
        out.close();
    }

    template <typename T>
    T* DataTable<T>::operator[](int index) const
    {
        // returns a row of data
        T data[_datatable_shape[1]];    // size of columns
        data = _data[index];
        return data;
    }

    template <typename T>
    DataTable<T> DataTable<T>::operator[](std::string column) const
    {
        if(std::find(_headers.begin(), _headers.end(), column) == _headers.end())
            throw DataTableException("ERROR (DataTable[]): Column '" + column + "' was not found in the data table.");

        DataTable<T> dt(_headers, _response, _data, _datatable_shape[0], _datatable_shape[1], _has_headers);
        // drop cols = all columns except column (so erase the column) [gauranteed to be in the vector]
        std::vector<std::string> drop_cols = _headers;
        drop_cols.erase(std::remove(drop_cols.begin(), drop_cols.end(), column), drop_cols.end());
        dt.drop_columns(drop_cols);
        return dt;
    }

    template <typename T>
    int DataTable<T>::_get_column_from_header(std::string header, bool should_error) const
    {
        auto header_iterator = std::find(_headers.begin(), _headers.end(), header);
        if(should_error && header_iterator == _headers.end())
            throw DataTableException("ERROR (_get_column_from_header): Column '" + header + "' is not in the data table.");
        return header_iterator - _headers.begin();  // calculate the index
    }

    template <typename T>
    const std::vector<std::string>& DataTable<T>::get_explanatory_headers() const
    {
        if(!_has_headers || !_response_column || !_data_loaded)
            return _headers;    // should be an empty vector

        auto response_it = std::find(_headers.begin(), _headers.end(), _response);
        std::vector<std::string> explanatory_headers = _headers;
        explanatory_headers.erase(response_it);
        return explanatory_headers;
    }

    template <typename T>
    void DataTable<T>::drop_columns(std::vector<int>& columns)
    {
        // sorted the columns vector and remove from end to beginning
        // this way only indexes past the current evaluation are changed
        T** new_data = new T*[_datatable_shape[0]];
        for(int i = 0; i < _datatable_shape[0]; i++)
            new_data[i] = new T[_datatable_shape[1] - columns.size()];  // smaller number of columns

        std::sort(columns.begin(), columns.end(), std::greater<int>());
        for(auto it : columns)
        {
            // the vector is sorted descending order so this will failr right away
            if(it > (_datatable_shape[1] - 1))       // column does not exist
            {
                throw DataTableException("ERROR (drop_columns): Column does not exist. Number columns=" +
                        std::to_string(_datatable_shape[1]) + ", column to erase=" + std::to_string(it));
            }

            if(_has_headers)
                _headers.erase(_headers.begin() + it);

            if(_date_columns_to_formats.find(it) != _date_columns_to_formats.end())
                _date_columns_to_formats.erase(it);
        }

        // update response column
        // what if we drop the response? probably shouldn't error
        if(_has_headers)
            _response_column = _get_column_from_header(_response, false);

        // copy data over to new data array
        std::vector<int> keep_cols(_datatable_shape[1]);
        std::iota(keep_cols.begin(), keep_cols.end(), 0);
        size_t new_count = 0;
        for(auto it : keep_cols)
        {
            if(std::find(columns.begin(), columns.end(), it) == columns.end()) // not in drop cols, copy
            {
                for(int j = 0; j < _datatable_shape[0]; j++)
                    new_data[j][new_count] = _data[j][it];
                new_count++;
            }
        }

        // unsafe, but causes issues when creating non-persistent data tables,
        // e.g. calling std::cout << dt["x"].min() << std::endl; twice will seg fault
        // since the data is deleted here but used in this column select.
        // It might be worthwhile to create a method to return the new data
        // and delete the old data in another function.
        // delete[] _data;
        _data = new_data;
        _datatable_shape.set_cols(_datatable_shape[1] - columns.size());
    }

    template <typename T>
    void DataTable<T>::drop_columns(std::vector<std::string>& column_names)
    {
        std::vector<int> column_numbers(column_names.size());
        size_t index = 0;
        for(auto it : column_names)
        {
            column_numbers.at(index) = _get_column_from_header(it);
            index++;
        }
        drop_columns(column_numbers);
    }

    template <typename T>
    void DataTable<T>::drop_column(int column)
    {
        std::vector<int> columns(1, column);
        drop_columns(columns);
    }

    template <typename T>
    void DataTable<T>::drop_column(const std::string& column)
    {
        std::vector<int> columns(1, _get_column_from_header(column));
        drop_columns(columns);
    }

    template <typename T>
    void DataTable<T>::shuffle_rows(int passes)
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<int> distribution(0, _datatable_shape[0] - 1);

        T *temp;
        for(int i = 0; i < passes; i++)
        {
            for(int j = 0; j < _datatable_shape[0]; j++)
            {
                int r1 = distribution(g);
                int r2 = distribution(g);
                temp = _data[r1];
                _data[r1] = _data[r2];
                _data[r2] = temp;
            }
        }
    }

    template <typename T>
    T* DataTable<T>::get_row(int row) const
    {
        if(row > _datatable_shape[0] || row < 0)
            throw DataTableException("ERROR (get_row): Row index out of range; passed:" +
                std::to_string(row) + ", num rows: " + std::to_string(_datatable_shape[0]) + ".");
        return _data[row];
    }

    template <typename T>
    std::string DataTable<T>::get_header_at(int col) const
    {
        if(!_has_headers)
            throw DataTableException("ERROR (get_header_at): Could not fetch header. The data table does not have headers.");
        if(col > _datatable_shape[1] || col < 0)
            throw DataTableException("ERROR (get_header_at): Could not fetch header. Column " + std::to_string(col) + " is out of range.");

        return _headers.at(col);
    }

    template <typename T>
    DataTable<T> DataTable<T>::select_rows(std::vector<int> row_numbers) const
    {
        if(row_numbers.size() > _datatable_shape[0])
            throw DataTableException("ERROR (select_rows): number of select rows is greater than the number of rows in the data table.");

        T** data = new T*[row_numbers.size()];
        for(int i = 0; i < row_numbers.size(); i++)
            data[i] = new T[_datatable_shape[1]];

        int count = 0 ;
        for(int row : row_numbers)
            memcpy(data[count++], _data[row], _datatable_shape[1] * sizeof(T));

        DataTable<T> new_dt(_headers, _response, data, row_numbers.size(), _datatable_shape[1]);
        return new_dt;
    }

    template <typename T>
    DataTable<T> DataTable<T>::select_row_range(int start, int end) const
    {
        if(end < start)
            throw DataTableException("ERROR (select_row_range): row range end is before row range start.");

        size_t size = end - start;
        std::vector<int> row_numbers(size);
        for(int i = 0; i < size; i++)
            row_numbers.at(i) = start + i;
        DataTable<T> new_dt = select_rows(row_numbers);
        return new_dt;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const DataTable<T>& table)
    {
        if(!table._data_loaded || (table._datatable_shape[1] == 0))
            return os;

        std::string row_buffer;
        if(table._has_headers)
        {
            for(int i = 0; i < table._datatable_shape[1]; i++)
                row_buffer += table._headers[i] + ",";
            row_buffer.pop_back();
            os << row_buffer << "\n";
        }

        for(int i = 0; i < table._datatable_shape[0]; i++)
        {
            row_buffer = "";
            for(int j = 0; j < table._datatable_shape[1]; j++)
            {
                if(table._date_columns_to_formats.find(j) != table._date_columns_to_formats.end())
                    row_buffer += table._date_utils.getStringFromTime((time_t)table._data[i][j], table._date_columns_to_formats.at(j));
                else
                    row_buffer += std::to_string(table._data[i][j]);
                row_buffer += ",";
            }
            row_buffer.pop_back();
            os << row_buffer << "\n";
        }

        return os;
    }

    template <typename T>
    DataTable<T> DataTable<T>::select_columns(std::vector<int> column_numbers) const
    {
		std::vector<std::string> headers(column_numbers.size());
		T** new_data = new T*[_datatable_shape[0]];
		for(int i = 0; i < _datatable_shape[0]; i++)
			new_data[i] = new T[_datatable_shape[1]];

		bool keep_response = false;
		size_t idx = 0;
		for(auto col : column_numbers)
		{
			if(col == _response_column)
				keep_response = true;

			for(int i = 0; i < _datatable_shape[0]; i++)
				new_data[i][idx] = _data[i][col];
			headers.at(idx++) = _headers[col];
		}
		DataTable<T> new_dt(headers, (keep_response ? _response : ""), new_data, _datatable_shape[0], column_numbers.size());
		return new_dt;
    }

    template <typename T>
    DataTable<T> DataTable<T>::select_columns(std::vector<std::string> column_names) const
    {
    	if(!_has_headers)
			throw DataTableException(
				"ERROR (select_columns): cannot select columns by header,the data table does not have headers."
			);

		std::vector<int> column_numbers(column_names.size());
		int idx = 0;
		for(auto name : column_names)
			column_numbers.at(idx++) = _get_column_from_header(name);
		return select_columns(column_numbers);
    }

	template <typename T>
	T* DataTable<T>::get_column(int column) const
	{
		if(column < 0 || column > _datatable_shape[1])
			throw DataTableException("ERROR (get_column): Column " + std::to_string(column) + " is out of range.");

		T* data = new T[_datatable_shape[0]];
		for(int i = 0; i < _datatable_shape[0]; i++)
			data[i] = _data[i][column];
		return data;
	}

	template <typename T>
	T* DataTable<T>::get_column(std::string column_name) const
	{
		int col = _get_column_from_header(column_name);
		if(col < 0 || col > _datatable_shape[1])
			throw DataTableException("ERROR (get_column): Column \"" + column_name + "\"not found");
		return get_column(col);
	}

	template <typename T>
	T* DataTable<T>::response() const
	{
		if(!has_response())
			throw DataTableException("ERROR (response): response variable not set.");
		return get_column(_response_column);
	}

	/*
        Becomes necessary when the response column contains typenamees (represented by integers)
        rather than doubles. Libraries like mlpack only accept integers for methods
        such as logistic regression.
    */
	template <typename T>
	size_t* DataTable<T>::int_response() const
	{
		size_t* data = new size_t[_datatable_shape[0]];
		for(size_t i = 0; i < _datatable_shape[0]; i++)
			data[i] = (size_t)_data[i];
		return data;
	}

	template <typename T>
	T** DataTable<T>::get_all_explanatory() const
    {
        T** data = new T*[_datatable_shape[0]];
        for(int i = 0; i < _datatable_shape[0]; i++)
            data[i] = new T[_datatable_shape[1]-1];

        for(int i = 0; i < _datatable_shape[0]; i++)
        {
            for(int j = 0; j < _datatable_shape[1]; j++)
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

    template <typename T>
	T* DataTable<T>::get_flat_explanatory() const
    {
        T* data = new T[_datatable_shape[0] * _datatable_shape[1]];
        for(int i = 0; i < _datatable_shape[0]; i++)
        {
            for(int j = 0; j < _datatable_shape[1]; j++)
            {
                 data[(i*_datatable_shape[1]) + j] = _data[i][j];
            }
        }
        return data;
    }
}
