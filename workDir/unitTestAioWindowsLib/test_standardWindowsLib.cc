#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef union sigval
  {
    int sival_int;
    void *sival_ptr;
  } sigval_t;



#define NB_WRITE	100
#define BUFFER_SIZE	9


struct _OVERLAPPED *pOverlapped;
HANDLE *pEventH;
HANDLE *pFile;



void CompletionRoutine(DWORD u32_ErrorCode, DWORD u32_BytesTransfered, OVERLAPPED* pk_Overlapped)
{
//    printf("CompletionRoutine: Transferred: %d Bytes, AddrOverlapped: 0x%X\n", (int)u32_BytesTransfered, (DWORD_PTR)pk_Overlapped);
    printf("CompletionRoutine ************\n");
    fflush(stdout);
}


int main()
{
	const char *pName = "test_standardWindowsLib.data";
	char *buffer[NB_WRITE];
	BOOL bErrorFlag = FALSE;

	int j=0;
	int i=0;

	// create / open the file
	pFile = (HANDLE*)malloc(sizeof(HANDLE));
	*pFile=CreateFile(	pName,
						GENERIC_WRITE,			// open for writing
						0,						// share write access
						NULL,					// default security
						CREATE_ALWAYS,			// create new/overwrite existing
						FILE_FLAG_OVERLAPPED,	// | FILE_FLAG_NO_BUFFERING,   // overlapped file
						NULL);			// no attr. template

	// check whether file opening was ok
	if(*pFile==INVALID_HANDLE_VALUE)
	{
		printf("%x\n",(int)GetLastError());
		exit(0);
	}

	// make the overlapped structure
	pOverlapped = (_OVERLAPPED*)calloc(1,sizeof(struct _OVERLAPPED));
	pOverlapped->Offset = 0;
	pOverlapped->OffsetHigh = 0;
	pOverlapped->hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (pOverlapped->hEvent == NULL)
	{
		printf("Unable to create wait event! Error: %u\n", (unsigned int)GetLastError());
		exit(0);
	}

	// put event handle in overlapped structure
	if(!(pOverlapped->hEvent = CreateEvent(NULL,TRUE,FALSE,NULL)))
	{
		printf("%x\n",(int)GetLastError());
		exit(0);
	}

	// create some output
	for(j=0;j<NB_WRITE;j++)
	{
		buffer[j] = (char*)malloc(sizeof(char) * BUFFER_SIZE);
		char *tmp = buffer[j];
		for (i=0; i<BUFFER_SIZE; i++)
		{
			*tmp = 'a'+j;
			tmp ++;
		}
	}

	for(i=0;i<NB_WRITE;i++)
	{
		// write the bytes
		printf("Write: %s\n", buffer[i]);
		if(!(bErrorFlag = WriteFileEx(*pFile, buffer[i], BUFFER_SIZE, pOverlapped, &CompletionRoutine)))
//		if(!(bErrorFlag = WriteFile(*pFile,buffer[i], BUFFER_SIZE, NULL, pOverlapped)))
		{
			// check whether io pending or some other error
			if(GetLastError()!=ERROR_IO_PENDING)
			{
				printf("lasterror: %x\n",(int)GetLastError());
				printf("error while writing file\n");
				exit(0);
			}
		}

		// wait to continue
/*
		if((WaitForSingleObject(pOverlapped->hEvent,INFINITE))!=WAIT_OBJECT_0)
		{
			printf("waiting did not succeed\n");
			exit(0);
		}

		// reset event structure
		if(!(ResetEvent(pOverlapped->hEvent)))
		{
			printf("%x\n",(int)GetLastError());
			printf("error in resetevent\n");
			exit(0);
		}

		pOverlapped->Offset+=BUFFER_SIZE;
*/
	}
	SleepEx(~0, TRUE);
	CloseHandle(*pFile);
	free(pFile);
	free(pOverlapped);

	return 1;
}
