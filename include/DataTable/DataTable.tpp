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
        const std::string* headers, 
        const std::string& response_name, 
        const T** data, 
        int nrows, 
        int ncols, 
        bool has_headers)
    {

    }

    template <typename T>
    DataTable<T>::DataTable(
        const std::string* headers, 
        int response_column, 
        const T** data, 
        int nrows, 
        int ncols, 
        bool has_headers)
    {
        
    }

    template <typename T>
    DataTable<T>::DataTable(const DataTable<T>&)
    {
        
    } 

    template <typename T>
    DataTable<T>::~DataTable()
    {
        if(_allocated_data)
            delete []_data;
    }

    // file manip
    template <typename T>
    void DataTable<T>::from_csv(
        const std::string& filename, 
        const std::string& response, 
        bool has_headers)
    {
        if(_data_loaded)
            throw DataTableException("Data has already been loaded for this table.");

        std::ifstream data_file(filename);
        if(!data_file.is_open())
            throw DataTableException("Unable to open file '" + filename + "'.");

        std::vector<std::string> lines;
        std::string line;
        while(std::getline(data_file, line)) 
            lines.push_back(line);

        // // read first line, regardless of headers, to get valuable data (ncols)
        line = lines.front();
        const size_t cols = std::count(line.begin(), line.end(), ',') + 1;
        std::string value;
        if(has_headers)		// read the header line if there is one
        {
            _headers.resize(cols);
            std::istringstream ss_headers(line);
            bool response_found = false;
            size_t count = 0;
            while(std::getline(ss_headers, value, ','))
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
                throw DataTableException("Response variable not found.");
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
        while(std::getline(ss, value, ','))
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
            while(std::getline(ss, value, ','))
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
                        throw DataTableException("Value '" + value + "' is not parsable as numeric.");
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
    void DataTable<T>::to_csv(
        const std::string& filename, 
        const char delimiter) const 
    {
        std::ofstream out(filename);
        if(!out.is_open()) 
            throw DataTableException("Could not open file '" + filename + "'.");
        
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
                output_string += (std::to_string(_data[i][j]) + delimiter);
            output_string.pop_back();
            write_stream << output_string << "\n";
        }
        out << write_stream.rdbuf();
        out.close();
    }
}