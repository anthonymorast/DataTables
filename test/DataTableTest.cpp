#include <DataTable/DataTable.hpp>
#include <armadillo>

#include <iostream>
using namespace std;
using namespace arma;

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
	double* flat_data = dt.get_flat_explanatory();
	for(int i = 0; i < dt.ncols()*dt.nrows(); i++)
	{
		cout << flat_data[i] << " ";
	}

    mat m(flat_data, dt.ncols(), dt.nrows());

	dt.to_file("same_but_dots.csv", '*');
}
