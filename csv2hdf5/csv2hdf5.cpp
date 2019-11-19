#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include <chrono>
#include <gdgem/nmx/Readout.h>

int printUsage(std::string errorMessage, char* argv);

int main(int argc, char**argv) {

	std::shared_ptr<Gem::ReadoutFile> readout_file_;
  	Gem::Readout readout;


	std::chrono::time_point<std::chrono::system_clock> timeEnd, timeStart;
	std::string pExperiment = "Budapest";
	std::string pFilename = "";
	int nLines = 0;
	float pBC = 20.0;
	float pTAC = 100.0;
	bool fFound = false;
	
	if (argc == 1 || argc % 2 == 0) {
		return printUsage("Wrong number of arguments!", argv[argc - 1]);
	}
	for (int i = 1; i < argc; i += 2) {
		if (strncmp(argv[i], "-f", 2) == 0) {
			fFound = true;
			pFilename = argv[i + 1];
			std::cout << pFilename << std::endl;
		} else if (strncmp(argv[i], "-bc", 3) == 0) {
			pBC = atof(argv[i + 1]);
		} else if (strncmp(argv[i], "-tac", 4) == 0) {
			pTAC = atof(argv[i + 1]);
		} else if (strncmp(argv[i], "-ex", 3) == 0) {
			pExperiment = argv[i + 1];
		} else if (strncmp(argv[i], "-n", 2) == 0) {
			nLines = atoi(argv[i + 1]);
		} else {
			return printUsage("Wrong type of argument!", argv[i]);
		}
	}

	if (!fFound) {
		return printUsage("Data file has to be loaded with -f data.csv!", nullptr);

	}

	if (fFound && pFilename.find(".csv") == std::string::npos) {
		return printUsage("Wrong extension: .csv file required!", nullptr);
	}

	timeStart = std::chrono::system_clock::now();
	
	std::ifstream inFile(pFilename.c_str());
	if (!inFile) {
		std::stringstream message;
		message << "File " << pFilename << " does not exist!" << std::endl;
		return printUsage(message.str(), nullptr);
	} else {
			std::cout << "Analysing " << pFilename << std::endl;
	}

	pFilename.replace(pFilename.size() - 4, pFilename.size(), "");
	readout_file_ = Gem::ReadoutFile::create(pFilename);
	
	uint16_t fec, vmmId, chNo, bcid, tdc, adc, overthreshold;
	uint32_t udp_timestamp, frameCounter;
	uint64_t srs_timestamp;
	uint16_t triggerOffset;
	double timestamp_ns;
	float chiptime;
	
	
	//(40132) {1, 4, 473811421, 23, 2136, 121, 146, 1, 106803}, {1, 4, 473811421, 46, 2767, 171, 106, 1, 138333}, {1, 4, 473811421, 46, 3380, 175, 112, 1, 168981},
    //    (40135) {1, 4, 474016221, 46, 1022, 185, 109, 1, 51077.4},
        
        
	//Budapest
	//3153854464, 1081164, 4141271517, 6, 2787280248742.451172, 1, 5, 25, 3387, 172, 381, 1
	//Grenoble
	//2709258240, 3582902, 1930840541, 2, 1, 6, 30, 1400, 132, 32, 0, 70001.015625
	
	int cnt[16] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int i = 0;
	int nSeparator = 0;
	if (inFile.is_open()) {
		std::string line;
		while (inFile.good() && std::getline(inFile, line)) {
			if (line.at(0) != '#') {
				
				nSeparator = count(begin(line), end(line), ',');
				std::stringstream iss(line);
				char ch;
				if (nSeparator != 11) {
					std::cout << "Wrong file format in csv file!" << std::endl;
					break;
				}
				
				if(i >= nLines && nLines > 0) {
					break;
				}
				i++;
				if(pExperiment == "budapest" || pExperiment == "Budapest") {
					iss >> udp_timestamp;
					iss >> ch;
					iss >> frameCounter;
					iss >> ch;
					iss >> srs_timestamp;
					iss >> ch;
					iss >> triggerOffset;
					iss >> ch;
					iss >> timestamp_ns;
					iss >> ch;
					iss >> fec;
					iss >> ch;
					iss >> vmmId;
					iss >> ch;
					iss >> chNo;
					iss >> ch;
					iss >> bcid;
					iss >> ch;
					iss >> tdc;
					iss >> ch;
					iss >> adc;
					iss >> ch;
					iss >> overthreshold;
					chiptime = static_cast<float>(1000.0*(static_cast<float>(bcid)+1.0)/pBC - static_cast<float>(tdc)*pTAC/255.0);
					//std::cout << bcid << " " << tdc << " " << pBC << " " << pTAC << " " << chiptime << std::endl;
					//printf("%f\n", chiptime);
					
				} else if (pExperiment == "grenoble" || pExperiment == "Grenoble") {
					iss >> udp_timestamp;
					iss >> ch;
					iss >> frameCounter;
					iss >> ch;
					iss >> srs_timestamp;
					iss >> ch;
					iss >> triggerOffset;
					iss >> ch;
					iss >> fec;
					iss >> ch;
					iss >> vmmId;
					iss >> ch;
					iss >> chNo;
					iss >> ch;
					iss >> bcid;
					iss >> ch;
					iss >> tdc;
					iss >> ch;
					iss >> adc;
					iss >> ch;
					iss >> overthreshold;
					iss >> ch;
					iss >> chiptime;
				}
				if(i%1000000 == 0) std::cout << i << std::endl;
				readout.srs_timestamp = srs_timestamp;
      			readout.chip_id = vmmId;
      			readout.channel = chNo;
      			readout.fec = fec;
      			readout.bcid = bcid;
      			readout.tdc = tdc;
      			readout.over_threshold = (overthreshold != 0);
            	readout.chiptime = chiptime;
       			readout.adc = adc;
				if (readout_file_) {
        			readout_file_->push(readout);
        		}
   			}
		}
		
		inFile.close();
		readout_file_->flush();
		std::cout << "Lines in csv file: " << i << std::endl;
		timeEnd = std::chrono::system_clock::now();

		int elapsed_seconds = std::chrono::duration_cast < std::chrono::milliseconds > (timeEnd - timeStart).count();
		std::cout << "finished computation in " << elapsed_seconds << " ms\n";
	}
	return 0;
}

int printUsage(std::string errorMessage, char* argv) {
	if (argv != nullptr) {
		std::cout << "\nERROR: " << errorMessage << ": " << argv << std::endl;
	} else {
		std::cout << "\nERROR: " << errorMessage << std::endl;
	}
	printf("\nUsages:\n");
	printf("convert csv to hdf5:\n\t./csv2hdf5 -ex 'Grenoble' -f ~/data/ILL_October_2018/Run21.csv -bc 40 -tac 60 -n 1000\n");

	printf("\nFlags:\n");
	printf("-f: csv data file with the extension .csv\n\tThe data file was created by ESS tool.\n");
	printf("-ex: Experiment and format (budapest or grenoble)\n\n");
	printf("-bc: bunch crossing clock. Optional argument (default 20 MHz)\n\n");
	printf("-tac: tac slope. Optional argument (default 100 ns)\n\n");
	printf("-n: number of lines to analyze\n\n");

	return -1;
}

