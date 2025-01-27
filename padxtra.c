/* extra functions  PADXTRA.c   2.1  7-17-95  */

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

VOID setPPMenuDef(INT num);
INT initLoad_index(HWND hwndList, CHAR *dataFileName);
VOID zeroRecs(VOID);
VOID zeroIndex(INT index);
VOID zeroDataRecs(CHAR *noteText);
VOID zeroAllIndex(VOID);
INT deleteRec(HWND hwndMain);
INT openFile(CHAR *fileName, CHAR *mode);
VOID closeFile(VOID);
INT truncFile(CHAR *fileName, INT noRecs, INT flrs);
VOID saveIndex(CHAR *dataFileName);
INT printRec(HWND hwndDlg, BOOL ASK);
BOOL FindFile (HWND hwndWnd);
BOOL FindDatFile (HWND hwndWnd);
INT existPath(CHAR *pathStr);
INT chkName(HWND hwnd, CHAR *lbSelect);
INT compName(HWND hwndM, INT cho);
INT compName2(CHAR *sName, INT nrec);
INT matchName(HWND hwndM, CHAR *lbSelect);
INT copyData(INT c);
VOID dBaseFull(HWND hwndDB);
INT osName(HWND hwndDlg);
VOID parseName(CHAR *name);
INT chkDupeName(HWND hwndDlg);
VOID setParams(HWND hwndP, SHORT dx, ULONG IDE, ULONG IDR);
VOID getPath(VOID);
VOID setMod(HWND hwndD);
INT parseNewName(CHAR *str);
INT saveRecFile(CHAR *str, SHORT index, CHAR *mode);
HFILE openTmp(HFILE hfile, CHAR *szFullPathName, CHAR *szFullDocName, CHAR *fileName);
INT getModList(HWND hwndML, INT mode);
INT loadIndex(HWND hwndI, CHAR *dataFileName);
INT dupeName(HWND hwndML);
INT dupeRecName(HWND hwndML);
INT existFile(CHAR *fileStr);
VOID zeroMostIndex(INT index);
INT saveToFile(CHAR *flName, FILE *hdl);
INT matchAnyName(HWND hwndM, CHAR *recname);
MRESULT EXPENTRY dupeNameProc( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY dupeRecNameProc(HWND hwndDlg, ULONG msg,
				 MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY gmlProc1( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY gmlProc2( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY setModProc( HWND hwndDlg, ULONG msg,
				     MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY osNProc( HWND hwndDlg, ULONG msg,
				  MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY findProc( HWND hwndDlg, ULONG msg,
				  MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY newPdfProc( HWND hwndDlg, ULONG msg,
				  MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY newModProc( HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY stickyProc( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 );
ULONG MLEGetT(HWND hwndDlg, ULONG ulID, LONG lIptPosition,
	      LONG lBoundary, PBYTE *ppText);
INT printIt(HWND hwndP, USHORT id, BOOL ASK);
INT setRGBColor(HWND hwndC, ULONG r, ULONG rg, ULONG rb);
INT removeSwEntry(HWND hwnd);
INT addSwEntry(HWND hwnd);
INT setDefMod(HWND hwndD);
INT strErrMsg(HWND hwnd, CHAR *str);
ULONG MLEGetItemTextS(HWND hwndDlg, ULONG ulID, LONG lIptPosition,
      LONG lBoundary, PBYTE *ppText);
VOID addFWHeader(FILE *hand, CHAR *dRec);
MRESULT EXPENTRY createPDFProc( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 );
INT createPDF(HWND hwndD);
INT newPDFLoad(HWND hwndList, CHAR *fileName, INT len);
// VOID setDateTime(HWND hwnd, ULONG ID);
VOID setDateTime(HWND hwnd, ULONG ID, INT mode, BOOL AMPM, BOOL DAYNAME);
ULONG MLEInsertTextS(HWND hwndDlg, ULONG ulID, LONG iptPosition, PBYTE pText);

extern INT initLoad(HWND hwndList, CHAR *fileName);
extern INT chkIntegrityPDF(HWND hwndQ, CHAR *PDFName);
extern INT multiRead(CHAR *inFile, INT datS, INT sizeF, INT recNum);
extern INT multiWrite(CHAR *inFile, INT datS, INT sizeF, INT recNum);
/**********  DEBUG FUNCS *************************/
extern VOID msgOK(CHAR *title, CHAR *message);
/**********  DEBUG FUNCS *************************/


extern struct DATAMOD
   {
   CHAR noteText[MAXNAME];
   }pdf[NUMPDFMODS];
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
extern CHAR EXTENTION[EXTSIZE];
extern HWND hwndMainFrame;
extern HWND hwnd1;
extern HWND hwndMainDlg;


CHAR fMask[NAMESIZE];
INT newPDFLen;



VOID setPPMenuDef(INT num)
{
pgmData[num] = 0;
switch( num )
   {
   case 4 :
      WinCheckMenuItem(hwndMenu, ID_DEFVIEW, FALSE);
      break;

   case 5 :
      WinCheckMenuItem(hwndMenu, ID_DEFEDIT, FALSE);
      break;

   case 6 :
      WinCheckMenuItem(hwndMenu, ID_DEFSEARCH, FALSE);
      break;

   case 7 :
      WinCheckMenuItem(hwndMenu, ID_DEFADD, FALSE);
      break;

   case 32 :
      WinCheckMenuItem(hwndMenu, ID_MAINLIST, FALSE);
      break;

   default :
      break;
   }
}


VOID setDateTime(HWND hwnd, ULONG ID, INT mode, BOOL AMPM, BOOL DAYNAME)
{
BOOL fHaveCtryInfo = FALSE;
CHAR *szDayName[] = { "Sun", "Mon", "Tue", "Wed", "Thu",
		      "Fri", "Sat" };
CHAR szDateFormat[] = " %s  %d%s%02d%s%02d ";
CHAR szDateFormatAlone[] = "%s  %d%s%02d%s%02d ";
COUNTRYCODE ctryc = { 0,0 };
COUNTRYINFO ctryi;
CHAR szBuffer[20];
DATETIME dt;
ULONG usDataLength, loc;
INT len;

if (!fHaveCtryInfo)
   {
   DosGetCtryInfo(sizeof(ctryi), &ctryc, &ctryi, &usDataLength);
   fHaveCtryInfo = TRUE;
   }
DosGetDateTime(&dt);
dt.year %= 100;

if( !DAYNAME )
   strcpy(szDayName[dt.weekday], "");
if( mode == 1 || mode == 3 )
   {
   if( mode == 3 )
      {
      if (ctryi.fsDateFmt == 0)
	 sprintf(szBuffer, szDateFormatAlone, szDayName[dt.weekday],
		 dt.month, ctryi.szDateSeparator,
		 dt.day, ctryi.szDateSeparator, dt.year);
      else if (ctryi.fsDateFmt == 1)
	 sprintf(szBuffer, szDateFormatAlone, szDayName[dt.weekday],
		 dt.day, ctryi.szDateSeparator,
		 dt.month, ctryi.szDateSeparator, dt.year);
      else
	 sprintf(szBuffer, szDateFormatAlone, szDayName[dt.weekday],
		 dt.year, ctryi.szDateSeparator,
		 dt.month, ctryi.szDateSeparator, dt.day);
      }
   else
      {
      if (ctryi.fsDateFmt == 0)
	 sprintf(szBuffer, szDateFormat, szDayName[dt.weekday],
		 dt.month, ctryi.szDateSeparator,
		 dt.day, ctryi.szDateSeparator, dt.year);
      else if (ctryi.fsDateFmt == 1)
	 sprintf(szBuffer, szDateFormat, szDayName[dt.weekday],
		 dt.day, ctryi.szDateSeparator,
		 dt.month, ctryi.szDateSeparator, dt.year);
      else
	 sprintf(szBuffer, szDateFormat, szDayName[dt.weekday],
		 dt.year, ctryi.szDateSeparator,
		 dt.month, ctryi.szDateSeparator, dt.day);
      }
   MLEInsertTextS(hwnd, ID, -1, szBuffer);
   len = strlen(szBuffer);
   }
else
   len = 0;

if( mode == 1 || mode == 2 )
   {
   if( AMPM )
      {
      if ((ctryi.fsTimeFmt & 1) == 0)
      sprintf(szBuffer, "%d%s%02d%s%02d %c.m. ",
			(dt.hours + 11)  % 12 + 1, ctryi.szTimeSeparator,
			dt.minutes, ctryi.szTimeSeparator,
			dt.seconds, dt.hours / 12 ? 'p' : 'a');
      }
   else
      {
      sprintf(szBuffer, "%02d%s%02d%s%02d ",
			(dt.hours), ctryi.szTimeSeparator,
			dt.minutes, ctryi.szTimeSeparator, dt.seconds);
      }
   MLEInsertTextS(hwnd, ID, -1, szBuffer);
   len += strlen(szBuffer);
   }
loc = LONGFROMMR(WinSendMsg(WinWindowFromID(hwnd, ID),
			    MLM_QUERYSEL,
			    (MPARAM) MLFQS_MINSEL,
			    (MPARAM) 0L));
WinSendMsg(WinWindowFromID(hwnd, ID),
	   MLM_SETSEL,
	   MPFROMP(loc+len),
	   MPFROMP(loc+len));
}

/*
VOID setDateTime(HWND hwnd, ULONG ID)
{
BOOL fHaveCtryInfo = FALSE;
CHAR *szDayName[] = { "Sun", "Mon", "Tue", "Wed", "Thu",
		      "Fri", "Sat" };
CHAR szDateFormat[] = " %s  %d%s%02d%s%02d ";
COUNTRYCODE ctryc = { 0,0 };
COUNTRYINFO ctryi;
CHAR szBuffer[20];
DATETIME dt;
ULONG usDataLength, loc;
INT len;

if (!fHaveCtryInfo)
   {
   DosGetCtryInfo(sizeof(ctryi), &ctryc, &ctryi, &usDataLength);
   fHaveCtryInfo = TRUE;
   }
DosGetDateTime(&dt);
dt.year %= 100;

if (ctryi.fsDateFmt == 0)
   sprintf(szBuffer, szDateFormat, szDayName[dt.weekday],
	   dt.month, ctryi.szDateSeparator,
	   dt.day, ctryi.szDateSeparator, dt.year);

else if (ctryi.fsDateFmt == 1)
   sprintf(szBuffer, szDateFormat, szDayName[dt.weekday],
	   dt.day, ctryi.szDateSeparator,
	   dt.month, ctryi.szDateSeparator, dt.year);

else
   sprintf(szBuffer, szDateFormat, szDayName[dt.weekday],
	   dt.year, ctryi.szDateSeparator,
	   dt.month, ctryi.szDateSeparator, dt.day);

MLEInsertTextS(hwnd, ID, -1, szBuffer);
len = strlen(szBuffer);

if ((ctryi.fsTimeFmt & 1) == 0)
   sprintf(szBuffer, "%d%s%02d%s%02d %c.m. ",
		     (dt.hours + 11)  % 12 + 1, ctryi.szTimeSeparator,
		     dt.minutes, ctryi.szTimeSeparator,
		     dt.seconds, dt.hours / 12 ? 'p' : 'a');

else
   sprintf(szBuffer, "%02d%s%02d%s%02d ",
		     (dt.hours), ctryi.szTimeSeparator,
		     dt.minutes, ctryi.szTimeSeparator, dt.seconds);

MLEInsertTextS(hwnd, ID, -1, szBuffer);
len += strlen(szBuffer);
loc = LONGFROMMR(WinSendMsg(WinWindowFromID(hwnd, ID),
			    MLM_QUERYSEL,
			    (MPARAM) MLFQS_MINSEL,
			    (MPARAM) 0L));
WinSendMsg(WinWindowFromID(hwnd, ID),
	   MLM_SETSEL,
	   MPFROMP(loc+len),
	   MPFROMP(loc+len));
}

*/

ULONG MLEInsertTextS(HWND hwndDlg, ULONG ulID, LONG iptPosition, PBYTE pText)
{
  PBYTE pMLEBuff=0;
  ULONG ulBuffSize;
  ULONG ulBytesCopied=0;
  HWND  hwndControl = WinWindowFromID(hwndDlg, ulID);

  /* Allocate MLE transfer buffer. MLE limited to 64K tiled memory */
  ulBuffSize = 65535;
  if (!DosAllocMem((PPVOID) &pMLEBuff, (ULONG) ulBuffSize,
	     PAG_COMMIT | PAG_READ | PAG_WRITE | OBJ_TILE)) {
     ULONG ulBytesToCopy;
     ULONG ulTextSize = strlen((char *)pText);

     /* Set import/export MLE buffer */
     WinSendMsg(hwndControl, MLM_SETIMPORTEXPORT,
	   MPFROMP(pMLEBuff), MPFROMLONG( ulBuffSize));
     do {
	if (ulBuffSize < ulTextSize-ulBytesCopied)
	   ulBytesToCopy=ulBuffSize;
	else
	   ulBytesToCopy=ulTextSize-ulBytesCopied;
	memcpy((PVOID) pMLEBuff, &(pText[ulBytesCopied]),
		     ulBytesToCopy);

	/* Insert text at given IPT */
	WinSendMsg(hwndControl, MLM_IMPORT,
		 (MPARAM) &iptPosition, (MPARAM) ulBytesToCopy);
	ulBytesCopied+=ulBytesToCopy;
     } while (ulBytesCopied < ulTextSize);

     /* Free MLE transfer buffer */
     DosFreeMem((PBYTE)pMLEBuff);
  }
  return ulBytesCopied;
}


INT newPDFLoad(HWND hwndList, CHAR *fileName, INT len)
{
FILE *Hanv;
INT i = 0;
struct DAT rec[MAXNUMRECS];

for( i=0;i<MAXNUMRECS;i++ )
    rec[i].recName[0] = '\0';
rec[0].alIndex = len;
if ((Hanv = fopen( fileName, "wb" )) == NULL)
   {
   DosBeep(1000, 100);
   exit(-1);
   }
fwrite(&rec, sizeof(rec), 1, Hanv);
fclose(Hanv);
return(1);
}


INT setRGBColor(HWND hwndC, ULONG r, ULONG g, ULONG b)
{
RGB2 rgb;

rgb.bRed      = r;
rgb.bGreen    = g;
rgb.bBlue     = b;
rgb.fcOptions = 0;
WinSetPresParam(hwndC,
		PP_FOREGROUNDCOLOR,
		(ULONG)sizeof(RGB2),
		(PVOID)&rgb);
return(1);
}

INT addSwEntry(HWND hwnd)
{
SWCNTRL sw;
PID pd;

WinQueryWindowProcess(hwnd, &pd, NULL);
sw.hwnd = hwnd;                /* window handle      */
sw.hwndIcon = NULLHANDLE;         /* icon handle        */
sw.hprog = NULLHANDLE;            /* program handle     */
sw.idProcess = pd;               /* process identifier */
sw.idSession = 0;                 /* session identifier */
sw.uchVisibility = SWL_VISIBLE;   /* visibility         */
sw.fbJump = SWL_JUMPABLE;         /* jump indicator     */

strcpy(sw.szSwtitle, "DataPad");
WinAddSwitchEntry(&sw);
return(1);
}

INT removeSwEntry(HWND hwnd)
{
HSWITCH hswitch;
SWCNTRL swctl;

hswitch = WinQuerySwitchHandle(hwnd, 0);
WinRemoveSwitchEntry(hswitch);
return(1);
}


HFILE openTmp(HFILE hfile, CHAR *szFullPathName, CHAR *szFullDocName, CHAR *fileName)
{
CHAR tnum[3];
INT rz;
INT cnum;
CHAR cwd[MAXNAME];
APIRET apiret;
ULONG ulAction, ulBufferSize, ulBytesWritten, newP;

apiret = DosOpen(szFullDocName,
		 &hfile,
		 &ulAction,
		 ulBufferSize,
		 FILE_NORMAL,
		 OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
		 OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYNONE | OPEN_FLAGS_SEQUENTIAL,
		 NULL);
if ( apiret != 0 )
   {
   for( rz=2;rz<NOTMPFILES;rz++ )
      {
      strcpy(szFullPathName, "");
      strcpy(szFullDocName, "");
      getcwd(cwd, MAXNAME);
      if( cwd != NULL )
	 {
	 strcpy(szFullPathName, cwd);
	 if( szFullPathName[strlen(szFullPathName)-1] != '\\' )
	    {
	    strcat(szFullPathName, "\\");
	    strcat(szFullPathName, "\0");
	    }
	 }
      strcpy(szFullDocName, szFullPathName);
      strcpy(fileName, "TEMP");
      itoa(rz, tnum, 10);
      strcat(fileName, tnum);
      strcat(fileName, ".TXT");
      strcat(szFullDocName, fileName);
      apiret = DosOpen(szFullDocName,
		 &hfile,
		 &ulAction,
		 ulBufferSize,
		 FILE_NORMAL,
		 OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
		 OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYNONE | OPEN_FLAGS_SEQUENTIAL,
		 NULL);
      if( apiret == 0 )
	 rz = NOTMPFILES;
      }
   }
if(  apiret != 0 )
   return(3);
else
   return(hfile);
}


INT truncFile(CHAR *fileName, INT noRecs, INT flrs)
{
ULONG size;
INT han;
struct Samp
   {
   SHORT indx;
   CHAR noteName[NAMESIZE];
   }sample;

size = ((noRecs - 1) * ( flrs+sizeof(sample))) + sizeof(recIndex);
han = open(fileName, O_RDWR, S_IRWXU);
chsize(han, size);
close(han);
return(1);
}


INT createPDF(HWND hwndD)
{
if(WinDlgBox(HWND_DESKTOP,
	    hwndD,
	    createPDFProc,
	    (HMODULE)0,
	    ID_MAKEPDF,
	    NULL))
   return(1);
else
   return(0);
}


MRESULT EXPENTRY createPDFProc( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 )
{
CHAR nFileName[MAXPATH];

switch ( msg )
   {
   case WM_INITDLG:
      WinSendDlgItemMsg(hwndDlg, ID_NEWPDFNAME, EM_SETTEXTLIMIT,
		  MPFROM2SHORT(12,0),NULL);
      WinSetDlgItemText(hwndDlg, ID_NEWPDFNAME, npdf);
      WinSendDlgItemMsg(hwndDlg ,ID_NEWPDFNAME, EM_SETSEL ,
		       MPFROM2SHORT(0,sizeof(npdf)), MPVOID  ) ;
      WinSendDlgItemMsg(hwndDlg, ID_FIXEDLEN,
			SPBM_SETLIMITS,
			MPFROMLONG(30),
			MPFROMLONG(1));
      break;

   case WM_COMMAND :
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_OK:
	    WinQueryDlgItemText(hwndDlg,
				ID_NEWPDFNAME,
				sizeof(npdf),
				npdf);
	    strcpy(nFileName, basePath);
	    if( nFileName[strlen(nFileName)-1] != '\\' )
	       strcat(nFileName, "\\");
	    strcat(nFileName, npdf);
	    if( existFile(nFileName) )
	       {
	       WinMessageBox(HWND_DESKTOP,
			     hwndDlg,
			     "File already exists. Please use another name.",
			     "ATTENTION!",
			     0,
			     MB_ICONEXCLAMATION |
			     MB_OK);
	       return(0);
	       }
	    if( strlen(npdf) < 5 || strlen(npdf) > 12)
	       return(0);
	    if( strnicmp(npdf+(strlen(npdf)-4), EXTENTION, 4) != 0 )
	       {
	       WinMessageBox(HWND_DESKTOP,
				hwndDlg,
				"Module name must have the proper extention.",
				"Improper ( or missing ) file extention!",
				0,
				MB_ICONEXCLAMATION | MB_OK );
	       return(0);
	       }
	    WinSendDlgItemMsg(hwndDlg, ID_FIXEDLEN,
			      SPBM_QUERYVALUE,
			      MPFROMP(&FIXEDLEN),
			      MPFROM2SHORT(0,SPBQ_ALWAYSUPDATE));
	    FIXEDLEN *= 1000;
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


INT setDefMod(HWND hwndD)
{
if(WinDlgBox(HWND_DESKTOP,
	    hwndD,
	    stickyProc,
	    (HMODULE)0,
	    ID_STICKYDEF,
	    NULL))
   return(1);
else
   return(0);
}

MRESULT EXPENTRY stickyProc( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 )
{
CHAR strItem[MAXNAME];

switch ( msg )
   {
   case WM_INITDLG:
      {
      INT i, kn;

      kn = 0;
      WinInsertLboxItem(WinWindowFromID(hwndDlg, ID_DDMODLIST),
			0, ndName);
      for( i=0;i<NUMPDFMODS+1;i++ )
	 {
	 if( strlen(pdf[i].noteText) > 0 &&
	     strcmp(pdf[i].noteText, ndName) != 0 )
	    WinInsertLboxItem(WinWindowFromID(hwndDlg, ID_DDMODLIST),
			      LIT_END, pdf[i].noteText);
	 }
      for( i=0;i<NUMPDFMODS;i++ )
	 {
	 WinQueryLboxItemText(WinWindowFromID(hwndDlg, ID_DDMODLIST),
			      i, strItem, sizeof(strItem));
	 if( strcmp(stickyName, strItem) == 0 )
	    kn = i;
	 }
      WinPostMsg(WinWindowFromID(hwndDlg, ID_DDMODLIST),
		   LM_SELECTITEM,
		   MPFROMSHORT(kn),
		   MPFROMSHORT(TRUE));
      }
      break;

   case WM_COMMAND :
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_OK:
	    WinQueryDlgItemText(hwndDlg,
				ID_DDMODLIST,
				sizeof(stickyName),
				stickyName);
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


VOID setMod(HWND hwndD)
{
WinDlgBox(HWND_DESKTOP,
	    hwndD,
	    setModProc,
	    (HMODULE)0,
	    ID_PDFMODULES,
	    NULL);
}




INT getModList(HWND hwndML, INT mode)
{
if( mode == 1 )
   {
   if(WinDlgBox(HWND_DESKTOP,
	    hwndML,
	    gmlProc1,
	    (HMODULE)0,
	    ID_PDFMODS,
	    NULL))
      return(1);
   else
      return(0);
   }
else
   {
   if(WinDlgBox(HWND_DESKTOP,
	    hwndML,
	    gmlProc2,
	    (HMODULE)0,
	    ID_PDFMODS,
	    NULL))
      return(1);
   else
      return(0);
   }
}




INT dupeName(HWND hwndML)
{
if(WinDlgBox(HWND_DESKTOP,
	    hwndML,
	    dupeNameProc,
	    (HMODULE)0,
	    ID_DUPERECNAME,
	    NULL))
   return(1);
else
   return(0);
}




//*******************************
//*******************************
INT chkDupeName(HWND hwndDlg)
{
INT nb, s;
CHAR tp[NAMESIZE] = "";
CHAR tp2[NAMESIZE+1] = "";
INT dret;


strcpy(tp2, ntName);
strcat(tp2, " 1");
strcat(tp2, "\0");
if( strlen(ntName) < 1 )
   return(-1);
nb = WinQueryLboxCount(WinWindowFromID(hwndDlg, ID_LISTBOX1));
for( s=0;s<nb;s++ )
   {
   WinSendMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
			      LM_QUERYITEMTEXT,
			      MPFROM2SHORT(s, sizeof(tp)),
			      MPFROMP(tp));
   if( strcmpi(tp, ntName) == 0 )
      {
      dret = dupeRecName(hwndDlg);
      /*
      WinMessageBox(HWND_DESKTOP,
		    hwndDlg,
		    "Please use another record name!",
		    "Duplicate Record Name ...",
		    0,
		    MB_ICONEXCLAMATION | MB_OK );
      */
      if( dret == 0 )
	 return(0);
      else
	 return(1);
      }
   if( strcmpi(tp, tp2) == 0 )
      {
      dret = dupeRecName(hwndDlg);
      /*
      WinMessageBox(HWND_DESKTOP,
		    hwndDlg,
		    "Please use another record name!",
		    "Duplicate Record Name ...",
		    0,
		    MB_ICONEXCLAMATION | MB_OK );
      */
      if( dret == 0 )
	 return(0);
      else
	 return(1);
      }
   }
return(2);
}




INT dupeRecName(HWND hwndML)
{
if(WinDlgBox(HWND_DESKTOP,
	    hwndML,
	    dupeRecNameProc,
	    (HMODULE)0,
	    ID_DUPERECNAME,
	    NULL))
   return(1);
else
   return(0);
}





MRESULT EXPENTRY dupeRecNameProc(HWND hwndDlg, ULONG msg,
				 MPARAM mp1, MPARAM mp2 )
{
CHAR strItem[MAXNAME];

switch ( msg )
   {
   case WM_INITDLG:
      break;

   case WM_COMMAND :
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_OK:
	    WinQueryDlgItemText(hwndDlg,
				   ID_NEWCLONENAME,
				   sizeof(ntName), ntName);
	    if( strlen(ntName) < 3 )
	       {
	       if ( WinMessageBox(HWND_DESKTOP,
		       hwndDlg,
		       "You must enter a Record Name at least 3 characters long to add a Record",
		       "Error ...",
		       0,
		       MB_ICONEXCLAMATION | MB_OKCANCEL) == MBID_CANCEL )
		  {
		  WinDismissDlg(hwndDlg, FALSE);
		  return(MRESULT)FALSE;
		  }
	       else
		  return(0);
	       }
	    else
	       {
	       WinDismissDlg(hwndDlg, TRUE);
	       return(MRESULT)TRUE;
	       }
	    break;

	 case DID_CANCEL:
	    strcpy(ntName, "");
	    WinDismissDlg(hwndDlg, FALSE);
	    return(MRESULT)FALSE;
	    break;
	 }
      break;

   default :
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
   }
return (MRESULT) FALSE;
}
//*******************************
//*******************************




MRESULT EXPENTRY dupeNameProc( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 )
{
CHAR strItem[MAXNAME];

switch ( msg )
   {
   case WM_INITDLG:
      break;

   case WM_COMMAND :
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_OK:
	    WinQueryDlgItemText(hwndDlg,
				   ID_NEWCLONENAME,
				   sizeof(dataRecs.noteName),
				   dataRecs.noteName);
	    if( strlen(dataRecs.noteName) < 3 )
	       {
	       if ( WinMessageBox(HWND_DESKTOP,
		       hwndDlg,
		       "You must enter a Record Name at least 3 characters long to add a Record",
		       "Error ...",
		       0,
		       MB_ICONEXCLAMATION | MB_OKCANCEL) == MBID_CANCEL )
		  WinDismissDlg(hwndDlg, FALSE);
	       else
		  return(0);
	       }
	    else
	       WinDismissDlg(hwndDlg, TRUE);
	    break;

	 case DID_CANCEL:
	    strcpy(dataRecs.noteName, "");
	    WinDismissDlg(hwndDlg, FALSE);
	    break;
	 }
      break;

   default :
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
   }
return (MRESULT) FALSE;
}


MRESULT EXPENTRY gmlProc1( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 )
{
CHAR strItem[MAXNAME];
SHORT h, k, rsel;
FILE *mHan;
static CHAR tempS[MAXNAME];

switch ( msg )
   {
   case WM_INITDLG:
      WinSetDlgItemText(hwndDlg, FID_TITLEBAR, "Select target for copy operation");
      strcpy(tempS, basePath);
      if( tempS[strlen(tempS)-1] == '\\' )
	 strcat(tempS, "DATAPAD.DAT");
      else
	 strcat(tempS, "\\DATAPAD.DAT");
      if ((mHan = fopen( tempS, MRO)) != NULL)
	 {
	 fseek(mHan, 0L, SEEK_SET);
	 fread(&pdf, sizeof(pdf), 1, mHan);
	 fclose(mHan);
	 }
      for( k=0,h=0;h<NUMPDFMODS;h++ )
	 {
	 if( strlen(pdf[h].noteText) > 0 )
	    {
	    WinInsertLboxItem(WinWindowFromID(hwndDlg, ID_MODLIST), k, pdf[h].noteText);
	    k++;
	    }
	 }
      WinPostMsg(WinWindowFromID(hwndDlg, ID_MODLIST),
		   LM_SELECTITEM,
		   MPFROMSHORT(0),
		   MPFROMSHORT(TRUE));
      break;

   case WM_CONTROL :
      if (SHORT2FROMMP(mp1) == LN_ENTER )
	 {
	 switch (LOUSHORT(mp1))
	    {
	    case ID_MODLIST :
	       WinQueryLboxItemText(WinWindowFromID(hwndDlg, ID_MODLIST),
				    rsel, strItem, sizeof(strItem));
	       WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(DID_OK), 0);
	       break;
	    }
	 }
      break;

   case WM_COMMAND :
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_OK:
	    rsel = (USHORT) WinSendMsg(WinWindowFromID(hwndDlg, ID_MODLIST), LM_QUERYSELECTION, 0L, 0L);
	    WinQueryLboxItemText(WinWindowFromID(hwndDlg, ID_MODLIST),
				 rsel, strItem, sizeof(strItem));
	    strcpy(mcString, strItem);
	    WinDismissDlg(hwndDlg, TRUE);
	    break;

	 case DID_CANCEL:
	    strcpy(mcString, "");
	    WinDismissDlg(hwndDlg, FALSE);
	    break;
	 }
      break;

   default :
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
   }
return (MRESULT) FALSE;
}


MRESULT EXPENTRY gmlProc2( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 )
{
CHAR strItem[MAXNAME];
SHORT h, k, rsel;
FILE *mHan;
static CHAR tempS[MAXNAME];

switch ( msg )
   {
   case WM_INITDLG:
      WinSetDlgItemText(hwndDlg, FID_TITLEBAR, "Select target for Move operation");
      strcpy(tempS, basePath);
      if( tempS[strlen(tempS)-1] == '\\' )
	 strcat(tempS, "DATAPAD.DAT");
      else
	 strcat(tempS, "\\DATAPAD.DAT");
      if ((mHan = fopen( tempS, MRO)) != NULL)
	 {
	 fseek(mHan, 0L, SEEK_SET);
	 fread(&pdf, sizeof(pdf), 1, mHan);
	 fclose(mHan);
	 }
      for( k=0,h=0;h<NUMPDFMODS;h++ )
	 {
	 if( strlen(pdf[h].noteText) > 0 )
	    {
	    WinInsertLboxItem(WinWindowFromID(hwndDlg, ID_MODLIST), k, pdf[h].noteText);
	    k++;
	    }
	 }
      WinPostMsg(WinWindowFromID(hwndDlg, ID_MODLIST),
		   LM_SELECTITEM,
		   MPFROMSHORT(0),
		   MPFROMSHORT(TRUE));
      break;

   case WM_CONTROL :
      if (SHORT2FROMMP(mp1) == LN_ENTER )
	 {
	 switch (LOUSHORT(mp1))
	    {
	    case ID_MODLIST :
	       WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(DID_OK), 0);
	       break;
	    }
	 }
      break;

   case WM_COMMAND :
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_OK:
	    rsel = (USHORT) WinSendMsg(WinWindowFromID(hwndDlg, ID_MODLIST), LM_QUERYSELECTION, 0L, 0L);
	    WinQueryLboxItemText(WinWindowFromID(hwndDlg, ID_MODLIST),
				 rsel, strItem, sizeof(strItem));
	    strcpy(mcString, strItem);
	    WinDismissDlg(hwndDlg, TRUE);
	    break;

	 case DID_CANCEL:
	    strcpy(mcString, "");
	    WinDismissDlg(hwndDlg, FALSE);
	    break;
	 }
      break;

   default :
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
   }
return (MRESULT) FALSE;
}


MRESULT EXPENTRY setModProc( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 )
{
FILE *mHan;
SHORT h, k, mcnt, rsel;
CHAR delMsg[] = "Are you sure you want to remove this module from the list?";
static CHAR tempS[MAXNAME];
PFNWP oldMWinProc;
BOOL isDat;

switch ( msg )
   {
   case WM_INITDLG:
      strcpy(tempS, basePath);
      if( tempS[strlen(tempS)-1] == '\\' )
	 strcat(tempS, "DATAPAD.DAT");
      else
	 strcat(tempS, "\\DATAPAD.DAT");
      if ((mHan = fopen( tempS, MRO)) != NULL)
	 {
	 fseek(mHan, 0L, SEEK_SET);
	 fread(&pdf, sizeof(pdf), 1, mHan);
	 fclose(mHan);
	 isDat = TRUE;
	 }
      else
	 isDat = FALSE;
      for( k=0,h=0;h<NUMPDFMODS;h++ )
	 {
	 if( strlen(pdf[h].noteText) > 0 )
	    {
	    WinInsertLboxItem(WinWindowFromID(hwndDlg, ID_PDFLB), k, pdf[h].noteText);
	    k++;
	    }
	 }
      oldMWinProc = WinSubclassWindow(WinWindowFromID(hwndDlg, ID_PDFLB),
				      newModProc);
      WinSetWindowPtr(WinWindowFromID(hwndDlg, ID_PDFLB),
		      0, (PVOID) oldMWinProc);
      WinPostMsg(WinWindowFromID(hwndDlg, ID_PDFLB),
		   LM_SELECTITEM,
		   MPFROMSHORT(0),
		   MPFROMSHORT(TRUE));
      break;

   case WM_CONTROL :
      if (SHORT2FROMMP(mp1) == LN_ENTER )
	 {
	 rsel = (USHORT) WinSendMsg(WinWindowFromID(hwndDlg, ID_PDFLB), LM_QUERYSELECTION, 0L, 0L);
	 switch (LOUSHORT(mp1))
	    {
	 case ID_PDFLB :
	       if( rsel > 0 )
		  {
		  CHAR strItem[MAXNAME];

		  WinQueryLboxItemText(WinWindowFromID(hwndDlg, ID_PDFLB),
					   rsel, strItem, sizeof(strItem));
		  if( chkIntegrityPDF(hwndDlg, strItem) == -1 )
		     break;
		  WinInsertLboxItem(WinWindowFromID(hwndDlg, ID_PDFLB), 0, strItem);
		  WinDeleteLboxItem(WinWindowFromID(hwndDlg, ID_PDFLB), rsel+1);
		  WinPostMsg(WinWindowFromID(hwndDlg, ID_PDFLB),
				LM_SELECTITEM,
				MPFROMSHORT(0),
				MPFROMSHORT(TRUE));
		  }
	       break;
	    }
	 }
      break;

   case WM_COMMAND :
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case ID_OKPDF :
	    mcnt = WinQueryLboxCount(WinWindowFromID(hwndDlg, ID_PDFLB));
	    if( mcnt < 1 )
	       {
	       WinMessageBox(HWND_DESKTOP,
				hwndDlg,
				"You must have at least ONE search module.",
				"Attention!",
				0,
				MB_ICONEXCLAMATION | MB_OK );
	       return(0);
	       }
	    for( h=0;h<NUMPDFMODS;h++ )
	       {
	       if( h < mcnt )
		  WinSendMsg(WinWindowFromID(hwndDlg, ID_PDFLB),
				LM_QUERYITEMTEXT,
				MPFROM2SHORT(h, sizeof(pdf[h].noteText)),
				MPFROMP(pdf[h].noteText));
	       else
		  pdf[h].noteText[0] = '\0';
	       }
	    if ( _dos_findfirst(pdf[0].noteText,_A_NORMAL,&ffblk) != 0 )
	       {
	       FILE *handle;

	       handle = fopen( pdf[0].noteText, "w+b" );
	       fclose(handle);
	       }
	    if ((mHan = fopen(tempS, MRW)) == NULL)
	       {
	       if ((mHan = fopen(tempS, "w+b" )) == NULL)
		  return(0);
	       }
	    fseek(mHan, 0L, SEEK_SET);
	    fwrite(&pdf, sizeof(pdf), 1, mHan);
	    fclose(mHan);
	    strcpy(npdf, pdf[0].noteText);
	    strcpy(datFile, pdf[0].noteText);
	    if( isDat )
	       {
	       initLoad(hwnd1, pdf[0].noteText);
	       WinPostMsg(hwnd1,
			  LM_SELECTITEM,
			  MPFROMSHORT(0),
			  MPFROMSHORT(TRUE));
	       WinPostMsg(WinQueryWindow(hwndDlg, QW_OWNER),
			  WM_COMMAND,
			  MPFROMSHORT(ID_GETRECCNT), 0);
	       }
	    WinDismissDlg(hwndDlg, TRUE);
	    break;

	 case ID_CANCELPDF :
	    strcpy(npdf, "");
	    WinDismissDlg(hwndDlg, TRUE);
	    break;

	 case ID_ADDPDF :
	    mcnt = WinQueryLboxCount(WinWindowFromID(hwndDlg, ID_PDFLB));
	    if( mcnt >= NUMPDFMODS )
	       {
	       WinMessageBox(HWND_DESKTOP,
			     hwndDlg,
			     "Module Search List Is Full!",
			     "Maximum number of modules in list ...",
			     0,
			     MB_ICONEXCLAMATION | MB_OK );
	       return(0);
	       }
	    FindDatFile(hwndDlg);
	    if( stricmp(fullFileName, "") == 0 )
	       return(0);
	    for( h=0;h<NUMPDFMODS;h++ )
	       {
	       CHAR temp[MAXNAME];

	       WinQueryLboxItemText(WinWindowFromID(hwndDlg, ID_PDFLB),
					h, temp, sizeof(temp));
	       if( stricmp(fullFileName, temp) == 0 )
		  {
		  WinMessageBox(HWND_DESKTOP,
				   hwndDlg,
				   "Module name already exists!",
				   "Duplicate data file Name ...",
				   0,
				   MB_ICONEXCLAMATION | MB_OK );
		  return(0);
		  }
	       }
	    if( chkIntegrityPDF(hwndDlg, fullFileName) == -1 )
	       break;
	    WinInsertLboxItem(WinWindowFromID(hwndDlg, ID_PDFLB), LIT_END, fullFileName);
	    mcnt = WinQueryLboxCount(WinWindowFromID(hwndDlg, ID_PDFLB));
	    WinPostMsg(WinWindowFromID(hwndDlg, ID_PDFLB),
			  LM_SELECTITEM,
			  MPFROMSHORT(mcnt-1),
			  MPFROMSHORT(TRUE));
	    break;

	 case ID_ADDPDFDROP :
	    mcnt = WinQueryLboxCount(WinWindowFromID(hwndDlg, ID_PDFLB));
	    if( mcnt >= NUMPDFMODS )
	       {
	       WinMessageBox(HWND_DESKTOP,
			     hwndDlg,
			     "Module Search List Is Full!",
			     "Maximum number of modules in list ...",
			     0,
			     MB_ICONEXCLAMATION | MB_OK );
	       return(0);
	       }
	    if( stricmp(fullFileName, "") == 0 )
	       return(0);
	    for( h=0;h<NUMPDFMODS;h++ )
	       {
	       CHAR temp[MAXNAME];

	       WinQueryLboxItemText(WinWindowFromID(hwndDlg, ID_PDFLB),
					h, temp, sizeof(temp));
	       if( stricmp(fullFileName, temp) == 0 )
		  {
		  WinMessageBox(HWND_DESKTOP,
				   hwndDlg,
				   "Module name already exists!",
				   "Duplicate data file Name ...",
				   0,
				   MB_ICONEXCLAMATION | MB_OK );
		  return(0);
		  }
	       }
	    if( chkIntegrityPDF(hwndDlg, fullFileName) == -1 )
	       break;
	    WinInsertLboxItem(WinWindowFromID(hwndDlg, ID_PDFLB), LIT_END, fullFileName);
	    mcnt = WinQueryLboxCount(WinWindowFromID(hwndDlg, ID_PDFLB));
	    WinPostMsg(WinWindowFromID(hwndDlg, ID_PDFLB),
			  LM_SELECTITEM,
			  MPFROMSHORT(mcnt-1),
			  MPFROMSHORT(TRUE));
	    break;

	 case ID_REMOVEPDF :
	    mcnt = WinQueryLboxCount(WinWindowFromID(hwndDlg, ID_PDFLB));
	    if( mcnt == 1 )
	       {
	       WinMessageBox(HWND_DESKTOP,
				hwndDlg,
				"You must have at least ONE search module.",
				"Attention!",
				0,
				MB_ICONEXCLAMATION | MB_OK );
	       return(0);
	       }
	    if ( WinMessageBox(HWND_DESKTOP,
				  hwndDlg,
				  delMsg,
				  "Are You Sure ...",
				  0,
				  MB_ICONQUESTION | MB_DEFBUTTON2 | MB_YESNO) == MBID_NO )
	       return(0);
	    rsel = (USHORT) WinSendMsg(WinWindowFromID(hwndDlg, ID_PDFLB),
					      LM_QUERYSELECTION, 0L, 0L);
	    WinSendMsg (WinWindowFromID(hwndDlg, ID_PDFLB),LM_DELETEITEM,MPFROMSHORT(rsel),0);
	    if( rsel == mcnt-1 )
	       rsel = mcnt-2;
	    WinPostMsg(WinWindowFromID(hwndDlg, ID_PDFLB),
			  LM_SELECTITEM,
			  MPFROMSHORT(rsel),
			  MPFROMSHORT(TRUE));
	    break;

	 case ID_DEFMOD :
	    {
	    CHAR tempT[MAXNAME];

	    mcnt = WinQueryLboxCount(WinWindowFromID(hwndDlg, ID_PDFLB));
	    if( mcnt >=NUMPDFMODS )
	       {
	       WinMessageBox(HWND_DESKTOP,
			     hwndDlg,
			     "Module Search List Is Full!",
			     "Maximum number of modules in list ...",
			     0,
			     MB_ICONEXCLAMATION | MB_OK );
	       return(0);
	       }
	    if( WinDlgBox(HWND_DESKTOP,
			  hwndDlg,
			  newPdfProc,
			  (HMODULE)0,
			  ID_MAKEPDF,
			  NULL) == FALSE )
	       return(0);
	    if( stricmp(fullFileName, "") == 0 )
	       return(0);
	    if( parseNewName(fullFileName) == -1 )
	       {
	       WinMessageBox(HWND_DESKTOP,
				hwndDlg,
				"Module name should include only letters and numbers with a valid default extention.",
				"Improper file name!",
				0,
				MB_ICONEXCLAMATION | MB_OK );
	       return(0);
	       }
	    for( h=0;h<NUMPDFMODS;h++ )
	       {
	       WinQueryLboxItemText(WinWindowFromID(hwndDlg, ID_PDFLB),
					h, tempT, sizeof(tempT));
	       if( stricmp(fullFileName, tempT) == 0 )
		  {
		  WinMessageBox(HWND_DESKTOP,
				   hwndDlg,
				   "Module already on the list! If you want this to be the default then click twice on it's name in the list box.",
				   "Duplicate data file Name ...",
				   0,
				   MB_ICONEXCLAMATION | MB_OK );
		  return(0);
		  }
	       }
	    strcpy(npdf, fullFileName);
	    WinQueryLboxItemText(WinWindowFromID(hwndDlg, ID_PDFLB),
				     0, tempT, sizeof(tempT));
	    WinInsertLboxItem(WinWindowFromID(hwndDlg, ID_PDFLB), LIT_END, tempT);
	    WinSendMsg (WinWindowFromID(hwndDlg, ID_PDFLB),LM_DELETEITEM,MPFROMSHORT(0),0);
	    WinInsertLboxItem(WinWindowFromID(hwndDlg, ID_PDFLB), 0, fullFileName);
	    WinPostMsg(WinWindowFromID(hwndDlg, ID_PDFLB),
			  LM_SELECTITEM,
			  MPFROMSHORT(0),
			  MPFROMSHORT(TRUE));
	    newPDFLoad(WinWindowFromID(hwndMainDlg, ID_LISTBOX1), fullFileName, newPDFLen);
	    WinPostMsg(WinWindowFromID(hwndMainDlg,ID_LISTBOX1),
		       LM_SELECTITEM,
		       MPFROMSHORT(0),
		       MPFROMSHORT(TRUE));
	    }
	    break;

	 default :
	   return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
	 }
      break;


   default :
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
   }

// return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
return (MRESULT) FALSE;
}



MRESULT EXPENTRY newModProc( HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2 )
{
PFNWP oldMWinProc;
HWND hwndFF;
ULONG attrFound;
CHAR szCont[MAXNAME] = "";
CHAR szSource[MAXNAME] = "";

oldMWinProc = (PFNWP) WinQueryWindowPtr(hwndWnd, 0);
switch(ulMsg)
   {
   case DM_DRAGOVER :
      {
      PDRAGINFO pdrginfo;
      PDRAGITEM pdrgitem;

      strcpy(szCont, "");
      strcpy(szSource, "");
      pdrginfo = (PDRAGINFO) PVOIDFROMMP(mp1);
      if( !DrgAccessDraginfo(pdrginfo) )
	 {
	 DrgFreeDraginfo(pdrginfo) ;
	 break;
	 }

      pdrgitem = DrgQueryDragitemPtr(pdrginfo, 0L);
      DrgQueryStrName(pdrgitem->hstrSourceName,
		      sizeof(szSource), szSource);

      if( strnicmp(szSource+(strlen(szSource)-4), EXTENTION, 4) != 0 )
	 {
	 DrgFreeDraginfo(pdrginfo) ;
	 break;
	 }

      if( DrgVerifyTrueType(pdrgitem, DRT_EXE) )
	 {
	 DrgFreeDraginfo(pdrginfo) ;
	 break;
	 }
      if( !DrgVerifyRMF(pdrgitem, (PSZ)"DRM_OS2FILE", NULL) )
	 {
	 DrgFreeDraginfo(pdrginfo) ;
	 break;
	 }
      if( (DrgQueryDragitemCount(pdrginfo)) != 1 )
	 {
	 DrgFreeDraginfo(pdrginfo) ;
	 break;
	 }
      DrgFreeDraginfo(pdrginfo);
      return MPFROM2SHORT(DOR_DROP, DO_COPY);
      }

   case DM_DROP :
      {
      PDRAGINFO pdrginfo = (PDRAGINFO)mp1;
      PDRAGITEM pdrgitem;

      if( !DrgAccessDraginfo(pdrginfo) )
	 {
	 DrgFreeDraginfo(pdrginfo) ;
	 break;
	 }
      pdrgitem = DrgQueryDragitemPtr(pdrginfo, 0L);

      DrgQueryStrName(pdrgitem->hstrContainerName,
		      sizeof(szCont), szCont);
      DrgQueryStrName(pdrgitem->hstrSourceName,
		      sizeof(szSource), szSource);
      strcat(szCont, szSource);
      DrgFreeDraginfo(pdrginfo);
      }
      strcpy(fullFileName, szCont);
      WinPostMsg(WinQueryWindow(hwndWnd, QW_PARENT), WM_COMMAND, MPFROMP(ID_ADDPDFDROP), 0L);
      break;

   default :
      break;
   }
return( *oldMWinProc ) (hwndWnd, ulMsg, mp1, mp2);
}


MRESULT EXPENTRY newPdfProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
switch ( msg )
   {
   case WM_INITDLG:
	 WinSendDlgItemMsg(hwndDlg, ID_NEWPDFNAME, EM_SETTEXTLIMIT,
			   MPFROM2SHORT(40,0),NULL);
	 WinSendDlgItemMsg(hwndDlg, ID_FIXEDLEN,
			   SPBM_SETLIMITS,
			   MPFROMLONG(30),
			   MPFROMLONG(1));
	 break;

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_OK :
	    WinQueryDlgItemText(hwndDlg,
				   ID_NEWPDFNAME,
				   sizeof(fullFileName),
				   fullFileName);
	    WinSendDlgItemMsg(hwndDlg, ID_FIXEDLEN,
			      SPBM_QUERYVALUE,
			      MPFROMP(&newPDFLen),
			      MPFROM2SHORT(0,SPBQ_ALWAYSUPDATE));
	    newPDFLen *= 1000;
	    WinDismissDlg( hwndDlg, TRUE );
	    break;

	 case DID_CANCEL :
	    WinDismissDlg( hwndDlg, FALSE );
	    break;

	 default :
	    return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
	 }
      break;
   default :
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
   }
return (MRESULT) FALSE;
}


VOID getPath(VOID)
{
CHAR cwd[MAXNAME];

getcwd(cwd, MAXNAME);
strcpy(pdf[0].noteText, cwd);
if( pdf[0].noteText[strlen(pdf[0].noteText)-1] != '\\' )
   strcat(pdf[0].noteText, "\\");
strcat(pdf[0].noteText, npdf);
strcat(pdf[0].noteText, "\0");
}


INT parseNewName(CHAR *str)
{
CHAR cwd[MAXNAME];

if( strnicmp(str+(strlen(str)-4), EXTENTION, 4) != 0 )
   {
   CHAR tstr[] = ".";


   if( strstr(str, tstr) )
      return(-1);
   strcat(str, EXTENTION);
   }
strupr(str);
if( str[1] != ':' )
   {
   CHAR temp[MAXNAME];

   getcwd(cwd, MAXNAME);
   strcpy(temp, cwd);
   if( temp[strlen(temp)-1] != '\\' )
      strcat(temp, "\\");
   strcat(temp, "\0");
   strcat(temp, str);
   strcat(temp, "\0");
   strcpy(str, temp);
   }
return(0);
}

INT loadIndex(HWND hwndI, CHAR *dataFileName)
{
INT i = 0;
div_t x;

for( i=0;i<MAXNUMRECS;i++ )
   recIndex[i].recName[0] = '\0';
WinSetPointer(HWND_DESKTOP,
	      WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE));
if ((datHan2 = fopen( dataFileName, "r+b" )) == NULL)
   {
   DosBeep(1000, 1500);
   WinMessageBox(HWND_DESKTOP,
		 hwndI,
		 "data file could NOT be opened!",
		 "Error ...",
		 0,
		 MB_OK | MB_ICONEXCLAMATION );
   return(0);
   }
fread(&recIndex, sizeof(recIndex), 1, datHan2);
fclose(datHan2);
if( recIndex[0].alIndex > 0 )
   FIXEDLEN = recIndex[0].alIndex;
else
   {
   recIndex[0].alIndex = NOTESIZE;
   FIXEDLEN = recIndex[0].alIndex;
   }
WinSetPointer(HWND_DESKTOP,
	      WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, FALSE));
return(1);
}


INT initLoad_index(HWND hwndList, CHAR *dataFileName)
{
INT i = 0;
div_t x;

for( i=0;i<MAXNUMRECS;i++ )
   recIndex[i].recName[0] = '\0';
WinSetPointer(HWND_DESKTOP,
	      WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE));
WinSendMsg(hwndList, LM_DELETEALL, 0L, 0L);
WinEnableWindowUpdate(hwndList, FALSE);
if ((datHan2 = fopen( dataFileName, "r+b" )) == NULL)
   {
   if( (datHan2 = fopen( dataFileName, "w+b" )) == NULL )
      {
      DosBeep(1000, 1500);
      exit(-1);
      }
   fread(&recIndex, sizeof(recIndex), 1, datHan2);
   recIndex[0].alIndex = FIXEDLEN;
   }
else
   fread(&recIndex, sizeof(recIndex), 1, datHan2);
fclose(datHan2);
i = 0;
while( strcmp(recIndex[i].recName, "") != 0 && i < MAXNUMRECS )
   {
   recIndex[i].posIndex = i;
   WinSendMsg(hwndList, LM_INSERTITEM,
	      MPFROMSHORT(LIT_SORTASCENDING), MPFROMP(recIndex[i].recName));
   numRecs = ++i;
   }
if( recIndex[0].alIndex > 0 )
   FIXEDLEN = recIndex[0].alIndex;
else
   {
   recIndex[0].alIndex = NOTESIZE;
   FIXEDLEN = recIndex[0].alIndex;
   }
WinEnableWindowUpdate(hwndList, TRUE);
WinSetPointer(HWND_DESKTOP,
	      WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, FALSE));
return(1);
}


VOID zeroRecs(VOID)
{
dataRecs.indx = -1;
dataRecs.noteName[0] = '\0';
dataRecs.noteText[0] = '\0';
}


VOID zeroMostIndex(INT index)
{
recIndex[index].posIndex = -1;
recIndex[index].recName[0] = '\0';
}

VOID zeroIndex(INT index)
{
recIndex[index].alIndex = -1;
recIndex[index].posIndex = -1;
recIndex[index].recName[0] = '\0';
}

VOID zeroDataRecs(CHAR *noteText)
{
ULONG sx;

for( sx=0;sx<FIXEDLEN;sx++ )
   noteText[sx] = '\0';
}

VOID zeroAllIndex(VOID)
{
INT i;

for ( i=0; i<MAXNUMRECS; i++ )
   {
   recIndex[i].alIndex = -1;
   recIndex[i].posIndex = -1;
   recIndex[i].recName[0] = '\0';
   }
}


INT deleteRec(HWND hwndMain)
{
CHAR delMsg[NAMESIZE + 36];

strcpy(delMsg, "Are you sure you want to delete \'");
strcat(delMsg, recIndex[nCho].recName);
strcat(delMsg, "\' ?");
if ( WinMessageBox(HWND_DESKTOP,
              hwndMain,
              delMsg,
	      "Are You Sure ...",
              0,
              MB_ICONQUESTION | MB_DEFBUTTON2 | MB_YESNO) == MBID_NO )
   return(-1);
return(TRUE);
}


INT openFile(CHAR *fileName, CHAR *mode)
{
if ((datHan = fopen( fileName, mode)) == NULL)
   {
   if ((datHan = fopen( fileName, "w+b" )) == NULL)
      return(-1);
   }
else
   return(1);
return(0);
}


VOID closeFile(VOID)
{
fclose(datHan);
}


VOID saveIndex(CHAR dataFileName[MAXNAME])
{
FILE *dh2;

dh2 = fopen(dataFileName, "r+b" );
fwrite(&recIndex, sizeof(recIndex), 1, dh2);
fclose(dh2);
}

/*
VOID addFWHeader(HFILE hfile, CHAR *dRec)
{
APIRET apiret;
ULONG ulAction, ulBufferSize, ulBytesWritten, newP;
CHAR tempRecHeader[NAMESIZE+18] = "RECORD NAME : ";
CHAR tempResetStr[] = "\r";
CHAR tempSpaceStr[] = "\n\n";

strcat(tempRecHeader, dRec);
if( strlen(dRec) > 0 )
   {
   DosWrite(hfile,
	    &tempRecHeader,
	    strlen(tempRecHeader)*sizeof(CHAR),
	    &ulBytesWritten);
   DosWrite(hfile,
	    &tempSpaceStr,
	    strlen(tempSpaceStr)*sizeof(CHAR),
	    &ulBytesWritten);
   DosWrite(hfile,
	    &tempResetStr,
	    strlen(tempResetStr)*sizeof(CHAR),
	    &ulBytesWritten);
   }
}
*/

VOID addFWHeader(FILE *hand, CHAR *dRec)
{
CHAR tempRecHeader[NAMESIZE+18] = "RECORD NAME : ";
CHAR tempResetStr[] = "\r";
CHAR tempSpaceStr[] = "\n\n";

strcat(tempRecHeader, dRec);
if( strlen(dRec) > 0 )
   {
   fwrite(&tempRecHeader, strlen(tempRecHeader)*sizeof(CHAR), 1, hand);
   fwrite(&tempSpaceStr, strlen(tempSpaceStr)*sizeof(CHAR), 1, hand);
   fwrite(&tempResetStr, strlen(tempResetStr)*sizeof(CHAR), 1, hand);
   }
}



BOOL FindDatFile (HWND hwndWnd) /* initiate find common file dialog */
{
HWND tempDlg;
CHAR parsedFile[MAXNAME];
SHORT k, i;

strcpy(fullFileName, "");
memset ( &fdFileDlg, 0, sizeof ( FILEDLG )) ;
fdFileDlg.cbSize = sizeof ( FILEDLG ) ;
fdFileDlg.fl = FDS_CUSTOM | FDS_OPEN_DIALOG | FDS_CENTER;
fdFileDlg.pfnDlgProc = (PFNWP)findProc;
fdFileDlg.pszOKButton = NULL;
fdFileDlg.usDlgId = ID_FOPEN;
fdFileDlg.hMod = 0;
fdFileDlg.x = 0;
fdFileDlg.y = 0;

strcpy(fMask, "*");
strcat(fMask, EXTENTION);
if( existPath(initFSpec) )
   {
   strcpy(parsedFile, "");
   for( k=0,i=0;i<strlen(initFSpec);i++ )
      {
      if( initFSpec[i] == '\\' )
	 k = i;
      }
   if( k != 0 )
      {
      strncpy(parsedFile, initFSpec, k);
      parsedFile[k] = '\0';
      }
   else
      strcpy(parsedFile, initFSpec);
   strcat(fdFileDlg.szFullFile, parsedFile);
   strcat(fdFileDlg.szFullFile, "\\");
   strcat(fdFileDlg.szFullFile, fMask);
   strcat(fdFileDlg.szFullFile, "\0");
   }
else
   {
   strcpy(fdFileDlg.szFullFile, fMask);
   }
tempDlg = WinFileDlg( HWND_DESKTOP, hwndWnd, (PFILEDLG)&fdFileDlg );

if ( !tempDlg || (fdFileDlg.lReturn != DID_OK) )
   return FALSE;

strcpy(fullFileName, fdFileDlg.szFullFile);
return TRUE ;
}


BOOL FindFile (HWND hwndWnd) /* initiate find common file dialog */
{
HWND tempDlg;

strcpy(fullFileName, "");
memset ( &fdFileDlg, 0, sizeof ( FILEDLG )) ;
fdFileDlg.cbSize = sizeof ( FILEDLG ) ;
fdFileDlg.fl = FDS_CUSTOM | FDS_OPEN_DIALOG | FDS_CENTER;
fdFileDlg.pfnDlgProc = (PFNWP)findProc;
fdFileDlg.pszOKButton = NULL;
fdFileDlg.usDlgId = ID_FOPEN;
fdFileDlg.hMod = 0;
fdFileDlg.x = 0;
fdFileDlg.y = 0;

// DosBeep(900, 100);

strcpy(fMask, "*.*");
if( existPath(initFSpec) )
   strcpy(fdFileDlg.szFullFile, initFSpec);
else
   strcpy(fdFileDlg.szFullFile, fMask);


/* msgOK(fMask, "fMask");
msgOK(fdFileDlg.szFullFile, "szFullFile"); */

tempDlg = WinFileDlg( HWND_DESKTOP, hwndWnd, (PFILEDLG)&fdFileDlg );

if ( !tempDlg || (fdFileDlg.lReturn != DID_OK) )
   return FALSE;

strcpy(fullFileName, fdFileDlg.szFullFile);
return TRUE ;
}




MRESULT EXPENTRY findProc( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 )
{
switch ( msg )
   {
   case WM_INITDLG:
      // DosBeep(100, 200);
      WinSendDlgItemMsg(hwndDlg, DID_FILENAME_ED,EM_SETTEXTLIMIT,
		  MPFROM2SHORT(NAMESIZE,0),NULL);
      // WinDefFileDlgProc( hwndDlg, msg, mp1, mp2 );
      break;

   case WM_CONTROL :
      if (SHORT2FROMMP(mp1) == LN_SELECT )
	 {
	 switch (LOUSHORT(mp1))
	    {
	    case DID_DIRECTORY_LB :
	       WinSetWindowText(WinWindowFromID(hwndDlg, DID_FILENAME_ED), fMask);
	       // DosSleep(1000);
	       break;

	    case DID_FILES_LB :
	       {
	       CHAR fsize[25] = "";
	       SHORT sSelect;
	       CHAR pchFile[MAXNAME];
	       CHAR f_name[MAXNAME];

	       sSelect = (USHORT) WinSendMsg(WinWindowFromID(hwndDlg, DID_FILES_LB),
									LM_QUERYSELECTION, 0L, 0L);
	       strcpy(pchFile, fdFileDlg.szFullFile);
	       WinSendMsg(WinWindowFromID(hwndDlg, DID_FILES_LB), LM_QUERYITEMTEXT,
			     MPFROM2SHORT(sSelect, sizeof(f_name)),
			     MPFROMP(f_name));
	       strcat(pchFile, f_name);
	       strcat(pchFile, "\0");
	       _dos_findfirst(pchFile,_A_NORMAL,&ffblk);
	       if ( sSelect < 0 )
		  WinSetWindowText(WinWindowFromID(hwndDlg, ID_BYTESOPEN), "");
	       else
		  {
		  ltoa(ffblk.size, fsize, 10);
		  WinSetDlgItemText(hwndDlg, ID_BYTESOPEN, fsize);
		  }
	       }
	       break;
	    }
	 }
      break;

   case WM_COMMAND :
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_CANCEL_PB :
	    strcpy(initFSpec, "");
	    strcpy(initFSpec, fdFileDlg.szFullFile);
	    WinDismissDlg(hwndDlg, FALSE);
	    // return(FALSE);
	    break;

	 case DID_OK_PB :
	    strcpy(initFSpec, "");
	    strcpy(initFSpec, fdFileDlg.szFullFile);
	    WinDismissDlg(hwndDlg, TRUE);
	    // return(TRUE);
	    break;
	 }
	 break;

   default :
      return WinDefFileDlgProc( hwndDlg, msg, mp1, mp2 );
   }

return WinDefFileDlgProc( hwndDlg, msg, mp1, mp2 );
}


INT existPath(CHAR *pathStr)
{
ULONG drvNum, ldMap;
ULONG maxPath;
CHAR curPath[256];
int len;
static CHAR path[256] = "";
APIRET rc;

pathStr = strupr(pathStr);
strcpy(path, pathStr);
len = strlen(path);

maxPath = 256;
DosQueryCurrentDisk(&drvNum, &ldMap);
DosQueryCurrentDir(drvNum, curPath, &maxPath);

rc = DosSetDefaultDisk(path[0] - '@');
if( rc != 0 )
   return(0);

if( path[len - 1] == '\\' && len > 3 )
   path[len - 1] = '\0';

rc = DosSetCurrentDir(path);
if( rc != 0 )
   {
   DosSetDefaultDisk(drvNum);
   return(0);
   }
else
   {
   DosSetDefaultDisk(drvNum);
   DosSetCurrentDir(curPath);
   return(1);
   }
}


INT chkName(HWND hwnd, CHAR *lbSelect)
{
INT i;
HWND hwndL;
CHAR tName[NAMESIZE];

hwndL = WinQueryWindow(hwnd, QW_OWNER);
for( i=0;i<MAXNUMRECS;i++ )
   {
   WinSendMsg(WinWindowFromID(hwndMainDlg, ID_LISTBOX1),
	      LM_QUERYITEMTEXT,
	      MPFROM2SHORT(i, sizeof(tName)),
	      MPFROMP(tName));
   if( strcmpi(lbSelect, tName) == 0 )
      return(i);
   }
return(-1);
}

INT compName(HWND hwndM, INT cho)
{
INT i, nrec;
CHAR sName[NAMESIZE];

nrec = WinQueryLboxCount(WinWindowFromID(hwndM, ID_LISTBOX1));
WinSendMsg(WinWindowFromID(hwndM, ID_LISTBOX1), LM_QUERYITEMTEXT,
	   MPFROM2SHORT(cho, sizeof(sName)),
	   MPFROMP(sName));

for( i=0;i<nrec;i++ )
   {
   if( strcmpi(sName, recIndex[i].recName) == 0 )
      return(i);
   }
return(-1);
}

INT compName2(CHAR *sName, INT nrec)
{
INT i;

for( i=0;i<nrec;i++ )
   {
   if( strcmpi(sName, recIndex[i].recName) == 0 )
      return(i);
   }
return(-1);
}

INT matchName(HWND hwndM, CHAR *lbSelect)
{
INT i;
SHORT cnt;
CHAR sName[NAMESIZE];

cnt = WinQueryLboxCount(WinWindowFromID(hwndM, ID_LISTBOX1));
for( i=0;i<cnt;i++ )
   {
   WinSendMsg(WinWindowFromID(hwndM, ID_LISTBOX1), LM_QUERYITEMTEXT,
	      MPFROM2SHORT(i, sizeof(sName)),
	      MPFROMP(sName));
   if( strcmpi(sName, lbSelect) == 0 )
      return(i);
   }
return(-1);
}



INT matchAnyName(HWND hwndM, CHAR *recname)
{
INT i;
SHORT cnt;
CHAR sName[NAMESIZE];

cnt = WinQueryLboxCount(hwndM);
for( i=0;i<cnt;i++ )
   {
   WinSendMsg(hwndM, LM_QUERYITEMTEXT,
	      MPFROM2SHORT(i, sizeof(sName)),
	      MPFROMP(sName));
   if( strcmpi(sName, recname) == 0 )
      return(i);
   }
return(-1);
}




INT copyData(INT c)
{
strcpy(dataR.noteName, dataRecs.noteName);
strcpy(dataR.noteText, dataRecs.noteText);
dataR.indx = dataRecs.indx;
return(0);
}

INT osName(HWND hwndDlg)
{
if (WinDlgBox(HWND_DESKTOP,
	      hwndDlg,
              osNProc,
              (HMODULE)0,
              ID_OSNAME,
	      NULL) == TRUE)
   return(1);
else
   return(0);
}

MRESULT EXPENTRY osNProc( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 )
{
INT nb, s;
CHAR tp[NAMESIZE] = "";
CHAR tp2[NAMESIZE+1] = "";

switch ( msg )
   {
   case WM_INITDLG:
      WinSendDlgItemMsg(hwndDlg, ID_ENTRY11,EM_SETTEXTLIMIT,
                  MPFROM2SHORT(NAMESIZE,0),NULL);
      WinSetDlgItemText(hwndDlg, ID_ENTRY11, ntName);
      WinSendDlgItemMsg ( hwndDlg ,ID_ENTRY11, EM_SETSEL ,
                       MPFROM2SHORT(0,sizeof(ntName)), MPVOID  ) ;
      WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_COMMAND :
      switch( SHORT1FROMMP( mp1 ) )
         {
         case ID_OK11 :
	    WinQueryDlgItemText(hwndDlg,
				ID_ENTRY11,
				sizeof(ntName),
				ntName);
            strcpy(tp2, ntName);
	    strcat(tp2, " 1");
	    strcat(tp2, "\0");
            if( strlen(ntName) < 1 )
	       break;
	    nb = WinQueryLboxCount(WinWindowFromID(hwndMainDlg,ID_LISTBOX1));
	    for( s=0;s<nb;s++ )
	       {
	       WinSendMsg(WinWindowFromID(hwndMainDlg,ID_LISTBOX1),
			  LM_QUERYITEMTEXT,
			  MPFROM2SHORT(s, sizeof(tp)),
			  MPFROMP(tp));
	       if( strcmpi(tp, ntName) == 0 )
		  {
		  WinMessageBox(HWND_DESKTOP,
				hwndDlg,
                                "Please use another record name!",
				"Duplicate Record Name ...",
				0,
				MB_ICONEXCLAMATION | MB_OK );
                  return(0);
                  }
	       if( strcmpi(tp, tp2) == 0 )
		  {
                  WinMessageBox(HWND_DESKTOP,
                                hwndDlg,
				"Please use another record name!",
				"Duplicate Record Name ...",
                                0,
				MB_ICONEXCLAMATION | MB_OK );
		  return(0);
                  }
	       }
            WinDismissDlg(hwndDlg, TRUE);
            return (MRESULT)TRUE;

	 case ID_CANCEL11 :
	    WinDismissDlg(hwndDlg, FALSE);
	    return (MRESULT)FALSE;
         }

   default :
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
   }
}

VOID dBaseFull(HWND hwndDB)
{
WinMessageBox(HWND_DESKTOP,
	      hwndDB,
	      "Please start another data file!",
              "Data File Is Full...",
	      0,
	      MB_ICONEXCLAMATION | MB_OK );
}




VOID parseName(CHAR *name)
{
INT i, k;

strcpy(ntName, "");
for( i=0;i<strlen(name);i++ )
   {
   if( name[i] == '\\' )
      k = i;
   }
strncpy(ntName, name+(k+1), strlen(name) - (k+1));
ntName[strlen(name) - (k+1)] = '\0';
}


VOID setParams(HWND hwndP, SHORT dx, ULONG IDE, ULONG IDR)
{
WinSetPresParam ( WinWindowFromID ( hwndP, IDE ),
		     PP_FONTNAMESIZE,
		     sizeof(pValue[dx])+1,
		     &pValue[dx] ) ;
WinSetPresParam ( WinWindowFromID ( hwndP, IDR ),
		     PP_FONTNAMESIZE,
		     sizeof(pValue[dx])+1,
		     &pValue[dx] ) ;
/*
if( (dx == 0 && !pgmData[4]) ||
   (dx == 3 && !pgmData[5]) ||
   (dx == 6 && !pgmData[6]) ||
   (dx == 9 && !pgmData[7]) ||
   (dx == 12 && !pgmData[32]) )
   {  */
   WinSetPresParam ( WinWindowFromID ( hwndP, IDE ),
		     PP_FOREGROUNDCOLOR,
		     sizeof(pValue[dx+1]),
		     &pValue[dx+1] ) ;
   WinSetPresParam ( WinWindowFromID ( hwndP, IDR ),
		     PP_FOREGROUNDCOLOR,
		     sizeof(pValue[dx+1]),
		     &pValue[dx+1] ) ;

   WinSetPresParam ( WinWindowFromID ( hwndP, IDE ),
		     PP_BACKGROUNDCOLOR,
		     sizeof(pValue[dx+2]),
		     &pValue[dx+2] ) ;
   WinSetPresParam ( WinWindowFromID ( hwndP, IDR ),
		     PP_BACKGROUNDCOLOR,
		     sizeof(pValue[dx+2]),
		     &pValue[dx+2] ) ;
  // }
}


INT saveRecFile(CHAR *str, SHORT index, CHAR *mode)
{
FILE *msHan;
ULONG pointerPos;

pointerPos = ((sizeof(dataRecs)) * index) + sizeof(recIndex);
if ((msHan = fopen( str, mode)) == NULL)
   exit(0);
fseek(msHan, pointerPos, SEEK_SET);
fwrite(&dataRecs, sizeof(dataRecs), 1, msHan);
fclose(msHan);
return(0);
}


INT strErrMsg(HWND hwnd, CHAR *str)
{

if( strlen(str) < 3 )
   {
   WinMessageBox(HWND_DESKTOP,
		 hwnd,
		 "Search string must be at least 3 characters long ...",
		 "Attention!",
		 0,
		 MB_ICONEXCLAMATION | MB_OK );
   return(0);
   }
return(1);
}



ULONG MLEGetItemTextS(HWND hwndDlg, ULONG ulID, LONG lIptPosition,
      LONG lBoundary, PBYTE *ppText)
{
  ULONG ulTextSize;
  PBYTE pText=0;
  HWND  hwndControl = WinWindowFromID(hwndDlg, ulID);

  /* Get text length */
  ulTextSize = (LONG)WinSendMsg(hwndControl,MLM_QUERYFORMATTEXTLENGTH,MPFROMLONG(lIptPosition),MPFROMLONG(lBoundary));
  if (ulTextSize>0) {
     PBYTE pMLEBuff=0;
     ULONG ulBuffSize;
     pText = malloc(ulTextSize);

     /* Allocate MLE transfer buffer. MLE limited to 64K tiled memory */
     ulBuffSize = 65535;
     if (pText && !DosAllocMem((PPVOID) &pMLEBuff, (ULONG) ulBuffSize,
		PAG_COMMIT | PAG_READ | PAG_WRITE | OBJ_TILE)) {
	ULONG ulBytesCopied=0;
	ULONG ulBytesToCopy;
	ULONG ulTransfer;

	/* Set import/export MLE buffer */
	WinSendMsg(hwndControl, MLM_SETIMPORTEXPORT,
	   MPFROMP(pMLEBuff), MPFROMLONG( ulBuffSize));

	do {
	   if (ulBuffSize < ulTextSize-ulBytesCopied)
	      ulBytesToCopy=ulBuffSize;
	   else
	      ulBytesToCopy=ulTextSize-ulBytesCopied;

	   /* Get text at given IPT */
	   ulTransfer = (ULONG) WinSendMsg(hwndControl,
		    MLM_EXPORT,
		    MPFROMLONG(&lIptPosition), MPFROMLONG(&ulBytesToCopy));

	   if (!ulTransfer)
	      ulBytesCopied=ulTextSize;
	   else {
	      memcpy((PVOID)&(pText[ulBytesCopied]),
		     (PVOID)pMLEBuff, ulTransfer);
	      ulBytesCopied+=ulTransfer;
	   }
	} while (ulBytesCopied < ulTextSize);

	/* Free MLE transfer buffer */
	DosFreeMem((PBYTE)pMLEBuff);
     }
  }
  pText[ulTextSize]=0;
  *ppText = pText;
  return ulTextSize;
}

INT existFile(CHAR *fileStr)
{
struct ffblk ffblk;

if ( findfirst(fileStr,&ffblk,0) == 0 )
   return(1);
else
   return(0);
}


INT printRec(HWND hwndDlg, BOOL ASK)
{
UINT pstLen;
FILE *handM;
CHAR tempRecHeader[NAMESIZE+18] = "Record Name : ";
CHAR tempResetStr[] = "\r";
CHAR tempSpaceStr[] = "\n\n";
CHAR tempEndStr[] = "\n\x0C";


if( ASK )
   {
   if (WinMessageBox(
	    HWND_DESKTOP,
	    hwndDlg,
	    "Press Yes to continue or No to abort print request.",
	    "Is Your Printer Ready?",
	    0,
	    MB_ICONQUESTION |
	    MB_YESNO | MB_DEFBUTTON2 ) == MBID_NO )
      return(0);
   }

handM = fopen( "PRN", "w+t" );
if( handM == NULL )
   {
   DosBeep(3600, 100);
   return(0);
   }
strcat(tempRecHeader, dataRecs.noteName);

pstLen = (strlen(tempResetStr))*sizeof(CHAR);
fwrite(&tempResetStr, pstLen, 1, handM);

pstLen = (strlen(tempRecHeader))*sizeof(CHAR);
fwrite(&tempRecHeader, pstLen, 1, handM);

pstLen = (strlen(tempResetStr))*sizeof(CHAR);
fwrite(&tempResetStr, pstLen, 1, handM);

pstLen = (strlen(tempSpaceStr))*sizeof(CHAR);
fwrite(&tempSpaceStr, pstLen, 1, handM);

pstLen = (strlen(tempResetStr))*sizeof(CHAR);
fwrite(&tempResetStr, pstLen, 1, handM);

pstLen = (strlen(dataRecs.noteText))*sizeof(CHAR);
fwrite(&dataRecs.noteText, pstLen, 1, handM);

pstLen = (strlen(tempEndStr))*sizeof(CHAR);
fwrite(&tempEndStr, pstLen, 1, handM);
if( ASK )
   {
   DosBeep(300, 50);
   DosBeep(1000, 70);
   DosBeep(1500, 90);
   DosBeep(2200, 130);
   }
fclose(handM);

return(1);
}


INT saveToFile(CHAR *flName, FILE *hdl)
{
UINT pstLen;
CHAR tempRecHeader[NAMESIZE+18] = "Record Name : ";
CHAR tempResetStr[] = "\r";
CHAR tempSpaceStr[] = "\n\n";
CHAR tempEndStr[] = "\n\x0C";


strcat(tempRecHeader, dataRecs.noteName);

pstLen = (strlen(tempResetStr))*sizeof(CHAR);
fwrite(&tempResetStr, pstLen, 1, hdl);

pstLen = (strlen(tempRecHeader))*sizeof(CHAR);
fwrite(&tempRecHeader, pstLen, 1, hdl);

pstLen = (strlen(tempResetStr))*sizeof(CHAR);
fwrite(&tempResetStr, pstLen, 1, hdl);

pstLen = (strlen(tempSpaceStr))*sizeof(CHAR);
fwrite(&tempSpaceStr, pstLen, 1, hdl);

pstLen = (strlen(tempResetStr))*sizeof(CHAR);
fwrite(&tempResetStr, pstLen, 1, hdl);

pstLen = (strlen(dataRecs.noteText))*sizeof(CHAR);
fwrite(&dataRecs.noteText, pstLen, 1, hdl);


pstLen = (strlen(tempResetStr))*sizeof(CHAR);
fwrite(&tempResetStr, pstLen, 1, hdl);

pstLen = (strlen(tempSpaceStr))*sizeof(CHAR);
fwrite(&tempSpaceStr, pstLen, 1, hdl);

pstLen = (strlen(tempSpaceStr))*sizeof(CHAR);
fwrite(&tempSpaceStr, pstLen, 1, hdl);

pstLen = (strlen(tempSpaceStr))*sizeof(CHAR);
fwrite(&tempSpaceStr, pstLen, 1, hdl);


// pstLen = (strlen(tempEndStr))*sizeof(CHAR);
// fwrite(&tempEndStr, pstLen, 1, hdl);

return(1);
}


INT printIt(HWND hwndP, USHORT id, BOOL ASK)
{
APIRET rc;
CHAR      szFileName[CCHMAXPATH];    /* File name */
HFILE     fl;
PBYTE     pText;
ULONG     action;
CHAR tempRecHeader[NAMESIZE+18] = "Record Name : ";
CHAR tempResetStr[] = "\r";
CHAR tempSpaceStr[] = "\n\n";
CHAR tempEndStr[] = "\n\x0C";
UINT pstLen;
ULONG bytesW;

if( ASK )
   {
   if (WinMessageBox(
	    HWND_DESKTOP,
	    hwndP,
	    "Press Yes to continue or No to abort print request.",
	    "Is Your Printer Ready?",
	    0,
	    MB_ICONQUESTION |
	    MB_YESNO | MB_DEFBUTTON2 ) == MBID_NO )
      return(0);
   }
rc = DosOpen("PRN",
	        &fl,
	        &action,
	        0L,
	        0x0000,
	        0x0001,
	        0x0041,
	        0L);
if( rc == 0 )
   {
   MLEGetT(hwndP, id,
		  0,       /* Starting from the beginning        */
		  -1,      /* Until the end of the text          */
		  &pText); /* Ptr for text buffer area           */
   if (pText)
      {
      strcat(tempRecHeader, dataRecs.noteName);

      pstLen = (strlen(tempResetStr))*sizeof(CHAR);
      DosWrite(fl, &tempResetStr, pstLen, &bytesW);

      pstLen = (strlen(tempRecHeader))*sizeof(CHAR);
      DosWrite(fl, &tempRecHeader, pstLen, &bytesW);

      pstLen = (strlen(tempResetStr))*sizeof(CHAR);
      DosWrite(fl, &tempResetStr, pstLen, &bytesW);

      pstLen = (strlen(tempSpaceStr))*sizeof(CHAR);
      DosWrite(fl, &tempSpaceStr, pstLen, &bytesW);

      pstLen = (strlen(tempResetStr))*sizeof(CHAR);
      DosWrite(fl, &tempResetStr, pstLen, &bytesW);

      DosWrite(fl, (PSZ)pText, strlen(pText), &action);

      pstLen = (strlen(tempEndStr))*sizeof(CHAR);
      DosWrite(fl, &tempEndStr, pstLen, &bytesW);

      free(pText);
      }
   }
else
   return(0);
DosClose(fl);
if( ASK )
   {
   DosBeep(300, 50);
   DosBeep(1000, 70);
   DosBeep(1500, 90);
   DosBeep(2200, 130);
   }
return(1);               /* Close file handle    */
}


ULONG MLEGetT(HWND hwndDlg, ULONG ulID, LONG lIptPosition,
      LONG lBoundary, PBYTE *ppText)
{
  ULONG ulTextSize;
  PBYTE pText=0;
  HWND  hwndControl = WinWindowFromID(hwndDlg, ulID);

  /* Get text length */
  ulTextSize = (LONG)WinSendMsg(hwndControl,MLM_QUERYFORMATTEXTLENGTH,MPFROMLONG(lIptPosition),MPFROMLONG(lBoundary));
  if (ulTextSize>0) {
     PBYTE pMLEBuff=0;
     ULONG ulBuffSize;
     pText = malloc(ulTextSize);

     /* Allocate MLE transfer buffer. MLE limited to 64K tiled memory */
     ulBuffSize = 65535;
     if (pText && !DosAllocMem((PPVOID) &pMLEBuff, (ULONG) ulBuffSize,
		PAG_COMMIT | PAG_READ | PAG_WRITE | OBJ_TILE)) {
	ULONG ulBytesCopied=0;
	ULONG ulBytesToCopy;
	ULONG ulTransfer;

	/* Set import/export MLE buffer */
	WinSendMsg(hwndControl, MLM_SETIMPORTEXPORT,
	   MPFROMP(pMLEBuff), MPFROMLONG( ulBuffSize));

	do {
	   if (ulBuffSize < ulTextSize-ulBytesCopied)
	      ulBytesToCopy=ulBuffSize;
	   else
	      ulBytesToCopy=ulTextSize-ulBytesCopied;

	   /* Get text at given IPT */
	   ulTransfer = (ULONG) WinSendMsg(hwndControl,
		    MLM_EXPORT,
		    MPFROMLONG(&lIptPosition), MPFROMLONG(&ulBytesToCopy));

	   if (!ulTransfer)
	      ulBytesCopied=ulTextSize;
	   else {
	      memcpy((PVOID)&(pText[ulBytesCopied]),
		     (PVOID)pMLEBuff, ulTransfer);
	      ulBytesCopied+=ulTransfer;
	   }
	} while (ulBytesCopied < ulTextSize);

	/* Free MLE transfer buffer */
	DosFreeMem((PBYTE)pMLEBuff);
     }
  }
  pText[ulTextSize]=0;
  *ppText = pText;
  return ulTextSize;
}




