#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "RawdataParser.h"

const int BUFFER_SIZE_INT = 1;

FILE *in=0;
FILE *out=0;
int n;

unsigned int rawdata_before_two = 0;
unsigned int rawdata_before = 0;
unsigned int rawdata = 0;

bool isRawPedestal = false;
bool isPedestal = false;
bool isZS = false;
unsigned int numEvents = 0;
std::string fileName = "";
std::string pedestalName = "";
unsigned long counter = 0;
int eventNr = 0;
int limit = 0;
float zsCut = 0;
bool commonModeOn = true;
bool clusteringOn = true;
int mappingType = 0;

bool pFound = false;
bool rdFound = false;
bool rpFound = false;
bool commonModeFound = false;
bool clusteringFound = false;
bool zsCutFound = false;
bool xFound = false;
bool yFound = false;

std::vector<int> xChips;
std::vector<int> yChips;

int printUsage(std::string errorMessage);

int main(int argc, char**argv)
{
	if (argc == 1 || argc % 2 == 0)
	{
		return printUsage("Wrong number of arguments!");
	}
	for (int i = 1; i < argc; i += 2)
	{
		if (strncmp(argv[i], "-rd", 3) == 0)
		{
			rdFound = true;
			fileName = argv[i + 1];
		}
		else if (strncmp(argv[i], "-rp", 3) == 0)
		{
			rpFound = true;
			isRawPedestal = true;
			fileName = argv[i + 1];
		}
		else if (strncmp(argv[i], "-p", 2) == 0)
		{
			pFound = true;
			pedestalName = argv[i + 1];
		}
		else if (strncmp(argv[i], "-zsc", 4) == 0)
		{
			zsCutFound = true;
			zsCut = atof(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-cm", 3) == 0)
		{
			commonModeFound = true;
			commonModeOn = atoi(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-clu", 4) == 0)
		{
			clusteringFound = true;
			clusteringOn = atoi(argv[i + 1]);

		}
		else if (strncmp(argv[i], "-map", 4) == 0)
		{
			mappingType = atoi(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-n", 2) == 0)
		{
			numEvents = atoi(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-x", 2) == 0)
		{
			xFound = true;
			std::string xString = argv[i + 1];
			std::string delims = ",";
			size_t lastOffset = 0;

			while (true)
			{
				size_t offset = xString.find_first_of(delims, lastOffset);
				xChips.push_back(
						atoi(
								xString.substr(lastOffset, offset - lastOffset).c_str()));
				if (offset == std::string::npos)
				{
					break;
				}
				else
				{
					lastOffset = offset + 1; // add one to skip the delimiter
				}
			}

		}
		else if (strncmp(argv[i], "-y", 2) == 0)
		{
			yFound = true;
			std::string yString = argv[i + 1];
			std::string delims = ",";
			size_t lastOffset = 0;

			while (true)
			{
				size_t offset = yString.find_first_of(delims, lastOffset);
				yChips.push_back(
						atoi(
								yString.substr(lastOffset, offset - lastOffset).c_str()));
				if (offset == std::string::npos)
				{
					break;
				}
				else
				{
					lastOffset = offset + 1; // add one to skip the delimiter
				}
			}

		}
		else
		{
			return printUsage("Wrong type of argument!");
		}
	}

	if (rpFound && pFound)
	{
		return printUsage("Wrong combination of arguments!");

	}
	if (rdFound && rpFound)
	{
		return printUsage("Wrong combination of arguments!");

	}
	
	if (!rdFound && commonModeFound)
	{
		return printUsage("Wrong combination of arguments!");

	}
	if (!rdFound && clusteringFound)
	{
		return printUsage("Wrong combination of arguments!");

	}
	if (!rdFound && zsCutFound)
	{
		return printUsage("Wrong combination of arguments!");

	}
	if (zsCutFound && zsCut < 0)
	{
		return printUsage("Wrong combination of arguments!");
	}

	if (fileName.find(".raw") == std::string::npos)
	{
		return printUsage("Wrong extension: .raw file required!");
	}
	if (pFound && pedestalName.find(".root") == std::string::npos)
	{
		return printUsage("Wrong extension: .root file required!");
	}
	if (rdFound && !pFound)
	{
		isZS = true;
	}

	time_t start = time(0);

	if (isRawPedestal)
	{
		limit = 2;
	}
	else
	{
		limit = 1;
	}
	RawdataParser *parser = 0;
	for (int z = 0; z < limit; z++)
	{
		eventNr = 0;
		counter = 0;
		rawdata_before_two = 0;
		rawdata_before = 0;
		rawdata = 0;
		in = fopen(fileName.c_str(), "rb");
		if (!in)
		{
			std::stringstream message;
			message << "File " << fileName << " does not exist!" << std::endl;
			return printUsage(message.str());
		}
		if (z == 0)
		{
		
			parser = new RawdataParser(fileName, pedestalName, isRawPedestal,
					isPedestal, isZS, zsCut, commonModeOn, clusteringOn, xChips, yChips, mappingType);


		}
		else
		{
			
			isPedestal = true;
			isRawPedestal = false;
			parser->SetRunFlags(isRawPedestal, isPedestal);
		}

		if (in)
		{
			rewind(in);
			while (!feof(in) && (eventNr <= numEvents || numEvents == 0))
			{
				rawdata_before_two = rawdata_before;
				rawdata_before = rawdata;
			
				n = fread(&rawdata, sizeof(unsigned int), BUFFER_SIZE_INT, in);
				counter += n;

				eventNr = parser->AnalyzeWord(rawdata, rawdata_before,
						rawdata_before_two);
	
				if (eventNr < 0)
				{
					fclose(in);
					delete parser;
					return -1;
				}
			}

			time_t end = time(0);
			unsigned int duration = difftime(end, start);

			printf("\n%ld kB, %ld words read from library in %d s.\n",
					(long int) (counter * 4 / 1024), counter, duration);
			int cnt=0;
			if(feof(in))
			{
				cnt = eventNr;
			}
			else
			{
				cnt = eventNr-1;
			}
			if (isRawPedestal)
			{
				printf("RawPedestal Events %d\n\n", cnt);
			}
			else if (isPedestal)
			{
				printf("Pedestal Events %d\n\n", cnt);
			}
			else
			{
				limit = 0;
				printf("Events %d\n", cnt);
			}
			fclose(in);
			in = 0;

		}
		else
		{
			printf("fail\n");
		}
		parser->CreateHistograms();

	}

	delete parser;

	return 0;

}

int printUsage(std::string errorMessage)
{
	std::cout << "\nERROR: " << errorMessage << std::endl;

	printf("\nUsages:\n");
	printf(
			"1a) analyse non zero-suppressed pedestal data:\n\tapv2root -rp pedestal.raw\n");
	printf(
			"1b) analyse non zero-suppressed data and subtract pedestal:\n\tapv2root -rd data.raw -p pedestal.root -zsc ZSCut -cm 1 -x 0,1 -y 2,3 [-n events] [-cl 1]\n");
	printf(
			"2) analyse zero-suppressed data:\n\tapv2root -rd data.raw -x 0,1 -y 2,3 [-n events] [-cl 0]\n");

	printf("\nFlags:\n");
	printf(
			"-rd: raw data file with the extension .raw\n\tUsed for non-zero suppressed and zero suppressed data\n\tThe data file was created by DATE.\n");
	printf(
			"-rp: raw pedestal file with the extension .raw\n\tUsed only for non-zero suppressed data.\n\tThe pedestal file was created by DATE.\n");
	printf(
			"-p: pedestal root file with the extension .root\n\tUsed only for non-zero suppressed data.\n\tThis file has to be created with apv2root in step 1a)\n");
	printf(
			"-zsc: zero-supression cut for non-zero supressed data.\n\tA threshold is calculated by multiplying the pedestal noise with the cut value for each strip.\n\t"
					"Per event it is checked whether the mean strip ADC is over the threshold, in which case the strip is included in the data.\n");
	printf(
			"-cm: common_mode subtraction for non-zero supressed data.\n\tFor the common mode subtraction, for each time bin the avearage ADC value from all strips is calculated. This average is then subtracted from the time bin ADC value of the strip that is being analyzed.");
	
	printf("-cl: Add clustering.\n\n");
	printf("-x: mapping of chips, list of chips in x direction separated by comma\n\n");
	printf("-y: mapping of chips, list of chips in y direction separated by comma\n\n");
	printf("-map: mapping of detector strips to APV channels: 0 [APV mapping, default], 1 [CMS mapping], 2 [CMS inverted mapping]\n\n");

	return -1;
}

