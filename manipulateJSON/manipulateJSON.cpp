#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "TFile.h"
#include <vector>
#include <map>
#include "TStyle.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include <TObject.h>
#include <TString.h>
#include "TBufferJSON.h"

#include <chrono>

TString fileInName = "";
TString fileOutNamePDF = "";
TString fileOutNameCanvas = "";
TString fileOutName = "";
TString fileNormName = "";
std::string pAction = "";
std::string pPlot = "";

bool fileInFound = false;
bool fileOutFound = false;
bool fileNormFound = false;

bool plotLog = false;
bool plotPdf = false;
bool plotJSON = false;
bool plotCanvas = false;
int pMap = 53;
int pRebinFactor = 1;
double pzmin = 0;
double pzmax = 0;
double pTimeSignal = 0;
double pTimeBackground = 0;
double pxmin = 0;
double pxmax=1280;
double pymin = 0;
double pymax=1280;

int printUsage(std::string errorMessage, char* argv);

int main(int argc, char**argv) {
	std::chrono::time_point<std::chrono::system_clock> timeEnd, timeStart;

	if (argc == 1 || argc % 2 == 0) {
		return printUsage("Wrong number of arguments!", argv[argc - 1]);
	}
	for (int i = 1; i < argc; i += 2) {
		if (strncmp(argv[i], "-fin", 3) == 0) {
			fileInFound = true;
			fileInName = argv[i + 1];
		} else if (strncmp(argv[i], "-fnorm", 6) == 0) {
			fileNormFound = true;
			fileNormName = argv[i + 1];
		} else if (strncmp(argv[i], "-fout", 5) == 0) {
			fileOutFound = true;
			fileOutName = argv[i + 1];
			fileOutNamePDF = fileOutName;
			fileOutNameCanvas = fileOutName;
		} else if (strncmp(argv[i], "-ns", 3) == 0) {
			pTimeSignal = atof(argv[i + 1]);
		} else if (strncmp(argv[i], "-nb", 3) == 0) {
			pTimeBackground = atof(argv[i + 1]);
		} else if (strncmp(argv[i], "-act", 4) == 0) {
			pAction = argv[i + 1];
		} else if (strncmp(argv[i], "-plot", 5) == 0) {
			pPlot = argv[i + 1];
			if (pPlot.find("j") != std::string::npos) {
				plotJSON = true;
			}
			if (pPlot.find("l") != std::string::npos) {
				plotLog = true;
			}
			if (pPlot.find("p") != std::string::npos) {
				plotPdf = true;
			}
			if (pPlot.find("c") != std::string::npos) {
				plotCanvas = true;
			}

		} else if (strncmp(argv[i], "-map", 4) == 0) {
			pMap = atoi(argv[i + 1]);

		} 
		else if (strncmp(argv[i], "-rebin", 6) == 0) {
			pRebinFactor = atoi(argv[i + 1]);

		}
		else if (strncmp(argv[i], "-xmin", 5) == 0) {
			pxmin = atof(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-xmax", 5) == 0) {
			pxmax = atof(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-ymin", 5) == 0) {
			pymin = atof(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-ymax", 5) == 0) {
			pymax = atof(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-zmin", 5) == 0) {
			pzmin = atof(argv[i + 1]);
		}
		else if (strncmp(argv[i], "-zmax", 5) == 0) {
			pzmax = atof(argv[i + 1]);
		}
		else {
			return printUsage("Wrong type of argument!", argv[i]);
		}
	}

	if (!fileInFound) {
		return printUsage("Data file has to be loaded with -in data.json!", nullptr);

	}

	if (fileInFound && !fileInName.EndsWith(".json")) {
		return printUsage("Wrong extension: .json file required!", nullptr);
	}
	if(!fileOutFound) {fileOutName = "out_" + fileInName;}
	
	if (fileInFound) {
		timeStart = std::chrono::system_clock::now();

		std::ifstream fileIn;
		fileIn.open(fileInName);
		std::stringstream s1;
		s1 << fileIn.rdbuf();
		TString jsonIn = s1.str();
		TH2D *hIn = nullptr;
		TH2D *hOut = nullptr;
		TH2D *hNorm = nullptr;
		TBufferJSON::FromJSON(hIn, jsonIn);
		int bins = 4;
		if(pRebinFactor == 2) {
			bins = 2;
		}
		else if(pRebinFactor == 4) {
			bins = 1;
		}
		int nx = (pxmax-pxmin)*(bins);
		int ny = (pymax-pymin)*(bins);  
		
					
		if(pRebinFactor > 0) {
			hIn->Rebin2D(pRebinFactor,pRebinFactor);	
		}
		//hIn->GetXaxis()->SetRangeUser(pxmin,pxmax);
		//hIn->GetYaxis()->SetRangeUser(pymin,pymax);
		gStyle->SetPalette(pMap);
		gStyle->SetPadRightMargin(0.13);

		TCanvas *c1 = new TCanvas("c1", "c1", 630, 600);

		c1->SetFillColor(0);
		c1->SetBorderMode(0);
		c1->SetBorderSize(2);
		c1->SetFrameBorderMode(0);
		
		hOut = new TH2D("out", "out", nx, pxmin, pxmax, ny, pymin, pymax);
		
		if (pTimeSignal * pTimeBackground > 0) {
			std::ifstream fileNorm;
			fileNorm.open(fileNormName);
			std::stringstream s2;
			s2 << fileNorm.rdbuf();
			TString jsonNorm = s2.str();
			
			TBufferJSON::FromJSON(hNorm, jsonNorm);
			if(pRebinFactor > 0) {
				hNorm->Rebin2D(pRebinFactor,pRebinFactor);	
			}
		}
				
		
		//std::cout << nx << " " << ny << " " << pxmin << " " << pxmax << std::endl;
		//std::cout << nx << " " << ny << " " << pymin << " " << pymax << std::endl;
		for (Int_t x = 0; x < nx; x++) {
			for (Int_t y = 0; y < ny; y++) {
				if (pTimeSignal * pTimeBackground > 0) {
					double valSignal =  pTimeBackground * hIn->GetBinContent(x + 1 + pxmin*bins, y + 1+pymin*bins) ;
					double valBackground = pTimeSignal * hNorm->GetBinContent(x + 1 + pxmin*bins, y + 1+pymin*bins) ;
					if (pAction == "n") {
						double val = 0;
						if (valBackground > 0) {
							val = valSignal / valBackground;
						}
					//std::cout << x << " " << y << " " << val << " " << valSignal << " " << valBackground << std::endl;
					//hOut->SetBinContent(x + 1, y + 1, val);
						hOut->SetBinContent(x + 1, y + 1, val);
					} else {
						hOut->SetBinContent(x + 1, y + 1, (valSignal - valBackground));
					}
				}
				else {
					double val =  hIn->GetBinContent(x + 1 + pxmin*bins, y + 1+pymin*bins) ;
					hOut->SetBinContent(x + 1, y + 1, val);	
				}

			}
		}
		
		if(pRebinFactor == 4) {
			hOut->GetYaxis()->SetTitle("y coordinate [pitch 400 um]");
			hOut->GetXaxis()->SetTitle("x coordinate [pitch 400 um]");
		} else if (pRebinFactor == 2) {
			hOut->GetYaxis()->SetTitle("y coordinate [pitch 200 um]");
			hOut->GetXaxis()->SetTitle("x coordinate [pitch 200 um]");
		} else if (pRebinFactor == 1) {
			hOut->GetYaxis()->SetTitle("y coordinate [pitch 100 um]");
			hOut->GetXaxis()->SetTitle("x coordinate [pitch 100 um]");
		}
		gPad->SetRightMargin(0.18);
		hOut->GetXaxis()->SetTitleOffset(1.4);	
		hOut->GetYaxis()->SetTitleOffset(1.4);
		hOut->GetZaxis()->SetTitleOffset(1.5);						
		hOut->GetZaxis()->SetTitle("counts");
		hOut->GetXaxis()->CenterTitle();
		hOut->GetYaxis()->CenterTitle();
		hOut->GetZaxis()->CenterTitle();
		hOut->SetTitle("");
		hOut->SetStats(0);
		if(pzmax > 0) {
			hOut->GetZaxis()->SetRangeUser(0, pzmax);
		}
		hOut->GetXaxis()->SetRangeUser(pxmin,pxmax);
		hOut->GetYaxis()->SetRangeUser(pymin,pymax);
		hOut->Draw("COLZ");
		
		if (plotJSON) {
			TString jsonOut = TBufferJSON::ToJSON(hOut, 3);
			std::ofstream f3;
			f3.open(fileOutName, std::ios::out);
			f3 << jsonOut;
			f3.close();
		}

		if (plotPdf) {
			TString pdfName = "";
			if (plotLog) {
				c1->SetLogz(true);
				pdfName = fileOutNamePDF.ReplaceAll(".json", "_LOG.pdf");
			} else {
				pdfName = fileOutNamePDF.ReplaceAll(".json", ".pdf");
			}
			c1->SaveAs(pdfName);
		}
		if (plotCanvas) {
			TString rootName = fileOutNameCanvas.ReplaceAll(".json", ".root");
			c1->SaveAs(rootName);
		}

		timeEnd = std::chrono::system_clock::now();

		int elapsed_seconds = std::chrono::duration_cast < std::chrono::milliseconds > (timeEnd - timeStart).count();

		std::cout << "finished computation in " << elapsed_seconds << " ms\n";
	}
	return 0;

}

int printUsage(std::string errorMessage, char* argv) {
	if(argv != nullptr) {
		std::cout << "\nERROR: " << errorMessage << ": " << argv << std::endl;
	}
	else {
		std::cout << "\nERROR: " << errorMessage << std::endl;	
	}

	printf("\nUsages:\n");
	printf("normalize or substract background:\n\t./normalize -fin Run1.json -fnorm Run2.json -fout Run3.json"
			" -ns 0 -nb 0 -act normalize\n");

	return -1;
}

