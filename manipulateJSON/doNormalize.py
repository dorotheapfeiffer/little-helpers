

#!/usr/bin/python
import os
import subprocess
import re
import sys



try:


	fin = "Neutrons_Quadrant1_charge2.json"
	fnorm = "Mask_Quadrant1_charge2.json"
	fout = "Normalize_Quadrant1.json"
	args = ['/Users/dpfeiffe/programming/little-helpers/manipulateJSON/manipulateJSON', '-fin', fin, '-fout', fout,'-fnorm', fnorm, '-ns', '1', '-nb','1', '-act','n', '-plot','cpj', '-map', '53', '-rebin', '4', '-xmin', '0', '-xmax', '640', '-ymin', '0', '-ymax', '640', '-zmax', '10.0']		
	subprocess.call(args)
except OSError:
	pass




