#include "common.h"
#include <iostream>



int main()
{
	vector<string> filesToRead;

	ifstream myfile1;
	myfile1.open("filesToRead.txt");

	ofstream overallcompresults;
	overallcompresults.open("Computational Results.txt");

	overallcompresults << "Instance\tNb of Pallets\tCPU Time\tCPU Time per Pallet\tAverage Volume Usage\tAverage Pack Density" << endl;

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

	for (int i = 0; i < v.size(); i++)
	{
		filesToRead.push_back(v.at(i).at(0).c_str());
	}

	string filename;

	v.clear();
	fields.clear();

	for (int q = 0; q < filesToRead.size(); q++)
	{
		cout << "Processing file" << q + 1 << "..." << endl;
		string filename = filesToRead[q];

		char solStats[200];
		sprintf(solStats, filename.c_str());
		sprintf(solStats + strlen(solStats), "SolutionStats.txt");

		ifstream myfile2;
		myfile2.open(solStats);

		string line;

		size_t pos = 0;
		string token;
		int tempTime;

		const string delimiter = "\t";
		vector<vector<string>> v;
		vector<string> fields;

		//Read data
		for (myfile2; getline(myfile2, line);)
		{
			v.push_back(split(fields, line, delimiter));
		}
		myfile2.close();

		int nBins;
		double cpuTime;
		/*int nbLayers;
		int leftover;*/

	


		nBins = atoi(v.at(0).at(2).c_str());
		cpuTime = atof(v.at(0).at(1).c_str());
		/*nbLayers = atoi(v.at(0).at(3).c_str());
		leftover = atoi(v.at(0).at(4).c_str());*/
		
		v.clear();
		fields.clear();

		parseInput(filename, nBins);
		//parseInputMartello(filename);
		vector<double> results = compileResults(bins);

		overallcompresults << filename << "\t" << results[2] << "\t" << cpuTime << "\t" << cpuTime / results[2] << "\t" << results[0] << "\t" << results[1] << endl;

		bins.clear();
	}

	filesToRead.clear();

	return 0;
}