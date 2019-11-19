

#!/usr/bin/python
import os
import subprocess
import re
import sys


fin_193_all_utpc= "run193_holepattern_1p0mmCd_20180705_095135_bc_20_tac_100_ccs_3_cs_2_dt_200_mst_5_spc_500_dp_200_ratio_100p00_coin_center-of-mass_time_calib_a_detector1_cluster_utpc.json"
fin_193_all_algo= "run193_holepattern_1p0mmCd_20180705_095135_bc_20_tac_100_ccs_3_cs_2_dt_200_mst_5_spc_500_dp_200_ratio_100p00_coin_center-of-mass_time_calib_a_detector1_cluster_algo.json"

try:


	fin = fin_193_all_algo
	fnorm = fin_189_all_algo
	fout = "run193_run189_all_algo_512.json"
	args = ['/Users/dpfeiffe/programming/scripts/manipulateJSON/manipulateJSON', '-fin', fin, '-fout', fout,'-fnorm', fnorm, '-ns', '82', '-nb','4', '-act','n', '-plot','pj', '-map', '53', '-rebin', '2', '-xmin', '0', '-xmax', '256', '-ymin', '0', '-ymax', '256', '-zmax', '1.0']		
	subprocess.call(args)
except OSError:
	pass




