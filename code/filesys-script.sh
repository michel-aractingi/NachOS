#!/bin/bash
#My Filesys Script

cd build
echo -e "################# Formating the disk #############\n"
./nachos-step5 -f
echo -e "################ Disk Formated ####################\n"

echo -e "\n########## Creating new directories: main ##########\n"
./nachos-step5 -mkdir main
./nachos-step5 -mkdir main/submain
./nachos-step5 -mkdir main/submain/subdir
./nachos-step5 -mkdir etc
./nachos-step5 -mkdir etc/sub-main

echo -e "########### Copying few files to test ############\n"
./nachos-step5 -cp ../filesys/test/big input
./nachos-step5 -cp ../filesys/test/big main/submain/input0
./nachos-step5 -cp ../filesys/test/big main/submain/input1
./nachos-step5 -cp ../filesys/test/big main/submain/input2
./nachos-step5 -cp ../filesys/test/big main/submain/input3
./nachos-step5 -cp ../filesys/test/big main/submain/input4
./nachos-step5 -cp ../filesys/test/big main/submain/input5
./nachos-step5 -cp ../filesys/test/big main/submain/input6
./nachos-step5 -cp ../filesys/test/big main/submain/input7
./nachos-step5 -cp ../filesys/test/small main/small0
./nachos-step5 -cp ../filesys/test/small main/small1
./nachos-step5 -cp ../filesys/test/small main/small2
./nachos-step5 -cp ../filesys/test/small main/small3
./nachos-step5 -cp ../filesys/test/small main/small4
./nachos-step5 -cp ../filesys/test/small main/small5
./nachos-step5 -cp ../filesys/test/small main/small6

./nachos-step5 -cp dirTest dirtest
./nachos-step5 -cp openfile openfile

echo -e "############ Disk Hierarchy #################\n"
./nachos-step5 -l

echo -e "######### Running directory Test ############\n"
./nachos-step5 -x dirtest

#echo -e "#############Printing contents of the created file ##########"
#./nachos-step5 -p dirmain/child/bar

echo -e "############Running OpenFileTest #####################"
./nachos-step5 -rs -x openfile

echo -e "############ Disk Hierarchy #################\n"
./nachos-step5 -l
