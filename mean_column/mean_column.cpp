#include <iostream>
#include <fstream>
#include <string>

#include "readwrite.h"

/* 各要素の全ファイルの平均をとる */

int main(int argc, char* argv[]){
	std::string PATH;
	int COLUMN;
	if(argc == 3){
		PATH = argv[1];
		COLUMN = std::atoi(argv[2]) - 1;
	}
	else{
		std::cerr << "Usage: ./mean_column PATH COLUMN(1start)" << std::endl;
		return 1;
	}
	
	int Nlines;
	int Ncolumns;
	std::string header;
	double** data = readwrite::read_data(PATH, header, Nlines, Ncolumns);
	
	double sum = 0.0;
	for(int i=0; i<Nlines; i++){
		sum = sum + data[COLUMN][i];
	}
	
	std::cout << "mean of column " << COLUMN <<" is: " << sum / (double)Nlines << std::endl;
			
}
		
