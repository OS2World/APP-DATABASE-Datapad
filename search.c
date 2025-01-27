/* search.c   2.1 7-17-95  */


#define INCL_WIN
#define INCL_DOS
#define INCL_SUB

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
#include <process.h>
#include "paddef.h"

MRESULT EXPENTRY nSearchProc( HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY searchRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY eSearchRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY sSearchRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY vSearchRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY searchViewProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY sNameProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY noMoreRecsProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY results1Proc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY results2Proc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY newResult1Proc(HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY newResult2Proc(HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY saveALLFProc(HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
VOID FAR searchNames(VOID *hwnd);
VOID FAR searchRecs(VOID *hwnd);
VOID FAR resultRecs(VOID *hwnd);
VOID FAR result2Recs(VOID *hwnd);
INT searchRec(HWND hwndDlg);
INT eSearchRec(HWND hwndDlg);
INT sSearchRec(HWND hwndDlg);
INT vSearchRec(HWND hwndDlg);
// INT searchView(HWND hwndDlg);
VOID FAR searchView(VOID *hwndDlg);
INT results1(HWND hwndR);
INT results2(HWND hwndR);
VOID enableButtons(VOID);
VOID disableButtons(VOID);
VOID unSubClassWin(VOID);
VOID subClassWin(VOID);
INT chkRetS(VOID);
INT chkRet(VOID);
INT initLoad(HWND hwndList, CHAR *fileName);
INT compIndexName(HWND hwndM, CHAR *sName);
INT chkIntegrityPDF(HWND hwndQ, CHAR *PDFName);
INT saveAllF(HWND hwndDlg);
VOID FAR saveATFileS(VOID *hwnd);
VOID loadNewSRec(HWND hwndM, HWND hwndD);
VOID loadNewR1RecV(HWND hwndM, HWND hwndD);
VOID loadNewR1RecE(HWND hwndM, HWND hwndD);
VOID fileFromDragItem ( PDRAGITEM pdiItem, PCHAR pchFile );

/**********  DEBUG FUNCS *************************/
VOID msgOK(CHAR *title, CHAR *message);
VOID debugMsgCH(CHAR ch, CHAR *varName);
VOID debugMsgStr(CHAR *str, CHAR *varName);
VOID debugMsgULong(ULONG num, CHAR *varName);
VOID debugMsgInt(INT num, CHAR *varName);
/**********  DEBUG FUNCS *************************/


// extern VOID strdelc( CHAR *s, CHAR c);
extern VOID setPPMenuDef(INT num);
extern INT validatePDF(CHAR *df);
extern INT multiRead(CHAR *inFile, INT datS, INT sizeF, INT recNum);
extern INT multiWrite(CHAR *inFile, INT datS, INT sizeF, INT recNum);
extern INT removeSwEntry(HWND hwnd);
extern INT addSwEntry(HWND hwnd);
INT printIt(HWND hwndP, USHORT id, BOOL ASK);
extern INT printRec(HWND hwndDlg, BOOL ASK);
extern VOID addFWHeader(FILE *hand, CHAR *dRec);
extern HFILE openTmp(HFILE hfile, CHAR *szFullPathName, CHAR *szFullDocName, CHAR *fileName);
extern INT setRGBColor(HWND hwndC, ULONG r, ULONG rg, ULONG rb);
extern ULONG MLEGetItemTextS(HWND hwndDlg, ULONG ulID, LONG lIptPosition,
			    LONG lBoundary, PBYTE *ppText);

INT start;
INT stT2;
INT m2 = 0;
CHAR globalV[NAMESIZE];
CHAR globalVV[NAMESIZE];
BOOL CURRENT;
INT gCurMod;
INT ORIGCASE;
HWND hwndResults;
HEV hev, hevS;
INT qFactor[NUMPDFMODS];
INT CONT;
HWND hwndSMLEPopup;
BOOL KILLSSTHRERAD;
HWND hwndSearchViewRec;
BOOL SVRET;
HWND hwndRes, hwndResn;
CHAR szFullDocNameS[MAXNAME] ;
HPOINTER  standardz;


extern MRESULT EXPENTRY MainDlgProc(HWND hwndF, ULONG msg,
			     MPARAM mp1, MPARAM mp2);
extern INT openFile(CHAR *fileName, CHAR *mode);
extern VOID closeFile(VOID);
extern INT matchName(HWND hwndM, CHAR *lbSelect);
extern INT copyData(INT c);
extern VOID setParams(HWND hwndP, SHORT dx, ULONG IDE, ULONG IDR);
extern INT initLoad_index(HWND hwndList, CHAR *dataFileName);
extern VOID setStatus(HWND hwndST, CHAR *pFile);
extern INT strErrMsg(HWND hwnd, CHAR *str);
extern VOID viewRec(HWND hwndDlg);
extern VOID editRec(HWND hwndDlg);
extern INT saveFileRec(HWND hwndDlg);
extern INT saveToFile(CHAR *flName, FILE *hdl);
extern INT existFile(CHAR *fileStr);
extern VOID loadNewRec(HWND hwndM, HWND hwndD);

extern INT INUSE;
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
extern PFNWP oldProc1;
extern PFNWP oldProc2;
extern PFNWP oldProc3;
extern PFNWP oldProc4;
extern PFNWP oldProc5;
extern PFNWP oldProc6;
extern PFNWP oldProc7;
extern PFNWP oldProc8;
extern PFNWP oldProc9;
extern PFNWP oldProc10;
extern HWND hwnd1, hwnd2, hwnd3, hwnd4, hwnd5,
	    hwnd6, hwnd7, hwnd8, hwnd9, hwnd10, hwnd11, hwnd12;
extern FILE *datHan;
extern HWND hwndMenu;
extern HWND hwndFrame;
// extern HWND hwndRFrame;
extern CHAR datFile[MAXNAME];
extern INT numRecs, WW;
extern INT eols[MAXLINES];
extern INT CASE;
extern INT BUSY;
extern CHAR *pdest;
extern CHAR schText[NAMESIZE];
extern CHAR *pT;
extern INT stT;
extern CHAR xtext[NAMESIZE];
extern ULONG pValue[16][32];
extern HOBJECT hobjBG;
extern HOBJECT hobjFNT;
extern pgmData[60];
extern LONG min, maxSel;
extern UINT iz;
extern HWND hwndMLBox;
extern BOOL ACCUM;
extern BOOL ESEARCH;
extern BOOL NSEARCH;
extern BOOL CURMODONLY;
extern BOOL CASESENSITIVE;
extern BOOL NAMEONLY;
extern BOOL NAMESEARCH;
extern CHAR basePath[MAXNAME];
extern ULONG scrLen, scrWid;
extern UINT FIXEDLEN;
extern CHAR docFlName[MAXNAME];
extern HWND hwndRFrame;
extern HWND hwndMenu;
extern CHAR cwdFound[MAXNAME+NAMESIZE];
extern CHAR EXTENTION[EXTSIZE];
extern HWND hwndCurViewRecord;
extern HWND hwndCurEViewRecord;
extern INT m;
extern HWND hwndMainDlg;
extern INT nChox;
extern CHAR datFilex[MAXNAME];
extern CHAR oldRecName[NAMESIZE];


/**********  DEBUG FUNCS *************************/
VOID msgOK(CHAR *title, CHAR *message)
{
WinMessageBox(HWND_DESKTOP,
	      HWND_DESKTOP,
	      message,
	      title,
	      0,
	      MB_OK | MB_ICONEXCLAMATION );
}
VOID debugMsgStr(CHAR *str, CHAR *varName)
{
CHAR dMsg[255] = "Debug Message string ... ";

strcat(dMsg, varName),
WinMessageBox(HWND_DESKTOP,
	      HWND_DESKTOP,
	      str,
	      dMsg,
	      0,
	      MB_OK | MB_ICONEXCLAMATION );
}

VOID debugMsgInt(INT num, CHAR *varName)
{
CHAR str[255] = "";
CHAR dMsg[255] = "Debug Message Int ... ";

strcat(dMsg, varName),
itoa(num, str, 10);
WinMessageBox(HWND_DESKTOP,
	      HWND_DESKTOP,
	      str,
	      dMsg,
	      0,
	      MB_OK | MB_ICONEXCLAMATION );
}

VOID debugMsgULong(ULONG num, CHAR *varName)
{
CHAR str[255] = "";
CHAR dMsg[255] = "Debug Message ULong ... ";

strcat(dMsg, varName),
ultoa(num, str, 10);
WinMessageBox(HWND_DESKTOP,
	      HWND_DESKTOP,
	      str,
	      dMsg,
	      0,
	      MB_OK | MB_ICONEXCLAMATION );
}


VOID debugMsgLong(LONG num, CHAR *varName)
{
CHAR str[255] = "";
CHAR dMsg[255] = "Debug Message Long ... ";

strcat(dMsg, varName),
ltoa(num, str, 10);
WinMessageBox(HWND_DESKTOP,
	      HWND_DESKTOP,
	      str,
	      dMsg,
	      0,
	      MB_OK | MB_ICONEXCLAMATION );
}


VOID debugMsgCH(CHAR ch, CHAR *varName)
{
CHAR str[255] = "";
CHAR dMsg[255] = "Debug Message char ... ";

strcat(dMsg, varName),
str[0] = ch;
str[1] = '\0';
WinMessageBox(HWND_DESKTOP,
	      HWND_DESKTOP,
	      str,
	      dMsg,
	      0,
	      MB_OK | MB_ICONEXCLAMATION );
}
/**********  DEBUG FUNCS *************************/


INT chkIntegrityPDF(HWND hwndQ, CHAR *PDFName)
{
div_t xv;
struct find_t ffblkv;

if ( _dos_findfirst(PDFName,_A_NORMAL,&ffblkv) != 0 )
   {
   CHAR dat[MAXNAME];

   strcpy(dat, basePath);
   if( dat[strlen(dat)-1] == '\\' )
      strcat(dat, "DATAPAD.DAT");
   else
      strcat(dat, "\\DATAPAD.DAT");
   WinMessageBox(HWND_DESKTOP,
		 hwndQ,
		 "Press OK to correct invalid entry in Search Module list.",
		 "Can NOT find or load data file!!",
		 0,
		 MB_ICONEXCLAMATION | MB_OK);
   validatePDF(dat);
   return(-1);
   }
return(1);
}

INT searchRec(HWND hwndDlg)
{
if( WinDlgBox(HWND_DESKTOP,
	      hwndDlg,
	      searchRecProc,
	      (HMODULE)0,
	      ID_SEARCHREC,
	      NULL) == TRUE )
   return(1);
else
   return(0);
}

INT saveAllF(HWND hwndDlg)
{
if( WinDlgBox(HWND_DESKTOP,
	      hwndDlg,
	      saveALLFProc,
	      (HMODULE)0,
	      ID_SAPROMPT,
	      NULL) == TRUE )
   return(1);
else
   return(0);
}

INT vSearchRec(HWND hwndDlg)
{
if (WinDlgBox(HWND_DESKTOP,
	      hwndDlg,
	      vSearchRecProc,
	     (HMODULE)0,
              ID_EDITSEARCH,
	      NULL) == TRUE)
   return(1);
else
   return(0);
}

INT eSearchRec(HWND hwndDlg)
{
if (WinDlgBox(HWND_DESKTOP,
	      hwndDlg,
	      eSearchRecProc,
	     (HMODULE)0,
	      ID_EDITSEARCH,
	      NULL) == TRUE)
   return(1);
else
   return(0);
}


INT sSearchRec(HWND hwndDlg)
{
if (WinDlgBox(HWND_DESKTOP,
	      hwndDlg,
	      sSearchRecProc,
	     (HMODULE)0,
	      ID_EDITSEARCH,
	      NULL) == TRUE)
   return(1);
else
   return(0);
}



// DosBeep(100, 100);
/*
INT searchView(HWND hwndDlg)
{
if( WinDlgBox(
	  HWND_DESKTOP,
	  hwndDlg,
	  searchViewProc,
	  (HMODULE)0,
	  ID_SEARCHVIEW,
	  NULL) == FALSE )
    return(-1);
else
   return(1);
}
*/


VOID searchView(VOID *hwndDlg)
{
HAB habT;
HMQ hmqT;
HWND hwndDl;

habT = WinInitialize(0);
hmqT = WinCreateMsgQueue(habT, 0);
WinCancelShutdown(hmqT, TRUE);

hwndDl = (HWND)hwndDlg;

if( WinDlgBox(
	  HWND_DESKTOP,
	  (HWND)hwndDlg,
	  searchViewProc,
	  (HMODULE)0,
	  ID_SEARCHVIEW,
	  NULL) == FALSE )
    SVRET = FALSE;
else
    SVRET = FALSE;

WinDestroyMsgQueue(hmqT);
WinTerminate(habT);
_endthread();
}



/*
VOID searchView(HWND hwndDlg)
{
if( WinDlgBox(
	  HWND_DESKTOP,
	  hwndDlg,
	  searchViewProc,
	  (HMODULE)0,
	  ID_SEARCHVIEW,
	  NULL) == FALSE )
    return(-1);
else
   return(1);
}
*/

MRESULT EXPENTRY saveALLFProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
switch ( msg )
   {
   case WM_INITDLG:
      if( strlen(cwdFound) == 0 )
	 {
	 CHAR defFile[] = "FOUND.DOC";

	 getcwd(cwdFound, MAXNAME);
	 if( cwdFound[strlen(cwdFound)-1] == '\\' )
	    strcat(cwdFound, defFile);
	 else
	    {
	    strcat(cwdFound, "\\");
	    strcat(cwdFound, defFile);
	    }
	 }
      WinSendDlgItemMsg(hwndDlg, ID_SAFILE,EM_SETTEXTLIMIT,
			MPFROM2SHORT(MAXNAME,0),NULL);
      WinSetDlgItemText(hwndDlg, ID_SAFILE, cwdFound);
      WinSendDlgItemMsg(hwndDlg, ID_SAFILE,
			EM_SETSEL, MPFROM2SHORT(0,strlen(cwdFound)), 0);
      WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwndDlg, ID_SAFILE));
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_DESTROY:
      WinSetFocus(HWND_DESKTOP,
		  WinWindowFromID(WinQueryWindow(hwndDlg,QW_OWNER),
				  ID_FINDLIST1));
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_OK:
	    {
	    WinQueryDlgItemText(hwndDlg, ID_SAFILE, sizeof(cwdFound), cwdFound);
	    if( existFile(cwdFound) )
	       {
	       if(WinMessageBox(HWND_DESKTOP,
				hwndDlg,
				"File already exists. Overwrite it?",
				"ATTENTION!",
				0,
				MB_ICONEXCLAMATION |
				MB_OKCANCEL) == MBID_OK)
		  WinDismissDlg( hwndDlg, TRUE );
	       else
		  break;
	       }
	    else
	       WinDismissDlg( hwndDlg, TRUE );
	    }
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


MRESULT EXPENTRY searchRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
HWND hwndM;

switch ( msg )
   {
   case WM_INITDLG:
      WinSendDlgItemMsg(hwndDlg, ID_ENTRY5,EM_SETTEXTLIMIT,
			MPFROM2SHORT(NAMESIZE,0),NULL);
      WinSetDlgItemText(hwndDlg, ID_ENTRY5, schText);
      WinSendDlgItemMsg(hwndDlg, ID_ENTRY5,
			EM_SETSEL, MPFROM2SHORT(0,strlen(schText)), 0);
      if( ACCUM )
	 WinCheckButton(hwndDlg, ID_ACUMSEARCH, TRUE);
      if( CURMODONLY )
	 WinCheckButton(hwndDlg, ID_CMODONLY, TRUE);
      if( CASESENSITIVE )
	 WinCheckButton(hwndDlg, ID_CASE, TRUE);
      if( NAMEONLY )
	 WinCheckButton(hwndDlg, ID_NAME, TRUE);
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case ID_OK4 :
	    hwndM = WinQueryWindow(hwndDlg,QW_OWNER);
	    WinQueryDlgItemText(hwndDlg,
				ID_ENTRY5,
				sizeof(schText),
				schText);
	    if( !strErrMsg(hwndDlg, schText) )
	       return(0);
	    numRecs = WinQueryLboxCount(WinWindowFromID(hwndMainDlg,ID_LISTBOX1));
	    if ( WinQueryButtonCheckstate( hwndDlg,ID_CASE) )
	       {
	       CASE = 1;
	       pgmData[10] = 1;
	       CASESENSITIVE = TRUE;
	       }
	    else
	       {
	       CASE = 0;
	       pgmData[10] = 0;
	       CASESENSITIVE = FALSE;
	       }
	    ORIGCASE = CASE;
	    if ( WinQueryButtonCheckstate( hwndDlg,ID_NAME) )
	       {
	       pgmData[11] = 1;
	       NAMEONLY = TRUE;
	       }
	    else
	       {
	       pgmData[11] = 0;
	       NAMEONLY = FALSE;
	       }
	    if ( WinQueryButtonCheckstate( hwndDlg,ID_ACUMSEARCH) )
	       {
	       pgmData[8] = 0;
	       ACCUM = TRUE;
	       }
	    else
	       {
	       pgmData[8] = 1;
	       ACCUM = FALSE;
	       }
	    if ( WinQueryButtonCheckstate( hwndDlg,ID_CMODONLY) )
	       {
	       pgmData[9] = 1;
	       CURMODONLY = TRUE;
	       }
	    else
	       {
	       pgmData[9] = 0;
	       CURMODONLY = FALSE;
	       }
	    if ( WinQueryButtonCheckstate( hwndDlg,ID_NAME) )
	       NAMESEARCH = TRUE;
	    else
	       NAMESEARCH = FALSE;
	    DosCreateEventSem(NULL, &hev, 0L, FALSE);
	    // DosCreateEventSem(NULL, &hevS, 0L, FALSE);
	    WinDismissDlg( hwndDlg, TRUE );
	    break;

	 case ID_CANCEL4 :
	    if ( WinQueryButtonCheckstate( hwndDlg,ID_CASE) )
	       {
	       pgmData[10] = 1;
	       CASESENSITIVE = TRUE;
	       }
	    else
	       {
	       pgmData[10] = 0;
	       CASESENSITIVE = FALSE;
	       }
	    if ( WinQueryButtonCheckstate( hwndDlg,ID_NAME) )
	       {
	       pgmData[11] = 1;
	       NAMEONLY = TRUE;
	       }
	    else
	       {
	       pgmData[11] = 0;
	       NAMEONLY = FALSE;
	       }
	    if ( WinQueryButtonCheckstate( hwndDlg,ID_CMODONLY) )
	       {
	       pgmData[9] = 1;
	       CURMODONLY = TRUE;
	       }
	    else
	       {
	       pgmData[9] = 0;
	       CURMODONLY = FALSE;
	       }
	    if ( WinQueryButtonCheckstate( hwndDlg,ID_ACUMSEARCH) )
	       {
	       pgmData[8] = 0;
	       ACCUM = TRUE;
	       }
	    else
	       {
	       pgmData[8] = 1;
	       ACCUM = FALSE;
	       }
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


MRESULT EXPENTRY sSearchRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
switch ( msg )
   {
   case WM_INITDLG:
      WinSendDlgItemMsg(hwndDlg, ID_ENTRYES,EM_SETTEXTLIMIT,
			MPFROM2SHORT(NAMESIZE,0),NULL);
      WinSetDlgItemText(hwndDlg, ID_ENTRYES, schText);
      WinSendDlgItemMsg(hwndDlg, ID_ENTRYES,
			EM_SETSEL, MPFROM2SHORT(0,strlen(schText)), 0);
      INUSE = 1;
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_DESTROY:
      INUSE = 0;
      WinSetFocus(HWND_DESKTOP,
		  WinWindowFromID(WinQueryWindow(hwndDlg,QW_OWNER),
				  ID_ENTRY9));
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case ID_OKES :
	    WinQueryDlgItemText(hwndDlg,
                                ID_ENTRYES,
				sizeof(schText),
				schText);
	    if( !strErrMsg(hwndDlg, schText) )
	       return(0);
            if ( WinQueryButtonCheckstate( hwndDlg,ID_CASEES) )
	       CASE = 1;
            else
	       CASE = 0;
	    if( strlen(schText) < 1 )
	       {
	       DosBeep(3100, 130);
               DosBeep(2200, 90);
	       DosBeep(1500, 70);
	       DosBeep(100, 50);
	       WinDismissDlg( hwndDlg, FALSE );
               INUSE = 0;
               }
	    else
               {
	       INUSE = 0;
               WinDismissDlg( hwndDlg, TRUE );
               }
	    break;

	 case ID_CANCELES :
            WinDismissDlg( hwndDlg, FALSE );
	    INUSE = 0;
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

MRESULT EXPENTRY vSearchRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
switch ( msg )
   {
   case WM_INITDLG:
      WinSendDlgItemMsg(hwndDlg, ID_ENTRYES,EM_SETTEXTLIMIT,
			MPFROM2SHORT(NAMESIZE,0),NULL);
      WinSetDlgItemText(hwndDlg, ID_ENTRYES, schText);
      WinSendDlgItemMsg(hwndDlg, ID_ENTRYES,
                        EM_SETSEL, MPFROM2SHORT(0,strlen(schText)), 0);
      INUSE = 1;
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_DESTROY:
      INUSE = 0;
      WinSetFocus(HWND_DESKTOP,
		  WinWindowFromID(WinQueryWindow(hwndDlg,QW_OWNER),
                                  ID_ENTRY4));
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
         case ID_OKES :
	    WinQueryDlgItemText(hwndDlg,
				ID_ENTRYES,
                                sizeof(schText),
				schText);
            if( !strErrMsg(hwndDlg, schText) )
	       return(0);
            if ( WinQueryButtonCheckstate( hwndDlg,ID_CASEES) )
	       CASE = 1;
	    else
	       CASE = 0;
	    if( strlen(schText) < 1 )
               {
	       DosBeep(3100, 130);
	       DosBeep(2200, 90);
	       DosBeep(1500, 70);
	       DosBeep(100, 50);
	       WinDismissDlg( hwndDlg, FALSE );
               INUSE = 0;
	       }
            else
               {
	       INUSE = 0;
	       WinDismissDlg( hwndDlg, TRUE );
	       }
	    break;

	 case ID_CANCELES :
	    WinDismissDlg( hwndDlg, FALSE );
            INUSE = 0;
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

MRESULT EXPENTRY eSearchRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
switch ( msg )
   {
   case WM_INITDLG:
      WinSendDlgItemMsg(hwndDlg, ID_ENTRYES,EM_SETTEXTLIMIT,
			MPFROM2SHORT(NAMESIZE,0),NULL);
      WinSetDlgItemText(hwndDlg, ID_ENTRYES, schText);
      WinSendDlgItemMsg(hwndDlg, ID_ENTRYES,
			EM_SETSEL, MPFROM2SHORT(0,strlen(schText)), 0);
      INUSE = 1;
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_DESTROY:
      INUSE = 0;
      WinSetFocus(HWND_DESKTOP,
		  WinWindowFromID(WinQueryWindow(hwndDlg,QW_OWNER),
				  ID_ENTRY3));
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case ID_OKES :
	    WinQueryDlgItemText(hwndDlg,
				ID_ENTRYES,
				sizeof(schText),
				schText);
	    if( !strErrMsg(hwndDlg, schText) )
	       return(0);
	    if ( WinQueryButtonCheckstate( hwndDlg,ID_CASEES) )
	       CASE = 1;
	    else
	       CASE = 0;
	    if( strlen(schText) < 1 )
	       {
	       DosBeep(3100, 130);
	       DosBeep(2200, 90);
               DosBeep(1500, 70);
	       DosBeep(100, 50);
               WinDismissDlg( hwndDlg, FALSE );
	       INUSE = 0;
	       }
	    else
	       {
               INUSE = 0;
	       WinDismissDlg( hwndDlg, TRUE );
	       }
	    break;

	 case ID_CANCELES :
	    WinDismissDlg( hwndDlg, FALSE );
	    INUSE = 0;
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


VOID result2Recs(VOID *hwnd) /* search NAMES ACCUMULATIVE */
{
INT cnt;
INT i, i2, x, q;
HAB habT;
HMQ hmqT;
INT CONT = 1;
CHAR recName[NAMESIZE];
INT currentMod;
INT numMatch;

habT = WinInitialize(0);
hmqT = WinCreateMsgQueue(habT, 0);
WinCancelShutdown(hmqT, TRUE);
DosWaitEventSem(hev, -1);
DosCloseEventSem(hev);
strcpy(xtext, schText);
x = 0;
while( strcmp(pdf[x].noteText, "") != 0 && x < MAXNUMRECS )
   x++;
currentMod = 0;
for( q=0;q<NUMPDFMODS;q++ )
   qFactor[q] = 0;
if( CURMODONLY )
   x = 1;
for( q=0;q<x;q++ )
   {
   numMatch = 0;
   if( CASE == 0 )
      strupr(schText);
   if( q != 0 )
      {
      currentMod++;
      initLoad(WinWindowFromID((HWND)hwnd, ID_LISTBOX1), pdf[q].noteText);
      WinPostMsg(WinWindowFromID((HWND)hwnd, ID_LISTBOX1),
		    LM_SELECTITEM,
		    MPFROMSHORT(0),
		    MPFROMSHORT(TRUE));
      setStatus((HWND)hwnd, pdf[q].noteText);
      }
   cnt = WinQueryLboxCount(WinWindowFromID((HWND)hwnd, ID_LISTBOX1));
   BUSY = 1;
   for( i=0;i<cnt;i++ )
      {
      strcpy(recName, recIndex[i].recName);
      if( CASE == 0 )
	 strupr(recName);
      if ( (pdest = strstr(recName, schText)) != NULL )
	 {
	 numMatch++;
	 qFactor[q] = numMatch;
	 i2 = matchName((HWND)hwnd, recName);
	 WinSendMsg(hwndResults, LM_INSERTITEM,
		    MPFROMSHORT(LIT_END),
		    MPFROMP(recName));
	  }
      if( CONT == 0 )
	 break;
      }
   strcpy(schText, xtext);
   BUSY = 0;
   if( CONT == 0 )
      break;
   }
if( x > 1 )
   initLoad(WinWindowFromID((HWND)hwnd, ID_LISTBOX1), pdf[0].noteText);
setStatus((HWND)hwnd, datFile);
WinPostMsg(WinWindowFromID((HWND)hwnd, ID_LISTBOX1),
	   LM_SELECTITEM,
	   MPFROMSHORT(0),
	   MPFROMSHORT(TRUE));
WinPostMsg(hwndResults,
	   LM_SELECTITEM,
	   MPFROMSHORT(0),
	   MPFROMSHORT(TRUE));
WinPostMsg(WinQueryWindow(hwndResults, QW_OWNER), ID_SEARCHOVER, 0, 0);
WinPostMsg(WinQueryWindow(hwndResults, QW_OWNER),
			  WM_COMMAND,
			  MPFROMSHORT(ID_SETNUMHITS),
			  MPFROMSHORT(TRUE));
DosBeep(1200, 100);
WinDestroyMsgQueue(hmqT);
WinTerminate(habT);
_endthread();
}

VOID searchNames(VOID *hwnd) /*  search NAMES SEQUENTIAL */
{
INT cnt;
INT i, i2, x, q;
HAB habT;
HMQ hmqT;
INT CONT = 1;
CHAR recName[NAMESIZE];
INT currentMod;

habT = WinInitialize(0);
hmqT = WinCreateMsgQueue(habT, 0);
WinCancelShutdown(hmqT, TRUE);
strcpy(xtext, schText);
x = 0;
while( strcmp(pdf[x].noteText, "") != 0 && x < MAXNUMRECS )
   x++;
currentMod = 0;
if( CURMODONLY )
   x = 1;
for( q=0;q<x;q++ )
   {
   if( CASE == 0 )
      strupr(schText);
   if( q != 0 )
      {
      currentMod++;
      initLoad(WinWindowFromID((HWND)hwnd, ID_LISTBOX1), pdf[q].noteText);
      WinPostMsg(WinWindowFromID((HWND)hwnd, ID_LISTBOX1),
		    LM_SELECTITEM,
		    MPFROMSHORT(0),
		    MPFROMSHORT(TRUE));
      setStatus((HWND)hwnd, pdf[q].noteText);
      }
   cnt = WinQueryLboxCount(WinWindowFromID((HWND)hwnd, ID_LISTBOX1));
   BUSY = 1;
   for( i=0;i<cnt;i++ )
      {
      strcpy(recName, recIndex[i].recName);
      if( CASE == 0 )
	 strupr(recName);
      if ( (pdest = strstr(recName, schText)) != NULL )
	 {
	 BUSY = 0;
	 i2 = matchName((HWND)hwnd, recName);
	 WinPostMsg(WinWindowFromID((HWND)hwnd, ID_LISTBOX1),
		      LM_SELECTITEM,
		      MPFROMSHORT(i2), MPFROMSHORT(TRUE));
	 if( WinDlgBox(HWND_DESKTOP,
			 HWND_DESKTOP,
			 sNameProc,
			 (HMODULE)0,
			 ID_SNAMEAGAIN,
			 NULL) == FALSE )
	    {
	    BUSY = 0;
	    CONT = 0;
	    }
	 else
	    BUSY = 1;
	  }
      if( CONT == 0 )
	 break;
      }
   strcpy(schText, xtext);
   BUSY = 0;
   if( CONT == 0 )
      break;
   }
if( CONT == 1 )
   WinDlgBox(HWND_DESKTOP,
	     (HWND)hwnd,
	     noMoreRecsProc,
	     (HMODULE)0,
	     ID_NOMORERECS,
	     NULL);
if( CURRENT == TRUE )
   {
   if( currentMod > 0 )
      {
      FILE *msHan;
      INT z;

      for( z=x;z<NUMPDFMODS;z++ )
	 pdf[z].noteText[0] = '\0';
      strcpy(datFile, pdf[currentMod].noteText);
      strcpy(pdf[currentMod].noteText, pdf[0].noteText);
      strcpy(pdf[0].noteText, datFile);
      if ((msHan = fopen( "DATAPAD.DAT", "w+b")) == NULL)
	    exit(0);
      fwrite(&pdf, sizeof(pdf), 1, msHan);
      fclose(msHan);
      }
   }
else
   {
   if( currentMod > 0 )
      {
      initLoad(WinWindowFromID((HWND)hwnd, ID_LISTBOX1), pdf[0].noteText);
      WinPostMsg(WinWindowFromID((HWND)hwnd, ID_LISTBOX1),
		   LM_SELECTITEM,
		   MPFROMSHORT(0),
		   MPFROMSHORT(TRUE));
      }
   }
setStatus((HWND)hwnd, datFile);
WinDestroyMsgQueue(hmqT);
WinTerminate(habT);
_endthread();
}


MRESULT EXPENTRY sNameProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
switch ( msg )
   {
   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case ID_OKNAMES :
	    WinDismissDlg( hwndDlg, TRUE );
            break;

	 case ID_CANCELCURN :
	    CURRENT = TRUE;
	    WinDismissDlg( hwndDlg, FALSE );
	    break;

	 case ID_CANCELORIGN :
	    CURRENT = FALSE;
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


MRESULT EXPENTRY noMoreRecsProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
switch ( msg )
   {
   case WM_INITDLG:
      DosBeep(800, 70);
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
         {
	 case ID_CANCELNMCUR :
	    CURRENT = TRUE;
            WinDismissDlg( hwndDlg, TRUE );
	    break;

	 case ID_CANCELNMORIG :
	    CURRENT = FALSE;
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


INT initLoad(HWND hwndList, CHAR *fileName)
{
FILE *Hanv;
INT i = 0;
for( i=0;i<MAXNUMRECS;i++ )
    recIndex[i].recName[0] = '\0';
recIndex[0].alIndex = -1;
WinSetPointer(HWND_DESKTOP,
	      WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE));
WinSendMsg(hwndList, LM_DELETEALL, 0L, 0L);
if ((Hanv = fopen( fileName, "rb" )) == NULL)
   {
   DosBeep(1000, 100);
   exit(-1);
   }
WinEnableWindowUpdate(hwndList, FALSE);
fread(&recIndex, sizeof(recIndex), 1, Hanv);
i = 0;
while( strcmp(recIndex[i].recName, "") != 0 && i < MAXNUMRECS)
   {
   WinSendMsg(hwndList, LM_INSERTITEM,
	      MPFROMSHORT(LIT_SORTASCENDING), MPFROMP(recIndex[i].recName));
   numRecs = ++i;
   }
WinEnableWindowUpdate(hwndList, TRUE);
fclose(Hanv);
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


INT results1(HWND hwndR)  /* TEXT DATA SEARCH */
{
if( WinDlgBox(HWND_DESKTOP,
	      hwndR,
	      results1Proc,
	      (HMODULE)0,
	      ID_RESULTS1,
	      NULL) == TRUE )
   return(1);
else
   return(0);
}

INT results2(HWND hwndR)  /* NAMES  SEARCH*/
{
if( WinDlgBox(HWND_DESKTOP,
	      hwndR,
	      results2Proc,
	      (HMODULE)0,
	      ID_RESULTS1,
	      NULL) == TRUE )
   return(1);
else
   return(0);
}


/*
VOID loadNewR1Rec(HWND hwndM, HWND hwndD)
{
INT p, r;
CHAR flr[6];
CHAR szModName[75] = "View Record       Module : ";
INT z;


// WinSendMsg(hwndD, WM_COMMAND, MPFROM2SHORT(ID_RESETV, 0), 0);
WinSetWindowText(WinWindowFromID(hwndD, ID_ENTRY4),
		 dataRecs.noteText);
WinSetWindowText(WinWindowFromID(hwndD, ID_RECNAME4),
		 dataRecs.noteName);
z = (FIXEDLEN - 1) - (strlen(dataRecs.noteText) * sizeof(CHAR));
WinSetDlgItemShort(hwndD, ID_BYTES, (SHORT)z, TRUE);
p = 0;
for( r=0;r<strlen(datFile);r++ )
   {
   if( datFile[r] == '\\' )
   p = r;
   }
strncat(szModName, datFile+(p+1), strlen(datFile) - (p+1));
strcat(szModName, "       ");
itoa(FIXEDLEN*.001,flr,10);
strcat(szModName, flr);
strcat(szModName, "k");
WinSetDlgItemText(hwndD, FID_TITLEBAR, szModName);

WinPostMsg(hwndD, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);

m = 1;
iz = 0;
strcpy(xtext, schText);
if( !CASESENSITIVE )
   {
   CASE = 0;
   strupr(schText);
   }
else
   CASE = 1;
// WinPostMsg(hwndD, WM_COMMAND, (MPARAM)ID_SEARCHV, 0);

// WinSendMsg(WinWindowFromID(hwndD, ID_ENTRY4),
//	   MLM_SETSEL,
//	   MPFROMP(0),
//	   MPFROMP(0));
}
*/



VOID loadNewR1RecV(HWND hwndM, HWND hwndD)
{
INT p, r;
CHAR flr[6];
CHAR szModName[75] = "View Record       Module : ";
INT z;


WinSendDlgItemMsg(hwndD, ID_ENTRY4,MLM_SETTEXTLIMIT,
		  MPFROM2SHORT(FIXEDLEN,0),NULL);
WinSetWindowText(WinWindowFromID(hwndD, ID_ENTRY4),
		 dataRecs.noteText);
WinSetWindowText(WinWindowFromID(hwndD, ID_RECNAME4),
		 dataRecs.noteName);
z = (FIXEDLEN - 1) - (strlen(dataRecs.noteText) * sizeof(CHAR));
WinSetDlgItemShort(hwndD, ID_BYTES, (SHORT)z, TRUE);
WinSendMsg(WinWindowFromID(hwndD, ID_ENTRY4),
	   MLM_SETSEL,
	   MPFROMP(0),
	   MPFROMP(0));
p = 0;
for( r=0;r<strlen(datFile);r++ )
   {
   if( datFile[r] == '\\' )
      p = r;
   }
strncat(szModName, datFile+(p+1), strlen(datFile) - (p+1));
strcat(szModName, "       ");
itoa(FIXEDLEN*.001,flr,10);
strcat(szModName, flr);
strcat(szModName, "k");
WinSetDlgItemText(hwndD, FID_TITLEBAR, szModName);
WinPostMsg(hwndD, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
if(ESEARCH)
   {
   m = 1;
   iz = 0;
   strcpy(xtext, schText);
   if( !CASESENSITIVE )
      {
      CASE = 0;
      strupr(schText);
      }
   else
      CASE = 1;
   WinPostMsg(hwndD, WM_COMMAND, (MPARAM)ID_SEARCHV, 0);
   }
}




VOID loadNewR1RecE(HWND hwndM, HWND hwndD)
{
INT p, r;
CHAR flr[6];
CHAR szModName[75] = "Edit Record       Module : ";
INT z;


WinSendDlgItemMsg(hwndD, ID_ENTRY3,MLM_SETTEXTLIMIT,
		  MPFROM2SHORT(FIXEDLEN,0),NULL);
WinSetWindowText(WinWindowFromID(hwndD, ID_ENTRY3),
		 dataRecs.noteText);
WinSetWindowText(WinWindowFromID(hwndD, ID_RECNAME3),
		 dataRecs.noteName);
z = (FIXEDLEN - 1) - (strlen(dataRecs.noteText) * sizeof(CHAR));
WinSetDlgItemShort(hwndD, ID_BYTES, (SHORT)z, TRUE);
WinSendMsg(WinWindowFromID(hwndD, ID_ENTRY3),
	   MLM_SETSEL,
	   MPFROMP(0),
	   MPFROMP(0));
p = 0;
for( r=0;r<strlen(datFile);r++ )
   {
   if( datFile[r] == '\\' )
      p = r;
   }
strncat(szModName, datFile+(p+1), strlen(datFile) - (p+1));
strcat(szModName, "       ");
itoa(FIXEDLEN*.001,flr,10);
strcat(szModName, flr);
strcat(szModName, "k");
WinSetDlgItemText(hwndD, FID_TITLEBAR, szModName);
WinPostMsg(hwndD, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);

if(ESEARCH || NSEARCH)
   {
   m = 1;
   iz = 0;
   strcpy(xtext, schText);
   if( !CASESENSITIVE )
      {
      CASE = 0;
      strupr(schText);
      }
   else
      CASE = 1;
   WinPostMsg(hwndD, WM_COMMAND, (MPARAM)ID_SEARCHED, 0);
   // WinEnableMenuItem(hwndEMLEPopup, ID_SAVENOWE, FALSE);
   }
}






MRESULT EXPENTRY results1Proc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
HWND hwndM;
ULONG pointerPos;
FILE *handM;
INT iw, rnum, r, x, cc, i;
INT choi, tot, seln;
CHAR cText[NAMESIZE];
BOOL ddone;
PFNWP oldResult1Proc;
INT origDOMode;
struct Samp
   {
   SHORT indx;
   CHAR noteName[NAMESIZE];
   }sample;
ULONG oneRecSize;
PSWP pOldSize;
PSWP pNewSize;
SWP ctrlpos, ctrlpos2, ctrlpos3;


pNewSize = (PSWP)mp1;
pOldSize = pNewSize + 1;
switch ( msg )
   {
   case WM_INITDLG:
      origDOMode = pgmData[12];
      hwndResults = WinWindowFromID(hwndDlg, ID_FINDLIST1);
      hwndRFrame = WinQueryWindow(hwndResults, QW_OWNER);
      hwndRes = hwndDlg;
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_FINDLIST1), FALSE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_VIEWF1), FALSE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_EDITF1), FALSE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_PRINTF1), FALSE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_SAVEALL), FALSE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_PRINTALLF1), FALSE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_QUITF1), FALSE);
      WinSetWindowPos(hwndDlg, HWND_TOP,
		      pgmData[40], pgmData[41], pgmData[38], pgmData[39],
		      SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ACTIVATE | SWP_ZORDER);
      DosPostEventSem(hev);
      oldResult1Proc = WinSubclassWindow(hwndResults, newResult1Proc);
      WinSetWindowPtr(hwndResults, 0, (PVOID) oldResult1Proc);
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_QUERYTRACKINFO:
      {
      MRESULT mr;
      TRACKINFO *pti;

      mr = WinDefDlgProc(hwndDlg, msg, mp1, mp2);
      pti = (TRACKINFO *)mp2;
      switch(scrWid)
	 {
	 case 640 :
	    pti->ptlMinTrackSize.x = scrWid * .269;
	    pti->ptlMinTrackSize.y = scrLen * .262;
	    break;

	 case 800 :
	    pti->ptlMinTrackSize.x = scrWid * .175;
	    pti->ptlMinTrackSize.y = scrLen * .193;
	    break;

	 default :
	    pti->ptlMinTrackSize.x = scrWid * .319;
	    pti->ptlMinTrackSize.y = scrLen * .283;
	    break;
	 }
      return(MRESULT)mr;
      }

   case WM_WINDOWPOSCHANGED:
      {
      if ((pNewSize->fl & SWP_SIZE))
	 {
	 ULONG hei, wid;
	 ULONG up, over;

	 WinQueryWindowPos(hwndDlg, &ctrlpos);
	 WinQueryWindowPos(WinWindowFromID(hwndDlg, ID_FINDLIST1), &ctrlpos2);
	 WinQueryWindowPos(WinWindowFromID(hwndDlg, ID_GBOX1), &ctrlpos3);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_FINDLIST1), FALSE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_GBOX1), FALSE);


	 hei = ctrlpos.cy - (ctrlpos3.y*2.8);
	 wid = ctrlpos.cx - (ctrlpos3.x*1.9);
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_GBOX1), (HWND)0,
			 0, 0, wid, hei, SWP_SIZE);

	 hei = (ctrlpos.cy-(ctrlpos2.y+(ctrlpos3.y*3.2)));
	 wid = (ctrlpos.cx-(ctrlpos2.x*1.9));
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_FINDLIST1), (HWND)0,
			 0, 0, wid, hei, SWP_SIZE);

	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_FINDLIST1), TRUE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_GBOX1), TRUE);
	 }
      return(WinDefDlgProc(hwndDlg, msg, mp1, mp2));
      }


   case WM_ADJUSTWINDOWPOS :
      {
      if( ((PSWP)mp1)->fl & SWP_MINIMIZE )
	 {
	 WinSetWindowPos(hwndFrame, (HWND)0, 0, 0, 0, 0, SWP_MINIMIZE);
	 WinShowWindow(WinQueryWindow(hwndDlg, QW_OWNER), FALSE);
	 }
      else
	 {
	 if( ((PSWP)mp1)->fl & SWP_RESTORE )
	    {
	    WinSetWindowPos(hwndFrame, (HWND)0, 0, 0, 0, 0, SWP_RESTORE);
	    WinShowWindow(WinQueryWindow(hwndDlg, QW_OWNER), TRUE);
	    }
	 }
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
      }
      // break;


   case WM_CONTROL:
      if (SHORT2FROMMP(mp1) == LN_ENTER )
	 {
	 switch (LOUSHORT(mp1))
	    {
	    case ID_FINDLIST1:
	       if( pgmData[12] )
		  WinSendMsg(hwndDlg, WM_COMMAND,  MPFROMSHORT(ID_EDITF1), 0);
	       else
		  WinSendMsg(hwndDlg, WM_COMMAND,  MPFROMSHORT(ID_VIEWF1), 0);
	       break;
	    }
	 }
      break;

   case WM_DESTROY:
      if( WinQueryWindowULong(hwndDlg, QWL_STYLE) & WS_MAXIMIZED )
	 {
	 pgmData[38] = WinQueryWindowULong(hwndDlg, QWS_CXRESTORE);
	 pgmData[39] = WinQueryWindowULong(hwndDlg, QWS_CYRESTORE);
	 pgmData[40] = WinQueryWindowULong(hwndDlg, QWS_XRESTORE);
	 pgmData[41] = WinQueryWindowULong(hwndDlg, QWS_YRESTORE);
	 }
      else
	 {
	 if( !(WinQueryWindowULong(hwndDlg, QWL_STYLE) & WS_MINIMIZED) )
	    {
	    WinQueryWindowPos(hwndDlg, &ctrlpos);
	    pgmData[38] = ctrlpos.cx;
	    pgmData[39] = ctrlpos.cy;
	    pgmData[40] = ctrlpos.x;
	    pgmData[41] = ctrlpos.y;
	    }
	 }
      pgmData[12] = origDOMode;
      break;

   case ID_SEARCHOVER:
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_FINDLIST1), TRUE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_VIEWF1), TRUE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_EDITF1), TRUE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_SAVEALL), TRUE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_PRINTF1), TRUE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_PRINTALLF1), TRUE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_QUITF1), TRUE);
      if( (LONG)WinSendMsg(hwndResults, LM_QUERYITEMCOUNT, 0, 0) == 0 )
	 {
	 WinMessageBox(HWND_DESKTOP,
		       hwndDlg,
		       "No matches found!",
		       "Attention ...",
		       0,
		       MB_INFORMATION | MB_OK);
	 CONT = 0;
	 WinDismissDlg( hwndDlg, FALSE );
	 }
      break;

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case ID_SETNUMHITS :
	    {
	    CHAR hits[10];
	    CHAR sResults[80];
	    INT tnumMatch;

	    tnumMatch = WinQueryLboxCount(hwndResults);
	    itoa(tnumMatch, hits, 10);
	    strcpy(sResults, "Number of records found : ");
	    strcat(sResults, hits);
	    if( tnumMatch > 0 )
	       WinSetDlgItemText(WinQueryWindow(hwndResults, QW_OWNER), FID_TITLEBAR, sResults);
	    }
	    break;

	 case ID_LOAD1 :
	    if( (LONG)WinSendMsg(hwndResults, LM_QUERYITEMCOUNT, 0, 0) == 0 )
	       return(0);
	    ddone = FALSE;
	    r = 0;
	    x = 0;
	    tot = 0;
	    seln = -1;
	    while( strcmp(pdf[x].noteText, "") != 0 && x < MAXNUMRECS )
	       x++;
	    choi = SHORT1FROMMR(WinSendMsg(hwndResults,
					   LM_QUERYSELECTION,
					   MPFROMSHORT(NULL), NULL));
	    do
	       {
	       tot += qFactor[r];
	       if( (choi + 1) <= tot )
		  {
		  seln = r;
		  ddone = TRUE;
		  }
	       r++;
	       }
	    while( !ddone || (r > x) );
	    WinSendMsg(hwndResults, LM_QUERYITEMTEXT,
		       MPFROM2SHORT(choi, sizeof(cText)),
		       MPFROMP(cText));
	    pointerPos = sizeof(recIndex);
	    strcpy(datFile, pdf[seln].noteText);
	    handM = fopen(pdf[seln].noteText, MRO);
	    fseek(handM, 0L, SEEK_SET);
	    fread(&recIndex, sizeof(recIndex), 1, handM);
	    if( recIndex[0].alIndex > 0 )
	       FIXEDLEN = recIndex[0].alIndex;
	    else
	       {
	       recIndex[0].alIndex = NOTESIZE;
	       FIXEDLEN = recIndex[0].alIndex;
	       }
	    oneRecSize = sizeof(sample) + FIXEDLEN;
	    rnum = compIndexName(hwndM, cText);
	    fseek(handM, pointerPos + (oneRecSize * rnum), SEEK_SET);
	    fread(&sample, sizeof(sample), 1, handM);
	    fread(&dataRecs.noteText, FIXEDLEN, 1, handM);
	    dataRecs.indx = sample.indx;
	    strcpy(dataRecs.noteName, sample.noteName);
	    fclose(handM);
	    break;

	 case ID_VIEWF1 :
	    {
	    HAB hab;

	    if( WinIsWindow(hab, hwndCurEViewRecord) )
	       return(0);
	    if( (LONG)WinSendMsg(hwndResults, LM_QUERYITEMCOUNT, 0, 0) == 0 )
	       return(0);
	    ddone = FALSE;
	    r = 0;
	    x = 0;
	    tot = 0;
	    seln = -1;
	    while( strcmp(pdf[x].noteText, "") != 0 && x < MAXNUMRECS )
	       x++;
	    choi = SHORT1FROMMR(WinSendMsg(hwndResults,
					   LM_QUERYSELECTION,
					   MPFROMSHORT(NULL), NULL));
	    do
	       {
	       tot += qFactor[r];
	       if( (choi + 1) <= tot )
		  {
		  seln = r;
		  ddone = TRUE;
		  }
	       r++;
	       }
	    while( !ddone || (r > x) );
	    WinSendMsg(hwndResults, LM_QUERYITEMTEXT,
		       MPFROM2SHORT(choi, sizeof(cText)),
		       MPFROMP(cText));
	    pointerPos = sizeof(recIndex);
	    strcpy(datFile, pdf[seln].noteText);

	    handM = fopen(pdf[seln].noteText, MRO);
	    fseek(handM, 0L, SEEK_SET);
	    fread(&recIndex, sizeof(recIndex), 1, handM);
	    if( recIndex[0].alIndex > 0 )
	       FIXEDLEN = recIndex[0].alIndex;
	    else
	       {
	       recIndex[0].alIndex = NOTESIZE;
	       FIXEDLEN = recIndex[0].alIndex;
	       }
	    oneRecSize = sizeof(sample) + FIXEDLEN;
	    rnum = compIndexName(hwndM, cText);
	    fseek(handM, pointerPos + (oneRecSize * rnum), SEEK_SET);
	    fread(&sample, sizeof(sample), 1, handM);
	    fread(&dataRecs.noteText, FIXEDLEN, 1, handM);
	    dataRecs.indx = sample.indx;
	    strcpy(dataRecs.noteName, sample.noteName);
	    fclose(handM);
	    removeSwEntry(WinQueryWindow(hwndDlg, QW_OWNER));
	    ESEARCH = TRUE;


	    if( WinIsWindow(hab, hwndCurViewRecord) )
	       {
	       ESEARCH = TRUE;
	       loadNewR1RecV(hwndDlg, hwndCurViewRecord);
	       WinSetFocus(HWND_DESKTOP,
			   WinWindowFromID(hwndCurViewRecord, ID_ENTRY4));
	       }
	    else
	       {
	       removeSwEntry(hwndCurViewRecord);
			      viewRec(hwndDlg);
	       WinSetFocus(HWND_DESKTOP, hwndResults);
	       WinSendMsg(WinWindowFromID(hwndDlg, ID_VIEWF1),
				   BM_SETDEFAULT, 0, 0);
	       ESEARCH = FALSE;
	       addSwEntry(hwndDlg);
	       }
	    addSwEntry(WinQueryWindow(hwndDlg, QW_OWNER));
	    }
	    break;

	 case ID_EDITF1 :
	    {
	    HAB hab;


	    if( WinIsWindow(hab, hwndCurViewRecord) )
	       return(0);
	    if( (LONG)WinSendMsg(hwndResults, LM_QUERYITEMCOUNT, 0, 0) == 0 )
	       return(0);
	    choi = SHORT1FROMMR(WinSendMsg(hwndResults,
					   LM_QUERYSELECTION,
					   MPFROMSHORT(NULL), NULL));
	    WinSendMsg(hwndResults, LM_QUERYITEMTEXT,
		       MPFROM2SHORT(choi, sizeof(oldRecName)),
		       MPFROMP(oldRecName));
	    if( nChox == choi && strcmp(datFile, datFilex) == 0 )
	       {
	       }
	    else
	       {
	       ddone = FALSE;
	       r = 0;
	       x = 0;
	       tot = 0;
	       seln = -1;
	       while( strcmp(pdf[x].noteText, "") != 0 && x < MAXNUMRECS )
		  x++;
	       choi = SHORT1FROMMR(WinSendMsg(hwndResults,
					   LM_QUERYSELECTION,
					   MPFROMSHORT(NULL), NULL));
	       do
		  {
		  tot += qFactor[r];
		  if( (choi + 1) <= tot )
		     {
		     seln = r;
		     ddone = TRUE;
		     }
		  r++;
		  }
	       while( !ddone || (r > x) );
	       WinSendMsg(hwndResults, LM_QUERYITEMTEXT,
			  MPFROM2SHORT(choi, sizeof(cText)),
			  MPFROMP(cText));
	       pointerPos = sizeof(recIndex);
	       strcpy(datFile, pdf[seln].noteText);
	       handM = fopen(pdf[seln].noteText, MRO);
	       fseek(handM, 0L, SEEK_SET);
	       fread(&recIndex, sizeof(recIndex), 1, handM);
	       if( recIndex[0].alIndex > 0 )
		  FIXEDLEN = recIndex[0].alIndex;
	       else
		  {
		  recIndex[0].alIndex = NOTESIZE;
		  FIXEDLEN = recIndex[0].alIndex;
		  }
	       oneRecSize = sizeof(sample) + FIXEDLEN;
	       rnum = compIndexName(hwndM, cText);
	       fseek(handM, pointerPos + (oneRecSize * rnum), SEEK_SET);
	       fread(&sample, sizeof(sample), 1, handM);
	       fread(&dataRecs.noteText, FIXEDLEN, 1, handM);
	       dataRecs.indx = sample.indx;
	       strcpy(dataRecs.noteName, sample.noteName);
	       fclose(handM);
	       nChox = choi;
	       strcpy(datFilex, datFile);

	       if( WinIsWindow(hab, hwndCurEViewRecord) )
		  {
		  ESEARCH = TRUE;
		  loadNewR1RecE(hwndDlg, hwndCurEViewRecord);
		  }
	       else
		  {
		  INT nchoi;


		  removeSwEntry(WinQueryWindow(hwndDlg, QW_OWNER));
		  ESEARCH = TRUE;
		  editRec(hwndDlg);
		  ESEARCH = FALSE;
		  addSwEntry(WinQueryWindow(hwndDlg, QW_OWNER));
		  }
	       }
	    }
	    break;

	 case ID_SAVEALL :
	    if( saveAllF(hwndDlg) )
	       {
	       _beginthread(saveATFileS, 16384,(void *)hwndDlg);
	       }
	    break;

	 case ID_PRINTALLF1 :
	    if( (LONG)WinSendMsg(hwndResults, LM_QUERYITEMCOUNT, 0, 0) == 0 )
	       return(0);
	    if( pgmData[14] )
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
	    {
	    INT b,norc;

	    norc = (LONG)WinSendMsg(hwndResults,
				    LM_QUERYITEMCOUNT, 0, 0);
	    for( b=0;b<norc;b++ )
	       {
	       ddone = FALSE;
	       r = 0;
	       x = 0;
	       tot = 0;
	       seln = -1;
	       while( strcmp(pdf[x].noteText, "") != 0 && x < MAXNUMRECS )
		  x++;
	       choi = b;
	       do
		  {
		  tot += qFactor[r];
		  if( (choi + 1) <= tot )
		     {
		     seln = r;
		     ddone = TRUE;
		     }
		  r++;
		  }
	       while( !ddone || (r > x) );
	       WinSendMsg(hwndResults, LM_QUERYITEMTEXT,
			  MPFROM2SHORT(choi, sizeof(cText)),
			  MPFROMP(cText));
	       pointerPos = sizeof(recIndex);
	       strcpy(datFile, pdf[seln].noteText);
	       handM = fopen(pdf[seln].noteText, MRO);
	       fseek(handM, 0L, SEEK_SET);
	       fread(&recIndex, sizeof(recIndex), 1, handM);
	       if( recIndex[0].alIndex > 0 )
		  FIXEDLEN = recIndex[0].alIndex;
	       else
		  {
		  recIndex[0].alIndex = NOTESIZE;
		  FIXEDLEN = recIndex[0].alIndex;
		  }
	       oneRecSize = sizeof(sample) + FIXEDLEN;
	       rnum = compIndexName(hwndM, cText);
	       fseek(handM, pointerPos + (oneRecSize * rnum), SEEK_SET);
	       fread(&sample, sizeof(sample), 1, handM);
	       fread(&dataRecs.noteText, FIXEDLEN, 1, handM);
	       dataRecs.indx = sample.indx;
	       strcpy(dataRecs.noteName, sample.noteName);
	       fclose(handM);
	       printRec(hwndDlg, FALSE);
	       }
	    DosBeep(300, 50);
	    DosBeep(1000, 70);
	    DosBeep(1500, 90);
	    DosBeep(2200, 130);
	    }
	    break;

	 case ID_STOPSEARCH :
	    CONT = 0;
	    WinEnableWindow(WinWindowFromID(hwndDlg, ID_FINDLIST1), TRUE);
	    WinEnableWindow(WinWindowFromID(hwndDlg, ID_VIEWF1), TRUE);
	    WinEnableWindow(WinWindowFromID(hwndDlg, ID_EDITF1), TRUE);
	    WinEnableWindow(WinWindowFromID(hwndDlg, ID_PRINTF1), TRUE);
	    WinEnableWindow(WinWindowFromID(hwndDlg, ID_PRINTALLF1), TRUE);
	    WinEnableWindow(WinWindowFromID(hwndDlg, ID_QUITF1), TRUE);
	    break;

	 case ID_NEXTV :
	    i = WinQueryLboxCount(hwndResults);
	    cc = (USHORT) WinSendMsg(hwndResults,
				     LM_QUERYSELECTION, 0L, 0L);
	    if( cc == i - 1 )
	       cc = 0;
	    else
	       cc += 1;
	    WinSendMsg(hwndResults,
		       LM_SELECTITEM,
		       MPFROMSHORT(cc),
		       MPFROMSHORT(TRUE));
	    WinSendMsg(hwndDlg,
		       WM_COMMAND,
		       MPFROM2SHORT(ID_VIEWF1, 0),
		       MPFROMP(0));
	    break;

	 case ID_LASTV :
	    i = WinQueryLboxCount(hwndResults);
	    cc = (USHORT) WinSendMsg(hwndResults,
				     LM_QUERYSELECTION, 0L, 0L);
	    if( cc == 0 )
	       cc = i - 1;
	    else
	       cc -= 1;
	    WinSendMsg(hwndResults,
		       LM_SELECTITEM,
		       MPFROMSHORT(cc),
		       MPFROMSHORT(TRUE));
	    WinSendMsg(hwndDlg,
		       WM_COMMAND,
		       MPFROM2SHORT(ID_VIEWF1, 0),
		       MPFROMP(0));
	    break;


	 case ID_QUITF1 :
	    {
	    HAB hab;


	    if( WinIsWindow(hab, hwndCurEViewRecord) )
	       WinSendMsg(hwndCurEViewRecord, WM_CLOSE, 0, 0);
	    if( WinIsWindow(hab, hwndCurViewRecord) )
	       WinSendMsg(hwndCurViewRecord, WM_CLOSE, 0, 0);
	    strcpy(datFile, pdf[0].noteText);
	    CONT = 0;
	    for( iw=1;iw<NOTMPFILES;iw++ )
	       {
	       CHAR sd[3];
	       CHAR sFullDocName[MAXNAME];

	       strcpy(sFullDocName, basePath);
	       if( sFullDocName[strlen(sFullDocName)-1] != '\\' )
		  strcat(sFullDocName, "\\");
	       strcat(sFullDocName, "TEMP");
	       itoa(iw, sd, 10);
	       strcat(sFullDocName, sd);
	       strcat(sFullDocName, ".TXT");
	       remove(sFullDocName);
	       }
	    WinDismissDlg( hwndDlg, FALSE );
	    initLoad_index(hwndMLBox, datFile);
	    WinPostMsg(hwndMLBox,
		       LM_SELECTITEM,
		       MPFROMSHORT(0),
		       MPFROMSHORT(TRUE));
	    }
	    break;

	 default :
	    return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
	    // break;
	 }
      break;

   default :
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
      // break;
   }
return (MRESULT) FALSE;
}





MRESULT EXPENTRY results2Proc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
HWND hwndM;
ULONG pointerPos;
FILE *handM;
INT iw, rnum, r, x, i, cc;
INT choi, tot, seln;
CHAR cText[NAMESIZE];
BOOL ddone;
PFNWP oldResult2Proc;
struct Samp2
   {
   SHORT indx;
   CHAR noteName[NAMESIZE];
   }sample;
ULONG oneRecSize;
INT origDOMode;
PSWP pOldSize;
PSWP pNewSize;
SWP ctrlpos, ctrlpos2, ctrlpos3;


pNewSize = (PSWP)mp1;
pOldSize = pNewSize + 1;
switch ( msg )
   {
   case WM_INITDLG:
      origDOMode = pgmData[12];
      hwndResults = WinWindowFromID(hwndDlg, ID_FINDLIST1);
      hwndRFrame = WinQueryWindow(hwndResults, QW_OWNER);
      hwndResn = hwndDlg;
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_FINDLIST1), FALSE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_VIEWF1), FALSE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_EDITF1), FALSE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_PRINTF1), FALSE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_PRINTALLF1), FALSE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_QUITF1), FALSE);
      oldResult2Proc = WinSubclassWindow(hwndResults, newResult2Proc);
      WinSetWindowPtr(hwndResults, 0, (PVOID) oldResult2Proc);
      WinSetWindowPos(hwndDlg, HWND_TOP,
		      pgmData[40], pgmData[41], pgmData[38], pgmData[39],
		      SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ACTIVATE | SWP_ZORDER);
      DosPostEventSem(hev);
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_QUERYTRACKINFO:
      {
      MRESULT mr;
      TRACKINFO *pti;

      mr = WinDefDlgProc(hwndDlg, msg, mp1, mp2);
      pti = (TRACKINFO *)mp2;
      switch(scrWid)
	 {
	 case 640 :
	    pti->ptlMinTrackSize.x = scrWid * .269;
	    pti->ptlMinTrackSize.y = scrLen * .262;
	    break;

	 case 800 :
	    pti->ptlMinTrackSize.x = scrWid * .175;
	    pti->ptlMinTrackSize.y = scrLen * .193;
	    break;

	 default :
	    pti->ptlMinTrackSize.x = scrWid * .319;
	    pti->ptlMinTrackSize.y = scrLen * .283;
	    break;
	 }
      return(MRESULT)mr;
      }

   case WM_WINDOWPOSCHANGED:
      {
      if ((pNewSize->fl & SWP_SIZE))
	 {
	 ULONG hei, wid;
	 ULONG up, over;

	 // DosBeep(100, 100);
	 WinQueryWindowPos(hwndDlg, &ctrlpos);
	 WinQueryWindowPos(WinWindowFromID(hwndDlg, ID_FINDLIST1), &ctrlpos2);
	 WinQueryWindowPos(WinWindowFromID(hwndDlg, ID_GBOX1), &ctrlpos3);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_FINDLIST1), FALSE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_GBOX1), FALSE);


	 hei = ctrlpos.cy - (ctrlpos3.y*2.8);
	 wid = ctrlpos.cx - (ctrlpos3.x*1.9);
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_GBOX1), (HWND)0,
			 0, 0, wid, hei, SWP_SIZE);

	 hei = (ctrlpos.cy-(ctrlpos2.y+(ctrlpos3.y*3.2)));
	 wid = (ctrlpos.cx-(ctrlpos2.x*1.9));
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_FINDLIST1), (HWND)0,
			 0, 0, wid, hei, SWP_SIZE);

	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_FINDLIST1), TRUE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_GBOX1), TRUE);
	 }
      return(WinDefDlgProc(hwndDlg, msg, mp1, mp2));
      }


   case WM_ADJUSTWINDOWPOS :
      {
      if( ((PSWP)mp1)->fl & SWP_MINIMIZE )
	 {
	 WinSetWindowPos(hwndFrame, (HWND)0, 0, 0, 0, 0, SWP_MINIMIZE);
	 WinShowWindow(WinQueryWindow(hwndDlg, QW_OWNER), FALSE);
	 }
      else
	 {
	 if( ((PSWP)mp1)->fl & SWP_RESTORE )
	    {
	    WinSetWindowPos(hwndFrame, (HWND)0, 0, 0, 0, 0, SWP_RESTORE);
	    WinShowWindow(WinQueryWindow(hwndDlg, QW_OWNER), TRUE);
	    }
	 }
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
      }
      // break;


   case WM_DESTROY:
      if( WinQueryWindowULong(hwndDlg, QWL_STYLE) & WS_MAXIMIZED )
	 {
	 pgmData[38] = WinQueryWindowULong(hwndDlg, QWS_CXRESTORE);
	 pgmData[39] = WinQueryWindowULong(hwndDlg, QWS_CYRESTORE);
	 pgmData[40] = WinQueryWindowULong(hwndDlg, QWS_XRESTORE);
	 pgmData[41] = WinQueryWindowULong(hwndDlg, QWS_YRESTORE);
	 }
      else
	 {
	 if( !(WinQueryWindowULong(hwndDlg, QWL_STYLE) & WS_MINIMIZED) )
	    {
	    WinQueryWindowPos(hwndDlg, &ctrlpos);
	    pgmData[38] = ctrlpos.cx;
	    pgmData[39] = ctrlpos.cy;
	    pgmData[40] = ctrlpos.x;
	    pgmData[41] = ctrlpos.y;
	    }
	 }
      pgmData[12] = origDOMode;
      break;

   case WM_CONTROL:
      if (SHORT2FROMMP(mp1) == LN_ENTER )
	 {
	 switch (LOUSHORT(mp1))
	    {
	    case ID_FINDLIST1:
	       if( pgmData[12] )
		  WinSendMsg(hwndDlg, WM_COMMAND,  MPFROMSHORT(ID_EDITF1), 0);
	       else
		  WinSendMsg(hwndDlg, WM_COMMAND,  MPFROMSHORT(ID_VIEWF1), 0);
	       break;
	    }
	 }
      break;

   case ID_SEARCHOVER:
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_FINDLIST1), TRUE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_VIEWF1), TRUE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_EDITF1), TRUE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_PRINTF1), TRUE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_PRINTALLF1), TRUE);
      WinEnableWindow(WinWindowFromID(hwndDlg, ID_QUITF1), TRUE);
      if( (LONG)WinSendMsg(hwndResults, LM_QUERYITEMCOUNT, 0, 0) == 0 )
	 {
	 WinMessageBox(HWND_DESKTOP,
		       hwndDlg,
		       "No matches found!",
		       "Attention ...",
		       0,
		       MB_INFORMATION | MB_OK);
	 CONT = 0;
	 WinDismissDlg( hwndDlg, FALSE );
	 }
      break;

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case ID_SETNUMHITS :
	    {
	    CHAR hits[10];
	    CHAR sResults[80];
	    INT tnumMatch;

	    tnumMatch = WinQueryLboxCount(hwndResults);
	    itoa(tnumMatch, hits, 10);
	    strcpy(sResults, "Number of records found : ");
	    strcat(sResults, hits);
	    if( tnumMatch > 0 )
	       WinSetDlgItemText(WinQueryWindow(hwndResults, QW_OWNER), FID_TITLEBAR, sResults);
	    }
	    break;

	 case ID_LOAD2 :
	    if( (LONG)WinSendMsg(hwndResults, LM_QUERYITEMCOUNT, 0, 0) == 0 )
	       return(0);
	    ddone = FALSE;
	    r = 0;
	    x = 0;
	    tot = 0;
	    seln = -1;
	    while( strcmp(pdf[x].noteText, "") != 0 && x < MAXNUMRECS )
	       x++;
	    choi = SHORT1FROMMR(WinSendMsg(hwndResults,
					   LM_QUERYSELECTION,
					   MPFROMSHORT(NULL), NULL));
	    do
	       {
	       tot += qFactor[r];
	       if( (choi + 1) <= tot )
		  {
		  seln = r;
		  ddone = TRUE;
		  }
	       r++;
	       }
	    while( !ddone || (r > x) );
	    WinSendMsg(hwndResults, LM_QUERYITEMTEXT,
		       MPFROM2SHORT(choi, sizeof(cText)),
		       MPFROMP(cText));
	    pointerPos = sizeof(recIndex);
	    strcpy(datFile, pdf[seln].noteText);
	    handM = fopen(pdf[seln].noteText, MRO);
	    fseek(handM, 0L, SEEK_SET);
	    fread(&recIndex, sizeof(recIndex), 1, handM);
	    if( recIndex[0].alIndex > 0 )
	       FIXEDLEN = recIndex[0].alIndex;
	    else
	       {
	       recIndex[0].alIndex = NOTESIZE;
	       FIXEDLEN = recIndex[0].alIndex;
	       }
	    oneRecSize = sizeof(sample) + FIXEDLEN;
	    rnum = compIndexName(hwndM, cText);
	    fseek(handM, pointerPos + (oneRecSize * rnum), SEEK_SET);
	    fread(&sample, sizeof(sample), 1, handM);
	    fread(&dataRecs.noteText, FIXEDLEN, 1, handM);
	    dataRecs.indx = sample.indx;
	    strcpy(dataRecs.noteName, sample.noteName);
	    fclose(handM);
	    break;

	 case ID_VIEWF1 :
	    {
	    HAB hab;

	    if( WinIsWindow(hab, hwndCurEViewRecord) )
	       return(0);
	    if( (LONG)WinSendMsg(hwndResults, LM_QUERYITEMCOUNT, 0, 0) == 0 )
	       return(0);
	    ddone = FALSE;
	    r = 0;
	    x = 0;
	    tot = 0;
	    seln = -1;
	    while( strcmp(pdf[x].noteText, "") != 0 && x < MAXNUMRECS )
	       x++;
	    choi = SHORT1FROMMR(WinSendMsg(hwndResults,
					   LM_QUERYSELECTION,
					   MPFROMSHORT(NULL), NULL));
	    do
	       {
	       tot += qFactor[r];
	       if( (choi + 1) <= tot )
		  {
		  seln = r;
		  ddone = TRUE;
		  }
	       r++;
	       }
	    while( !ddone || (r > x) );
	    WinSendMsg(hwndResults, LM_QUERYITEMTEXT,
		       MPFROM2SHORT(choi, sizeof(cText)),
		       MPFROMP(cText));
	    pointerPos = sizeof(recIndex);
	    strcpy(datFile, pdf[seln].noteText);
	    handM = fopen(pdf[seln].noteText, MRO);
	    fseek(handM, 0L, SEEK_SET);
	    fread(&recIndex, sizeof(recIndex), 1, handM);
	    if( recIndex[0].alIndex > 0 )
	       FIXEDLEN = recIndex[0].alIndex;
	    else
	       {
	       recIndex[0].alIndex = NOTESIZE;
	       FIXEDLEN = recIndex[0].alIndex;
	       }
	    oneRecSize = sizeof(sample) + FIXEDLEN;
	    rnum = compIndexName(hwndM, cText);
	    fseek(handM, pointerPos + (oneRecSize * rnum), SEEK_SET);
	    fread(&sample, sizeof(sample), 1, handM);
	    fread(&dataRecs.noteText, FIXEDLEN, 1, handM);
	    dataRecs.indx = sample.indx;
	    strcpy(dataRecs.noteName, sample.noteName);
	    fclose(handM);
	    NSEARCH = TRUE;

	    if( WinIsWindow(hab, hwndCurViewRecord) )
	       {
	       NSEARCH = TRUE;
	       loadNewR1RecV(hwndDlg, hwndCurViewRecord);
	       WinSetFocus(HWND_DESKTOP,
			   WinWindowFromID(hwndCurViewRecord, ID_ENTRY4));
	       }
	    else
	       {
	       removeSwEntry(hwndCurViewRecord);
			      viewRec(hwndDlg);
	       WinSetFocus(HWND_DESKTOP, hwndResults);
	       WinSendMsg(WinWindowFromID(hwndDlg, ID_VIEWF1),
				   BM_SETDEFAULT, 0, 0);
	       NSEARCH = FALSE;
	       addSwEntry(hwndDlg);
	       }
	    addSwEntry(WinQueryWindow(hwndDlg, QW_OWNER));
	    // viewRec(hwndDlg);
	    // NSEARCH = FALSE;
	    }
	    break;

	 case ID_EDITF1 :
	    {
	    HAB hab;


	    if( WinIsWindow(hab, hwndCurViewRecord) )
	       return(0);
	    if( (LONG)WinSendMsg(hwndResults, LM_QUERYITEMCOUNT, 0, 0) == 0 )
	       return(0);
	    ddone = FALSE;
	    r = 0;
	    x = 0;
	    tot = 0;
	    seln = -1;
	    while( strcmp(pdf[x].noteText, "") != 0 && x < MAXNUMRECS )
	       x++;
	    choi = SHORT1FROMMR(WinSendMsg(hwndResults,
					   LM_QUERYSELECTION,
					   MPFROMSHORT(NULL), NULL));
	    if( nChox == choi && strcmp(datFile, datFilex) == 0 )
	       {
	       }
	    else
	       {
	       do
		  {
		  tot += qFactor[r];
		  if( (choi + 1) <= tot )
		     {
		     seln = r;
		     ddone = TRUE;
		     }
		  r++;
		  }
	       while( !ddone || (r > x) );
	       WinSendMsg(hwndResults, LM_QUERYITEMTEXT,
		       MPFROM2SHORT(choi, sizeof(cText)),
		       MPFROMP(cText));
	       pointerPos = sizeof(recIndex);
	       strcpy(datFile, pdf[seln].noteText);
	       handM = fopen(pdf[seln].noteText, MRO);
	       fseek(handM, 0L, SEEK_SET);
	       fread(&recIndex, sizeof(recIndex), 1, handM);
	       if( recIndex[0].alIndex > 0 )
		  FIXEDLEN = recIndex[0].alIndex;
	       else
		  {
		  recIndex[0].alIndex = NOTESIZE;
		  FIXEDLEN = recIndex[0].alIndex;
		  }
	       oneRecSize = sizeof(sample) + FIXEDLEN;
	       rnum = compIndexName(hwndM, cText);
	       fseek(handM, pointerPos + (oneRecSize * rnum), SEEK_SET);
	       fread(&sample, sizeof(sample), 1, handM);
	       fread(&dataRecs.noteText, FIXEDLEN, 1, handM);
	       dataRecs.indx = sample.indx;
	       strcpy(dataRecs.noteName, sample.noteName);
	       fclose(handM);
	       nChox = choi;
	       strcpy(datFilex, datFile);

	       NSEARCH = TRUE;

	       if( WinIsWindow(hab, hwndCurEViewRecord) )
		  {
		  NSEARCH = TRUE;
		  loadNewR1RecE(hwndDlg, hwndCurEViewRecord);
		  }
	       else
		  {
		  INT nchoi;


		  removeSwEntry(WinQueryWindow(hwndDlg, QW_OWNER));
		  NSEARCH = TRUE;
		  editRec(hwndDlg);
		  NSEARCH = FALSE;
		  addSwEntry(WinQueryWindow(hwndDlg, QW_OWNER));
		  }


	       // editRec(hwndDlg);
	       // NSEARCH = FALSE;
	       // WinSetLboxItemText(hwndResults, choi, dataRecs.noteName);
	       }
	    }
	    break;

	 case ID_PRINTALLF1 :
	    if( (LONG)WinSendMsg(hwndResults, LM_QUERYITEMCOUNT, 0, 0) == 0 )
	       return(0);
	    if( pgmData[14] )
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
	    {
	    INT b,norc;

	    norc = (LONG)WinSendMsg(hwndResults,
				    LM_QUERYITEMCOUNT, 0, 0);
	    for( b=0;b<norc;b++ )
	       {
	       ddone = FALSE;
	       r = 0;
	       x = 0;
	       tot = 0;
	       seln = -1;
	       while( strcmp(pdf[x].noteText, "") != 0 && x < MAXNUMRECS )
		  x++;
	       choi = b;
	       do
		  {
		  tot += qFactor[r];
		  if( (choi + 1) <= tot )
		     {
		     seln = r;
		     ddone = TRUE;
		     }
		  r++;
		  }
	       while( !ddone || (r > x) );
	       WinSendMsg(hwndResults, LM_QUERYITEMTEXT,
			  MPFROM2SHORT(choi, sizeof(cText)),
			  MPFROMP(cText));
	       pointerPos = sizeof(recIndex);
	       strcpy(datFile, pdf[seln].noteText);
	       handM = fopen(pdf[seln].noteText, MRO);
	       fseek(handM, 0L, SEEK_SET);
	       fread(&recIndex, sizeof(recIndex), 1, handM);
	       if( recIndex[0].alIndex > 0 )
		  FIXEDLEN = recIndex[0].alIndex;
	       else
		  {
		  recIndex[0].alIndex = NOTESIZE;
		  FIXEDLEN = recIndex[0].alIndex;
		  }
	       oneRecSize = sizeof(sample) + FIXEDLEN;
	       rnum = compIndexName(hwndM, cText);
	       fseek(handM, pointerPos + (oneRecSize * rnum), SEEK_SET);
	       fread(&sample, sizeof(sample), 1, handM);
	       fread(&dataRecs.noteText, FIXEDLEN, 1, handM);
	       dataRecs.indx = sample.indx;
	       strcpy(dataRecs.noteName, sample.noteName);
	       fclose(handM);
	       printRec(hwndDlg, FALSE);
	       }
	    DosBeep(300, 50);
	    DosBeep(1000, 70);
	    DosBeep(1500, 90);
	    DosBeep(2200, 130);
	    }
	    break;

	 case ID_STOPSEARCH :
	    CONT = 0;
	    WinEnableWindow(WinWindowFromID(hwndDlg, ID_FINDLIST1), TRUE);
	    WinEnableWindow(WinWindowFromID(hwndDlg, ID_VIEWF1), TRUE);
	    WinEnableWindow(WinWindowFromID(hwndDlg, ID_EDITF1), TRUE);
	    WinEnableWindow(WinWindowFromID(hwndDlg, ID_PRINTF1), TRUE);
	    WinEnableWindow(WinWindowFromID(hwndDlg, ID_PRINTALLF1), TRUE);
	    WinEnableWindow(WinWindowFromID(hwndDlg, ID_QUITF1), TRUE);
	    break;

	 case ID_QUITF1 :
	    {
	    HAB hab;


	    if( WinIsWindow(hab, hwndCurEViewRecord) )
	       WinSendMsg(hwndCurEViewRecord, WM_CLOSE, 0, 0);
	    if( WinIsWindow(hab, hwndCurViewRecord) )
	       WinSendMsg(hwndCurViewRecord, WM_CLOSE, 0, 0);
	    strcpy(datFile, pdf[0].noteText);
	    CONT = 0;
	    for( iw=1;iw<NOTMPFILES;iw++ )
	       {
	       CHAR sd[3];
	       CHAR sFullDocName[MAXNAME];

	       strcpy(sFullDocName, basePath);
	       if( sFullDocName[strlen(sFullDocName)-1] != '\\' )
		  strcat(sFullDocName, "\\");
	       strcat(sFullDocName, "TEMP");
	       itoa(iw, sd, 10);
	       strcat(sFullDocName, sd);
	       strcat(sFullDocName, ".TXT");
	       remove(sFullDocName);
	       }
	    WinDismissDlg( hwndDlg, FALSE );
	    initLoad_index(hwndMLBox, datFile);
	    WinPostMsg(hwndMLBox,
		       LM_SELECTITEM,
		       MPFROMSHORT(0),
		       MPFROMSHORT(TRUE));
	    }
	    break;

	 case ID_NEXTV :
	    i = WinQueryLboxCount(hwndResults);
	    cc = (USHORT) WinSendMsg(hwndResults,
				     LM_QUERYSELECTION, 0L, 0L);
	    if( cc == i - 1 )
	       cc = 0;
	    else
	       cc += 1;
	    WinSendMsg(hwndResults,
		       LM_SELECTITEM,
		       MPFROMSHORT(cc),
		       MPFROMSHORT(TRUE));
	    WinSendMsg(hwndDlg,
		       WM_COMMAND,
		       MPFROM2SHORT(ID_VIEWF1, 0),
		       MPFROMP(0));
	    break;

	 case ID_LASTV :
	    i = WinQueryLboxCount(hwndResults);
	    cc = (USHORT) WinSendMsg(hwndResults,
				     LM_QUERYSELECTION, 0L, 0L);
	    if( cc == 0 )
	       cc = i - 1;
	    else
	       cc -= 1;
	    WinSendMsg(hwndResults,
		       LM_SELECTITEM,
		       MPFROMSHORT(cc),
		       MPFROMSHORT(TRUE));
	    WinSendMsg(hwndDlg,
		       WM_COMMAND,
		       MPFROM2SHORT(ID_VIEWF1, 0),
		       MPFROMP(0));
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



VOID searchRecs(VOID *hwnd)  /*  search RECS SEQUENTIAL */
{
ULONG pointerPos;
INT cnt, i, i2, x, q;
HAB habT;
HMQ hmqT;
INT CONT = 1;
INT curMod;
FILE *handQ;
struct Samp
   {
   SHORT indx;
   CHAR noteName[NAMESIZE];
   }sample;
ULONG oneRecSize;


SVRET = TRUE;
KILLSSTHRERAD = FALSE;
oneRecSize = sizeof(sample) + FIXEDLEN;
habT = WinInitialize(0);
hmqT = WinCreateMsgQueue(habT, 0);
WinCancelShutdown(hmqT, TRUE);
strcpy(xtext, schText);
x = 0;
while( strcmp(pdf[x].noteText, "") != 0 && x < MAXNUMRECS )
   x++;
subClassWin();
disableButtons();
// WinEnableMenuItem(hwndMenu, ID_MISC , FALSE);
// WinEnableMenuItem(hwndMenu, ID_OPTIONS , FALSE);
// WinEnableMenuItem(hwndMenu, ID_HELPME , FALSE);
// WinEnableMenuItem(hwndMenu, ID_SETMODULES , FALSE);
// WinEnableMenuItem(WinWindowFromID(hwndFrame, FID_SYSMENU), SC_CLOSE , FALSE);
curMod = 0;
gCurMod = 0;
if( CURMODONLY )
   x = 1;
for( q=0;q<x;q++ )
   {
   if( CASE == 0 )
      strupr(schText);
   if( q != 0 )
      {
      curMod++;
      gCurMod = curMod;
      initLoad(WinWindowFromID((HWND)hwnd, ID_LISTBOX1), pdf[q].noteText);
      oneRecSize = sizeof(sample) + FIXEDLEN;
      WinPostMsg(WinWindowFromID((HWND)hwnd, ID_LISTBOX1),
		    LM_SELECTITEM,
		    MPFROMSHORT(0),
		    MPFROMSHORT(TRUE));
      setStatus((HWND)hwnd, pdf[q].noteText);
      }
   handQ = fopen(pdf[q].noteText, MRO);
   cnt = WinQueryLboxCount(WinWindowFromID((HWND)hwnd, ID_LISTBOX1));
   BUSY = 1;
   pointerPos = sizeof(recIndex);
   fseek(handQ, pointerPos, SEEK_SET);
   for( i=0;i<cnt;i++ )
      {
      dataRecs.noteName[0] = '\0';
      dataRecs.noteText[0] = '\0';
      dataRecs.indx = -1;
      fread(&sample, sizeof(sample), 1, handQ);
      fread(&dataRecs.noteText, FIXEDLEN, 1, handQ);
      dataRecs.indx = sample.indx;
      strcpy(dataRecs.noteName, sample.noteName);
      if( CASE == 0 )
	 strupr(dataRecs.noteText);
      copyData(0);
      // msgOK(schText, "Line : 2069");
      if ( (pdest = strstr(dataRecs.noteText, schText)) != NULL )
	 {
	 HAB habw;

	 DosCreateEventSem(NULL, &hevS, 0L, FALSE);
	 removeSwEntry((HWND)hwnd);
	 BUSY = 0;
	 i2 = matchName((HWND)hwnd, dataRecs.noteName);
	 WinPostMsg(WinWindowFromID((HWND)hwnd, ID_LISTBOX1),
		    LM_SELECTITEM,
		    MPFROMSHORT(i2), MPFROMSHORT(TRUE));
	 if( CASE == 0 )
	    {
	    dataRecs.noteName[0] = '\0';
	    dataRecs.noteText[0] = '\0';
	    dataRecs.indx = -1;
	    fseek(handQ, pointerPos + (oneRecSize * i), SEEK_SET);
	    fread(&sample, sizeof(sample), 1, handQ);
	    fread(&dataRecs.noteText, FIXEDLEN, 1, handQ);
	    dataRecs.indx = sample.indx;
	    strcpy(dataRecs.noteName, sample.noteName);
	    }
	 if( WinIsWindow(habw, hwndSearchViewRec) )
	    {
	    // debugMsgInt((INT)pdest, "pdest LINE : 2173");
	    loadNewSRec((HWND)hwnd, (HWND)hwnd);
	    DosWaitEventSem(hevS, -1);
	    DosCloseEventSem(hevS);
	    if( SVRET == FALSE )
	       {
	       // DosBeep(900, 400);
	       CONT = 0;
	       q = x+1;
	       }
	    }
	 else
	    {
	    _beginthread(searchView, 16384,(void *)(HWND)hwnd);
	    DosWaitEventSem(hevS, -1);
	    DosCloseEventSem(hevS);
	    if ( SVRET == TRUE )
	       {
	       BUSY = 1;
	       }
	    else
	       {
	       BUSY = 0;
	       CONT = 0;
	       }
	    }
	 addSwEntry((HWND)hwnd);
	 }
      if( CONT == 0 )
	 break;
      if( KILLSSTHRERAD )
	 {
	 BUSY = 0;
	 CONT == 0;
	 DosCloseEventSem(hevS);
	 fclose(handQ);
	 WinPostMsg(hwndFrame, WM_QUIT, 0, 0);
	 WinDestroyMsgQueue(hmqT);
	 WinTerminate(habT);
	 _endthread();
	 }
      }
   strcpy(schText, xtext);
   BUSY = 0;
   fclose(handQ);
   if( CONT == 0 )
      break;
   }
if( CONT == 1 )
   {
   HAB habz;

   if( WinIsWindow(habz, hwndSearchViewRec) )
      {
      WinEnableWindow(WinWindowFromID(hwndSearchViewRec, ID_CONTINUE), FALSE);
      DosCreateEventSem(NULL, &hevS, 0L, FALSE);
      DosBeep(1200, 100);
      DosWaitEventSem(hevS, -1);
      DosCloseEventSem(hevS);
      }
   else
      {
      WinDlgBox(HWND_DESKTOP,
	       (HWND)hwnd,
	       noMoreRecsProc,
	       (HMODULE)0,
	       ID_NOMORERECS,
	       NULL);
      }
   }
if( CURRENT == TRUE )
   {
   if( curMod > 0 )
      {
      FILE *msHan;
      INT z;

      for( z=x;z<NUMPDFMODS;z++ )
	 pdf[z].noteText[0] = '\0';
      strcpy(datFile, pdf[curMod].noteText);
      strcpy(pdf[curMod].noteText, pdf[0].noteText);
      strcpy(pdf[0].noteText, datFile);
      if ((msHan = fopen( "DATAPAD.DAT", "w+b")) == NULL)
	    exit(0);
      fwrite(&pdf, sizeof(pdf), 1, msHan);
      fclose(msHan);
      }
   }
else
   {
   if( curMod > 0 )
      {
      initLoad(WinWindowFromID((HWND)hwnd, ID_LISTBOX1), pdf[0].noteText);
      WinPostMsg(WinWindowFromID((HWND)hwnd, ID_LISTBOX1),
		    LM_SELECTITEM,
		    MPFROMSHORT(0),
		    MPFROMSHORT(TRUE));
      }
   }
setStatus((HWND)hwnd, datFile);
DosCloseEventSem(hevS);
SVRET = FALSE;
enableButtons();
unSubClassWin();
// WinEnableMenuItem(hwndMenu, ID_OPTIONS , TRUE);
// WinEnableMenuItem(hwndMenu, ID_HELPME , TRUE);
// WinEnableMenuItem(hwndMenu, ID_MISC , TRUE);
// WinEnableMenuItem(hwndMenu, ID_SETMODULES , TRUE);
// WinEnableMenuItem(WinWindowFromID(hwndFrame, FID_SYSMENU), SC_CLOSE , TRUE);
WinDestroyMsgQueue(hmqT);
WinTerminate(habT);
_endthread();
}


INT compIndexName(HWND hwndM, CHAR *sName)
{
INT i;
for( i=0;i<MAXNUMRECS;i++ )
   {
   if( strcmpi(recIndex[i].recName, "") == 0 )
      return(-1);
   if( strcmpi(sName, recIndex[i].recName) == 0 )
      return(i);
   }
return(-1);
}



VOID resultRecs(VOID *hwnd) /*  search RECS ACCUMULATIVE */
{
ULONG pointerPos;
INT cnt, i, i2, x, q;
HAB habT;
HMQ hmqT;
INT curMod;
FILE *handQ;
INT numMatch;
ULONG postcnt;
struct Samp2
   {
   SHORT indx;
   CHAR noteName[NAMESIZE];
   }sample;
ULONG oneRecSize;


oneRecSize = sizeof(sample) + FIXEDLEN;
habT = WinInitialize(0);
hmqT = WinCreateMsgQueue(habT, 0);
WinCancelShutdown(hmqT, TRUE);
DosWaitEventSem(hev, -1);
DosCloseEventSem(hev);
strcpy(xtext, schText);
CONT = -1;
x = 0;
while( strcmp(pdf[x].noteText, "") != 0 && x < MAXNUMRECS)
   x++;
curMod = 0;
gCurMod = 0;
for( q=0;q<NUMPDFMODS;q++ )
   qFactor[q] = 0;
if( CURMODONLY )
   x = 1;
if( CASE == 0 )
   strupr(schText);
for( q=0;q<x;q++ )
   {
   numMatch = 0;
   if( q != 0 )
      {
      curMod++;
      gCurMod = curMod;
      initLoad(WinWindowFromID((HWND)hwnd, ID_LISTBOX1), pdf[q].noteText);
      oneRecSize = sizeof(sample) + FIXEDLEN;
      WinPostMsg(WinWindowFromID((HWND)hwnd, ID_LISTBOX1),
		    LM_SELECTITEM,
		    MPFROMSHORT(0),
		    MPFROMSHORT(TRUE));
      setStatus((HWND)hwnd, pdf[q].noteText);
      }
   handQ = fopen(pdf[q].noteText, MRO);
   cnt = WinQueryLboxCount(WinWindowFromID((HWND)hwnd, ID_LISTBOX1));
   if( cnt > MAXNUMRECS )
      cnt = MAXNUMRECS;
   BUSY = 1;
   pointerPos = sizeof(recIndex);
   fseek(handQ, pointerPos, SEEK_SET);
   for( i=0;i<cnt;i++ )
      {
      fread(&sample, sizeof(sample), 1, handQ);
      fread(&dataRecs.noteText, FIXEDLEN, 1, handQ);
      dataRecs.indx = sample.indx;
      strcpy(dataRecs.noteName, sample.noteName);
      if( CASE == 0 )
	 strupr(dataRecs.noteText);
      copyData(0);
      if ( (pdest = strstr(dataRecs.noteText, schText)) != NULL )
	 {
	 numMatch++;
	 qFactor[q] = numMatch;
	 BUSY = 0;
	 WinSendMsg(hwndResults, LM_INSERTITEM,
		    MPFROMSHORT(LIT_END),
		    MPFROMP(dataRecs.noteName));
	 if( CASE == 0 )
	    {
	    fseek(handQ, pointerPos + (oneRecSize * i), SEEK_SET);
	    fread(&sample, sizeof(sample), 1, handQ);
	    fread(&dataRecs.noteText, FIXEDLEN, 1, handQ);
	    dataRecs.indx = sample.indx;
	    strcpy(dataRecs.noteName, sample.noteName);
	    }
	 }
      if( CONT == 0 )
	 break;
      }
   BUSY = 0;
   fclose(handQ);
   if( CONT == 0 )
      break;
   }
strcpy(schText, xtext);
if( x > 1 )
   initLoad(WinWindowFromID((HWND)hwnd, ID_LISTBOX1), pdf[0].noteText);
oneRecSize = sizeof(sample) + FIXEDLEN;
setStatus((HWND)hwnd, pdf[0].noteText);
WinPostMsg(WinWindowFromID((HWND)hwnd, ID_LISTBOX1),
	   LM_SELECTITEM,
	   MPFROMSHORT(0),
	   MPFROMSHORT(TRUE));
WinPostMsg(hwndResults,
	   LM_SELECTITEM,
	   MPFROMSHORT(0),
	   MPFROMSHORT(TRUE));
WinPostMsg(WinQueryWindow(hwndResults, QW_OWNER), ID_SEARCHOVER, 0, 0);
WinPostMsg(WinQueryWindow(hwndResults, QW_OWNER),
			  WM_COMMAND,
			  MPFROMSHORT(ID_SETNUMHITS),
			  MPFROMSHORT(TRUE));
DosBeep(1200, 100);
WinDestroyMsgQueue(hmqT);
WinTerminate(habT);
_endthread();
}


VOID loadNewSRec(HWND hwndM, HWND hwndD)
{
INT p, r;
CHAR flr[6];
CHAR szModName[75] = "Search Record       Module : ";
INT z, k;
HWND hwndDD;
INT start2;
INT stLen;
CHAR *pTT;


hwndDD = hwndSearchViewRec;
// WinSendMsg(hwndDD, WM_COMMAND, MPFROM2SHORT(ID_RESETS, 0), 0);
// strcpy(globalV, schText);
// strcpy(globalVV, xtext);
WinSendDlgItemMsg(hwndDD, ID_ENTRY9,MLM_SETTEXTLIMIT,
		  MPFROM2SHORT(FIXEDLEN,0),NULL);
WinSetWindowText(WinWindowFromID(hwndDD, ID_ENTRY9),
		 dataRecs.noteText);
WinSetWindowText(WinWindowFromID(hwndDD, ID_RECNAME9),
		 dataRecs.noteName);
z = (FIXEDLEN - 1) - (strlen(dataRecs.noteText) * sizeof(CHAR));
WinSetDlgItemShort(hwndDD, ID_BYTESSV, (SHORT)z, TRUE);

WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwndSearchViewRec, ID_ENTRY9));
k = 0;
// pdest = strstr(dataRecs.noteText, schText);
start2 = pdest - dataRecs.noteText;
stLen = strlen(schText);
pTT = strtok(dataRecs.noteText, "\x0d");
while( (pTT - dataRecs.noteText) <= start2 &&
       (pTT - dataRecs.noteText) >= 0 )
   {
   pTT = strtok(NULL, "\x0d");
   k++;
   }
WinPostMsg(WinWindowFromID(hwndDD, ID_ENTRY9),
	   MLM_SETSEL,
	   MPFROMP(start2 - (k-1)),
	   MPFROMP((start2 - (k-1)) + stLen));
stT2 = (INT)start2;
chkRet();

m2++;
p = 0;
for( r=0;r<strlen(pdf[gCurMod].noteText);r++ )
   {
   if( pdf[gCurMod].noteText[r] == '\\' )
   p = r;
   }
strncat(szModName, pdf[gCurMod].noteText+(p+1), strlen(pdf[gCurMod].noteText) - (p+1));
strcat(szModName, "       ");
itoa(FIXEDLEN*.001,flr,10);
strcat(szModName, flr);
strcat(szModName, "k");
WinSetDlgItemText(hwndDD, FID_TITLEBAR, szModName);
WinPostMsg(hwndDD, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
if( WW )
   {
   setRGBColor(WinWindowFromID(hwndDD, ID_WWTOG), 204, 0, 0);
   WinSetWindowText(WinWindowFromID(hwndDD, ID_WWTOG), "ww ON");
   }
else
   {
   setRGBColor(WinWindowFromID(hwndDD, ID_WWTOG), 0, 0, 0);
   WinSetWindowText(WinWindowFromID(hwndDD, ID_WWTOG), "ww OFF");
   }
// WinSendMsg(hwndDD, WM_COMMAND, MPFROM2SHORT(ID_RESETS, 0), 0);
/* WinSendMsg(WinWindowFromID(hwndDD, ID_ENTRY9),
	   MLM_SETSEL,
	   MPFROMP(0),
	   MPFROMP(0)); */
}




MRESULT EXPENTRY searchViewProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
INT p, r;
CHAR flr[6];
CHAR szModName[75] = "Search Record       Module : ";
INT start2;
INT stLen;
CHAR *pTT;
INT k, z;
SHORT flags, key;
PID pidsv;
SWCNTRL swctlsv;
PFNWP osWinProc;
CHAR temp[NAMESIZE];
INT sLen, zE;
BOOL CONTINUE, FOUND;
IPT stopPoint = 0;
INT cz;
PSWP pOldSize;
PSWP pNewSize;
SWP ctrlpos, ctrlpos2, ctrlpos3;

pNewSize = (PSWP)mp1;
pOldSize = pNewSize + 1;
switch ( msg )
   {
   case WM_INITDLG:
      hwndSearchViewRec = WinQueryWindow(WinWindowFromID(hwndDlg, ID_RECNAME9), QW_OWNER);
      strcpy(globalV, schText);
      strcpy(globalVV, xtext);
      WinSendDlgItemMsg(hwndDlg, ID_RECNAME9,EM_SETTEXTLIMIT,
		  MPFROM2SHORT(NAMESIZE,0),NULL);
      WinSendDlgItemMsg(hwndDlg, ID_ENTRY9,MLM_SETTEXTLIMIT,
		  MPFROM2SHORT(FIXEDLEN,0),NULL);
      WinSendDlgItemMsg(hwndDlg, ID_ENTRY9,MLM_SETWRAP,
		  MPFROM2SHORT(WW,0),NULL);
      WinSetWindowText(WinWindowFromID(hwndDlg, ID_ENTRY9),
		       dataRecs.noteText);
      WinSetWindowText(WinWindowFromID(hwndDlg, ID_RECNAME9),
		       dataRecs.noteName);
      osWinProc = WinSubclassWindow(WinWindowFromID(hwndDlg, ID_ENTRY9),
					   nSearchProc);
      WinSetWindowPtr(WinWindowFromID(hwndDlg, ID_ENTRY9),
					      0, (PVOID) osWinProc);
      WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwndDlg, ID_ENTRY9));
      z = (FIXEDLEN - 1) - (strlen(dataRecs.noteText) * sizeof(CHAR));
      WinSetDlgItemShort(hwndDlg, ID_BYTESSV, (SHORT)z, TRUE);
      k = 0;
      start2 = pdest - dataRecs.noteText;
      stLen = strlen(schText);
      pTT = strtok(dataRecs.noteText, "\x0d");
      while( (pTT - dataRecs.noteText) <= start2 &&
	     (pTT - dataRecs.noteText) >= 0 )
	 {
	 pTT = strtok(NULL, "\x0d");
	 k++;
	 }
      WinPostMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
		 MLM_SETSEL,
		 MPFROMP(start2 - (k-1)),
		 MPFROMP((start2 - (k-1)) + stLen));
      stT2 = (INT)start2;
      chkRet();
      WinQueryWindowProcess(hwndDlg, &pidsv, NULL);
      swctlsv.hwnd = hwndDlg;                /* window handle      */
      swctlsv.hwndIcon = NULLHANDLE;         /* icon handle        */
      swctlsv.hprog = NULLHANDLE;            /* program handle     */
      swctlsv.idProcess = pidsv;               /* process identifier */
      swctlsv.idSession = 0;                 /* session identifier */
      swctlsv.uchVisibility = SWL_VISIBLE;   /* visibility         */
      swctlsv.fbJump = SWL_JUMPABLE;         /* jump indicator     */
      strcpy(swctlsv.szSwtitle, "DataPad Search View Window");
      WinAddSwitchEntry(&swctlsv);
      if( !pgmData[6] )
	 setParams(hwndDlg, 6, ID_ENTRY9, ID_RECNAME9);
      m2++;
      p = 0;
      for( r=0;r<strlen(pdf[gCurMod].noteText);r++ )
	 {
	 if( pdf[gCurMod].noteText[r] == '\\' )
	 p = r;
	 }
      strncat(szModName, pdf[gCurMod].noteText+(p+1), strlen(pdf[gCurMod].noteText) - (p+1));
      strcat(szModName, "       ");
      itoa(FIXEDLEN*.001,flr,10);
      strcat(szModName, flr);
      strcat(szModName, "k");
      WinSetDlgItemText(hwndDlg, FID_TITLEBAR, szModName);
      WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
      hwndSMLEPopup = WinLoadMenu(WinWindowFromID(hwndDlg, ID_CONTINUE),
				  0,
				  ID_SMLEPOPUP);
      WinSetWindowPos(hwndDlg, HWND_TOP,
		      pgmData[25], pgmData[26], pgmData[23], pgmData[24],
		      SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ACTIVATE | SWP_ZORDER);
      if( WW )
	 {
	 setRGBColor(WinWindowFromID(hwndDlg, ID_WWTOG), 204, 0, 0);
	 WinSetWindowText(WinWindowFromID(hwndDlg, ID_WWTOG), "ww ON");
	 }
      else
	 {
	 setRGBColor(WinWindowFromID(hwndDlg, ID_WWTOG), 0, 0, 0);
	 WinSetWindowText(WinWindowFromID(hwndDlg, ID_WWTOG), "ww OFF");
	 }
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_CHAR :
      flags = SHORT1FROMMP(mp1);
      key = SHORT2FROMMP(mp2);
      if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_F2)
	 WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_CONTL), 0);
      if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_F3)
	 WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_CONTLR), 0);
      if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_F4)
	 WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_CLRS), 0);
      if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_F5)
	 WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_CONTINUE), 0);
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_QUERYTRACKINFO:
      {
      MRESULT mr;
      TRACKINFO *pti;
      SWP boxpos;

      mr = WinDefDlgProc(hwndDlg, msg, mp1, mp2);
      pti = (TRACKINFO *)mp2;
      switch(scrWid)
	 {
	 case 640 :
	    pti->ptlMinTrackSize.x = scrWid * .615;
	    pti->ptlMinTrackSize.y = scrLen * .406;
	    break;

	 case 800 :
	    pti->ptlMinTrackSize.x = scrWid * .492;
	    pti->ptlMinTrackSize.y = scrLen * .35;
	    break;

	 default :
	    pti->ptlMinTrackSize.x = scrWid * .51;
	    pti->ptlMinTrackSize.y = scrLen * .333;
	    break;
	 }
      return mr;
      }

   case WM_WINDOWPOSCHANGED:
      if ((pNewSize->fl & SWP_SIZE))
	 {
	 ULONG hei, wid;
	 ULONG up, over;

	 WinQueryWindowPos(hwndDlg, &ctrlpos);
	 WinQueryWindowPos(WinWindowFromID(hwndDlg, ID_ENTRY9), &ctrlpos2);
	 WinQueryWindowPos(WinWindowFromID(hwndDlg, ID_SGBOX1), &ctrlpos3);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_ENTRY9), FALSE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_SGBOX1), FALSE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RECNAME9), FALSE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RNAME), FALSE);

	 hei = ctrlpos.cy - (ctrlpos3.y*3.1);
	 wid = ctrlpos.cx - (ctrlpos3.x*2);
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_SGBOX1), (HWND)0,
			 0, 0, wid, hei, SWP_SIZE);

	 hei = (ctrlpos.cy-(ctrlpos2.y+(ctrlpos3.y*6.4)));
	 wid = (ctrlpos.cx-(ctrlpos2.x*2));
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_ENTRY9), (HWND)0,
			 0, 0, wid, hei, SWP_SIZE);

	 hei = (ctrlpos3.y*1.9);
	 wid = (ctrlpos.cx-(ctrlpos2.x*7));
	 up = (ctrlpos.cy-(5.6*ctrlpos3.y));
	 over = (ctrlpos3.x*9.1);
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_RECNAME9), (HWND)0,
			 over, up, wid, hei, SWP_SIZE | SWP_MOVE);

	 up = (ctrlpos.cy-(5.6*ctrlpos3.y));
	 over = (ctrlpos3.x *2.5);
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_RNAME), (HWND)0,
			 over, up, 0, 0, SWP_MOVE);

	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_ENTRY9), TRUE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_SGBOX1), TRUE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RECNAME9), TRUE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RNAME), TRUE);
	 }
      return (WinDefDlgProc(hwndDlg, msg, mp1, mp2));

   case WM_DESTROY :
      if( WinQueryWindowULong(hwndDlg, QWL_STYLE) & WS_MAXIMIZED )
	 {
	 pgmData[23] = WinQueryWindowULong(hwndDlg, QWS_CXRESTORE);
	 pgmData[24] = WinQueryWindowULong(hwndDlg, QWS_CYRESTORE);
	 pgmData[25] = WinQueryWindowULong(hwndDlg, QWS_XRESTORE);
	 pgmData[26] = WinQueryWindowULong(hwndDlg, QWS_YRESTORE);
	 }
      else
	 {
	 if( !(WinQueryWindowULong(hwndDlg, QWL_STYLE) & WS_MINIMIZED) )
	    {
	    WinQueryWindowPos(hwndDlg, &ctrlpos);
	    pgmData[23] = ctrlpos.cx;
	    pgmData[24] = ctrlpos.cy;
	    pgmData[25] = ctrlpos.x;
	    pgmData[26] = ctrlpos.y;
	    }
	 }
      if( WinQueryWindowULong(hwndDlg, QWL_STYLE) & WS_MINIMIZED )
	 {
	 WinSendMsg(hwndDlg, WM_COMMAND, MPFROM2SHORT(ID_CANCEL9,0), NULL);
	 KILLSSTHRERAD = TRUE;
	 }
      CURRENT = FALSE;
      SVRET = FALSE;
      DosPostEventSem(hevS);
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_ADJUSTWINDOWPOS :
      if( ((PSWP)mp1)->fl & SWP_MINIMIZE )
	 {
	 WinShowWindow(WinQueryWindow(hwndDlg, QW_OWNER), FALSE);
	 }
      else
	 if( ((PSWP)mp1)->fl & SWP_RESTORE )
	 {
	 WinShowWindow(WinQueryWindow(hwndDlg, QW_OWNER), TRUE);
	 }
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_CONTEXTMENU :
      {
      POINTL ptlMouse;

      WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
      WinPopupMenu(HWND_DESKTOP,
		   hwndDlg,
		   hwndSMLEPopup,
		   ptlMouse.x, ptlMouse.y,
		   0,
		   PU_HCONSTRAIN |
		   PU_VCONSTRAIN |
		   PU_KEYBOARD |
		   PU_MOUSEBUTTON2 |
		   PU_MOUSEBUTTON1 );
      }
      break;

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case ID_CONTINUE :
	    strcpy(schText, globalV);
	    strcpy(xtext, globalVV);
	    CURRENT = FALSE;
	    CASE = ORIGCASE;
	    // WinDismissDlg( hwndDlg, TRUE );
	    SVRET = TRUE;
	    DosPostEventSem(hevS);
	    break;

	 case ID_CONTL :
	    if( m2 == 0 )
	       {
	       iz = 0;
	       if( sSearchRec(hwndDlg) == FALSE )
		  {
		  return(0);
		  }
	       strcpy(xtext, schText);
	       if( CASE == 0  )
		  strupr(schText);
	       m2++;
	       }
	    sLen = strlen(schText);
	    min = 0;
	    WinSetFocus(HWND_DESKTOP,
			   WinWindowFromID(hwndDlg, ID_ENTRY9));
	    WinQueryDlgItemText(hwndDlg,
				ID_ENTRY9,
				sizeof(dataR.noteText),
				dataR.noteText);
	    if( CASE == 0  )
	       strupr(dataR.noteText);
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
		       MLM_SETSEL,
		       MPFROMP(-1),
		       MPFROMP(-1));
	    maxSel = LONGFROMMR(WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
				MLM_QUERYSEL,
				(MPARAM) MLFQS_MAXSEL,
				(MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
		       MLM_SETSEL,
		       MPFROMP(maxSel),
		       MPFROMP(maxSel));
	    min = LONGFROMMR (WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY9),
					  MLM_QUERYSEL,
					  (MPARAM) MLFQS_MINSEL,
					  (MPARAM) 0L));
	    if( min > 0 )
	       {
	       for( cz=0;cz<(min);cz++ )
		 {
		 if( dataR.noteText[cz] == '\x0d' )
		    min++;
		 }
	       }
	    if( min >=  FIXEDLEN-(sLen+1) )
	       {
	       DosBeep(1000, 50);
	       DosBeep(1500, 70);
	       DosBeep(2200, 90);
	       DosBeep(3100, 130);
	       m2 = 0;
	       break;
	       }
	    FOUND = FALSE;
	    CONTINUE = TRUE;
	    iz = min;
	    while( iz < (FIXEDLEN - (sLen+1)) && CONTINUE )
	       {
	       if( dataR.noteText[iz] == schText[0] &&
		   dataR.noteText[iz+1] == schText[1] &&
		   dataR.noteText[iz+2] == schText[2])
		  {
		  strncpy(temp, dataR.noteText+iz, sLen);
		  temp[sLen] = '\0';
		  if( strcmpi(temp, schText) == 0 )
		     {
		     DosBeep(3600, 50);
		     chkRet();
		     zE = 0;
		     while( eols[zE] <= iz )
			zE++;
		     WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
				MLM_SETSEL,
				MPFROMP(iz - zE),
				MPFROMP( (iz - zE) + sLen ));
		     FOUND = TRUE;
		     CONTINUE = FALSE;
		     }
		  else
		     FOUND = FALSE;
		  }
	       iz++;
	       }
	    if( iz > (strlen(dataR.noteText) - (sLen-2))  )
	       FOUND = FALSE;
	    if( !FOUND )
	       WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_NMMATCHESS), 0);
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_CONTLR :
	    if( m2 == 0 )
	       {
	       iz = 0;
	       if( sSearchRec(hwndDlg) == FALSE )
		  {
		  return(0);
		  }
	       strcpy(xtext, schText);
	       if( CASE == 0 )
		  strupr(schText);
	       m2++;
	       }
	    sLen = strlen(schText);
	    min = 0;
	    WinSetFocus(HWND_DESKTOP,
			   WinWindowFromID(hwndDlg, ID_ENTRY9));
	    WinQueryDlgItemText(hwndDlg,
				ID_ENTRY9,
				sizeof(dataR.noteText),
				dataR.noteText);
	    if( CASE == 0 )
	       strupr(dataR.noteText);
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
		       MLM_SETSEL,
		       MPFROMP(-1),
		       MPFROMP(-1));
	    maxSel = LONGFROMMR(WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
				MLM_QUERYSEL,
				(MPARAM) MLFQS_MAXSEL,
				(MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
		       MLM_SETSEL,
		       MPFROMP(maxSel),
		       MPFROMP(maxSel));
	    min = LONGFROMMR (WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY9),
					  MLM_QUERYSEL,
					  (MPARAM) MLFQS_MINSEL,
					  (MPARAM) 0L));
	    if( min > 0 )
	       {
	       for( cz=0;cz<(min);cz++ )
		 {
		 if( dataR.noteText[cz] == '\x0d' )
		    min++;
		 }
	       }
	    if( min >=  FIXEDLEN-(sLen+1) )
	       {
	       DosBeep(1000, 50);
	       DosBeep(1500, 70);
	       DosBeep(2200, 90);
	       DosBeep(3100, 130);
	       m2 = 0;
	       break;
	       }
	    CONTINUE = TRUE;
	    FOUND = FALSE;
	    iz = min-(sLen+1);
	    while( iz < (FIXEDLEN - (sLen+1)) && CONTINUE )
	       {
	       if( dataR.noteText[iz] == schText[0] &&
		   dataR.noteText[iz+1] == schText[1] &&
		   dataR.noteText[iz+2] == schText[2])
		  {
		  strncpy(temp, dataR.noteText+iz, sLen);
		  temp[sLen] = '\0';
		  if( strcmpi(temp, schText) == 0 )
		     {
		     DosBeep(3600, 50);
		     chkRet();
		     zE = 0;
		     while( eols[zE] <= iz )
			zE++;
		     WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
				MLM_SETSEL,
				MPFROMP(iz - zE),
				MPFROMP( (iz - zE) + sLen ));
		     FOUND = TRUE;
		     CONTINUE = FALSE;
		     }
		  else
		     FOUND = FALSE;
		  }
	       iz--;
	       }
	    if( !FOUND )
	       WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_NMMATCHESS), 0);
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_NMMATCHESS :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY9));
	    stopPoint = LONGFROMMR(
			   WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
			   MLM_QUERYSEL,
			   (MPARAM) MLFQS_MAXSEL,
			   (MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
		       MLM_SETSEL,
		       MPFROMP(stopPoint),
                       MPFROMP(stopPoint));
            DosBeep(2200, 30);
	    DosBeep(1200, 70);
	    DosBeep(800, 40);
	    pdest = 0;
	    stT = 0;
	    return(0);

	 case ID_SELALL :
	    {
	    INT wlen;

	    wlen = (ULONG)WinSendDlgItemMsg(hwndDlg, ID_ENTRY9,
					    MLM_QUERYFORMATTEXTLENGTH ,
					    MPFROMLONG(0),
					    MPFROMLONG(-1));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9), MLM_SETSEL ,
			      MPFROM2SHORT(0,wlen), MPVOID  ) ;
	    WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwndDlg, ID_ENTRY9));
	    }
	    break;

	 case ID_RESETS :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY9));
	    stopPoint = LONGFROMMR(
			   WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
			   MLM_QUERYSEL,
			   (MPARAM) MLFQS_MAXSEL,
			   (MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
		       MLM_SETSEL,
		       MPFROMP(stopPoint),
		       MPFROMP(stopPoint));
	    // DosBeep(1000, 50);
	    // DosBeep(1500, 70);
	    // DosBeep(2200, 90);
	    // DosBeep(3100, 130);
	    pdest = 0;
	    stT = 0;
	    m2 = 0;
	    CASE = 0;
	    strcpy(schText, xtext);
	    break;

	 case ID_CLRS :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY9));
	    stopPoint = LONGFROMMR(
			   WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
			   MLM_QUERYSEL,
			   (MPARAM) MLFQS_MAXSEL,
			   (MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
		       MLM_SETSEL,
		       MPFROMP(stopPoint),
		       MPFROMP(stopPoint));
	    DosBeep(1000, 50);
	    DosBeep(1500, 70);
	    DosBeep(2200, 90);
	    DosBeep(3100, 130);
	    pdest = 0;
	    stT = 0;
	    m2 = 0;
	    CASE = 0;
	    strcpy(schText, xtext);
	    break;

	 case ID_BGCOLOR :
            hobjBG = WinCreateObject( "WPColorPalette",
					 "Color Palette",
					 "OPEN=DEFAULT",
					 "<WP_CONFIG>",
                                         CO_UPDATEIFEXISTS);
	    break;

	 case ID_FONTSELECT :
	    hobjFNT = WinCreateObject( "WPFontPalette",
					  "Font Palette",
					  "OPEN=DEFAULT",
					  "<WP_CONFIG>",
					  CO_UPDATEIFEXISTS);
	    break;

	 case ID_WWTOG :
	    WinSendMsg(hwndFrame, WM_COMMAND,
		       MPFROM2SHORT(ID_WORDWRAP, 0), NULL);
	    WinSendDlgItemMsg(hwndDlg, ID_ENTRY9,MLM_SETWRAP,
			      MPFROM2SHORT(WW,0),NULL);
	    if( WW )
	       {
	       setRGBColor(WinWindowFromID(hwndDlg, ID_WWTOG), 204, 0, 0);
	       WinSetWindowText(WinWindowFromID(hwndDlg, ID_WWTOG), "ww ON");
	       WinCheckMenuItem(hwndMenu, ID_WORDWRAP, TRUE);
	       }
	    else
	       {
	       setRGBColor(WinWindowFromID(hwndDlg, ID_WWTOG), 0, 0, 0);
	       WinSetWindowText(WinWindowFromID(hwndDlg, ID_WWTOG), "ww OFF");
	       WinCheckMenuItem(hwndMenu, ID_WORDWRAP, FALSE);
	       }
	    break;

	 case ID_SAVETODISKSEARCH :
	    {
	    FILE *handS;

	    // WinSendMsg(hwndDlg, WM_COMMAND, MPFROMP(ID_CLRS), 0);
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY9));
	    strcpy(docFlName, "");
	    if( !saveFileRec(hwndDlg) )
	       return(0);
	    {
	    PBYTE pText;

	    if( WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY9)) > 0 )
	       {
	       MLEGetItemTextS(hwndDlg, ID_ENTRY9, 0, -1, &pText);
	       strcpy(dataR.noteText, pText);
	       free(pText);
	       }
	    else
	       strcpy(dataR.noteText, "");
	    }
	    if( (handS = fopen(docFlName, "w+b")) == NULL )
	       return(0);
	    {
	    CHAR tempRecHeader[NAMESIZE+18] = "RECORD NAME : ";
	    CHAR tempResetStr[] = "\r";
	    CHAR tempSpaceStr[] = "\n\n";
	    CHAR nName[NAMESIZE];

	    WinQueryDlgItemText(hwndDlg,
				ID_RECNAME9,
				sizeof(nName),
				nName);
	    strcat(tempRecHeader, nName);
	    if( strlen(nName) > 0 )
	      {
	      fwrite(&tempRecHeader, (strlen(tempRecHeader))*sizeof(CHAR), 1, handS);
	      fwrite(&tempSpaceStr, (strlen(tempSpaceStr))*sizeof(CHAR), 1, handS);
	      fwrite(&tempResetStr, (strlen(tempResetStr))*sizeof(CHAR), 1, handS);
	      }
	    }
	    fwrite(&dataR.noteText, strlen(dataR.noteText), 1, handS);
	    fclose(handS);
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY9));
	    }
	    break;

	 case ID_PRINTSEARCH :
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9), MLM_FORMAT,
		       MPFROMSHORT (MLFIE_WINFMT), (MPARAM) 0L);
	    {
	    PBYTE pText;

	    if( WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY9)) > 0 )
	       {
	       MLEGetItemTextS(hwndDlg, ID_ENTRY9, 0, -1, &pText);
	       strcpy(dataRecs.noteText, pText);
	       free(pText);
	       }
	    else
	       strcpy(dataRecs.noteText, "");
	    }
	    WinQueryDlgItemText(hwndDlg,
				ID_RECNAME9,
				sizeof(dataRecs.noteName),
				dataRecs.noteName);
	    if( pgmData[13] )
	       printIt(hwndDlg, ID_ENTRY9, TRUE);
	    else
	       {
	       printIt(hwndDlg, ID_ENTRY9, TRUE);
	       DosBeep(300, 50);
	       DosBeep(1000, 70);
	       DosBeep(1500, 90);
	       DosBeep(2200, 130);
	       }
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY9));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9), MLM_FORMAT,
		       MPFROMSHORT (MLFIE_CFTEXT), (MPARAM) 0L);
	    break;

	 case ID_CANCEL9 :
	    strcpy(xtext, globalVV);
	    strcpy(schText, globalV);
	    CURRENT = FALSE;
	    WinSendMsg(hwndDlg, WM_COMMAND, MPFROM2SHORT(ID_RESETS, 0), 0);
	    WinDismissDlg( hwndDlg, FALSE );
	    SVRET = FALSE;
	    DosPostEventSem(hevS);
	    break;

	 case ID_CANCELCUR :
	    strcpy(xtext, globalVV);
	    strcpy(schText, globalV);
	    CURRENT = TRUE;
	    WinSendMsg(hwndDlg, WM_COMMAND, MPFROM2SHORT(ID_RESETS, 0), 0);
	    WinDismissDlg( hwndDlg, FALSE );
	    SVRET = -FALSE;
	    DosPostEventSem(hevS);
	    break;

	 case ID_COPYSEARCH :
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9), MLM_COPY, NULL, NULL);
	    break;

	 case ID_GETLINE :
	    {
	    LONG minT;
	    UINT zP;
	    CHAR lineNum[80] = "Line : ";
	    static CHAR szNum[6];

	    minT = LONGFROMMR(WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY9),
					  MLM_QUERYSEL,
					  (MPARAM) MLFQS_MINSEL, (MPARAM) 0L));
	    zP = (int) LONGFROMMR(WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
					     MLM_LINEFROMCHAR,
					     MPFROMLONG (minT),
					     (MPARAM) 0L));
	    itoa(zP+1,szNum,10);
	    strcat(lineNum, szNum);
	    WinSetWindowText(WinWindowFromID(hwndDlg, ID_MODS), lineNum);
	    }
	    break;

	 case ID_ENDS :
	    cz = WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY9));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
		       MLM_SETSEL,
		       MPFROMP(cz),
		       MPFROMP(cz));
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_HOMES :
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY9),
		       MLM_SETSEL,
		       MPFROMP(0),
		       MPFROMP(0));
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
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


MRESULT EXPENTRY nSearchProc( HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2 )
{
PFNWP osWinProc;
ULONG attrFoundS;
PDRAGINFO pdrginfox;
CHAR szFullDocNameS[ MAXNAME] ;

osWinProc = (PFNWP) WinQueryWindowPtr(hwndWnd, 0);
switch(ulMsg)
   {
   case WM_CONTEXTMENU :
      {
      POINTL ptlMouse;

      WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
      WinPopupMenu(HWND_DESKTOP,
		   WinQueryWindow(hwndWnd, QW_OWNER),
		   hwndSMLEPopup,
		   ptlMouse.x, ptlMouse.y,
		   0,
		   PU_HCONSTRAIN |
		   PU_VCONSTRAIN |
		   PU_KEYBOARD |
		   PU_MOUSEBUTTON2 |
		   PU_MOUSEBUTTON1 );
      }
      break;
/*
   case MLM_SETBACKCOLOR :
      if( pgmData[6] )
	 {
	 WinMessageBox(HWND_DESKTOP,
			   hwndWnd,
			   "To set colors, first uncheck the default color setting in Settings menu.",
			   "Attention ...",
			   0,
			   MB_ICONEXCLAMATION | MB_OK);
	 return(0);
	 }
      break;

   case MLM_SETTEXTCOLOR :
      if( pgmData[6] )
	 {
	 WinMessageBox(HWND_DESKTOP,
			   hwndWnd,
			   "To set colors, first uncheck the default color setting in Settings menu.",
			   "Attention ...",
			   0,
			   MB_ICONEXCLAMATION | MB_OK);
	 return(0);
	 }
      break;
*/
   case WM_PRESPARAMCHANGED :
      setPPMenuDef(6);
      WinQueryPresParam(hwndWnd, PP_FONTNAMESIZE, 0, &attrFoundS,
			    sizeof(pValue[6]), &pValue[6], QPF_NOINHERIT);
      WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndWnd, QW_PARENT), ID_RECNAME9 ),
			   PP_FONTNAMESIZE,
			   sizeof(pValue[6]),
			   &pValue[6] ) ;
      WinQueryPresParam(hwndWnd, PP_FOREGROUNDCOLOR, 0, &attrFoundS,
			    sizeof(pValue[7]), &pValue[7], QPF_NOINHERIT);
      // if( !pgmData[6] )
	 WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndWnd, QW_PARENT), ID_RECNAME9 ),
			      PP_FOREGROUNDCOLOR,
			      sizeof(pValue[7]),
			      &pValue[7] ) ;
      WinQueryPresParam(hwndWnd, PP_BACKGROUNDCOLOR, 0, &attrFoundS,
			    sizeof(pValue[8]), &pValue[8], QPF_NOINHERIT);
      // if( !pgmData[6] )
	 WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndWnd, QW_PARENT), ID_RECNAME9 ),
			      PP_BACKGROUNDCOLOR,
			      sizeof(pValue[8]),
			      &pValue[8] ) ;
      break;

   case WM_SINGLESELECT :
      WinPostMsg(WinQueryWindow(hwndWnd, QW_PARENT), WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
      break;

   case WM_CHAR :
      if( !(CHARMSG(&ulMsg)->fs & KC_KEYUP) )
	 break;
      if( CHARMSG(&ulMsg)->chr < 32 || CHARMSG(&ulMsg)->chr > 126  )
	 {
	 WinPostMsg(WinQueryWindow(hwndWnd, QW_PARENT), WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	 }
      break;

   case WM_BEGINDRAG:
	{
	DRAGITEM drgitem, *pdrgitem ;
	DRAGIMAGE drgimage ;
	CHAR szFullPathName[ MAXNAME] ;
	CHAR fileName[ MAXNAME] ;
	CHAR cwd[MAXNAME];
	FILE *hand;
	HWND dWnd;
	HOBJECT hobj;
	CHAR targetName[ MAXNAME] ;
	CHAR rfiles[MAXNAME];
	CHAR rfile[MAXNAME];
	BOOL foundsome;
	FILEFINDBUF3 findbuf;
	HDIR hDir;
	ULONG usSearchCount;
	ULONG ALLFILES;


	if( standardz  != NULLHANDLE )
	   break;
	if( WinQueryDlgItemTextLength(WinQueryWindow(hwndWnd, QW_PARENT),
					     ID_ENTRY9) <= 0 )
	   {
	   standardz = NULLHANDLE;
	   break;
	   }
	standardz = WinLoadPointer(HWND_DESKTOP, 0L, ID_POINTER2);
	ALLFILES = FILE_NORMAL;
	dWnd = NULLHANDLE;

	strcpy(szFullPathName, "");
	strcpy(szFullDocNameS, "");
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
	strcat(szFullPathName, "TMPDIR");
	hobj = WinCreateObject("WPFolder",
			       "TMPDIR",
			       "CLOSED=DEFAULT",
			       cwd,
			       CO_FAILIFEXISTS);
	strcat(szFullPathName, "\\");
	strcpy(rfiles, szFullPathName);
	strcat(rfiles, "*");

	do
	   {
	   hDir = HDIR_SYSTEM;
	   usSearchCount = 1;
	   if(DosFindFirst(rfiles, &hDir, ALLFILES, (PVOID)&findbuf,
	      sizeof(findbuf), &usSearchCount, FIL_STANDARD))
	      {
	      DosFindClose(hDir);
	      foundsome = FALSE;
	      }
	   else
	      {
	      DosFindClose(hDir);
	      strcpy(rfile, szFullPathName);
	      strcat(rfile, findbuf.achName);
	      if( DosDelete(rfile) != 0 )
		 {
		 foundsome = FALSE;
		 }
	      else
		 foundsome = TRUE;
	      }
	   }
	while(foundsome);

	strcpy(targetName, dataRecs.noteName);
	strcpy(szFullDocNameS, szFullPathName);
	strcat(szFullDocNameS, dataRecs.noteName);

	{
	PBYTE pText;

	MLEGetItemTextS(WinQueryWindow(hwndWnd, QW_PARENT), ID_ENTRY9, 0, -1, &pText);
	strcpy(dataRecs.noteText, pText);
	free(pText);
	}

	hand = fopen(szFullDocNameS, "w+b");
	if( hand == NULL )
	   {
	   standardz = NULLHANDLE;
	   break;
	   }
	addFWHeader(hand, dataRecs.noteName);
	fwrite(&dataRecs.noteText, strlen(dataRecs.noteText), 1, hand);
	fclose(hand);

	pdrginfox = DrgAllocDraginfo( 1L) ;
	pdrginfox -> hwndSource = hwndWnd;

	drgitem.hwndItem = hwndWnd;
	drgitem.ulItemID = 100L ;
	drgitem.hstrType = DrgAddStrHandle( DRT_TEXT) ;
	drgitem.hstrRMF = DrgAddStrHandle( "(DRM_OS2FILE)x(DRF_TEXT,DRF_UNKNOWN)" ) ;
	drgitem.hstrContainerName = DrgAddStrHandle(szFullPathName) ;
	drgitem.hstrSourceName = DrgAddStrHandle(targetName) ;
	drgitem.hstrTargetName = DrgAddStrHandle(targetName) ;
	drgitem.cxOffset = 0 ;
	drgitem.cyOffset = 0 ;
	drgitem.fsControl = DC_REMOVEABLEMEDIA ;
	drgitem.fsSupportedOps = DO_COPYABLE;

	pdrgitem = (PDRAGITEM)( pdrginfox + 1) ;

	if( !DrgSetDragitem( pdrginfox, &drgitem, sizeof( DRAGITEM), 0L))
		WinAlarm( HWND_DESKTOP, WA_ERROR) ;


	drgimage.cb = sizeof( DRAGIMAGE) ;
	drgimage.cptl = 0 ;
	drgimage.hImage = standardz;
	drgimage.sizlStretch.cx = 0L ;
	drgimage.sizlStretch.cy = 0L ;
	drgimage.fl = DRG_ICON ;
	drgimage.cxOffset = 0 ;
	drgimage.cyOffset = 0 ;
	dWnd = DrgDrag(hwndWnd,
		       pdrginfox,
		       &drgimage,
		       1L,
		       VK_ENDDRAG,
		       NULL);
	if( dWnd == NULLHANDLE )
	   {
	   standardz = NULLHANDLE;
	   DrgFreeDraginfo(pdrginfox);
	   remove(szFullDocNameS);
	   }
	else
	   {
	   dWnd = NULLHANDLE;
	   standardz = NULLHANDLE;
	   }
	}
	break;

   case DM_ENDCONVERSATION:
	{
	standardz = NULLHANDLE;
	DrgFreeDraginfo (pdrginfox) ;
	remove(szFullDocNameS);
	}
	break ;

   default :
      break;
   }
return( *osWinProc ) (hwndWnd, ulMsg, mp1, mp2);
}


VOID enableButtons(VOID)
{
// WinEnableWindow(hwnd1, TRUE);
WinEnableWindow(hwnd2, TRUE);
WinEnableWindow(hwnd3, TRUE);
WinEnableWindow(hwnd4, TRUE);
WinEnableWindow(hwnd5, TRUE);
WinEnableWindow(hwnd6, TRUE);
WinEnableWindow(hwnd7, TRUE);
// WinEnableWindow(hwnd8, TRUE);
WinEnableWindow(hwnd9, TRUE);
WinEnableWindow(hwnd10, TRUE);
WinEnableWindow(hwnd11, TRUE);
WinEnableWindow(hwnd12, TRUE);
}

VOID disableButtons(VOID)
{
// WinEnableWindow(hwnd1, FALSE);
WinEnableWindow(hwnd2, FALSE);
WinEnableWindow(hwnd3, FALSE);
WinEnableWindow(hwnd4, FALSE);
WinEnableWindow(hwnd5, FALSE);
WinEnableWindow(hwnd6, FALSE);
WinEnableWindow(hwnd7, FALSE);
// WinEnableWindow(hwnd8, FALSE);
WinEnableWindow(hwnd9, FALSE);
WinEnableWindow(hwnd10, FALSE);
WinEnableWindow(hwnd11, FALSE);
WinEnableWindow(hwnd12, FALSE);
}

VOID unSubClassWin(VOID)
{
WinSubclassWindow(hwnd1, oldProc1);
WinSubclassWindow(hwnd2, oldProc2);
WinSubclassWindow(hwnd3, oldProc3);
WinSubclassWindow(hwnd4, oldProc4);
WinSubclassWindow(hwnd5, oldProc5);
WinSubclassWindow(hwnd6, oldProc6);
WinSubclassWindow(hwnd7, oldProc7);
WinSubclassWindow(hwnd8, oldProc8);
WinSubclassWindow(hwnd9, oldProc9);
WinSubclassWindow(hwnd10, oldProc10);
}

VOID subClassWin(VOID)
{
oldProc1 = WinSubclassWindow(hwnd1, (PFNWP)MainDlgProc);
oldProc2 = WinSubclassWindow(hwnd2, (PFNWP)MainDlgProc);
oldProc3 = WinSubclassWindow(hwnd3, (PFNWP)MainDlgProc);
oldProc4 = WinSubclassWindow(hwnd4, (PFNWP)MainDlgProc);
oldProc5 = WinSubclassWindow(hwnd5, (PFNWP)MainDlgProc);
oldProc6 = WinSubclassWindow(hwnd6, (PFNWP)MainDlgProc);
oldProc7 = WinSubclassWindow(hwnd7, (PFNWP)MainDlgProc);
oldProc8 = WinSubclassWindow(hwnd8, (PFNWP)MainDlgProc);
oldProc9 = WinSubclassWindow(hwnd9, (PFNWP)MainDlgProc);
oldProc10 = WinSubclassWindow(hwnd10, (PFNWP)MainDlgProc);
}


INT chkRet(VOID)
{
INT l, j;
INT len;

for( l=0;l<MAXLINES;l++ )
   eols[l] = -1;
l = 0;
len = strlen(dataR.noteText);
for(j=0;j<len;j++)
   {
   if( (dataR.noteText[j] == '\x0d' && dataR.noteText[j+1] == '\x0a') ||
	((dataR.noteText[j] == '\x0d') && (dataR.noteText[j+1] == '\x0d') &&
	(dataR.noteText[j+2] == '\x0a')) )
      {
      eols[l] = j;
      l++;
      }
   }
return(l);
}


INT chkRetS(VOID)
{
INT l, j;
INT len;
CHAR tmpText[NOTESIZE];

strcpy(tmpText, dataRecs.noteText);
for( l=0;l<MAXLINES;l++ )
   eols[l] = -1;
l = 0;
len = strlen(tmpText);
for(j=0;j<len;j++)
   {
   if(tmpText[j] == '\n')
      {
      eols[l] = j;
      l++;
      }
   }
return(l);
}


MRESULT EXPENTRY newResult1Proc(HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2)
{
PFNWP oldResult1Proc;
HAB habA;
PDRAGINFO pdrginfox;

oldResult1Proc = (PFNWP) WinQueryWindowPtr(hwndWnd, 0);

switch(ulMsg)
   {
   case WM_BEGINDRAG:
	{
	DRAGITEM drgitem, *pdrgitem ;
	DRAGIMAGE drgimage ;
	CHAR szFullPathName[ MAXNAME] ;
	CHAR fileName[ MAXNAME] ;
	HPOINTER  standard;
	CHAR cwd[MAXNAME];
	FILE *hand;
	HWND dWnd;
	HOBJECT hobj;
	CHAR targetName[ MAXNAME] ;
	CHAR rfiles[MAXNAME];
	CHAR rfile[MAXNAME];
	BOOL foundsome;
	FILEFINDBUF3 findbuf;
	HDIR hDir;
	ULONG usSearchCount;
	ULONG ALLFILES;


	WinSendMsg(WinQueryWindow(hwndWnd, QW_OWNER), WM_COMMAND,
		     MPFROMSHORT(ID_LOAD1), 0);
	ALLFILES = FILE_NORMAL;
	dWnd = NULLHANDLE;
	if( (LONG)WinSendMsg(hwndWnd, LM_QUERYITEMCOUNT, 0, 0) == 0 )
	   return(0);
	standard = WinLoadPointer(HWND_DESKTOP, 0L, ID_POINTER2);

	strcpy(szFullPathName, "");
	strcpy(szFullDocNameS, "");
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
	strcat(szFullPathName, "TMPDIR");
	hobj = WinCreateObject("WPFolder",
			       "TMPDIR",
			       "CLOSED=DEFAULT",
			       cwd,
			       CO_FAILIFEXISTS);
	strcat(szFullPathName, "\\");
	strcpy(rfiles, szFullPathName);
	strcat(rfiles, "*");

	do
	   {
	   hDir = HDIR_SYSTEM;
	   usSearchCount = 1;
	   if(DosFindFirst(rfiles, &hDir, ALLFILES, (PVOID)&findbuf,
	      sizeof(findbuf), &usSearchCount, FIL_STANDARD))
	      {
	      DosFindClose(hDir);
	      foundsome = FALSE;
	      }
	   else
	      {
	      DosFindClose(hDir);
	      strcpy(rfile, szFullPathName);
	      strcat(rfile, findbuf.achName);
	      if( DosDelete(rfile) != 0 )
		 {
		 foundsome = FALSE;
		 }
	      else
		 foundsome = TRUE;
	      }
	   }
	while(foundsome);

	strcpy(targetName, dataRecs.noteName);
	strcpy(szFullDocNameS, szFullPathName);
	strcat(szFullDocNameS, dataRecs.noteName);

	hand = fopen(szFullDocNameS, "w+b");
	if( hand == NULL )
	   {
	   standard = NULLHANDLE;
	   break;
	   }
	addFWHeader(hand, dataRecs.noteName);
	fwrite(&dataRecs.noteText, strlen(dataRecs.noteText), 1, hand);
	fclose(hand);

	pdrginfox = DrgAllocDraginfo( 1L) ;
	pdrginfox -> hwndSource = hwndWnd;

	drgitem.hwndItem = hwndWnd;
	drgitem.ulItemID = 100L ;
	drgitem.hstrType = DrgAddStrHandle( DRT_TEXT) ;
	drgitem.hstrRMF = DrgAddStrHandle( "(DRM_OS2FILE)x(DRF_TEXT,DRF_UNKNOWN)" ) ;
	drgitem.hstrContainerName = DrgAddStrHandle( szFullPathName) ;
	drgitem.hstrSourceName = DrgAddStrHandle(targetName);
	drgitem.hstrTargetName = DrgAddStrHandle(targetName);
	drgitem.cxOffset = 0 ;
	drgitem.cyOffset = 0 ;
	drgitem.fsControl = DC_REMOVEABLEMEDIA ;
	drgitem.fsSupportedOps = DO_COPYABLE;

	pdrgitem = (PDRAGITEM)( pdrginfox + 1) ;

	if( !DrgSetDragitem( pdrginfox, &drgitem, sizeof( DRAGITEM), 0L))
		WinAlarm( HWND_DESKTOP, WA_ERROR) ;


	drgimage.cb = sizeof( DRAGIMAGE) ;
	drgimage.cptl = 0 ;
	drgimage.hImage = standard;
	drgimage.sizlStretch.cx = 0L ;
	drgimage.sizlStretch.cy = 0L ;
	drgimage.fl = DRG_ICON ;
	drgimage.cxOffset = 0 ;
	drgimage.cyOffset = 0 ;
	dWnd = DrgDrag(hwndWnd,
		       pdrginfox,
		       &drgimage,
		       1L,
		       VK_ENDDRAG,
		       NULL) ;
	if( dWnd == NULLHANDLE )
	   {
	   DrgFreeDraginfo(pdrginfox);
	   remove(szFullDocNameS);
	   }
	else
	   dWnd = NULLHANDLE;
	}
	break;

   case DM_ENDCONVERSATION:
	{
	DrgFreeDraginfo (pdrginfox) ;
	remove(szFullDocNameS);
	}
	break ;

   default :
	break;
   }
return( *oldResult1Proc ) (hwndWnd, ulMsg, mp1, mp2);
}



VOID fileFromDragItem ( PDRAGITEM pdiItem, PCHAR pchFile )
{
   CHAR       achPath [CCHMAXPATH] ;
   CHAR       achFile [CCHMAXPATH] ;

   DrgQueryStrName ( pdiItem -> hstrContainerName,
		     sizeof ( achPath ) ,
		     achPath ) ;

   DosQueryPathInfo ( achPath,
		      FIL_QUERYFULLNAME,
		      achPath,
		      sizeof ( achPath )) ;

   if ( achPath [strlen ( achPath ) - 1] != '\\' ) {
      strcat ( achPath, "\\" ) ;
   } /* endif */

   DrgQueryStrName ( pdiItem -> hstrSourceName,
                     sizeof ( achFile ) ,
                     achFile ) ;

   sprintf ( pchFile, "%s%s", achPath, achFile ) ;
   return ;
}




MRESULT EXPENTRY newResult2Proc(HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2)
{
PFNWP oldResult2Proc;
HAB habA;
PDRAGINFO pdrginfox;


oldResult2Proc = (PFNWP) WinQueryWindowPtr(hwndWnd, 0);
switch(ulMsg)
   {
   case WM_BEGINDRAG:
	{
	DRAGITEM drgitem, *pdrgitem ;
	DRAGIMAGE drgimage ;
	CHAR szFullPathName[ MAXNAME] ;
	CHAR szFullDocName[ MAXNAME] ;
	CHAR fileName[ MAXNAME] ;
	HPOINTER  standard;
	CHAR cwd[MAXNAME];
	FILE *hand;
	HWND dWnd;
	HOBJECT hobj;
	CHAR targetName[ MAXNAME] ;
	CHAR rfiles[MAXNAME];
	CHAR rfile[MAXNAME];
	BOOL foundsome;
	FILEFINDBUF3 findbuf;
	HDIR hDir;
	ULONG usSearchCount;
	ULONG ALLFILES;


	WinSendMsg(WinQueryWindow(hwndWnd, QW_OWNER), WM_COMMAND,
		     MPFROMSHORT(ID_LOAD2), 0);
	ALLFILES = FILE_NORMAL;
	dWnd = NULLHANDLE;
	if( (LONG)WinSendMsg(hwndWnd, LM_QUERYITEMCOUNT, 0, 0) == 0 )
	   return(0);
	standard = WinLoadPointer(HWND_DESKTOP, 0L, ID_POINTER2);

	strcpy(szFullPathName, "");
	strcpy(szFullDocNameS, "");
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
	strcat(szFullPathName, "TMPDIR");
	hobj = WinCreateObject("WPFolder",
			       "TMPDIR",
			       "CLOSED=DEFAULT",
			       cwd,
			       CO_FAILIFEXISTS);
	strcat(szFullPathName, "\\");
	strcpy(rfiles, szFullPathName);
	strcat(rfiles, "*");

	do
	   {
	   hDir = HDIR_SYSTEM;
	   usSearchCount = 1;
	   if(DosFindFirst(rfiles, &hDir, ALLFILES, (PVOID)&findbuf,
	      sizeof(findbuf), &usSearchCount, FIL_STANDARD))
	      {
	      DosFindClose(hDir);
	      foundsome = FALSE;
	      }
	   else
	      {
	      DosFindClose(hDir);
	      strcpy(rfile, szFullPathName);
	      strcat(rfile, findbuf.achName);
	      if( DosDelete(rfile) != 0 )
		 {
		 foundsome = FALSE;
		 }
	      else
		 foundsome = TRUE;
	      }
	   }
	while(foundsome);

	strcpy(targetName, dataRecs.noteName);
	strcpy(szFullDocNameS, szFullPathName);
	strcat(szFullDocNameS, dataRecs.noteName);

	hand = fopen(szFullDocNameS, "w+b");
	if( hand == NULL )
	   break;
	addFWHeader(hand, dataRecs.noteName);
	fwrite(&dataRecs.noteText, strlen(dataRecs.noteText), 1, hand);
	fclose(hand);

	pdrginfox = DrgAllocDraginfo( 1L) ;
	pdrginfox -> hwndSource = hwndWnd;

	drgitem.hwndItem = hwndWnd;
	drgitem.ulItemID = 100L ;
	drgitem.hstrType = DrgAddStrHandle( DRT_TEXT) ;
	drgitem.hstrRMF = DrgAddStrHandle( "(DRM_OS2FILE)x(DRF_TEXT,DRF_UNKNOWN)" ) ;
	drgitem.hstrContainerName = DrgAddStrHandle(szFullPathName) ;
	drgitem.hstrSourceName = DrgAddStrHandle(targetName) ;
	drgitem.hstrTargetName = DrgAddStrHandle(targetName) ;
	drgitem.cxOffset = 0 ;
	drgitem.cyOffset = 0 ;
	drgitem.fsControl = DC_REMOVEABLEMEDIA ;
	drgitem.fsSupportedOps = DO_COPYABLE;

	pdrgitem = (PDRAGITEM)( pdrginfox + 1) ;

	if( !DrgSetDragitem( pdrginfox, &drgitem, sizeof( DRAGITEM), 0L))
		WinAlarm( HWND_DESKTOP, WA_ERROR) ;


	drgimage.cb = sizeof( DRAGIMAGE) ;
	drgimage.cptl = 0 ;
	drgimage.hImage = standard;
	drgimage.sizlStretch.cx = 0L ;
	drgimage.sizlStretch.cy = 0L ;
	drgimage.fl = DRG_ICON ;
	drgimage.cxOffset = 0 ;
	drgimage.cyOffset = 0 ;
	dWnd = DrgDrag(hwndWnd,
		       pdrginfox,
		       &drgimage,
		       1L,
		       VK_ENDDRAG,
		       NULL) ;
	if( dWnd == NULLHANDLE )
	   {
	   DrgFreeDraginfo(pdrginfox);
	   remove(szFullDocNameS);
	   }
	else
	   dWnd = NULLHANDLE;
	}
	break;

   case DM_ENDCONVERSATION:
	{
	DrgFreeDraginfo (pdrginfox) ;
	remove(szFullDocNameS);
	}
	break ;

   default :
      break;
   }
return( *oldResult2Proc ) (hwndWnd, ulMsg, mp1, mp2);
}


VOID saveATFileS(VOID *hwnd)
{
HAB habT;
HMQ hmqT;
INT b,norc, rNum;
FILE *handOut;
HWND hwndM;
ULONG pointerPos;
FILE *handM;
INT iw, rnum, r, x, cc, i;
INT choi, tot, seln;
CHAR cText[NAMESIZE];
BOOL ddone;
struct Samp
   {
   SHORT indx;
   CHAR noteName[NAMESIZE];
   }sample;
ULONG oneRecSize;


habT = WinInitialize(0);
hmqT = WinCreateMsgQueue(habT, 0);
WinCancelShutdown(hmqT, TRUE);

norc = (LONG)WinSendMsg(WinWindowFromID((HWND)hwnd, ID_FINDLIST1),
		       LM_QUERYITEMCOUNT, 0, 0);
handOut = fopen(cwdFound, "w+b");
for( b=0;b<norc;b++ )
   {
   ddone = FALSE;
   r = 0;
   x = 0;
   tot = 0;
   seln = -1;
   while( strcmp(pdf[x].noteText, "") != 0 && x < MAXNUMRECS )
      x++;
   choi = b;
   do
      {
      tot += qFactor[r];
      if( (choi + 1) <= tot )
	 {
	 seln = r;
	 ddone = TRUE;
	 }
      r++;
      }
   while( !ddone || (r > x) );
   WinSendMsg(WinWindowFromID((HWND)hwnd, ID_FINDLIST1),
	      LM_QUERYITEMTEXT,
	      MPFROM2SHORT(choi, sizeof(cText)),
	      MPFROMP(cText));
   pointerPos = sizeof(recIndex);
   strcpy(datFile, pdf[seln].noteText);
   handM = fopen(pdf[seln].noteText, MRO);
   fseek(handM, 0L, SEEK_SET);
   fread(&recIndex, sizeof(recIndex), 1, handM);
   if( recIndex[0].alIndex > 0 )
      FIXEDLEN = recIndex[0].alIndex;
   else
      {
      recIndex[0].alIndex = NOTESIZE;
      FIXEDLEN = recIndex[0].alIndex;
      }
   oneRecSize = sizeof(sample) + FIXEDLEN;
   rnum = compIndexName(hwndM, cText);
   fseek(handM, pointerPos + (oneRecSize * rnum), SEEK_SET);
   fread(&sample, sizeof(sample), 1, handM);
   fread(&dataRecs.noteText, FIXEDLEN, 1, handM);
   dataRecs.indx = sample.indx;
   strcpy(dataRecs.noteName, sample.noteName);
   fclose(handM);
   saveToFile(cwdFound, handOut);
   }
fclose(handOut);
DosBeep(2400, 100);

WinDestroyMsgQueue(hmqT);
WinTerminate(habT);
_endthread();
}





