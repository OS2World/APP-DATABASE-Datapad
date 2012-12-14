/* pstruct  ver. 2.1 7-17-95*/

#define INCL_WIN
#define INCL_DOS
#define INCL_SUB
#define  INCL_GPI
#include <os2.h>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <sys/types.h>
#include <process.h>
#include "paddef.h"

VOID QueryDiskSpace (ULONG drvNum, PULONG totalSpace,
		     PULONG allocated, PULONG available);
MRESULT EXPENTRY newSizeProc( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 );
INT newSizePrompt(HWND hwndD);
INT multiRead(CHAR *inFile, INT datS, INT sizeF, INT recNum);
INT multiWrite(CHAR *inFile, INT datS, INT sizeF, INT recNum);
INT getFLRecSize(CHAR *inFile);
VOID FAR resizePDF(VOID *hwnd);

extern VOID enableButtons(VOID);
extern VOID disableButtons(VOID);
extern VOID unSubClassWin(VOID);
extern VOID subClassWin(VOID);
extern INT initLoad(HWND hwndList, CHAR *fileName);
extern VOID setStatus(HWND hwndST, CHAR *pFile);


HEV hevrs;
INT sModSize;

extern struct DATAMOD
   {
   CHAR noteText[MAXNAME];
   }pdf[NUMPDFMODS];
extern struct DATAMODX
   {
   CHAR noteText[MAXNAME];
   }pdf2[NUMPDFMODS];
extern struct DAT
   {
   SHORT posIndex;
   SHORT alIndex;
   CHAR recName[NAMESIZE];
   } recIndex[MAXNUMRECS];
extern struct DATA
   {
   SHORT indx;
   CHAR noteName[NAMESIZE];
   CHAR noteText[NOTESIZE];
   } dataRecs;
extern struct DATAS
   {
   SHORT indx;
   CHAR noteName[NAMESIZE];
   CHAR noteText[SPECIALDATA];
   }dataR;
extern FILE *datHan, *datHan2;
extern CHAR fullFileName[MAXNAME];
extern CHAR datFile[MAXNAME];
extern INT numRecs;
extern SHORT choice;
extern FILEDLG fdFileDlg;
extern CHAR initFSpec[MAXNAME];
extern INT nCho;
extern CHAR ntName[NAMESIZE];
extern struct find_t ffblk;
extern HOBJECT hobjFG;
extern HOBJECT hobjBG;
extern HOBJECT hobjFNT;
extern pValue[16][32];
extern pgmData[60];
extern CHAR basePath[MAXNAME];
extern BOOL ACCUM;
extern CHAR mcString[MAXNAME];
extern CHAR stickyName[MAXNAME];
extern CHAR ndName[];
extern CHAR npdf[260];
extern UINT FIXEDLEN;
extern HWND hwndMenu;
extern HWND hwndFrame;
extern CHAR EXTENTION[EXTSIZE];



VOID QueryDiskSpace (ULONG drvNum, PULONG totalSpace,
		     PULONG allocated, PULONG available)
{
FSALLOCATE fsAllocate;

DosError (FERR_DISABLEHARDERR);

if (!DosQueryFSInfo (drvNum, FSIL_ALLOC, &fsAllocate, sizeof(FSALLOCATE)))
   {
   *totalSpace =
	    fsAllocate.cSectorUnit * fsAllocate.cUnit * fsAllocate.cbSector;
   *available  =
	    fsAllocate.cSectorUnit * fsAllocate.cUnitAvail * fsAllocate.cbSector;
   *allocated  = *totalSpace - *available;
   }
else
   *totalSpace = *allocated = *available  = 0L;

DosError (FERR_ENABLEHARDERR);
return;
}


INT getFLRecSize(CHAR *inFile)
{
FILE *mHan;
INT flrs;
struct DAT rcI[MAXNUMRECS];

mHan = fopen( inFile, MRO);
fseek(mHan, 0L, SEEK_SET);
fread(&rcI, sizeof(rcI), 1, mHan);
fclose(mHan);
flrs = rcI[0].alIndex;
if(flrs > 0 )
   return(flrs);
else
   return(NOTESIZE);
}


INT multiRead(CHAR *inFile, INT datS, INT sizeF, INT recNum)
{
struct Samp
   {
   SHORT indx;
   CHAR noteName[NAMESIZE];
   }sample;
ULONG oneRecSize, start;
APIRET apiret;
ULONG ulAction, ulBufferSize, ulBytesRead, newP;
HFILE hfile;

ulBufferSize = sizeF;
apiret = DosOpen(inFile,
		 &hfile,
		 &ulAction,
		 ulBufferSize,
		 FILE_NORMAL,
		 OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
		 OPEN_ACCESS_READONLY | OPEN_SHARE_DENYWRITE | OPEN_FLAGS_SEQUENTIAL,
		 NULL);
oneRecSize = sizeof(sample) + sizeF;
start = sizeof(recIndex);
DosSetFilePtr(hfile, start+(oneRecSize*(recNum)), FILE_BEGIN, &newP);
DosRead (hfile, &sample, sizeof(sample), &ulBytesRead);
if( datS == 1 )
   {
   DosRead (hfile, &dataRecs.noteText, sizeF, &ulBytesRead);
   dataRecs.indx = sample.indx;
   strcpy(dataRecs.noteName, sample.noteName);
   }
else
   {
   DosRead (hfile, &dataR.noteText, sizeF, &ulBytesRead);
   dataR.indx = sample.indx;
   strcpy(dataR.noteName, sample.noteName);
   }
DosClose(hfile);
return(1);
}



INT multiWrite(CHAR *inFile, INT datS, INT sizeF, INT recNum)
{
struct Samp
   {
   SHORT indx;
   CHAR noteName[NAMESIZE];
   }sample;
ULONG oneRecSize, start;
CHAR noteText[NOTESIZE];
APIRET apiret;
ULONG ulAction, ulBufferSize, ulBytesWritten, newP;
HFILE hfile;

ulBufferSize = sizeF;
apiret = DosOpen(inFile,
		 &hfile,
		 &ulAction,
		 ulBufferSize,
		 FILE_NORMAL,
		 OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
		 OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYWRITE | OPEN_FLAGS_SEQUENTIAL,
		 NULL);
oneRecSize = sizeof(sample) + sizeF;
start = sizeof(recIndex);
DosSetFilePtr(hfile, start+(oneRecSize*(recNum)), FILE_BEGIN, &newP);
if( datS == 1 )
   {
   sample.indx = dataRecs.indx;
   strcpy(sample.noteName, dataRecs.noteName);
   DosWrite(hfile, &sample, sizeof(sample), &ulBytesWritten);
   DosWrite(hfile, &dataRecs.noteText, sizeF, &ulBytesWritten);
   }
else
   {
   sample.indx = dataR.indx;
   strcpy(sample.noteName, dataR.noteName);
   DosWrite(hfile, &sample, sizeof(sample), &ulBytesWritten);
   DosWrite(hfile, &dataR.noteText, sizeF, &ulBytesWritten);
   }
DosClose(hfile);
return(1);
}


VOID resizePDF(VOID *hwnd)
{
HAB habT;
HMQ hmqT;
CHAR errMsg1[80];
CHAR sizeStr[15];
ULONG totalSpace, allocated, available;
ULONG drvNum, drvNumOrg, ulDriveMap;
struct find_t ffblk;
struct Smp
   {
   SHORT indx;
   CHAR noteName[NAMESIZE];
   }tplate;

habT = WinInitialize(0);
hmqT = WinCreateMsgQueue(habT, 0);
WinCancelShutdown(hmqT, TRUE);
subClassWin();
disableButtons();
/*
WinEnableMenuItem(hwndMenu, ID_MISC , FALSE);
WinEnableMenuItem(hwndMenu, ID_OPTIONS , FALSE);
WinEnableMenuItem(hwndMenu, ID_HELPME , FALSE);
WinEnableMenuItem(hwndMenu, ID_SETMODULES , FALSE);
WinEnableMenuItem(WinWindowFromID(hwndFrame, FID_SYSMENU), SC_CLOSE , FALSE);
*/

_dos_findfirst(datFile, _A_NORMAL, &ffblk);
DosQueryCurrentDisk(&drvNumOrg, &ulDriveMap);
drvNum = pdf[0].noteText[0] - '@';
DosSetDefaultDisk(drvNum);
QueryDiskSpace (drvNum,
		&totalSpace,
		&allocated,
		&available);
if( ((ffblk.size*2)+44000) >= available )
   {
   strcpy(errMsg1, "You need at least ");
   ultoa((ffblk.size*2)+44000, sizeStr, 10);
   strcat(errMsg1, sizeStr);
   strcat(errMsg1, " bytes free and possibly more to resize this file.");
   WinMessageBox(HWND_DESKTOP,
		 (HWND)hwnd,
		 errMsg1,
		 "Not enough disk space to accomplish task!",
		 0,
		 MB_ICONEXCLAMATION | MB_OK);
   }
else
   {
   INT LRECSIZE;
   INT xx, i;
   FILE *rHan, *hanTMP;
   div_t dvt;
   INT origFL;

   origFL = FIXEDLEN;
   rHan = fopen(datFile, MRW);
   fseek(rHan, 0L, SEEK_SET);
   fread(&recIndex, sizeof(recIndex), 1, rHan);
   xx = WinQueryLboxCount(WinWindowFromID((HWND)hwnd, ID_LISTBOX1));
   LRECSIZE = 0;
   for(i=0;i<xx;i++)
      {
      fread(&tplate, sizeof(tplate), 1, rHan);
      fread(&dataRecs.noteText, FIXEDLEN, 1, rHan);
      if( strlen(dataRecs.noteText) > LRECSIZE )
	 LRECSIZE = strlen(dataRecs.noteText);
      }
   fclose(rHan);
   if( LRECSIZE < 1000 )
      sModSize = 1;
   if( LRECSIZE > 28999 )
      sModSize = 30;
   if( (LRECSIZE > 999) && (LRECSIZE < 29000) )
      {
      dvt = div(LRECSIZE, 1000);
      if( dvt.rem == 0 )
	 sModSize = dvt.quot;
      else
	 sModSize = dvt.quot + 1;
      }
   if( newSizePrompt((HWND)hwnd) )
      {
      rHan = fopen(datFile, MRW);
      fseek(rHan, 0L, SEEK_SET);
      fread(&recIndex, sizeof(recIndex), 1, rHan);
      recIndex[0].alIndex = sModSize;
      hanTMP = fopen("TMP$$$$$.PDF", "w+b" );
      fwrite(&recIndex, sizeof(recIndex), 1, hanTMP);
      for(i=0;i<xx;i++)
	 {
	 fread(&tplate, sizeof(tplate), 1, rHan);
	 fread(&dataRecs.noteText, FIXEDLEN, 1, rHan);
	 fwrite(&tplate, sizeof(tplate), 1, hanTMP);
	 fwrite(&dataRecs.noteText, sModSize, 1, hanTMP);
	 }
      fclose(rHan);
      fclose(hanTMP);
      if( rename(datFile, "X$X$$$$$.PDF") != 0 )
	 {
	 FIXEDLEN = origFL;
	 recIndex[0].alIndex = FIXEDLEN;
	 sModSize = FIXEDLEN;
	 }
      if( rename("TMP$$$$$.PDF", datFile) != 0 )
	 {
	 FIXEDLEN = origFL;
	 recIndex[0].alIndex = FIXEDLEN;
	 rename("X$X$$$$$.PDF", datFile);
	 sModSize = FIXEDLEN;
	 }
      remove("X$X$$$$$.PDF");
      FIXEDLEN = sModSize;
      initLoad(WinWindowFromID((HWND)hwnd, ID_LISTBOX1), datFile);
      setStatus((HWND)hwnd, datFile);
      WinPostMsg(WinWindowFromID((HWND)hwnd, ID_LISTBOX1),
		 LM_SELECTITEM,
		 MPFROMSHORT(0),
		 MPFROMSHORT(TRUE));
      }
   }
DosSetDefaultDisk(drvNumOrg);
enableButtons();
unSubClassWin();
/*
WinEnableMenuItem(hwndMenu, ID_OPTIONS , TRUE);
WinEnableMenuItem(hwndMenu, ID_HELPME , TRUE);
WinEnableMenuItem(hwndMenu, ID_MISC , TRUE);
WinEnableMenuItem(hwndMenu, ID_SETMODULES , TRUE);
WinEnableMenuItem(WinWindowFromID(hwndFrame, FID_SYSMENU), SC_CLOSE , TRUE);
*/
WinDestroyMsgQueue(hmqT);
WinTerminate(habT);
_endthread();
}


INT newSizePrompt(HWND hwndD)
{
if(WinDlgBox(HWND_DESKTOP,
	    hwndD,
	    newSizeProc,
	    (HMODULE)0,
	    ID_NEWSIZE,
	    NULL))
   return(1);
else
   return(0);
}


MRESULT EXPENTRY newSizeProc( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 )
{
switch ( msg )
   {
   case WM_INITDLG:
      WinSetDlgItemShort(hwndDlg, ID_CURSIZE, FIXEDLEN*.001, TRUE);
      WinSendDlgItemMsg(hwndDlg, ID_NSIZE,
			SPBM_SETLIMITS,
			MPFROMLONG(30),
			MPFROMLONG(sModSize));
      break;

   case WM_COMMAND :
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_OK:
	    WinSendDlgItemMsg(hwndDlg, ID_NSIZE,
			      SPBM_QUERYVALUE,
			      MPFROMP(&sModSize),
			      MPFROM2SHORT(0,SPBQ_ALWAYSUPDATE));
	    sModSize *= 1000;
	    WinDismissDlg(hwndDlg, TRUE);
	    break;

	 case DID_CANCEL:
	    WinDismissDlg(hwndDlg, FALSE);
	    break;
	 }
      break;

   default :
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
   }
return (MRESULT) FALSE;
}



