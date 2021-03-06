#include "stdafx.h"
#include "ColorPalette.h"
#include <iostream>
#include <fstream>
#include <string>

using std::cout;


void ColorPalette::LinearInterpolateEx(double magnitude, int& rValue, int& gValue, int& bValue)
{
	if (magnitude > 1) magnitude = 1;
	if (magnitude < 0) magnitude = 0;

	if (cyclicColors) {
		if (magnitude > 0.5) magnitude = 1 - magnitude; // "reverse" values greater than 5 to make palette cyclic
	}

	double increment = 1 / ((double)numberOfColors);

	// we figure out between which 2 colors of our palette we want to interpolate depending on where the magnitude falls on 0-1
	int color2 = 1;
	while (increment * color2 < magnitude) {
		color2++;
		if (color2 > numberOfColors) {
			color2 = 1;
		}
	}
	// there is some logic error above, this is to safeguard
	if (color2 == numberOfColors) {
		color2 = numberOfColors - 1;
	}
	// at this point we have to have found the color region
	// now we use our linear interpolate formula

	int color1 = color2 - 1;

	double xa = increment * color1;
	double xb = increment * color2;
	double slope = 0;

	double redValue, greenValue, blueValue = 0;

	slope = (magnitude - xa) / (xb - xa);

	redValue = Rvalues[color1] + ((double)Rvalues[color2] - (double)Rvalues[color1]) * slope;
	greenValue = Gvalues[color1] + ((double)Gvalues[color2] - (double)Gvalues[color1]) * slope;
	blueValue = Bvalues[color1] + ((double)Bvalues[color2] - (double)Bvalues[color1]) * slope;

	rValue = static_cast<int>(redValue);
	gValue = static_cast<int>(greenValue);
	bValue = static_cast<int>(blueValue);
}

void ColorPalette::SetPaletteLenght(int l)
{
	// this will repeat out color palette vector by palette lenght
	vector<int> RTemp;
	vector<int> GTemp;
	vector<int> BTemp;

	for (int i = 0; i < l; i++) {
		for (int j = 0; j < numberOfColors; j++) {
			RTemp.push_back(Rvalues[j]);
			GTemp.push_back(Gvalues[j]);
			BTemp.push_back(Bvalues[j]);
		}
	}

	// reset our color vector to temp
	Rvalues.clear();
	Gvalues.clear();
	Bvalues.clear();

	Rvalues = RTemp;
	Gvalues = GTemp;
	Bvalues = BTemp;

	numberOfColors = l * numberOfColors;
}

void ColorPalette::LinearInterpolate(double magnitude, int & rValue, int & gValue, int & bValue)
{
	// make cyclic colors i.e after .5 everything repeats backwards

	if (magnitude > 1) magnitude = 1;
	if (magnitude < 0) magnitude = 0;

	if (magnitude > 0.5) magnitude = 1 - magnitude; // "reverse" values greater than 5 to make palette cyclic

	double xa = 0;
	double xb = .1;
	double slope = 0;

	double redValue, greenValue, blueValue = 0;

	if (magnitude < 0.1) {
		slope = (magnitude - xa) / (xb - xa);

		redValue = Rvalues[0] + ((double)Rvalues[1] - (double)Rvalues[0])*slope;
		greenValue = Gvalues[0] + ((double)Gvalues[1] - (double)Gvalues[0])*slope;
		blueValue = Bvalues[0] + ((double)Bvalues[1] - (double)Bvalues[0])*slope;
	}
	else if (magnitude < 0.2) {
		xa = 0.1;
		xb = 0.2;
		slope = (magnitude - xa) / (xb - xa);

		redValue = Rvalues[1] + ((double)Rvalues[2] - (double)Rvalues[1])*slope;
		greenValue = Gvalues[1] + ((double)Gvalues[2] - (double)Gvalues[1])*slope;
		blueValue = Bvalues[1] + ((double)Bvalues[2] - (double)Bvalues[1])*slope;
	}
	else if (magnitude < 0.3) {
		xa = 0.2;
		xb = 0.3;
		slope = (magnitude - xa) / (xb - xa);

		redValue = Rvalues[2] + ((double)Rvalues[3] - (double)Rvalues[2])*slope;
		greenValue = Gvalues[2] + ((double)Gvalues[3] - (double)Gvalues[2])*slope;
		blueValue = Bvalues[2] + ((double)Bvalues[3] - (double)Bvalues[2])*slope;
	}
	else if (magnitude < 0.4) {
		xa = 0.3;
		xb = 0.4;
		slope = (magnitude - xa) / (xb - xa);

		redValue = Rvalues[3] + ((double)Rvalues[4] - (double)Rvalues[3])*slope;
		greenValue = Gvalues[3] + ((double)Gvalues[4] - (double)Gvalues[3])*slope;
		blueValue = Bvalues[3] + ((double)Bvalues[4] - (double)Bvalues[3])*slope;
	}
	else {
		xa = 0.4;
		xb = 0.5;
		slope = (magnitude - xa) / (xb - xa);

		redValue = Rvalues[4] + ((double)Rvalues[0] - (double)Rvalues[4])*slope;
		greenValue = Gvalues[4] + ((double)Gvalues[0] - (double)Gvalues[4])*slope;
		blueValue = Bvalues[4] + ((double)Bvalues[0] - (double)Bvalues[4])*slope;
	}

	rValue = static_cast<int>(redValue);
	gValue = static_cast<int>(greenValue);
	bValue = static_cast<int>(blueValue);

}

int ColorPalette::GetRandomColor()
{
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(0, 257);

	return (int)(floor(dis(rd)));
}

void ColorPalette::EnterPalette(vector<int> red, vector<int> green, vector<int> blue)
{
	for (int i = 0; i < 5; i++) {
		Rvalues[i] = red[i];
		Gvalues[i] = green[i];
		Bvalues[i] = blue[i];
	}
}

bool ColorPalette::SavePaletteToFile(std::string fileName)
{
	std::ofstream outFile;

	std::string filePath = paletteDir+fileName + ".txt";

	outFile.open(filePath);

	if (outFile.is_open()) {

		outFile << "Number of Colors: " << std::endl;
		outFile << numberOfColors << std::endl;

		outFile << "Red Values: " << std::endl;

		for (int i = 0; i < numberOfColors; i++) {
			outFile << Rvalues[i] << std::endl;
		}

		outFile << "Green Values: " << std::endl;

		for (int i = 0; i < numberOfColors; i++) {
			outFile << Gvalues[i] << std::endl;
		}

		outFile << "Blue Values: " << std::endl;

		for (int i = 0; i < numberOfColors; i++) {
			outFile << Bvalues[i] << std::endl;
		}

		outFile.close();
	}
	else
	{
		cout << "Could not open file" << std::endl;
		return false;
	}
	return true;
}

bool ColorPalette::LoadPaletteFromFile(std::string fileName)
{
	std::ifstream inFile;

	std::string filePath = paletteDir + fileName + ".txt";

	inFile.open(filePath);

	std::string line;

	if (inFile.is_open()) {

		Rvalues.clear();
		Gvalues.clear();
		Bvalues.clear();

		getline(inFile, line);
		inFile >> numberOfColors;

		getline(inFile, line);
		getline(inFile, line);

		for (int i = 0; i < numberOfColors; i++) {
			int val;
			inFile >> val;
			Rvalues.push_back(val);
		}

		getline(inFile, line);
		getline(inFile, line);

		for (int i = 0; i < numberOfColors; i++) {
			int val;
			inFile >> val;
			Gvalues.push_back(val);
		}

		getline(inFile, line);
		getline(inFile, line);
		
		for (int i = 0; i < numberOfColors; i++) {
			int val;
			inFile >> val;
			Bvalues.push_back(val);
		}
		
		inFile.close();
		return true;
	}
	else return false;
}

void ColorPalette::GenerateRandomColorPalette()
{
	for (int i = 0; i < numberOfColors; i++) {
		Rvalues.push_back(GetRandomColor());
		Gvalues.push_back(GetRandomColor());
		Bvalues.push_back(GetRandomColor());
	}
}
