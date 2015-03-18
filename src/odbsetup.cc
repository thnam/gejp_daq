// ****************************************************************************
// *     Filename:  odbsetup.cc
// *  Description:  Setup paths for odb
// *      License:	
// *       Author:  Tran Hoai Nam (nam at kuno-g.phys.sci.osaka-u.ac.jp)
// * Organization:  Osaka University
// *      Created:  2013-04-28 07:34:57
// ****************************************************************************
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <algorithm>

#include "midas.h"
int main()
{
	HNDLE hDB, hKey;
	int status = cm_connect_experiment("","","ODBSetup",NULL);
	if (status != CM_SUCCESS)
	{
		printf("ODB set up failed! You have to do it manually.\n");
		return 1;
	}

	cm_get_experiment_database(&hDB,&hKey);

	char keystr[256];
	char cwd[256];
	getcwd(cwd,sizeof(cwd));
	strcat(cwd,"/");

	// Directories
	strcpy(keystr,cwd);
	strcat(keystr,"log/");
	db_set_value(hDB,0,"Logger/Data dir",keystr,sizeof(keystr),1,TID_STRING);

	strcpy(keystr,cwd);
	strcat(keystr,"log/history/");
	//db_create_key(hDB, 0, "Logger/History dir", TID_STRING); // not necessary
	db_set_value( hDB, 0, "Logger/History dir", // key
			keystr, sizeof(keystr), 1, TID_STRING); 

	strcpy(keystr,cwd);
	strcat(keystr,"elog/");
	db_set_value( hDB, 0, "Logger/Elog dir",
			keystr, sizeof(keystr), 1, TID_STRING); 

	int boolvalue = 1;
	db_set_value(hDB,0,"Logger/ODB Dump",&boolvalue,sizeof(boolvalue),1,TID_BOOL);
	strcpy(keystr,cwd);
	strcat(keystr,"log/odbdump/run%05d.xml");
	db_set_value(hDB,0,"Logger/ODB Dump File",keystr,sizeof(keystr),1,TID_STRING);
	
	int compress = 1;
	db_set_value(hDB,0,"Logger/Channels/0/Settings/Compression",
			&compress, sizeof(compress), 1, TID_INT);

	strcpy(keystr,cwd);
	strcat(keystr,"/output/mid/run%05d.mid.gz");
	db_set_value(hDB,0,
			"Logger/Channels/0/Settings/Filename",keystr,sizeof(keystr),1,TID_STRING);

	// Finish
	cm_disconnect_experiment();
	printf("Done ODB set up\n");
	return 1;
}
