#!/bin/bash
#############################################
#  Find a data file and 
#



# $1 option, 0 last run, 1 find particular run
# $2 run number
# $3 min number of hits



#myPath=/nfs/dust/ilc/group/flchcal/AHCAL_Commissioning_2017_2018/2018Apr/Full_stack_test/cosmics/slcio/
#myPath=$reconstructedSlcioPath
#myPath=/home/lomidze/Qt_Examples/fakedata/
myPath="/media/sf_TB2018-data/slcio/"


# find last created .slcio file
function lastRun(){
    myfile=
    myfile=$(find $myPath*.slcio -type f -printf '%T@ %p\n' | sort -n | tail -1 | cut -f2- -d" ")

    if [ -z "$myfile" ]; then
       	echo -e "Run $1 have not found!"
	exit 1;
    else
	 echo -e "LAST RECORDED File: $myfile"
    fi
}


# find particular run
function particularRun(){
#    echo $myPath
#    echo $1
    myfile=
    myfile=$(find "$myPath" -name "*$1*.slcio")
    
    if [ -z "$myfile" ]; then
       	echo -e "Run $1 have not found!"
	exit 1;
    else
	 echo -e "Run number FOUND! File: $myfile"
    fi
}

# get file according to selection option
if [ $1 = "0" ]; then
    lastRun
else
    particularRun $2
fi



######################################################
#         prepare xml file                           #
######################################################

filename=$(basename -- "$myfile");
myRunNumber=$(echo $filename | cut -c4-9);

fileDir=$(dirname "${myfile}")

#get fresh new xml file 
#cp $evenDpath/CED_Cosmics_May2018.xml $evenDpath/eventDisplay.xml
cp ../xml/steering_raw.xml ../xml/steering.xml
# add run file to the xml
sed -i "s#MyRunNumber#$myRunNumber#g" ../xml/steering.xml
sed -i "s#IFP#$fileDir#g" ../xml/steering.xml
echo "File search DONE!"
