namespace datatable
{
    template <typename T>
    T* DataTable<T>::pct_change(std::string column) const
    {
        return pct_change(_get_column_from_header(column));
    }

    template <typename T>
    T* DataTable<T>::pct_change(int column) const
    {
        T* changes = new T[_datatable_shape[0] - 1];
        for(int i = 1; i < _datatable_shape[0]; i++)
            changes[i - 1] = (_data[i][column] - _data[i-1][column]) / _data[i-1][column];
        return changes;
    }

    template <typename T>
    T* DataTable<T>::sma(std::string column, int periods) const
    {
        return sma(_get_column_from_header(column), periods);
    }

    template <typename T>
    T* DataTable<T>::sma(int column, int periods) const
    {
        T* sma = new T[_datatable_shape[0]-periods+1];
        for(int i = periods; i <= _datatable_shape[0]; i++)
        {
            T s = 0;
            for(int j = periods; j > 0; j--)
                s += _data[i-j][column];
            sma[i-periods] = (s/periods);
        }

        return sma;
    }

    template <typename T>
    T* DataTable<T>::ema(std::string column, int periods) const
    {
        return ema(_get_column_from_header(column), periods);
    }

    template <typename T>
    T* DataTable<T>::ema(int column, int periods) const
    {
        T* ema = new T[_datatable_shape[0] - periods + 1];
        T smoothing = 2.0/(T)(periods + 1);
        ema[0] = sma(column, periods)[0];

        for(int i = 1; i <= (_datatable_shape[0] - periods); i++)
            ema[i] = (_data[i + periods - 1][column] * smoothing) + (ema[i-1] * (1-smoothing));

        return ema;
    }

    template <typename T>
    T* DataTable<T>::rsi(std::string column, int periods) const
    {
        return rsi(_get_column_from_header(column), periods);
    }

    template <typename T>
    T* DataTable<T>::rsi(int column, int periods) const
    {
        // TODO: not working
        T* rsi = new T[_datatable_shape[0] - periods];
        T* changes = pct_change(column);
        T avg_up = 0;
        T avg_down = 0;

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
        T smoothing = 2.0/((T)(periods + 1));
        for(int i = 1; i < (_datatable_shape[0]-periods); i++)
        {
            if(changes[i+periods-1] > 0)
                avg_up = (changes[i + periods - 1] * smoothing) + (avg_up * (1 - smoothing));
            else
                avg_down = (changes[i + periods - 1] * smoothing) + (avg_down * (1 - smoothing));
            rsi[i] = 100.0 - (100.0 / (1.0 + ((avg_up/avg_down))));
        }

        return rsi;
    }

    template <typename T>
    T DataTable<T>::min(int col) const
    {
        if (col < 0 || col > _datatable_shape[1])
            throw DataTableException("ERROR (min(int)): Column number out of range.");

        T min {std::numeric_limits<T>::max()};
        for(int i = 0; i < _datatable_shape[0]; i++)
            min = (min < _data[i][col]) ? min : _data[i][col];

        return min;
    }

    template <typename T>
    T DataTable<T>::max(int col) const
    {
        if (col < 0 || col > _datatable_shape[1])
            throw DataTableException("ERROR (max(int)): Column number out of range.");

        T max {std::numeric_limits<T>::min()};
        for(int i = 0; i < _datatable_shape[0]; i++)
            max = (max > _data[i][col]) ? max : _data[i][col];

        return max;
    }

    template <typename T>
    T DataTable<T>::min() const
    {
        if(_datatable_shape[1] > 1)
            throw DataTableException("ERROR (min(void)): Column number is required for data tables with multiple columns.");
        return min(0);
    }

    template <typename T>
    T DataTable<T>::max() const
    {
        if(_datatable_shape[1] > 1)
            throw DataTableException("ERROR (max(void)): Column number is required for data tables with multiple columns.");
        return max(0);
    }

    template <typename T>
    T DataTable<T>::sum(int col) const
    {
        if (col < 0 || col > _datatable_shape[1])
            throw DataTableException("ERROR (sum(int)): Column number out of range.");

        T s = 0;
        for(int i = 0; i < _datatable_shape[0]; i++)
            s += _data[i][col];
        return s;
    }

    template <typename T>
    T DataTable<T>::sum() const
    {
        if(_datatable_shape[1] > 1)
            throw DataTableException("ERROR (sum(void)): Column number is required for data tables with multiple columns.");
        return sum(0);
    }

    template <typename T>
    T DataTable<T>::mean(int col) const
    {
        if (col < 0 || col > _datatable_shape[1])
            throw DataTableException("ERROR (mean(int)): Column number out of range.");

        T mean = sum(col);
        return (mean / _datatable_shape[0]);
    }

    template <typename T>
    T DataTable<T>::mean() const
    {
        if(_datatable_shape[1] > 1)
            throw DataTableException("ERROR (mean(void)): Column number is required for data tables with multiple columns.");
        return mean(0);
    }
}
