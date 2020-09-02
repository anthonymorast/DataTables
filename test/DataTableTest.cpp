#include <DataTable/DataTable.hpp>

#include <iostream>
using namespace std;

int main()
{
	datatable::DataTable dt("x_to_x_squared.csv", "x2", true);
	cout << dt << endl;
	dt.print_headers(cout);
	dt.print_column(cout, 0);
	dt.print_row(cout, 10);

	cout << dt.nrows() << ", " << dt.ncols() << endl;
	int* shape = dt.shape();
	cout << shape[0] << ", " << shape[1] << endl;
	dt.print_shape(cout);

    datatable::DataTable dt2 = dt.top_n_rows(10);
    dt.print(cout);
    dt2.print(cout);

	double* col = dt.get_column(0);

	dt.to_file("same_but_dots.csv", '*');
}
