#include <vector>
#include <list>
#include <map>

#include "RootFile.h"

class RawdataParser
{
public:
	RawdataParser(std::string fileName, std::string pedestalName,
		bool isRawPedestal, bool isPedestal, bool isZS, float zsCut,
		bool commonModeOn, bool clusteringOn, std::vector<int> xChips,
		std::vector<int> yChips, int mappings);
	~RawdataParser();
	void SetRunFlags(bool isRawPedestal, bool isPedestal);
	int AnalyzeWord(int rawdata, int rawdata_before, int rawdata_before_two);
	void AnalyzeEventZS();
	void AnalyzeEvent();
	void ComputeRawPedestalData(int theApvID);
	void ComputePedestalData(int theApvID);
	void ComputeCorrectedData(int theApvID);
	void CreateHistograms();
private:
/* C++ 2011
	RootFile *fRoot = 0;
	int format=0;
	bool isRawPedestalRun = false;
	bool isPedestalRun = false;
	bool isZSRun = false;
	float fZsCut = 0;
	bool commonModeOn = false;

	signed int unixtimestamp = 0;
	int timestamp_us = 0;
	int eventNr = 1;
	unsigned int runNr = 0;
	int headerLDC = 0;
	int headerEquipment = 0;
	int header = 0;
	bool inEquipmentHeader = 0;

	int fecID = 0;
	int apvID = -1;
	int minFECID = 9999;
	int maxFECID = 0;
	int minAPVID = 9999;
	int maxAPVID = 0;

	int idata = 0;
	bool inEvent = false;

	bool startDataFlag = false;

	int wordCountEquipmentHeader = 0;
	int wordCountEvent = 0;
	int packetSize = 0;

	int numTimeBins = 0;
	int chNo = 0;
	float maxADC = 0;
	int timeBinMaxADC = 0;
	int apvheaderlevel = 1300;
	int theTimeBin = 0;
*/
	RootFile *fRoot;
	int format;
	bool isRawPedestalRun;
	bool isPedestalRun;
	bool isZSRun;
	float fZsCut;
	bool commonModeOn;

	signed int unixtimestamp;
	int timestamp_us;
	int eventNr;
	unsigned int runNr;
	int headerLDC;
	int headerEquipment;
	int header;
	bool inEquipmentHeader;

	int fecID;
	int apvID;
	int minFECID;
	int maxFECID;
	int minAPVID;
	int maxAPVID;

	int idata;
	bool inEvent;

	bool startDataFlag;

	int wordCountEquipmentHeader;
	int wordCountEvent;
	int packetSize;

	int numTimeBins;
	int chNo;
	float maxADC;
	int timeBinMaxADC;
	int apvheaderlevel;
	int theTimeBin;
	
	unsigned int fRawData16bits[2];
	std::vector<float> timeBinADCs;
	std::vector<float> commonModeOffset;
	std::vector<float> timeBinOffset;

};
