#include <algorithm>

void RootTreeFromDegrad(TString fname)
{
	bool writeExcitations = true;
	
	Int_t eventNumber = 0, electronNumber = 0, excitationNumber = 0, nullCollisionNumber = 0, nullCollisionID = 0, Nclus = 0, Nstexc = 0, Nexc1 = 0, Nexc2 = 0, Nexc3=0,
			Nexc4 = 0, Nexc5 = 0, Nexc6 = 0, Ncompton = 0, Npair = 0, NnullCollisions = 0;
	Double_t posX = 0, posY = 0, posZ = 0, time = 0, n = 0;
	Int_t FlagFluo = 0, FlagPair = 0, FlagBrem = 0;

	Int_t nline, i = 0;
	string line;
	vector<Double_t> v;
	TString fileName_short, fileName;
	ifstream inFile;

	fileName_short = fname;
	fileName_short.ReplaceAll(".dat", "");
	fileName_short.ReplaceAll("/./", "/");

	fileName = fileName_short + ".root";
	TFile *file = new TFile(fileName, "RECREATE");
	
	TTree *events = new TTree( "events", "events");
	events->Branch("eventNumber", &eventNumber, "eventNumber/I");
	events->Branch("Nclus", &Nclus, "Nclus/I");
	events->Branch("Nstexc", &Nstexc, "Nstexc/I");
	events->Branch("Nexc1", &Nexc1, "Nexc1/I");
	events->Branch("Nexc2", &Nexc2, "Nexc2/I");
	events->Branch("Nexc3", &Nexc3, "Nexc3/I");
	events->Branch("Nexc4", &Nexc4, "Nexc4/I");
	events->Branch("Nexc5", &Nexc5, "Nexc5/I");
	events->Branch("Nexc6", &Nexc6, "Nexc6/I");
	events->Branch("Ncompton", &Ncompton, "Ncompton/I");
	events->Branch("Npair", &Npair, "Npair/I");
	events->Branch("NnullCollisions", &NnullCollisions, "NnullCollisions/I");
	
	
	TTree *electrons = new TTree("electrons", "electrons");
	electrons->Branch("eventNumber", &eventNumber, "eventNumber/I");
	electrons->Branch("electronNumber", &electronNumber, "electronNumber/I");
	electrons->Branch("posX", &posX, "posX/D");
	electrons->Branch("posY", &posY, "posY/D");
	electrons->Branch("posZ", &posZ, "posZ/D");
	electrons->Branch("time", &time, "time/D");
	electrons->Branch("FlagFluo", &FlagFluo, "FlagFluo/I");
	electrons->Branch("FlagPair", &FlagPair, "FlagPair/I");
	electrons->Branch("FlagBrem", &FlagBrem, "FlagBrem/I");


	TTree *excitations = new TTree("excitations", "excitations");
	excitations->Branch("eventNumber", &eventNumber, "eventNumber/I");
	excitations->Branch("excitationNumber", &excitationNumber, "excitationNumber/I");
	excitations->Branch("posX", &posX, "posX/D");
	excitations->Branch("posY", &posY, "posY/D");
	excitations->Branch("posZ", &posZ, "posZ/D");
	excitations->Branch("time", &time, "time/D");

	
	TTree *nullCollisions = new TTree("nullCollision", "nullCollision");
	nullCollisions->Branch("eventNumber", &eventNumber, "eventNumber/I");
	nullCollisions->Branch("nullCollisionNumber", &nullCollisionNumber, "nullCollisionNumber/I");
	nullCollisions->Branch("posX", &posX, "posX/D");
	nullCollisions->Branch("posY", &posY, "posY/D");
	nullCollisions->Branch("posZ", &posZ, "posZ/D");
	nullCollisions->Branch("time", &time, "time/D");
	nullCollisions->Branch("nullCollisionID", &nullCollisionID, "nullCollisionID/I");
	
	
	inFile.open(fname, ifstream::in);

	cout << "Working in " << fname << endl;

	nline = 1;
	int totalCluster = 0;
	int totalElectrons = 0;

	while (getline(inFile, line, '\n'))
	{
		istringstream iss(line);
		if (nline == 1)
		{
			while (iss >> n)
			{
				v.push_back(n);
			}
			eventNumber = v[0];
			Nclus = v[1];
			Nstexc = v[2];
			Nexc1 = v[3];
			Nexc2 = v[4];
			Nexc3 = v[5];
			Nexc4 = v[6];
			Nexc5 = v[7];
			Nexc6 = v[8];
			Ncompton = v[9];
			Npair = v[10];
			NnullCollisions = v[11];
			
			events->Fill(); //just fill here the tree
			v.clear();
			
		}
		if (nline == 2) //Ionizations
		{
			electronNumber = 0;
			while (iss >> n)
			{
				v.push_back(n);
			}
			for (i = 0; i < v.size(); i = i + 7)
			{
				posX = v[i];
				posY = v[i + 1];
				posZ = v[i + 2];
				time = v[i + 3];
				FlagFluo = v[i + 4];
				FlagPair = v[i + 5];
				FlagBrem = v[i + 6];


				if (!(posX == 0 && posY == 0 && posZ == 0 && time == 0))
				{
					electronNumber++;
					totalElectrons++;
					electrons->Fill(); //just fill here the tree
				}
			}
			v.clear();
			if(!writeExcitations)
			{
				nline = 0;
			}

		}
		if (nline == 3) //Excitations
		{
			excitationNumber = 0;
			while (iss >> n)
			{
				v.push_back(n);
			}
			for (i = 0; i < v.size(); i = i + 4)
			{
				posX = v[i];
				posY = v[i + 1];
				posZ = v[i + 2];
				time = v[i + 3];
			

				if (!(posX == 0 && posY == 0 && posZ == 0 && time == 0))
				{
					excitationNumber++;
					excitations->Fill(); //just fill here the tree
				}
			}
			v.clear();
		}
		if (nline == 4) //Null Collisions
		{
			nullCollisionNumber = 0;
			while (iss >> n)
			{
				v.push_back(n);
			}
			for (i = 0; i < v.size(); i = i + 5)
			{
				posX = v[i];
				posY = v[i + 1];
				posZ = v[i + 2];
				time = v[i + 3];
				nullCollisionID = v[i + 4];

				if (!(posX == 0 && posY == 0 && posZ == 0 && time == 0))
				{
					nullCollisionNumber++;
					nullCollisions->Fill(); //just fill here the tree
				}
			}
			v.clear();
			nline = 0;
		}
		if(Nclus>0)
		{
			nline++; 
		}
		else
		{
			nline=1;
		}//restart nline after 2 lines or 4 in case of excitation in DEGRAD
	} //end while

	inFile.close();

	events->Write();
	electrons->Write();
	if(writeExcitations)
	{
		if(excitationNumber>0)
		{
			excitations->Write();
		}
		if(nullCollisionNumber > 0)
		{
			nullCollisions->Write();
		}
	}
	file->Close();
	std::cout << "total cluster " << totalCluster << " total electrons " << totalElectrons <<std::endl;
}
