#include <DataTable/DataTable.hpp>
#include <armadillo>
#include <iostream>
#include <string>
#include <chrono>

#include "yfapi.hpp"

using namespace std;
using namespace arma;

int main()
{
	datatable::DataTable<double> dt("mnist_train.csv", "", false);
	datatable::DataTable<double> x2dt("x_to_x_squared.csv", "x2", true);
	auto start = chrono::steady_clock::now();
	x2dt.to_csv("x_to_x_squared.copy.csv");
	auto end = chrono::steady_clock::now();
	cout << "x_to_x write time (ms): " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
		 << " ms" << endl;

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

    // yfapi::YahooFinanceAPI<double> api; 
    // datatable::DataTable<double> dt = api.get_ticker_data("spy", "2021-03-01", "2021-04-14");
	// cout << dt.nrows() << endl;
	// cout << dt << endl;
	// dt.print_row(cout, 10);
	// dt.print_column(cout, "Date");
	// dt.print_column(cout, "Close");
	// dt.print(cout);

	// double* rsi14 = dt.rsi("Close");
	// for(int i = 0; i < dt.nrows()-14; i++)
	// 	cout << i << ": " << rsi14[i] << endl;

	// dt.print_headers(cout);
	// dt.print_column(cout, 0);
	// dt.print_row(cout, 10);

	cout << dt.nrows() << ", " << dt.ncols() << endl;
	auto shape = dt.shape();
	cout << dt.shape() << endl;
	cout << shape[0] << ", " << shape[1] << endl;
	start = chrono::steady_clock::now();
	dt.to_csv("mnist_train.copy.csv");
	end = chrono::steady_clock::now();
	cout << "mnist write time (ms): " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
		 << " ms" << endl;

    // datatable::DataTable<double> dt2 = dt.top_n_rows(10);
    // dt.print(cout);
    // dt2.print(cout);

	// double* col = dt.get_column(0);
	// double* flat_data = dt.get_flat_explanatory();
	// for(int i = 0; i < dt.ncols()*dt.nrows(); i++)
	// {
	// 	cout << flat_data[i] << " ";
	// }

	// datatable::DataTable<double> dt3 = dt;
	// dt3.drop_column("Date");
	// cout << dt3 << endl << dt << endl;
	// dt3.drop_column(0);
	// cout << dt3 << endl;
	// cout << dt3.get_response_column_name() << endl;

	// dt.to_file("same_but_dots.csv", '*');
}
