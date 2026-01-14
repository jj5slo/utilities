#include <iostream>
#include <fstream>
#include <string>

#include "readwrite.h"

/* 全ファイルからある列の各行のデータを抽出して、各行を列、ファイル名を行にして保存する。第一列はindexとする */

int main(int argc, char* argv[]){
	std::string PATHS_PATH;
	int PICKUP_COLUMN;
	if(argc == 3){
		PATHS_PATH    = argv[1];
		PICKUP_COLUMN = std::atoi(argv[2]) - 1;
	}
	else{
		std::cerr << "Usage: ./min_max PATHs.txt PICKUP_COLUMN(Number, starting from 1, 1 is index)" << std::endl;
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
	
	/* Pickup a column */
	double** output_arr = new double* [Nlines];
	for(int i=0; i<Nlines; i++){
		output_arr[i] = new double [Nfiles];
	}
	for(int i=0; i<Nfiles; i++){
		for(int j=0; j<Nlines; j++){
			output_arr[j][i] = data_arr[i][PICKUP_COLUMN][j];
		}
	}
	std::string line2col_index = "# ";
	for(int i=0; i<Nlines; i++){
		line2col_index = line2col_index + " " + std::to_string(data_arr[0][0][i]);
	}
		
	save_data("pickuped.dat", line2col_index, Nfiles, Nlines, output_arr);
	return 0;
}
		
