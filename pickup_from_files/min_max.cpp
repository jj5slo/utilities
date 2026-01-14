#include <iostream>
#include <fstream>
#include <string>

#include "readwrite.h"

/* 全ファイル中での各行各列の最大最小を探す */

int main(int argc, char* argv[]){
	std::string PATHS_PATH;
	int Number_of_index_columns;
	if(argc == 3){
		PATHS_PATH = argv[1];
		Number_of_index_columns = std::atoi(argv[2]);
	}
	else{
		std::cerr << "Usage: ./min_max PATHs.txt NUMBER_OF_INDEX_COLUMNS" << std::endl;
		return 1;
	}
	
	int Nfiles = 0;
	std::string* paths = read_list(PATHS_PATH, Nfiles);

	double*** data_arr = new double** [Nfiles];
	std::string header_0;
	int Nlines;
	int Ncolumns;
	data_arr[0] = read_data(paths[0], header_0, Nlines, Ncolumns);
	for(int i=1; i<Nfiles; i++){
		std::string t_header;
		data_arr[i] = read_fixed_data(paths[i], t_header, Nlines, Ncolumns);
	}

	/* Search min, max */
	/* data[COL][LINE] */
	/* Allocate_memory すればキャッシュで高速化が期待できる */
	double** mins = new double* [Ncolumns];
	int** file_index_mins = new int* [Ncolumns];
	double** maxs = new double* [Ncolumns];
	int** file_index_maxs = new int* [Ncolumns];
	for(int j=0; j<Number_of_index_columns; j++){
		mins[j] = new double[Nlines];
		maxs[j] = new double [Nlines];
		file_index_mins[j] = new int [Nlines];
		file_index_maxs[j] = new int [Nlines];
		for(int i=0; i<Nlines; i++){
			mins[j][i] = data_arr[0][j][i];
			file_index_mins[j][i] = data_arr[0][j][i];
			maxs[j][i] = data_arr[0][j][i];
			file_index_maxs[j][i] = data_arr[0][j][i];
		}
	}
	for(int j=Number_of_index_columns; j<Ncolumns; j++){
		mins[j] = new double [Nlines];
		maxs[j] = new double [Nlines];
		file_index_mins[j] = new int [Nlines];
		file_index_maxs[j] = new int [Nlines];
		for(int i=0; i<Nlines; i++){
			double minv = data_arr[0][j][i];
			int file_index_min = 0;
			double maxv = data_arr[0][j][i];
			int file_index_max = 0;
			for(int k=1; k<Nfiles; k++){
				if(data_arr[k][j][i] < minv){
					minv = data_arr[k][j][i];
					file_index_min = k;
				}
				if(maxv < data_arr[k][j][i]){
					maxv = data_arr[k][j][i];
					file_index_max = k;
				}
			}
			mins[j][i] = minv;
			file_index_mins[j][i] = file_index_min;
			maxs[j][i] = maxv;
			file_index_maxs[j][i] = file_index_max;
		}
	}
	
	save_data("min.dat", "#min from "+PATHS_PATH+"\n", Nlines, Ncolumns, mins);
	save_data("min_file_i.dat", "#min file index in "+PATHS_PATH+"\n", Nlines, Ncolumns, file_index_mins);
	save_data("max.dat", "#min from "+PATHS_PATH+"\n", Nlines, Ncolumns, maxs);
	save_data("max_file_i.dat", "#min file index in "+PATHS_PATH+"\n", Nlines, Ncolumns, file_index_maxs);
}
		
