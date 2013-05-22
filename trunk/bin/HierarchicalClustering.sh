#!/bin/sh

#
#*********************************************************************************************************************************************************
#Authors         :       Amar Bhagwandas Lalwani (MT2012073) and Raghav Bali (MT2012108)
#
#Date            :       May 18 2013
#
#Project         :       Hierarchical Clustering System (HCS)
#Version         :       1.0
#Description     :       This is a demo to showcase the working of "Hierarchical Clustering Algorithm" based on globa;l and cluster frequent-#itemsets.
#                        This demo works on a sample corpus which is preprocessed by "The Boolean Retrieval System (TBRS)" developed by the same #authors.
#                        TBRS is used to generate "Term-Document-Frequency" triplet as output which is used to generate Hierarchical Clusters.
#			 The script executes the complete demo from terminal. Execute and follow the instructions.
#
#Dependencies    :       The Boolean Retrieval System (TBRS) for preprocessing documents for clustering. [Developed by same authors]
#*                        "python-fp-growth" for finding frequent itemsets. [Developed by Eric Naeseth]
#*********************************************************************************************************************************************************
#


echo "Please Enter input list filename (relative path from current directory) :"
read inputfile
echo " "
echo "Starting Corpus Preprocessing..."
../bin/ClusterPreprocessing $inputfile
if [ $? -eq 0 ]
then
	echo "[*]Tokenization...........[done]"
	echo "[*]Stopword removal.......[done]"
	echo "[*]Stemming...............[done]"
	echo "[*]Document Indexing......[done]"
	echo "Note : Preprocessing output files : output/"
	echo " "
	echo "Starting Hierarchical Clustering..."
	echo " "
	cd ../cluster/python-fp-growth-master
	python main.py
	echo "[*]Clustering.............[done]"
	echo "Note : Clustering input files (input.txt and postingslist.txt): cluster/python-fp-growth-master/"
	echo "       Clustering output files : cluster/ClusterOutput/"
	echo " "
else
	echo " "
	echo "Something went wrong, please check input file's path entered above. "
	echo " "
fi
echo "Press any key to exit :"
read userinput
echo "************************************************************************************************"
echo "Thank you for using HCS v1.0 . Authors : Amar Bhagwandas Lalwani and Raghav Bali "
echo "************************************************************************************************"


