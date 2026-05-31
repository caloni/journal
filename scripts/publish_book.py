#!/usr/bin/env python3
"""
Wrapper to call what is needed to publish the book.
It converts to Kindle supported file format using
kindlegen or calibre.
"""

import os
import shutil
import subprocess

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
method = "kindlegen" # calibre, kindlegen
baseName = "caloni"
initialFormat = "epub"
finalFormat = "azw3" # mobi, azw3 (calibre only)
initialFile = baseName + "." + initialFormat
finalFile = baseName + "." + finalFormat
destinationFolder = "K:/documents/"
clippingsKindleFile = destinationFolder + "/" + baseName + ".sdr"
destinationKindleFile = destinationFolder + "/" + finalFile

os.chdir(dname)
os.chdir('..')

process = subprocess.run(['python', os.path.join(dname, 'journal2book.py')], check=True)
if process.stdout:
  print(process.stdout)
if process.returncode:
  print('journal2book.py returned', process.returncode)

old_dir = os.getcwd()
os.chdir(r'publish/output/kindle')
if method == "kindlegen":
  process = subprocess.run(['kindlegen', initialFile, '-o', finalFile], check=False)
elif method == "calibre":
  process = subprocess.run(['ebook-convert', initialFile, finalFile], check=False)

if os.path.exists(destinationFolder):
  if os.path.exists(clippingsKindleFile):
    shutil.rmtree(clippingsKindleFile)
    print('old clippings cleared (maybe restart the device to clear the cache)')
  shutil.copy(finalFile, destinationKindleFile)
  print('book published successfully')
os.chdir(old_dir)
