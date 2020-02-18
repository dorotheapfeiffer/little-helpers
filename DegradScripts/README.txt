In this repository there are several scripts that make DEGRAD simulations easy. DEGRAD is a program written by Stephen Biagi (stephen.biagi@cern.ch), and can be downloaded here: 

http://degrad.web.cern.ch/degrad/

The scripts convertDegrad.py and RootTreeFromDegrad.C used here build on code written by Carlos Azevedo (cdazevedo@ua.pt). THANKS!

The lastest version at the time of writing this README (18.02.2020) is version 3.9, which is also included in the repository. To use DEGRAD, one first has to compile the program:

On MacOS:
> gfortran degrad-3.9.f -o degrad

On Linux:
> f95 degrad-3.9.f -o degrad

To run DEGRAD, a text file with 6 lines of input parameters has to be created. Here is an example of the input parameters:

2,1000,2,1,0,1000.0,2.0,0.0
2,1,0,0,0,0
80.0,20.0,0.0,0.0,0.0,0.0,20.0,76.0
0.0,0.0,0.0,2,1
100.0,0.5,1,1,1,1,1,1,1
0,0,0,0,0,0

The parameters here are for a gas mixture with two components, Ar and CF4, in 80/20 ratio. 1000 electrons with an energy of 1 keV are simulated. They are fired in z-direction, and tracked until total absorption. As output, all the electrons created and all excitations are written into a text file with extension .dat.

This output .dat file is quite large, since it contains a lot of blank characters and new line characters. These can be removed with the python script convertDegrad.py. It has been tested with Python 3.

Finally, the reduced text file is read by the root script RootTreeFromDegrad.C, which creates a root tree. The root tree contains a summary event branch, that lists per event the numbers of clusters and excitations, and by which process they have been produced.


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
	
Electrons, excitations and null collisions are also listed separately, with the position in x,y,z and the time. 	
	
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

The bash script startSims.sh executes all these steps, in the example simulations for different energies and pressures are carried out.

#!/bin/bash     

 
energy=(1 2 5 10)
for e in "${energy[@]}";do
	pressure=(100 500 1000)
	for p in "${pressure[@]}";do
		echo "running electrons_${e}keV_Ar_CF4_80_20_${p}mbar.txt"
   		./degrad < electrons_${e}keV_Ar_CF4_80_20_${p}mbar.txt
		python convertDegrad.py
		mv DEGRAD.dat electrons_${e}keV_Ar_CF4_80_20_${p}mbar.dat
		root -q -b 'MakeTreeDegradIonizations.C("'electrons_${e}keV_Ar_CF4_80_20_${p}mbar.dat'")'
		rm electrons_${e}keV_Ar_CF4_80_20_${p}mbar.dat
	done
done






******************************************************************************************
PARAMETER EXPLANATION:
******************************************************************************************


The exact meaning of the parameters in the six lines can be found in the DEGRAD source code, for reference it is shown below:

C--------------------------------------------------------------- 
C   INPUT CARDS :                                                       
C-------------------------------------------------------------------   
C  FIRST CARD: 5I10,3F10.5 : NGAS,NDELTA,IMIP,NDVEC,NSEED,
C                                    ESTART,ETHRM,ECUT
C  NGAS   = NUMBER OF GASES IN MIXTURE                                
C  NDELTA = NUMBER OF DELTA ELECTRONS (CONVERTED X-RAYS) OR MIPS  
C           MAXIMUM NUMBER OF MIPS   = 100000   
C           MAXIMUM NUMBER OF GAMMAS =  10000
C           MAXIMUM NUMBER OF BETAS  =  10000
C           MAXIMUM NUMBER OF E-BEAM =  10000
C
C  IMIP   = 1 MIPS SIMULATION  (DE/DX, CLUSTERS)
C  IMIP   = 2 ELECTRON BEAM  (TOTAL ABSORPTION)
C  IMIP   = 3 X-RAY
C  IMIP   = 4 BETA DECAY
C  IMIP   = 5 DOUBLE BETA DECAY
C
C  NOTE THE DOUBLE BETA DECAY EVENTS ARE SPLIT IN TWO THE FIRST
C  BETA IS OUTPUT AS A SEPARATE EVENT FROM THE SECOND BETA WHICH
C  ARE AT 180 DEGREES TO EACH OTHER (USE EVEN NUMBER OF TOTAL EVENTS
C  IN ORDER TO HAVE FULL TWO BETA EVENTS)   
C 
C  NDVEC  = 2 MIP X-RAY OR BETA IN RANDOM DIRECTION
C  NDVEC  = 1 MIP X-RAY OR BETA DIRECTION PARALLEL TO E-FIELD (Z) 
C  NDVEC  =-1 MIP X-RAY OR BETA DIRECTION ANTI PARALLEL TO E-FIELD (-Z) 
C  NDVEC  = 0 MIP X-RAY OR BETA IN RANDOM DIRECTION IN X-Y PLANE
C         
C  NOTE :   ELECTRON BEAM WITH NDVEC =0 GIVES DIRECTION ALONG X-AXIS
C           THIS ALLOWS TRACK PARALLEL TO BFIELD WHEN THE B-FIELD IS
C           AT 90 DEGREES TO E-FIELD
C  NSEED  = 0 USES STANDARD SEED VALUE = 54217137
C  NSEED.NE.0 USES VALUE OF NSEED AS SEED VALUE 
C
C  ESTART = MIP,ELECTRON,BETA DECAY OR X-RAY ENERGY IN EV.
C           NOTE DOUBLE BETA DECAY ENERGY IS TO BE ENTERED AS
C           THE ENERGY OF EACH BETA (0.5 TIMES TOTAL DECAY ENERGY)
C            (IF X-RAY MAX ENERGY=2.0MEV)
C            
C  ETHRM  = ELECTRONS TRACKED UNTIL THEY FALL TO THIS ENERGY EV.
C           FOR FAST CALCULATION THE THERMALISATION ENERGY SHOULD BE 
C          SET TO THE LOWEST IONISATION POTENTIAL IN THE GAS MIXTURE.
C          FOR MORE ACCURATE THERMALISATION RANGE THE THERMALISATION
C          ENERGY SHOULD BE SET TO THE LOWEST EXCITATION ENERGY IN 
C          PURE NOBLE GASES OR TO 2.0EV FOR MIXTURES WITH MOLECULAR
C          GAS
C  ECUT   = FOR MIPS ONLY.  APPLIES ENERGY CUT IN EV TO GIVE THE 
C           MAXIMUM ALLOWED PRIMARY CLUSTER ENERGY ( SHOULD BE SET
C           TO LESS THAN 10000. EV TO GIVE MAXIMUM PRIMARY CLUSTER SIZE
C           OF TYPICALLY 400 ELECTRONS
C------------------------------------------------------------------
C  SECOND CARD : 6I5   : NGAS1 , NGAS2, NGAS3 , NGAS4 , NGAS5 , NGAS6
C       NGAS1,ETC :  GAS NUMBER IDENTIFIERS (BETWEEN 1 AND 80)
C                   SEE GAS LIST BELOW FOR IDENTIFYING NUMBERS.
C                                                                      
C-------------------------------------------------------------          
C THIRD CARD: 8F10.4  : FRAC1,FRAC2,FRAC3,FRAC4,FRAC5,FRAC6,TEMP,TORR   
C  FRAC1,ETC : PERCENTAGE FRACTION OF GAS1,ETC                          
C  TEMP : TEMPERATURE OF GAS IN CENTIGRADE                              
C  TORR :  PRESSURE OF GAS IN TORR                                      
C ------------------------------------------------------------          
C FOURTH CARD :  3F10.3,2I5  : EFIELD,BMAG,BTHETA,IWRITE,IPEN             
C  EFIELD : ELECTRIC FIELD IN VOLTS/ CM.                                
C   BMAG  : MAGNITUDE OF THE MAGNETIC FIELD IN KILOGAUSS
C  BTHETA : ANGLE BETWEEN THE ELECTRIC AND MAGNETIC FIELDS IN DEGREES. 
C
C  IWRITE : =0                STANDARD OUTPUT
C  IWRITE : =1    
C   LINE1   OUTPUT NO OF ELECTRONS AND NO OF EXCITATIONS FOR EACH EVENT
C   LINE2   OUTPUTS X,Y,Z AND T FOR EACH THERMALISED ELECTRON
C  IWRITE : =2
C   LINE1   OUTPUT NO OF ELECTRONS AND NO OF EXCITATIONS FOR EACH EVENT
C   LINE2   OUTPUTS X,Y,Z AND T FOR EACH THERMALISED ELECTRON
C   LINE3   OUTPUTS X,Y,Z AND T FOR EACH EXCITATION
C   
C   IPEN :  =0 NO PENNING TRANSFERS
C           =1  PENNING TRANSFERS ALLOWED
C            ( MODIFY GAS SUBROUTINE TO CHANGE PENNING FRACTIONS)
C -----------------------------------------------------------------
C FIFTH CARD : 2F10.3,7I5 : 
C          DETEFF,EXCWGHT,KGAS,LGAS,LCMP,LRAY,LPAP,LBRM,IECASC
C  DETEFF: DETECTION EFFICIENCY OF PHOTONS. USED FOR CALCULATION OF
C          FANO FACTORS FOR COMBINED ELECTRON AND PHOTON DETECTION 
C          IN PURE NOBLE GASES  :  
C          USE BETWEEN 0.0  -  100.0  DETECTION EFFICIENCY
C EXCWGHT: WEIGHT GIVEN TO EXCITATION EVENTS IN FANO CALCULATION
C          WITH RESPECT TO IONISATION. TYPICALLY 0.5 TO 0.6
C          USE WEIGHT GIVEN BY SQRT(Fele/Fexc)  
C          WHERE Fele= ELECTRON FANO FACTOR
C                Fexc= EXCITATION FANO FACTOR
C  KGAS:   GAS IDENTIFIER FOR WHICH GAS IN MIXTURE HAS BETA DECAYED
C          IDENTIFIER NUMBERS : NGAS1 etc   
C  LGAS:   IF MOLECULAR GAS THEN LGAS IDENTIFIES THE COMPONENT ATOM
C          IN THE MOLECULE WHICH HAS BETA DECAYED:
C          E.G. IN CO2  1 = CARBON  2 = OXYGEN
C               IN CF4  1 = CARBON  2 = FLUORINE
C  LCMP:   = 0 NO COMPTON SCATTERING
C          = 1 INCLUDE COMPTON SCATTERING
C  LRAY:   = 0 NO RAYLEIGH SCATTERING
C          = 1 INCLUDE RAYLEIGH SCATTERING
C  LPAP:   = 0 NO PAIR PRODUCTION
C          = 1 INCLUDE PAIR PRODUCTION                  
C  LBRM;   = 0 NO BREMSSTRAHLUNG
C          = 1 INCLUDE BREMSSTRAHLUNG
C
C IECASC   = 0 USE PARAMETERISED CASCADE FOR 2ND TO NTH GENERATION OF
C               ELECTRON IONISING COLLISIONS
C          = 1 USE EXACT CASCADE FOR 2ND TO NTH GENERATION OF ELECTRON
C               IONISING COLLISIONS
C-----------------------------------------------------------------------
C CARD 4*N+1 USES NGAS=0 TO TERMINATE CORRECTLY                         
C-------------------------------------------------------------------- 


Here is the description of the output file:


C   -----------------------------------------------------------
C  IF IWRITE EQ 2 : OUTPUT TO FILE DEGRAD.OUT
C 
C      LINE 1   SAME AS ABOVE 
C          
C      LINE 2   SAME AS ABOVE
C
C      LINE 3   X,Y,Z AND T FOR EACH OF THE NSTEXC EXCITATIONS
C              
C      LINE 4    X,Y,Z,T AND ID FOR EACH OF THE NEXCNUL DISSOCIATION AND 
C                LIGHT EMISSION NULL COLLISIONS  IN THE MOLECULAR GAS. 
C       NB. THE ID OF THE NULL COLLISION IS GIVEN IN THE PRINTED OUTPUT.
c                IF NEXCNUL=0 THEN WRITE LINE OF ZEROS
C
C                   TIME IN PICOSECONDS
C                   X,Y,Z IN MICRONS
C
C FLAGS : FLUORESCENCE: 
C                     VALUES FROM 0 TO N WHERE N IS THE CLUSTER NUMBER
C                     FOR EACH OF THE N ABSORBED FLUORESCENCE PHOTONS IN
C                     THE EVENT 
C      PAIRPRODUCTION :  VALUES 0 1 OR 2
C                        0 : NOT PRODUCED BY PAIR PRODUCTION  
C                        1 : PRODUCED BY PAIR ELECTRON TRACK
C                        2 : PRODUCED BY PAIR POSITRON TRACK
C       BREMSSTRAHLUNG:  VALUES 0 1 OR 2
C                        O: NOT PRODUCED BY BREMSSTRAHLUNG 
C                        1: PRODUCED FROM REMAINING ELECTRON
C                        2: PRODUCED FROM BREMSSTRAHLUNG GAMMA          
C   
C ********************************************************************* 