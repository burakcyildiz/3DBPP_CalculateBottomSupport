#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>

using namespace std;

#define BinWidth 1220
#define BinDepth 820
#define BinHeight 2200				
#define LayerHeightTolerance 10


struct calcBin
{
	vector<int> xCoords;
	vector<int> yCoords;
	vector<int> zCoords;
	vector<int> widths;
	vector<int> depths;
	vector<int> heights;

	struct calcBin()
	{

	}

	~calcBin()
	{
		xCoords.clear();
		yCoords.clear();
		zCoords.clear();
		widths.clear();
		depths.clear();
		heights.clear();
	}
};

vector<calcBin> bins;
int nbItems;
vector<int> nbBins;
double minSupport;
int goodSupCt;
int Nbin;

template <typename Container>
Container& split(Container& result,
	const typename Container::value_type& s,
	const typename Container::value_type& delimiters)
{
	result.clear();
	size_t current;
	size_t next = -1;
	do {
		current = next + 1;
		next = s.find_first_of(delimiters, current);
		result.push_back(s.substr(current, next - current));
	} while (next != Container::value_type::npos);
	return result;
}

void parseInput(string fileName, int nbBins)
{
	nbItems = 0;
	//bins.resize(nbBins);

	for (int j = 0; j < nbBins; j++)
	{
		ifstream myfile1;
		myfile1.open(fileName + "currentbin" + to_string(j) + ".txt");

		string line;

		size_t pos = 0;
		string token;
		int tempTime;

		const string delimiter = " ";
		vector<vector<string>> v;
		vector<string> fields;

		//Read data
		for (myfile1; getline(myfile1, line);)
		{
			v.push_back(split(fields, line, delimiter));
		}
		myfile1.close();

		calcBin bin;

		for (int i = 0; i < v.size(); i++)
		{
			bin.widths.push_back(atoi(v.at(i).at(3).c_str()));
			bin.depths.push_back(atoi(v.at(i).at(4).c_str()));
			bin.heights.push_back(atoi(v.at(i).at(5).c_str()));
			bin.xCoords.push_back(atoi(v.at(i).at(0).c_str()));
			bin.yCoords.push_back(atoi(v.at(i).at(1).c_str()));
			bin.zCoords.push_back(atoi(v.at(i).at(2).c_str()));
			nbItems++;
		}
		bins.push_back(bin);
		fields.clear();
		v.clear();
	}
	
}

double calculateBottomSupport(vector<calcBin> binList)
{
	double percentageSupport = 0;
	double minSup = 100;
	int goodSupportCt = 0;

	for (int i = 0; i < binList.size(); i++)
	{
		for (int j = 0; j < binList[i].xCoords.size(); j++)
		{
			double itemSupport = 0;

			if(binList[i].zCoords[j] == 0)
				percentageSupport += 100;
			else
			{
				for (int k = 0; k < binList[i].xCoords.size(); k++)
				{
					if(binList[i].zCoords[j] >= binList[i].zCoords[k] + binList[i].heights[k] && binList[i].zCoords[j] - binList[i].zCoords[k] - binList[i].heights[k] <= LayerHeightTolerance)
					{
						int xDiff = 0;
						int yDiff = 0;

						int x2 = binList[i].xCoords[j];
						int w2 = binList[i].widths[j];
						int x1 = binList[i].xCoords[k];
						int w1= binList[i].widths[k];
						int y2 = binList[i].yCoords[j];
						int d2 = binList[i].depths[j];
						int y1 = binList[i].yCoords[k];
						int d1 = binList[i].depths[k];

						if(x1 + w1 >= x2 && x1 <= x2 && x2 + w2 >= x1 + w1)
							xDiff = x1 + w1 - x2;
						else if(x2 + w2 >= x1 && x2 + w2 <= x1 + w1 && x2 <= x1)
							xDiff = x2 + w2 - x1;
						else if(x2 >= x1 && x2 + w2 <= x1 + w1)
							xDiff = w2;
						else if(x2 <= x1 && x2 + w2 >= x1 + w1)
							xDiff = w1;
						else
							xDiff = 0;

						if(y1 + d1 >= y2 && y1 <= y2 && y2 + d2 >= y1 + d1)
							yDiff = y1 + d1 - y2;
						else if(y2 + d2 >= y1 && y2 + d2 <= y1 + d1 && y2 <= y1)
							yDiff = y2 + d2 - y1;
						else if(y2 >= y1 && y2 + d2 <= y1 + d1)
							yDiff = d2;
						else if(y2 <= y1 && y2 + d2 >= y1 + d1)
							yDiff = d1;
						else
							yDiff = 0;

						itemSupport += xDiff * yDiff;
					}
				}//for (int k = 0; k < binList[i].xCoords.size(); k++)

				percentageSupport += (double)(itemSupport / (binList[i].widths[j] * binList[i].depths[j]))*100;

				if((double)(itemSupport / (binList[i].widths[j] * binList[i].depths[j]))*100 < minSup && (double)(itemSupport / (binList[i].widths[j] * binList[i].depths[j]))*100 > 0)
					minSup = (double)(itemSupport / (binList[i].widths[j] * binList[i].depths[j]))*100;

				if((double)(itemSupport / (binList[i].widths[j] * binList[i].depths[j]))*100.0 > 70.0)
					goodSupportCt++;
			}//else

		}//for (int j = 0; j < binList[i].xCoords.size(); j++)
	}//for (int i = 0; i < binList.size(); i++)

	percentageSupport = percentageSupport / nbItems;
	minSupport = minSup;
	goodSupCt = goodSupportCt;

	if (minSup == 100) minSup = 0;

	return percentageSupport;
}

vector<double> compileResults(vector<calcBin> binList)
{
	/*	•         Volume Usage(based on max allowed load unit height) - average per load unit / per order / per test - set
	•         Pack Density(based on used load unit height) - average per load unit / per order / per test - set
	•         Total number of load units - per order / per test - set*/

	double overallVolUsage = 0;
	double overallPackDensity = 0;

	for (int b = 0; b < binList.size(); b++)
	{
		if (binList[b].widths.size() <= 6)
		{
			binList.erase(binList.begin() + b);
			b--;
			continue;
		}

		double volUsage = 0;
		double packDensity = 0;
		int actualBinHeight = 0;

		for (int i = 0; i < binList[b].xCoords.size(); i++)
		{
			if (binList[b].zCoords[i] + binList[b].heights[i] > actualBinHeight)
				actualBinHeight = binList[b].zCoords[i] + binList[b].heights[i];

			volUsage += binList[b].widths[i] * binList[b].depths[i] * binList[b].heights[i] / 1000;
		}

		overallVolUsage += volUsage * 100 / (122 * 82 * 220);
		overallPackDensity += volUsage * 100 / (122 * 82 * actualBinHeight / 10);
	}

	overallVolUsage /= binList.size();
	overallPackDensity /= binList.size();

	vector<double> results;
	results.push_back(overallVolUsage);
	results.push_back(overallPackDensity);
	results.push_back(binList.size());

	return results;
}


void parseInputMartello(string fileName)
{
	nbItems = 0;
	bins.resize(1);

	ifstream myfile1;
	myfile1.open(fileName);

	string line;

	size_t pos = 0;
	string token;
	int tempTime;

	const string delimiter = "\t";
	vector<vector<string>> v;
	vector<string> fields;

	//Read data
	for (myfile1; getline(myfile1, line);)
	{
		v.push_back(split(fields, line, delimiter));
	}
	myfile1.close();

	calcBin bin;

	for (int i = 0; i < v.size(); i++)
	{
		int binNb = atoi(v.at(i).at(6).c_str());
		if (binNb > bins.size())
			bins.resize(binNb);

		bins[binNb - 1].widths.push_back(atoi(v.at(i).at(3).c_str()));
		bins[binNb - 1].depths.push_back(atoi(v.at(i).at(4).c_str()));
		bins[binNb - 1].heights.push_back(atoi(v.at(i).at(5).c_str()));
		bins[binNb - 1].xCoords.push_back(atoi(v.at(i).at(0).c_str()));
		bins[binNb - 1].yCoords.push_back(atoi(v.at(i).at(1).c_str()));
		bins[binNb - 1].zCoords.push_back(atoi(v.at(i).at(2).c_str()));
		nbItems++;
	}
	fields.clear();
	v.clear();
	Nbin = bins.size();
}
