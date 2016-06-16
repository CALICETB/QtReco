#!/bin/bash

echo "Run $1"
echo "Input File $2"
echo "Output File ROOT $3"
echo "Output File SLCIO $4"
echo "Pedestal Substraction $5"
echo "ADC to MIP $6"

rm -rf /tmp/Reco_Run_$1.xml
cp ${PWD}/../xml/Reco.xml /tmp/Reco_Run_$1.xml

sed -i "s#INPUT#$2#g" /tmp/Reco_Run_$1.xml
sed -i "s#RUNNUMBER#$1#g" /tmp/Reco_Run_$1.xml
sed -i "s#OUTPUT_ROOT#$3#g" /tmp/Reco_Run_$1.xml
sed -i "s#OUTPUT_SLCIO#$4#g" /tmp/Reco_Run_$1.xml
sed -i "s#PEDSUB#$5#g" /tmp/Reco_Run_$1.xml
sed -i "s#MIPCONV#$6#g" /tmp/Reco_Run_$1.xml
