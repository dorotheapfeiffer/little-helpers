#!/bin/bash     

 
energy=(1 2)
for e in "${energy[@]}";do
	pressure=(500 1000)
	for p in "${pressure[@]}";do
		echo "running electrons_${e}keV_Ar_CF4_80_20_${p}mbar.txt"
   		./degrad < electrons_${e}keV_Ar_CF4_80_20_${p}mbar.txt
		python convertDegrad.py
		mv DEGRAD.dat electrons_${e}keV_Ar_CF4_80_20_${p}mbar.dat
		root -q -b 'RootTreeFromDegrad.C("'electrons_${e}keV_Ar_CF4_80_20_${p}mbar.dat'")'
		rm electrons_${e}keV_Ar_CF4_80_20_${p}mbar.dat
	done
done
