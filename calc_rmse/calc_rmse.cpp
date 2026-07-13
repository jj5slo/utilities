/*
 * calc_rmse.cpp
 * 温度同定結果、msis、auraからRMSE（いまほしいやつ）を計算する
 *
 */

#include <iostream>
#include <cmath>

#include "readwrite.h"
#include "linear_interpolation.h"

constexpr double threshold = 0.1;/* km */

int main(int argc, char* argv[]){
	std::string tempfile = "";
	std::string msisfile = "";
	std::string aurafile = "";
	int topheight    = 0;
	int bottomheight = 0;
	if(argc == 6){
		tempfile = std::string(argv[1]);
		msisfile = std::string(argv[2]);
		aurafile = std::string(argv[3]);
		topheight    = std::stod(argv[4]);
		bottomheight = std::stod(argv[5]);
	}else{
		std::cerr << "Usage ./calc_rmse TEMP.dat MSIS.dat AURA.dat TOPaltitude BOTTOMaltitude" << std::endl;
		return 1;
	}
	int heightrange = bottomheight - topheight + 1;
	if(heightrange <= 0){
		std::cerr << "altitude values invalid!" << std::endl;
		return 1;
	}

	/* 読み込み */
	int Nlines   = 0;
	int Ncolumns = 0;
	std::string header = "";
	double** tempdata = readwrite::read_data(tempfile, header, Nlines, Ncolumns);
	double** msisdata = readwrite::read_data(msisfile, header, Nlines, Ncolumns);
	int aura_Nlines   = 0;
	int aura_Ncolumns = 0;
	double** auradata = readwrite::read_data(aurafile, header, aura_Nlines, aura_Ncolumns);
	double** auradata_temp = new double* [2];
	auradata_temp[0] = auradata[2];
	auradata_temp[1] = auradata[3];

	/* 温度を集積 */
	double** gathered_temperature = new double* [4];
	for(int i=0; i<4; ++i){
		gathered_temperature[i] = new double[heightrange];
	}
	for(int i=0; i<heightrange; ++i){
		gathered_temperature[0][i] = (double)(bottomheight+i);
		gathered_temperature[1][i] = 0.0;/* retrieved */
		gathered_temperature[2][i] = 0.0;/* msis */
		gathered_temperature[3][i] = 0.0;/* aura */
	}

	for(int i=0; i<heightrange; ++i){
		for(int j=0; j<Nlines; ++j){
			if(std::abs(tempdata[0][j] - gathered_temperature[0][i]) < threshold){
				gathered_temperature[1][i] = tempdata[2][j];
				break;
			}
		}
		for(int j=0; j<Nlines; ++j){
			if(std::abs(msisdata[0][j] - gathered_temperature[0][i]) < threshold){
				gathered_temperature[2][i] = msisdata[2][j];
				break;
			}
		}
		gathered_temperature[3][i] = linear_interpolation(auradata_temp, aura_Nlines, gathered_temperature[0][i]);
	}

	/* RMSE [K/km] を計算（積算） */
	double RMSE_temp_aura = 0.0;
	double RMSE_temp_msis = 0.0;
	double RMSE_aura_msis = 0.0;
	for(int i=0; i<heightrange; ++i){
		RMSE_temp_aura += (gathered_temperature[1][i] - gathered_temperature[3][i])*(gathered_temperature[1][i] - gathered_temperature[3][i]);
		RMSE_temp_msis += (gathered_temperature[1][i] - gathered_temperature[2][i])*(gathered_temperature[1][i] - gathered_temperature[2][i]);
		RMSE_aura_msis += (gathered_temperature[3][i] - gathered_temperature[2][i])*(gathered_temperature[3][i] - gathered_temperature[2][i]);
	}
	RMSE_temp_aura = RMSE_temp_aura / (double)heightrange;
	RMSE_temp_aura = std::sqrt(RMSE_temp_aura);
	RMSE_temp_msis = RMSE_temp_msis / (double)heightrange;
	RMSE_temp_msis = std::sqrt(RMSE_temp_msis);
	RMSE_aura_msis = RMSE_aura_msis / (double)heightrange;
	RMSE_aura_msis = std::sqrt(RMSE_aura_msis);
	double** rmse_result = new double*[3];
	rmse_result[0] = new double[1];
	rmse_result[1] = new double[1];
	rmse_result[2] = new double[1];
	rmse_result[0][0] = RMSE_temp_aura;
	rmse_result[1][0] = RMSE_temp_msis;
	rmse_result[2][0] = RMSE_aura_msis;

	/* 保存 */
	readwrite::save_data(tempfile+"_gathered_temp.dat", "# altitude retrieved msis aura\n", heightrange, 4, gathered_temperature);
	readwrite::save_data(tempfile+"_RMSE.dat", "# altitude"+std::to_string(topheight)+" to "+std::to_string(bottomheight)+"\n# temp_aura temp_msis aura_msis\n", 1, 3, rmse_result);

	
	delete[] auradata_temp;
	AndoLab::deallocate_memory2d(tempdata);
	AndoLab::deallocate_memory2d(msisdata);
	AndoLab::deallocate_memory2d(auradata);
	for(int i=0; i<4; ++i){
		delete[] gathered_temperature[i];
	}
	delete[] gathered_temperature;
	delete[] rmse_result[0];
	delete[] rmse_result[1];
	delete[] rmse_result[2];
	delete[] rmse_result;

	return 0;
}
