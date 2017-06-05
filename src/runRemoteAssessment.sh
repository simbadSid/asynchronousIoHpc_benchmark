#!/bin/bash



function localRequest
{
	make plotPointCompareHPC
}


function remoteRequest
{
	rm ../resource/statistic/*

#	nohup make runAllBenchmarkHPC &
	make runAllBenchmarkHPC
}






ssh_remote_id=sidlakhr
ssh_remote_domain=jureca.fz-juelich.de

svn_commitTitle="Automatic commit: in order to run tests on HPC-JURECA_"`date "+%F-%T"`

remote_pathResult=AIO-Benchmarks/resource/statistic/
remote_pathRun=AIO-Benchmarks/trunk/
local_pathResult=../resource/statistic/


function sendRequest
{
	make mrproper
	cd ../
	svn commit -m \""$svn_commitTitle"\"
	cd trunk

	echo; echo; echo; echo; echo; echo; echo; echo;
	echo "-----------------------------"
	echo "Start the remote execution"
	echo "-----------------------------"
	ssh ${ssh_remote_id}@${ssh_remote_domain} "bash -s -l" -- < ${0} "remote"

	echo; echo; echo; echo; echo; echo; echo; echo; 
	echo "-----------------------------"
	echo "End the remote execution"
	echo "-----------------------------"
	scp ${ssh_remote_id}@${ssh_remote_domain}:"~"/${remote_pathResult}/* ${local_pathResult}
	localRequest
}


function processRemoteRequest
{
	source ~/.bashrc
	cd ${remote_pathRun}
	make mrproper
	cd ../
	svn revert -R .
	svn up
	cd trunk
	remoteRequest
}



if [ ${#} -eq "1" ] && [ ${1} == remote ]; then
	processRemoteRequest
else
	sendRequest
fi




