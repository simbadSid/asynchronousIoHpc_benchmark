#!/bin/bash



PATH_STATISTIC=${1}
PATH_STATISTIC_ARCHIVE=${2}
dateAndTime=`date | tr \  _ | tr \: -`
BACKUP_FILE="z_backup_${dateAndTime}.tar"

HEIGHT=25
WIDTH=100
CHOICE_HEIGHT=15
BACKTITLE="Backtitle here"
TITLE="Title here"
MENU="Choose one of the following options:"

OPTIONS=()
counter=0
CHOICE=""


####################################################
# Auxiliary functions
####################################################
function scanArchivDir
{
	for file in `ls ${PATH_STATISTIC_ARCHIVE}`;
	do
		OPTIONS+=("$counter")
		OPTIONS+=("$file")
		counter=$((counter + 1))
	done;
}


function getUserChoice
{
	CHOICE=$(dialog --clear \
		        --backtitle "$BACKTITLE" \
		        --title "$TITLE" \
		        --menu "$MENU" \
		        $HEIGHT $WIDTH $CHOICE_HEIGHT \
		        "${OPTIONS[@]}" \
		        2>&1 >/dev/tty)
	clear
}


function updateStatisticDir
{
	rm -rf ${PATH_STATISTIC}/*
	cp ${PATH_STATISTIC_ARCHIVE}/${CHOICE} ${PATH_STATISTIC}/
	dirSave=`pwd`
	cd ${PATH_STATISTIC}/
	tar xvf ${CHOICE}
	rm ${CHOICE}
	cd $dirSave
}


function backupStatisticDir
{
	dirSave=`pwd`
	cd ${PATH_STATISTIC}/
	tar cvf ${BACKUP_FILE} *
	cd $dirSave
	mv ${PATH_STATISTIC}/${BACKUP_FILE} ${PATH_STATISTIC_ARCHIVE}/
}


####################################################
# Main function
####################################################
scanArchivDir
getUserChoice
if [ ! ${CHOICE} ];
then
	echo "exit"
	exit
fi

CHOICE=$((CHOICE * 2)); CHOICE=$((CHOICE + 1))
CHOICE=${OPTIONS[ ${CHOICE} ]}
echo User statistic choice = ${CHOICE}
backupStatisticDir
updateStatisticDir
ls -al ${PATH_STATISTIC}

