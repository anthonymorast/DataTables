#include <DataTable/DataTable.hpp>
#include <armadillo>
#include <iostream>
#include <string>

#include "yfapi.hpp"

using namespace std;
using namespace arma;

int main()
{
	// datatable::DataTable dt("x_to_x_squared.csv", "x2", true);
    // dt = dt.select_columns(new string[1]{"x2"}, 1);

	// dt.print_column(cout, "x2");
	// double* changes = dt.pct_change("x2");
	// for(int i = 0; i < dt.nrows()-1; i++)
	// 	cout << i << ": " << changes[i] << endl;

	// double* sma3 = dt.sma("x2", 3);
	// for(int i = 0; i < dt.nrows()-2; i++)
	// 	cout << i << ": " << sma3[i] << endl;

	// double* sma30 = dt.sma("x2", 30);
	// for(int i = 0; i < dt.nrows()-29; i++)
	// 	cout << i << ": " << sma30[i] << endl;

	// double* ema30 = dt.ema("x2", 30);
	// for(int i = 0; i < dt.nrows()-29; i++)
	// 	cout << i << ": " << ema30[i] << endl;

	// double* ema3 = dt.ema("x2", 3);
	// for(int i = 0; i < dt.nrows()-2; i++)
	// 	cout << i << ": " << ema3[i] << endl;

    yfapi::YahooFinanceAPI api; 
    datatable::DataTable dt = api.get_ticker_data("spy", "2021-03-01", "2021-04-14");
	cout << dt.nrows() << endl;
	cout << dt << endl;
	dt.print_row(cout, 10);
	dt.print_column(cout, "Date");
	dt.print_column(cout, "Close");
	dt.print(cout);

	// double* rsi14 = dt.rsi("Close");
	// for(int i = 0; i < dt.nrows()-14; i++)
	// 	cout << i << ": " << rsi14[i] << endl;

	// dt.print_headers(cout);
	// dt.print_column(cout, 0);
	// dt.print_row(cout, 10);

	// cout << dt.nrows() << ", " << dt.ncols() << endl;
	// int* shape = dt.shape();
	// cout << shape[0] << ", " << shape[1] << endl;
	// dt.print_shape(cout);

    // datatable::DataTable dt2 = dt.top_n_rows(10);
    // dt.print(cout);
    // dt2.print(cout);

	// double* col = dt.get_column(0);
	// double* flat_data = dt.get_flat_explanatory();
	// for(int i = 0; i < dt.ncols()*dt.nrows(); i++)
	// {
	// 	cout << flat_data[i] << " ";
	// }

    // mat m(flat_data, dt.ncols(), dt.nrows());

	// datatable::DataTable dt3 = dt;
	// dt3.drop_column("x2");
	// cout << dt3 << endl << dt << endl;
	// dt3.drop_column(0);
	// cout << dt3 << endl;
	// cout << dt3.get_response_column_name() << endl;

	// dt.to_file("same_but_dots.csv", '*');
}
