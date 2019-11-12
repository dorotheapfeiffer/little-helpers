#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
//#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include <TObject.h>
#include "TString.h"

#define NFEC 4
#define NAPV 16
#define NCH 128
#define DISCARD_THRESHOLD 200

using namespace std;

class RootFile: public TObject
{

public:

	RootFile(TString fileName, TString pedestalName, bool isRawPedestalRun,
			bool isPedestalRun, bool isZSRun, bool clusteringOn,
			std::vector<int> xChips,
			std::vector<int> yChips);
	~RootFile();

	void InitRootFile();
	void InitPedestalHistograms(int fecID, int apvID);
	void InitPedestalData(int fecID, int apvID);
	void SetRunFlags(bool isRawPedestalRun, bool isPedestalRun);
	float GetStripRawPedestalNoise(int fecID, int apvID, int stripNr);
	float GetStripRawPedestalOffset(int fecID, int apvID, int stripNr);
	float GetStripPedestalNoise(int fecID, int apvID, int stripNr);
	float GetStripPedestalOffset(int fecID, int apvID, int stripNr);
	float GetMeanPedestalOffset(int fecID, int apvID);

	void FillPedestalData(int fecID, int apvID, int stripNo, float mean);
	void CreateHistograms(int minFECID, int maxFECID, int minAPVID,
			int maxAPVID);
	void FillStripData(int stripNo, float rawdata);
	bool StripDataExists(int stripNo);
	float GetMeanStripData(int stripNo);
	float GetStripData(int stripNo, int timeBin);
	void ClearStripData(int stripNo);
	void WriteRootFile();
	void AddHits(signed int timestamp, int us, int eventId, int fecID,
			int apvID, int chNo, float maxADC, int timeBinMaxADC,
			std::vector<float> &timeBinADCs);
	void FillHits();

	void DeleteHitsTree();

	int GetStripNumber(int chNo);
	void resetClusterData();
	unsigned int GetPlaneID(unsigned int chipID);
	unsigned int GetChannelX(unsigned int chipID, unsigned int channelID);
	unsigned int GetChannelY(unsigned int chipID, unsigned int channelID);

private:
	bool isRawPedestalRun;
	bool isPedestalRun;
	bool isZSRun;
	bool clusteringOn;
	
	ofstream fTextFile;
 
	TFile * fFile;
	TFile * fFilePedestal;
	TTree * fHitTree;
	TString fFileName;
	TString fPedestalName;

	std::vector<TH1F*> rawPedestalNoise;
	std::vector<TH1F*> rawPedestalOffset;
	std::vector<TH1F*> pedestalNoise;
	std::vector<TH1F*> pedestalOffset;
	std::vector<std::vector<TH1F*> > chipData;
	std::vector<std::vector<float>*> stripData;

	std::vector<int> xChipIDs;
	std::vector<int> yChipIDs;

	int m_timestamp; 		//Unix time stamp
	int m_us;
	int m_evtID;
	long m_chID;
	long m_nchX;
	long m_nchY;
	int * m_planeID;      // Plane Number
	int * m_fecID;        // APVId
	int * m_apvID;        // APVId
	int * m_strip_chip;   // Strip Number chip
	int * m_strip;        // Strip Number
	short * m_x;
	short * m_y;
	int * m_hitTimeBin;     //time bin with maximum ADC
	short* m_hitMaxADC; //Maximum ADC value of hit

	short * m_adc0;     //ADC value for 1st time sample
	short * m_adc1;     //ADC value for 2nd time sample
	short * m_adc2;     //ADC value for 3rd time sample
	short * m_adc3;     //ADC value for 4th time sample
	short * m_adc4;     //ADC value for 5th time sample
	short * m_adc5;     //ADC value for 6th time sample
	short * m_adc6;     //ADC value for 7th time sample
	short * m_adc7;     //ADC value for 8th time sample
	short * m_adc8;     //ADC value for 9th time sample
	short * m_adc9;     //ADC value for 10th time sample
	short * m_adc10;     //ADC value for 11th time sample
	short * m_adc11;     //ADC value for 12th time sample
	short * m_adc12;     //ADC value for 13th time sample
	short * m_adc13;     //ADC value for 14th time sample
	short * m_adc14;     //ADC value for 15th time sample
	short * m_adc15;     //ADC value for 16th time sample
	short * m_adc16;     //ADC value for 17th time sample
	short * m_adc17;     //ADC value for 18th time sample
	short * m_adc18;     //ADC value for 19th time sample
	short * m_adc19;     //ADC value for 20th time sample
	short * m_adc20;     //ADC value for 21th time sample
	short * m_adc21;     //ADC value for 22th time sample
	short * m_adc22;     //ADC value for 23th time sample
	short * m_adc23;     //ADC value for 24th time sample
	short * m_adc24;     //ADC value for 25th time sample
	short * m_adc25;     //ADC value for 26th time sample
	short * m_adc26;     //ADC value for 27th time sample
	short * m_adc27;     //ADC value for 28th time sample
	short * m_adc28;     //ADC value for 29th time sample
	short * m_adc29;     //ADC value for 30th time sample

	std::vector<float> stripMaximaX;
	std::vector<float> stripMaximaY;
	std::vector<float> timeMaximaX;
	std::vector<float> timeMaximaY;
	std::vector<float> energyMaximaX;
	std::vector<float> energyMaximaY;
	
	/* C++ 2011
	bool isFirstLine = true;
	bool newData = false;

	int amplitudeThreshold = 0;
	
	int numMaximaX = 0;
	int numMaximaY = 0;

	double x0 = 0;
	double y0 = 0;
	double xf = 0;
	double yf = 0;
	double xm = 0;
	double ym = 0;
	double xi = 0;
	double yi = 0;
	double xs = 0;
	double ys = 0;
	double xd = 0;
	double yd = 0;
	double xmax = 0;
	double ymax = 0;
	double xAmplitude = 0;
	double xIntegral = 0;
	double yAmplitude = 0;
	double yIntegral = 0;
	double maxAmplitudeX = 0;
	double maxAmplitudeY = 0;
	double tMaxAmplitudeX = 0;
	double tMaxAmplitudeY = 0;

	double tx0 = 0;
	double ty0 = 0;
	double txf = 0;
	double tyf = 0;
	double txs = 0;
	double tys = 0;
	double txd = 0;
	double tyd = 0;
	double txmax = 0;
	double tymax = 0;
	double ex0 = 0;
	double ey0 = 0;
	double exf = 0;
	double eyf = 0;
	double exs = 0;
	double eys = 0;
	double exd = 0;
	double eyd = 0;
	double exmax = 0;
	double eymax = 0;

	int nx = 0;
	int ny = 0;
	int ntx = 0;
	int nty = 0;
	bool discardFlag = false;
	*/
	
	bool isFirstLine;
	bool newData;

	int amplitudeThreshold;
	
	int numMaximaX;
	int numMaximaY;

	double x0;
	double y0;
	double xf;
	double yf;
	double xm;
	double ym;
	double xi;
	double yi;
	double xs;
	double ys;
	double xd;
	double yd;
	double xmax;
	double ymax;
	double xAmplitude;
	double xIntegral;
	double yAmplitude;
	double yIntegral;
	double maxAmplitudeX;
	double maxAmplitudeY;
	double tMaxAmplitudeX;
	double tMaxAmplitudeY;

	double tx0;
	double ty0;
	double txf;
	double tyf;
	double txs;
	double tys;
	double txd;
	double tyd;
	double txmax;
	double tymax;
	double ex0;
	double ey0;
	double exf;
	double eyf;
	double exs;
	double eys;
	double exd;
	double eyd;
	double exmax;
	double eymax;

	int nx;
	int ny;
	int ntx;
	int nty;
	
	bool discardFlag;
	
	ClassDef(RootFile,1)
};

