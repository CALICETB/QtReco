#!/bin/bash

echo "Run $1"
echo "Input File $2"
echo "Output File ROOT $3"
echo "Output File SLCIO $4"
echo "Pedestal Substraction $5"
echo "ADC to MIP $6"
echo "SLCIO_OUTPUT $7"

rm -rf /tmp/saivahu/Reco_Run_$1.xml
rm -rf /tmp/saivahu/Slcio_Run_$1.slcio
cp ${PWD}/../xml/Reco.xml /tmp/saivahu/Reco_Run_$1.xml
cp ${PWD}/../xml/Reco.xml /tmp/saivahu/Slcio_Run_$1.slcio

sed -i "s#INPUT#$2#g" /tmp/saivahu/Reco_Run_$1.xml
sed -i "s#RUNNUMBER#$1#g" /tmp/saivahu/Reco_Run_$1.xml
sed -i "s#OUTPUT_ROOT#$3#g" /tmp/saivahu/Reco_Run_$1.xml
sed -i "s#OUTPUT_SLCIO#$4#g" /tmp/saivahu/Reco_Run_$1.xml
sed -i "s#PEDSUB#$5#g" /tmp/saivahu/Reco_Run_$1.xml
sed -i "s#MIPCONV#$6#g" /tmp/saivahu/Reco_Run_$1.xml
sed -i "s#SLCIO_OUTPUT#$7#g" /tmp/saivahu/Slcio_Run_$1.slcio
