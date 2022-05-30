// for shape of the DT create a SHAPE_TYPE class. This will hold the _ncols and _nrows values.
    // when the SHAPE_TYPE is printed (i.e. cout << dt.shape();) pandas-like behaviour (prints (nrows, ncols))
    // when the SHAPE_TYPE, provide get rows/columns functions
    // override the [idx] operator: error if idx > 1, idx == 0 return nrows, idx == 1 return ncols

#ifndef SHAPE_TYPE
#define SHAPE_TYPE

#include <iostream>
#include <string>

namespace datatable 
{
    using ostream = std::ostream;
    struct SHAPE_TYPE_EXCEPTION : public std::runtime_error 
    {
        ~SHAPE_TYPE_EXCEPTION() {}
        SHAPE_TYPE_EXCEPTION(const std::string msg) : runtime_error(msg) {}
    };

    struct DT_SHAPE_TYPE
    {
        DT_SHAPE_TYPE() {}
        DT_SHAPE_TYPE(int nrows, int ncols) : _nrows(nrows), _ncols(ncols) {}
        
        // formal
        int get_columns() { return _ncols; }
        int get_rows() { return _nrows; }

        // useful
        void update(int rows, int cols) { _nrows = rows; _ncols = cols; }
        void set_cols(int cols) { _ncols = cols; }
        void set_rows(int rows) { _nrows = rows; }

        friend ostream& operator<<(ostream& os, const DT_SHAPE_TYPE st) 
        {
            os << "(" << st._nrows << ", " << st._ncols << ")";
            return os;
        }

        int operator[](int index) const
        {
            if(index != 0 && index != 1)    // only two valid situations
                throw SHAPE_TYPE_EXCEPTION("Invalid shape index. Valid indices are 0 for rows and 1 for columns.");
            
            return index == 0 ? _nrows : _ncols;
        }

        private:
            int _nrows {0};
            int _ncols {0};
    };
}

#endif