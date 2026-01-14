#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "readwrite.h"

/* 二乗平均誤差と二乗平均平方根誤差(点間をN_INTERPOLATION点に補間)を計算して保存する */

int main(int argc, char* argv[]){
	int N_INTERPOLATION;
	std::string PATHS_PATH;
	int COLUMN_TRUE;
	int COLUMN_DATA;
	int LINE_START;
	int LINE_END;
	if(argc == 7){
		N_INTERPOLATION = std::atoi(argv[1]);
		PATHS_PATH =                argv[2];
		COLUMN_TRUE =     std::atoi(argv[3]) - 1;
		COLUMN_DATA =     std::atoi(argv[4]) - 1;
		LINE_START  =     std::atoi(argv[5]) - 1;
		LINE_END    =     std::atoi(argv[6]) - 1;
	}
	else if(argc == 6){
		N_INTERPOLATION = std::atoi(argv[1]);
		PATHS_PATH =                argv[2];
		COLUMN_TRUE =     std::atoi(argv[3]) - 1;
		COLUMN_DATA =     std::atoi(argv[4]) - 1;
		LINE_START  =     std::atoi(argv[5]) - 1;
		LINE_END    = -1;
	}
	else{
		std::cerr << "Usage: ./rmse-interpolated N_INTERPOLATION PATHs.txt COLUMN_TRUE COLUMN_DATA LINE_START [LINE_END](1 start, comment is not counted)" << std::endl;
		return 1;
	}

	double total_sum_squareerr = 0.0;
	int Nlines_within = LINE_END - LINE_START + 1;
	
	int Nfiles = 0;
	std::string* paths = read_list(PATHS_PATH, Nfiles);

	std::string header_0;
	int Nlines;
	int Ncolumns;
	double** data = read_data(paths[0], header_0, Nlines, Ncolumns);
	
	double index_start = data[0][LINE_START];
	double index_end   = data[0][LINE_END];

	if(LINE_END == -1){ LINE_END = Nlines - 1; }// 最後まで
	double** error = new double*[2];
	error[0] = data[0];
	error[1] = new double[Nlines];

	for(int i=0; i<Nlines*N_INTERPOLATION; i++){
		error[1][i] = (data_interpolated[COLUMN_DATA][i*N_INTERPOLATION] + (data_interpolated[COLUMN_DATA][(i+1)*N_INTERPOLATION] - data_interpolated[COLUMN_DATA][i*N_INTERPOLATION]) * (i - i%N_INTERPOLATION))
		            - (data_interpolated[COLUMN_TRUE][i*N_INTERPOLATION] + (data_interpolated[COLUMN_TRUE][(i+1)*N_INTERPOLATION] - data_interpolated[COLUMN_TRUE][i*N_INTERPOLATION]) * (i - i%N_INTERPOLATION))	
	}

	double sum_squareerr = 0.0;
	for(int i=LINE_START; i<=LINE_END; i++){
		sum_squareerr += error[1][i]*error[1][i];
	}
	total_sum_squareerr += sum_squareerr;
	double MSE                   = sum_squareerr / (double)Nlines_within;
	double RMSE                  = std::sqrt(MSE);

	double** results = new double* [2];
	results[0] = new double [Nfiles];
	results[1] = new double [Nfiles];
	results[0][0] = MSE;
	results[1][0] = RMSE;

	save_data(paths[0]+"_error.dat", "#z error", Nlines, 2, error);

	delete[] error;
	delete[] data;

	for(int j=1; j<Nfiles; j++){
		std::string t_header;
		double** data = read_fixed_data(paths[j], t_header, Nlines, Ncolumns);
		double** error = new double*[2];
		error[0] = data[0];
		error[1] = new double[Nlines];

		for(int i=0; i<Nlines; i++){
			error[1][i] = data[COLUMN_DATA][i] - data[COLUMN_TRUE][i];
		}

		double sum_squareerr = 0.0;
		for(int i=LINE_START; i<=LINE_END; i++){
			sum_squareerr += error[1][i]*error[1][i];
		}
		total_sum_squareerr += sum_squareerr;
		double MSE                   = sum_squareerr / (double)Nlines_within;
		double RMSE                  = std::sqrt(MSE);

		results[0][j] = MSE;
		results[1][j] = RMSE;

		save_data(paths[j]+"_error.dat", "#z error", Nlines, 2, error);

		delete[] error;
		delete[] data;
	}
	save_string_and_data("results.dat", "# index from "+std::to_string(index_start)+" to "+std::to_string(index_end)+"\n# filename MSE RMSE", Nfiles, 2, paths, results);
	
	double total_MSE  = total_sum_squareerr / ( (double)Nlines_within * Nfiles );
	double total_RMSE = std::sqrt(total_MSE);
	double* total_result = new double[2];
	total_result[0] = total_MSE;
	total_result[1] = total_RMSE;
	std::string* description = new std::string[2];
	description[0] = "totalMSE";
	description[1] = "totalRMSE";
	save_string_and_data("total_result.dat", "# index from "+std::to_string(index_start)+" to "+std::to_string(index_end), 2, 1, description, &total_result);
	
	return 0;
}
		
