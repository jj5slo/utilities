#include <iostream>
#include <fstream>
#include <string>

#include "readwrite.h"

/* 誤差を計算して保存する */

int main(int argc, char* argv[]){
	std::string PATHS_PATH;
	int COLUMN_TRUE;
	int COLUMN_DATA;
	if(argc == 4){
		PATHS_PATH = argv[1];
		COLUMN_TRUE = std::atoi(argv[2]) - 1;
		COLUMN_DATA = std::atoi(argv[3]) - 1;
	}
	else{
		std::cerr << "Usage: ./error PATHs.txt COLUMN_TRUE COLUMN_DATA(1 start)" << std::endl;
		return 1;
	}
	
	int Nfiles = 0;
	std::string* paths = read_list(PATHS_PATH, Nfiles);

	std::string header_0;
	int Nlines;
	int Ncolumns;
	double** data = read_data(paths[0], header_0, Nlines, Ncolumns);
	double** error = new double*[2];
	error[0] = data[0];
	error[1] = new double[Nlines];

	for(int i=0; i<Nlines; i++){
		error[1][i] = data[COLUMN_DATA][i] - data[COLUMN_TRUE][i];
	}
	save_data(paths[0]+"_error.dat", "#z error", Nlines, 2, error);

	delete error;
	delete data;


	for(int j=1; j<Nfiles; j++){
		std::string t_header;
		double** data = read_data(paths[j], t_header, Nlines, Ncolumns);
		double** error = new double*[2];
		error[0] = data[0];
		error[1] = new double[Nlines];

		for(int i=0; i<Nlines; i++){
			error[1][i] = data[COLUMN_DATA][i] - data[COLUMN_TRUE][i];
		}
		save_data(paths[j]+"_error.dat", "#z error", Nlines, 2, error);

		delete error;
		delete data;
	}
	return 0;
}
		
