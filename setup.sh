#!echo You must source

#export CVS_RSH=ssh
#export MIDASSYS=/home/nam/sw/midas/
#export PATH=.:$MIDASSYS/linux/bin:/home/nam/sw/roody/bin:$PATH

# Get experiment name from the current directory
# add a random string to make sure no duplication in client name
expname=${PWD##*/}
rndstr=$(tr -dc "[:alpha:]" < /dev/urandom | head -c 8)

# Uncomment next line for the first run only
# echo ""$expname"_$rndstr $PWD $(whoami)" > exptab

export MIDAS_EXPTAB=$(pwd)/exptab

mkdir -p output/root output/mid log/history log/odbdump elog

#git update-index --assume-unchanged exptab
#end
