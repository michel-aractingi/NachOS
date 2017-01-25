#!/bin/bash
#My Filesys Script

cd build
echo -e "#################Formating the disk#############\n"
./nachos-step5 -f
echo -e "################Disk Formated####################\n"

echo -e "#################Disk Hierarchy\n##################"
./nachos-step5 -l

echo -e "##############Creating new directories: main##########\n"
./nachos-step5 -mkdir main
./nachos-step5 -mkdir main/submain
./nachos-step5 -mkdir main/submain/subdir
./nachos-step5 -mkdir etc
./nachos-step5 -mkdir etc/sub-main

echo -e "###########Copying few files to test############\n"
./nachos-step5 -cp ../filesys/test/big input
./nachos-step5 -cp ../filesys/test/small main/submain/small
./nachos-step5 -cp dirTest dirtest
./nachos-step5 -cp openfile openfile

echo -e "###########Disk Hierarchy#################\n"
./nachos-step5 -l

echo -e "#########Running directory Test############\n"
./nachos-step5 -x dirtest
./nachos-step5 -l
