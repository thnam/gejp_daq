#!/bin/bash

# Check exptab
if [[ ! -f exptab ]]; then
  echo "No exptab found, will generate one ..."
  expname=${PWD##*/}
  rndstr=$(tr -dc "[:alpha:]" < /dev/urandom | head -c 8)

  echo ""$expname"_$rndstr $PWD $(whoami)" > exptab
else
  echo "Will use the current exptab: " $(cat exptab)
fi

export MIDAS_EXPTAB=$(pwd)/exptab

# Create some useful dirs
mkdir -p output/root output/mid log/history log/odbdump elog

# support drivers
echo "Making vme drivers if needed ..."
cd bit3_vme_drivers
make -j2 2>&1 > /dev/null

# check make status
if [[ $? -ne 0 ]] ; then
  echo "Making drivers failed!"
  cd ..
else
  echo "Done!"
  cd ..
fi

