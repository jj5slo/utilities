#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "readwrite.h"

/* 一致した場合に全ファイルの平均をとる */

double threshold = 0.1;

int main(int argc, char* argv[]){
	std::string PATHS_PATH;
	int NUMBEROF_MATCHING_COLUMNS;
	int* MATCHING_COLUMNS;
	int COLUMN;
	if(argc >= 3){
		PATHS_PATH = argv[1];
		NUMBEROF_MATCHING_COLUMNS = std::atoi(argv[2]);
	}
	else{
		std::cerr << "Usage: ./mean_column PATHS_LIST NUMBEROF_MATCHING_COLUMNS MATCHING_COLUMN(1start) MATCHING_COLUMN ... MATCHING_COLUMN COLUMN(1start)" << std::endl;
		return 1;
	}
	if(argc >= 4+NUMBEROF_MATCHING_COLUMNS){
		MATCHING_COLUMNS = new int [NUMBEROF_MATCHING_COLUMNS];
		for(int i=0; i<NUMBEROF_MATCHING_COLUMNS; ++i){
			MATCHING_COLUMNS[i] =	std::atoi(argv[3+i]) - 1;
		}
		COLUMN = std::atoi(argv[3+NUMBEROF_MATCHING_COLUMNS]) - 1;
	}
	else{
		std::cerr << "Usage: ./mean_column PATHS_LIST NUMBEROF_MATCHING_COLUMNS MATCHING_COLUMN(1start) MATCHING_COLUMN ... MATCHING_COLUMN COLUMN(1start)" << std::endl;
		return 1;
	}


	int Nfiles = 0;
	std::string* paths = readwrite::read_list(PATHS_PATH, Nfiles);

	double*** data_arr = new double** [Nfiles];
	std::string header_0;
	int Nlines;
	int Ncolumns;
	data_arr[0] = readwrite::read_data(paths[0], header_0, Nlines, Ncolumns);

	double** mean = new double*[Ncolumns];
	for(int i=0; i<Ncolumns; ++i){
		mean[i] = new double [Nlines];
		for(int j=0; j<Nlines; ++j){
			mean[i][j] = data_arr[0][i][j];
		}
	}
			

	int* t_Nlines = new int [Nfiles - 1];
	int* t_Ncolumns = new int [Nfiles - 1];
	for(int i=1; i<Nfiles; i++){
		std::string t_header;
		data_arr[i] = readwrite::read_data(paths[i], t_header, t_Nlines[i-1], t_Ncolumns[i-1]);
	}

	for(int j=0; j<Nlines; ++j){
		int match_count = 0;
		double sum = 0.0;
		for (int f=1; f<Nfiles; ++f){
			for(int ii=0; ii<t_Nlines[f-1]; ++ii){
				int flag = 1;
				for(int i_if=0; i_if<NUMBEROF_MATCHING_COLUMNS; ++i_if){
					if(std::abs(data_arr[f][MATCHING_COLUMNS[i_if]][ii] - mean[MATCHING_COLUMNS[i_if]][j]) >= threshold){
						flag = 0;
						break;
					}
				}
				if(flag){
					sum = sum + data_arr[f][COLUMN][ii];
					match_count = match_count + 1;
				}
			}
		}
		mean[COLUMN][j] = (mean[COLUMN][j] + sum) / (match_count + 1.0);
	}
	

	std::string str_matching_columns = "";
	for(int i=0; i<NUMBEROF_MATCHING_COLUMNS; ++i){
		str_matching_columns += std::to_string(MATCHING_COLUMNS[i]+1) + " ";
	}
		
	readwrite::save_data("mean_match.dat", "#mean from "+PATHS_PATH+"\n"+"#column_match: "+str_matching_columns+"\n"+"meaned_column: "+std::to_string(COLUMN+1)+"\n", Nlines, Ncolumns, mean);

	delete[] paths;
	delete[] MATCHING_COLUMNS;
	for(int i=0; i<Nfiles; ++i){
		AndoLab::deallocate_memory2d(data_arr[i]);
	}
	delete[] data_arr;
	for(int i=0; i<Ncolumns; ++i){
		delete[] mean[i];
	}
	delete[] mean;
	delete[] t_Nlines;
	delete[] t_Ncolumns;

	return 0;
}
		
