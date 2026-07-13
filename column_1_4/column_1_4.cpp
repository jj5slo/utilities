#include <string>

#include "readwrite.h"


int main(int argc, char* argv[]){
	std::string path = "";
	if(argc == 2){
		path = std::string(argv[1]);
	}
	else{
		return 1;
	}
		
	std::string header;
	int Nlines;
	int Ncolumns;

	double** data = readwrite::read_data(path, header, Nlines, Ncolumns);
	double** ret = new double* [2];
	ret[0] = data[0];
	ret[1] = data[3];

	readwrite::save_data(path+"_1_4.dat", header, 101, 2, ret);

	delete[] ret;
	AndoLab::deallocate_memory2d(data);
	return 0;
}
