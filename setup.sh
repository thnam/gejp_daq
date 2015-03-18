#!echo You must source

if [[ ! -f exptab ]]; then
  echo "No exptab found, will generate one ..."
  expname=${PWD##*/}
  rndstr=$(tr -dc "[:alpha:]" < /dev/urandom | head -c 8)

  echo ""$expname"_$rndstr $PWD $(whoami)" > exptab
else
  echo "Will use the current exptab: " $(cat exptab)
fi

export MIDAS_EXPTAB=$(pwd)/exptab

mkdir -p output/root output/mid log/history log/odbdump elog

