#!/usr/bin/env python
import os
WORKING_DIRECTORY=os.getcwd()
print(WORKING_DIRECTORY)
for files in os.listdir(WORKING_DIRECTORY):
  if files.endswith(".OUT"):
  #replace all space characters by tab
    infile = open(files,"r")
    outfile = open("tempFile.dat","w")
    n = 1
    replacements = {' ':'\t'}
    print("replacing lines...")
    for line in infile:
      for src, target in replacements.items():
        if n==1:
          line = line.replace(src, target)
          n=2
        elif n==2:
          n=1
      outfile.write(line)
    infile.close()
    outfile.close
    os.system("rm DEGRAD.OUT")
    #replace all double tab by tab
    infile = open("tempFile.dat","r")
    filename, file_extension = os.path.splitext(files)
    outfile = open(filename+".dat","w")
    replacements = {'\t\t':'\t'}
    for line in infile:
      while '\t\t' in line:
        for src, target in replacements.items():
          line = line.replace(src, target)
      line = line[1:]
      outfile.write(line)
    infile.close()
    outfile.close()
    os.system("rm tempFile.dat")
