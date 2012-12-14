/* edit.c 2.1 7-17-95*/


#define INCL_WIN
#define INCL_PM
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

MRESULT EXPENTRY saveFileProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY viewRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY editRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY nCharProc( HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY nViewProc( HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY newRecName3Proc( HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2 );
INT editRecAS(HWND hwndDlg);
INT saveFileRec(HWND hwndDlg);
// VOID viewRec(HWND hwndDlg);
HWND viewRec(HWND hwndDlg);
VOID loadNewRec(HWND hwndM, HWND hwndD);
INT loadNewERec(HWND hwndM, HWND hwndD);

INT m = 0;
LONG min, maxSel;
UINT iz;
ULONG col = 0xFFFFCF;
CHAR docFlName[MAXNAME];
CHAR oldRecName[NAMESIZE];
HWND hwndCurViewRecord;
HWND hwndCurEViewRecord;
HPOINTER  standard;

INT matchAnyName(HWND hwndM, CHAR *recname);
extern VOID enableMainScrX(HWND hwndz);
extern VOID disableMainScrX(HWND hwndz, INT mode);
extern VOID setPPMenuDef(INT num);
extern VOID setDateTime(HWND hwnd, ULONG ID, INT mode, BOOL AMPM, BOOL DAYNAME);
// extern VOID setDateTime(HWND hwnd, ULONG ID);
extern ULONG MLEInsertTextS(HWND hwndDlg, ULONG ulID, LONG iptPosition, PBYTE pText);
extern INT matchName(HWND hwndM, CHAR *lbSelect);
extern INT multiRead(CHAR *inFile, INT datS, INT sizeF, INT recNum);
extern INT multiWrite(CHAR *inFile, INT datS, INT sizeF, INT recNum);
extern INT setRGBColor(HWND hwndC, ULONG r, ULONG rg, ULONG rb);
extern VOID saveIndex(CHAR *dataFileName);
extern VOID zeroDataRecs(CHAR *noteText);
extern INT openFile(CHAR *fileName, CHAR *mode);
extern VOID closeFile(VOID);
extern BOOL FindFile (HWND hwndWnd);
extern INT eSearchRec(HWND hwndDlg);
extern INT vSearchRec(HWND hwndDlg);
extern INT chkRet(VOID);
extern INT chkName(HWND hwnd, CHAR *lbSelect);
extern INT writeFile(INT index);
extern VOID setParams(HWND hwndP, SHORT dx, ULONG IDE, ULONG IDR);
extern INT saveRecFile(CHAR *str, SHORT index, CHAR *mode);
extern INT compIndexName(HWND hwndM, CHAR *sName);
extern VOID addFWHeader(FILE *hand, CHAR *dRec);
extern HFILE openTmp(HFILE hfile, CHAR *szFullPathName, CHAR *szFullDocName, CHAR *fileName);
extern ULONG MLEGetItemTextS(HWND hwndDlg, ULONG ulID, LONG lIptPosition,
			    LONG lBoundary, PBYTE *ppText);
extern ULONG MLEGetT(HWND hwndDlg, ULONG ulID, LONG lIptPosition,
		     LONG lBoundary, PBYTE *ppText);
extern INT printIt(HWND hwndP, USHORT id, BOOL ASK);

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
extern INT numRecs, WW, nCho;
extern SHORT choice;
extern INT eols[MAXLINES];
extern INT CASE;
extern CHAR *pdest;
extern CHAR *pdestT;
extern INT stT;
extern CHAR schText[NAMESIZE];
extern CHAR xtext[NAMESIZE];
extern INT INUSE;
extern struct find_t ffblk;
extern ULONG pValue[16][32];
extern HOBJECT hobjBG;
extern HOBJECT hobjFNT;
extern pgmData[60];
extern BOOL ESEARCH;
extern BOOL NSEARCH;
extern BOOL ACCUM;
extern BOOL CASESENSITIVE;
extern BOOL NAMEONLY;
extern CHAR szFullDocName[MAXNAME] ;
extern HWND hwndEMLEPopup, hwndVMLEPopup;
extern ULONG scrLen, scrWid;
extern HWND hwndFrame, hwndMenu;
extern UINT FIXEDLEN;
extern HWND hwndRFrame;
extern CHAR EXTENTION[EXTSIZE];
extern hwndMainDlg;
extern INT nChox;
extern CHAR datFilex[MAXNAME];
extern HWND hwndResults;
extern HWND hwndRes, hwndResn;


INT editRec(HWND hwndDlg)
{
if ( WinDlgBox(
	HWND_DESKTOP,
	hwndDlg,
	editRecProc,
	(HMODULE)0,
	ID_EDITREC,
	NULL) == TRUE )
   return(1);
else
   return(0);
}


INT saveFileRec(HWND hwndDlg)
{
if ( WinDlgBox(
	HWND_DESKTOP,
	hwndDlg,
	saveFileProc,
	(HMODULE)0,
	ID_SAVETOFILE,
	NULL) == TRUE )
   return(1);
else
   return(0);
}

/*
VOID viewRec(HWND hwndDlg)
{
WinDlgBox(
   HWND_DESKTOP,
   HWND_DESKTOP,
   // hwndDlg,
   viewRecProc,
   (HMODULE)0,
   ID_VIEWREC,
   NULL);
}
*/

/*
HWND viewRec(HWND hwndDlg)
{
HWND hwndr;

hwndr = WinLoadDlg(HWND_DESKTOP,
		   // HWND_DESKTOP,
		   hwndDlg,
		   viewRecProc,
		   (HMODULE)0,
		   ID_VIEWREC,
		   NULL);
return(hwndr);
}
*/


HWND viewRec(HWND hwndDlg)
{
WinDlgBox(
   HWND_DESKTOP,
   hwndDlg,
   viewRecProc,
   (HMODULE)0,
   ID_VIEWREC,
   NULL);
return(NULLHANDLE);
}



MRESULT EXPENTRY saveFileProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
CHAR defName[] = "TEMP.DOC";

switch ( msg )
   {
   case WM_INITDLG:
      WinSendDlgItemMsg(hwndDlg, ID_ENTRYFILE,EM_SETTEXTLIMIT,
		  MPFROM2SHORT(MAXNAME,0),NULL);
      WinSetWindowText(WinWindowFromID(hwndDlg, ID_ENTRYFILE),
		       defName);
      WinSendDlgItemMsg ( hwndDlg ,ID_ENTRYFILE, EM_SETSEL ,
		       MPFROM2SHORT(0,sizeof(defName)), MPVOID  ) ;
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case ID_OKFILE :
	    WinQueryDlgItemText(hwndDlg,
				ID_ENTRYFILE,
				sizeof(docFlName),
				docFlName);
	    if ( _dos_findfirst(docFlName,_A_NORMAL,&ffblk) == 0 )
	       {
	       if (WinMessageBox(
		     HWND_DESKTOP,
		     hwndDlg,
		     "File already exists! Do you want to overwrite it?",
		     "ATTENTION!",
		     0,
		     MB_ICONEXCLAMATION |
		     MB_YESNO | MB_DEFBUTTON2 ) == MBID_NO )
		  return(0);
	       }
	    WinDismissDlg( hwndDlg, TRUE );
	    break;

	 case ID_CANCELFILE :
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




VOID loadNewRec(HWND hwndM, HWND hwndD)
{
INT p, r;
CHAR flr[6];
CHAR szModName[75] = "View Record       Module : ";
INT z;


WinSendMsg(hwndD, WM_COMMAND, MPFROM2SHORT(ID_RESETV, 0), 0);
// WinSetWindowText(WinWindowFromID(hwndD, ID_ENTRY4), "Hello");
WinSendDlgItemMsg(hwndD, ID_ENTRY4,MLM_SETTEXTLIMIT,
		  MPFROM2SHORT(FIXEDLEN,0),NULL);
WinSetWindowText(WinWindowFromID(hwndD, ID_ENTRY4), dataRecs.noteText);
WinSetWindowText(WinWindowFromID(hwndD, ID_RECNAME4), dataRecs.noteName);
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
WinSendMsg(WinWindowFromID(hwndD, ID_ENTRY4),
	   MLM_SETSEL,
	   MPFROMP(0),
	   MPFROMP(0));
}





MRESULT EXPENTRY viewRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
INT p, r;
CHAR flr[6];
CHAR szModName[75] = "View Record       Module : ";
INT z, handle;
SHORT key, flags;
SWCNTRL swctlv;
PID pidv;
INT cz;
PFNWP oWinProc;
INT sLen, zE;
BOOL CONTINUE, FOUND;
IPT stopPoint = 0;
CHAR temp[NAMESIZE];
PSWP pOldSize;
PSWP pNewSize;
SWP ctrlpos, ctrlpos2, ctrlpos3;
ULONG lColor;
ULONG curPt, lineN;

pNewSize = (PSWP)mp1;
pOldSize = pNewSize + 1;
switch ( msg )
   {
   case WM_INITDLG:
      WinEnableWindow(WinWindowFromID(hwndRes, ID_EDITF1), FALSE);
      WinEnableWindow(WinWindowFromID(hwndRes, ID_STOPSEARCH), FALSE);
      WinEnableWindow(WinWindowFromID(hwndResn, ID_EDITF1), FALSE);
      WinEnableWindow(WinWindowFromID(hwndResn, ID_STOPSEARCH), FALSE);
      hwndCurViewRecord = WinQueryWindow(WinWindowFromID(hwndDlg, ID_RECNAME4), QW_OWNER);
      WinSendDlgItemMsg(hwndDlg, ID_RECNAME4,EM_SETTEXTLIMIT,
		  MPFROM2SHORT(NAMESIZE,0),NULL);
      WinSendDlgItemMsg(hwndDlg, ID_ENTRY4,MLM_SETTEXTLIMIT,
		  MPFROM2SHORT(FIXEDLEN,0),NULL);
      WinSendDlgItemMsg(hwndDlg, ID_ENTRY4,MLM_SETWRAP,
		  MPFROM2SHORT(WW,0),NULL);
      WinSetWindowText(WinWindowFromID(hwndDlg, ID_ENTRY4),
		       dataRecs.noteText);
      WinSetWindowText(WinWindowFromID(hwndDlg, ID_RECNAME4),
		       dataRecs.noteName);
      oWinProc = WinSubclassWindow(WinWindowFromID(hwndDlg, ID_ENTRY4),
				     nViewProc);
      WinSetWindowPtr(WinWindowFromID(hwndDlg, ID_ENTRY4),
		      0, (PVOID) oWinProc);
      z = (FIXEDLEN - 1) - (strlen(dataRecs.noteText) * sizeof(CHAR));
      WinSetDlgItemShort(hwndDlg, ID_BYTES, (SHORT)z, TRUE);
      WinQueryWindowProcess(hwndDlg, &pidv, NULL);
      swctlv.hwnd = hwndDlg;
      swctlv.hwndIcon = NULLHANDLE;
      swctlv.hprog = NULLHANDLE;
      swctlv.idProcess = pidv;
      swctlv.idSession = 0;
      swctlv.uchVisibility = SWL_VISIBLE;
      swctlv.fbJump = SWL_JUMPABLE;
      strcpy(swctlv.szSwtitle, "DataPad View Window");
      // WinAddSwitchEntry(&swctlv);
      if( !pgmData[4] )
	 setParams(hwndDlg, 0, ID_ENTRY4, ID_RECNAME4);
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
      WinSetDlgItemText(hwndDlg, FID_TITLEBAR, szModName);
      WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
      if(ESEARCH || NSEARCH)
	 {
	 pgmData[12] = 0;
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
	 WinPostMsg(hwndDlg, WM_COMMAND, (MPARAM)ID_SEARCHV, 0);
	 }
      hwndVMLEPopup = WinLoadMenu(WinWindowFromID(hwndDlg, ID_OK4),
				  0,
				  ID_VMLEPOPUP);
      WinSetWindowPos(hwndDlg, HWND_TOP,
		      pgmData[17], pgmData[18], pgmData[15], pgmData[16],
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
      disableMainScrX(hwndDlg, 3); // NEW
      WinSetOwner(hwndDlg, NULLHANDLE);  // NEW
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_CHAR :
      flags = SHORT1FROMMP(mp1);
      key = SHORT2FROMMP(mp2);
      /*
      if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_RIGHT)
	 {
	 DosBeep(200, 100);
	 }
      if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_LEFT)
	 {
	 DosBeep(200, 100);
	 }
      */
      if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_F2)
	 {
	 if( !INUSE )
	    {
	    WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_SEARCHV), 0);
	    INUSE = 0;
	    }
	 }
      if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_F3)
	 {
	 if( !INUSE )
	    {
	    WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_SEARCHVR), 0);
	    INUSE = 0;
	    }
	 }
      if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_F4)
	 {
	 if( !INUSE )
	    {
	    WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_STOPV), 0);
	    INUSE = 0;
	    }
	 }
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_DESTROY:
      WinSetOwner(hwndDlg, hwndFrame);  // NEW
      pdest = 0;
      stT = 0;
      m = 0;
      CASE = 0;
      INUSE = 0;
      if( ESEARCH || NSEARCH )
	 strcpy(schText, xtext);
      if( WinQueryWindowULong(hwndDlg, QWL_STYLE) & WS_MAXIMIZED )
	 {
	 pgmData[15] = WinQueryWindowULong(hwndDlg, QWS_CXRESTORE);
	 pgmData[16] = WinQueryWindowULong(hwndDlg, QWS_CYRESTORE);
	 pgmData[17] = WinQueryWindowULong(hwndDlg, QWS_XRESTORE);
	 pgmData[18] = WinQueryWindowULong(hwndDlg, QWS_YRESTORE);
	 }
      else
	 {
	 if( !(WinQueryWindowULong(hwndDlg, QWL_STYLE) & WS_MINIMIZED) )
	    {
	    WinQueryWindowPos(hwndDlg, &ctrlpos);
	    pgmData[15] = ctrlpos.cx;
	    pgmData[16] = ctrlpos.cy;
	    pgmData[17] = ctrlpos.x;
	    pgmData[18] = ctrlpos.y;
	    }
	 }
      enableMainScrX(hwndDlg); /* new */
      WinEnableWindow(WinWindowFromID(hwndRes, ID_EDITF1), TRUE);
      WinEnableWindow(WinWindowFromID(hwndRes, ID_STOPSEARCH), TRUE);
      WinEnableWindow(WinWindowFromID(hwndResn, ID_EDITF1), TRUE);
      WinEnableWindow(WinWindowFromID(hwndResn, ID_STOPSEARCH), TRUE);
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
	    pti->ptlMinTrackSize.x = scrWid * .469;
	    pti->ptlMinTrackSize.y = scrLen * .462;
	    break;

	 case 800 :
	    pti->ptlMinTrackSize.x = scrWid * .375;
	    pti->ptlMinTrackSize.y = scrLen * .393;
	    break;

	 default :
	    pti->ptlMinTrackSize.x = scrWid * .388;
	    pti->ptlMinTrackSize.y = scrLen * .373;
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
	 WinQueryWindowPos(WinWindowFromID(hwndDlg, ID_ENTRY4), &ctrlpos2);
	 WinQueryWindowPos(WinWindowFromID(hwndDlg, ID_VGBOX1), &ctrlpos3);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_ENTRY4), FALSE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_VGBOX1), FALSE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RECNAME4), FALSE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RNAME), FALSE);

	 hei = ctrlpos.cy - (ctrlpos3.y*3.1);
	 wid = ctrlpos.cx - (ctrlpos3.x*2);
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_VGBOX1), (HWND)0,
			 0, 0, wid, hei, SWP_SIZE);

	 hei = (ctrlpos.cy-(ctrlpos2.y+(ctrlpos3.y*6.4)));
	 wid = (ctrlpos.cx-(ctrlpos2.x*2));
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_ENTRY4), (HWND)0,
			 0, 0, wid, hei, SWP_SIZE);

	 hei = (ctrlpos3.y*1.9);
	 wid = (ctrlpos.cx-(ctrlpos2.x*7));
	 up = (ctrlpos.cy-(5.6*ctrlpos3.y));
	 over = (ctrlpos3.x*7.6);
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_RECNAME4), (HWND)0,
			 over, up, wid, hei, SWP_SIZE | SWP_MOVE);

	 up = (ctrlpos.cy-(5.6*ctrlpos3.y));
	 over = (ctrlpos3.x *2.1);
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_RNAME), (HWND)0,
			 over, up, 0, 0, SWP_MOVE);

	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_ENTRY4), TRUE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_VGBOX1), TRUE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RECNAME4), TRUE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RNAME), TRUE);
	 }
      return (WinDefDlgProc(hwndDlg, msg, mp1, mp2));

   case WM_ADJUSTWINDOWPOS :
      if( ((PSWP)mp1)->fl & SWP_MINIMIZE )
	 {
	 WinSetWindowPos(hwndFrame, (HWND)0, 0, 0, 0, 0, SWP_MINIMIZE);
	 WinShowWindow(WinQueryWindow(hwndDlg, QW_OWNER), FALSE);
	 }
      else
	 if( ((PSWP)mp1)->fl & SWP_RESTORE )
	    {
	    WinSetWindowPos(hwndFrame, (HWND)0, 0, 0, 0, 0, SWP_RESTORE);
	    WinShowWindow(WinQueryWindow(hwndDlg, QW_OWNER), TRUE);
	    }
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

    case WM_CONTEXTMENU :
      {
      POINTL ptlMouse;

      WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
      WinPopupMenu(HWND_DESKTOP,
		   hwndDlg,
		   hwndVMLEPopup,
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
	 case ID_OK4 :
	    pdest = 0;
	    stT = 0;
	    m = 0;
	    CASE = 0;
	    strcpy(schText, xtext);
	    // DosBeep(100, 100);
	    WinSetOwner(hwndDlg, hwndFrame);  // NEW
	    enableMainScrX(hwndDlg); /* new */
	    WinDismissDlg( hwndDlg, TRUE );
	    break;

	 case ID_SELALL :
	    {
	    INT wlen;

	    wlen = (ULONG)WinSendDlgItemMsg(hwndDlg, ID_ENTRY4,
					    MLM_QUERYFORMATTEXTLENGTH ,
					    MPFROMLONG(0),
					    MPFROMLONG(-1));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4), MLM_SETSEL ,
			      MPFROM2SHORT(0,wlen), MPVOID  ) ;
	    WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwndDlg, ID_ENTRY4));
	    }
	    break;

	 case ID_COPYVIEW :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY4));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4), MLM_COPY, NULL, NULL);
	    break;

	 case ID_PRINTVIEW :
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4), MLM_FORMAT,
		       MPFROMSHORT (MLFIE_WINFMT), (MPARAM) 0L);
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY4));
	    if( pgmData[13] )
	       printIt(hwndDlg, ID_ENTRY4, TRUE);
	    else
	       {
	       printIt(hwndDlg, ID_ENTRY4, FALSE);
	       DosBeep(300, 50);
	       DosBeep(1000, 70);
	       DosBeep(1500, 90);
	       DosBeep(2200, 130);
	       }
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4), MLM_FORMAT,
		       MPFROMSHORT (MLFIE_CFTEXT), (MPARAM) 0L);
	    break;

	 case ID_SEARCHV :
	    if( WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY4)) <= 0 )
	       break;
	    if( m == 0 )
	       {
	       iz = 0;
	       if( vSearchRec(hwndDlg) == FALSE )
		  return(0);
	       strcpy(xtext, schText);
	       if( CASE == 0  )
		  strupr(schText);
	       m++;
	       }
	    sLen = strlen(schText);
	    min = 0;
	    WinSetFocus(HWND_DESKTOP,
			   WinWindowFromID(hwndDlg, ID_ENTRY4));
	    {
	    PBYTE pText;

	    MLEGetItemTextS(hwndDlg, ID_ENTRY4, 0, -1, &pText);
	    strcpy(dataR.noteText, pText);
	    free(pText);
	    }
	    if( CASE == 0  )
	       strupr(dataR.noteText);
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
		       MLM_SETSEL,
		       MPFROMP(-1),
		       MPFROMP(-1));
	    maxSel = LONGFROMMR(WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
				MLM_QUERYSEL,
				(MPARAM) MLFQS_MAXSEL,
				(MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
		       MLM_SETSEL,
		       MPFROMP(maxSel),
		       MPFROMP(maxSel));
	    min = LONGFROMMR (WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY4),
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
	       m = 0;
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
		     WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
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
	       WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_NMMATCHESV), 0);
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_SEARCHVR :
	    if( WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY4)) <= 0 )
	       break;
	    if( m == 0 )
	       {
	       iz = 0;
	       if( vSearchRec(hwndDlg) == FALSE )
		  return(0);
	       strcpy(xtext, schText);
	       if( CASE == 0 )
		  strupr(schText);
	       m++;
	       }
	    sLen = strlen(schText);
	    min = 0;
	    WinSetFocus(HWND_DESKTOP,
			   WinWindowFromID(hwndDlg, ID_ENTRY4));
	    {
	    PBYTE pText;

	    MLEGetItemTextS(hwndDlg, ID_ENTRY4, 0, -1, &pText);
	    strcpy(dataR.noteText, pText);
	    free(pText);
	    }
	    if( CASE == 0 )
	       strupr(dataR.noteText);
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
		       MLM_SETSEL,
		       MPFROMP(-1),
		       MPFROMP(-1));
	    maxSel = LONGFROMMR(WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
				MLM_QUERYSEL,
				(MPARAM) MLFQS_MAXSEL,
				(MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
		       MLM_SETSEL,
		       MPFROMP(maxSel),
		       MPFROMP(maxSel));
	    min = LONGFROMMR (WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY4),
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
	       m = 0;
	       break;
	       }
	    CONTINUE = TRUE;
	    FOUND = FALSE;
	    iz = min-(sLen+1);
	    while( iz < (FIXEDLEN - (sLen+1)) && CONTINUE )
	       {
	       if(  dataR.noteText[iz] == schText[0] &&
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
		     WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
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
	       WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_NMMATCHESV), 0);
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_NMMATCHESV :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY4));
	    stopPoint = LONGFROMMR(
			   WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
			   MLM_QUERYSEL,
			   (MPARAM) MLFQS_MAXSEL,
			   (MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
		       MLM_SETSEL,
		       MPFROMP(stopPoint),
		       MPFROMP(stopPoint));
	    DosBeep(2200, 30);
	    DosBeep(1200, 70);
	    DosBeep(800, 40);
	    pdest = 0;
	    stT = 0;
	    return(0);

	 case ID_RESETV :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY4));
	    stopPoint = LONGFROMMR(
			   WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
			   MLM_QUERYSEL,
			   (MPARAM) MLFQS_MAXSEL,
			   (MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
		       MLM_SETSEL,
		       MPFROMP(stopPoint),
		       MPFROMP(stopPoint));
	    // DosBeep(1000, 50);
	    // DosBeep(1500, 70);
	    // DosBeep(2200, 90);
	    // DosBeep(3100, 130);
	    pdest = 0;
	    stT = 0;
	    m = 0;
	    CASE = 0;
	    strcpy(schText, xtext);
	    return(0);

	 case ID_STOPV :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY4));
	    stopPoint = LONGFROMMR(
			   WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
			   MLM_QUERYSEL,
			   (MPARAM) MLFQS_MAXSEL,
			   (MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
		       MLM_SETSEL,
		       MPFROMP(stopPoint),
		       MPFROMP(stopPoint));
	    DosBeep(1000, 50);
	    DosBeep(1500, 70);
	    DosBeep(2200, 90);
	    DosBeep(3100, 130);
	    pdest = 0;
	    stT = 0;
	    m = 0;
	    CASE = 0;
	    strcpy(schText, xtext);
	    return(0);

	 case ID_SAVEV :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY4));
	    strcpy(docFlName, "");
	    if( !saveFileRec(hwndDlg) )
	       return(0);
	    WinQueryDlgItemText(hwndDlg,
				ID_RECNAME4,
				sizeof(dataRecs.noteName),
				dataRecs.noteName);
	    {
	    PBYTE pText;

	    if( WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY4)) > 0 )
	       {
	       MLEGetItemTextS(hwndDlg, ID_ENTRY4, 0, -1, &pText);
	       strcpy(dataRecs.noteText, pText);
	       free(pText);
	       }
	    else
	       strcpy(dataRecs.noteText, "");
	    }
	    {
	    FILE *hand;

	    hand = fopen(docFlName, "w+b");
	    addFWHeader(hand, dataRecs.noteName);
	    if( hand == NULL )
	       break;
	    fwrite(&dataRecs.noteText, strlen(dataRecs.noteText), 1, hand);
	    fclose(hand);
	    }
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
	    WinSendMsg(hwndMainDlg, WM_COMMAND,
		       MPFROM2SHORT(ID_WORDWRAP, 0), NULL);
	    WinSendDlgItemMsg(hwndDlg, ID_ENTRY4,MLM_SETWRAP,
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

	 case ID_GETLINE :
	    {
	    LONG minT;
	    UINT zP;
	    CHAR lineNum[80] = "Line : ";
	    CHAR static szNum[6];

	    minT = LONGFROMMR(WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY4),
					  MLM_QUERYSEL,
					  (MPARAM) MLFQS_MINSEL, (MPARAM) 0L));
	    zP = (int) LONGFROMMR(WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
					     MLM_LINEFROMCHAR,
					     MPFROMLONG (minT),
					     (MPARAM) 0L));
	    itoa(zP+1,szNum,10);
	    strcat(lineNum, szNum);
	    WinSetWindowText(WinWindowFromID(hwndDlg, ID_MODVIEW), lineNum);
	    }
	    break;

	 case ID_ENDV :
	    cz = WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY4));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
		       MLM_SETSEL,
		       MPFROMP(cz),
		       MPFROMP(cz));
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_HOMEV :
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY4),
		       MLM_SETSEL,
		       MPFROMP(0),
		       MPFROMP(0));
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_P :
	    if(ESEARCH || NSEARCH)
	       {
	       /* WinSendMsg(hwndDlg,
			  WM_COMMAND,
			  MPFROM2SHORT(ID_OK4,0),
			  MPFROMP(0)); */
	       WinSendMsg(hwndRFrame,
			  WM_COMMAND,
			  MPFROM2SHORT(ID_NEXTV,0),
			  MPFROMP(0));
	       WinSetFocus(HWND_DESKTOP,
			   WinWindowFromID(hwndDlg, ID_ENTRY4));
	       }
	    else
	       {
	       /* WinSendMsg(hwndDlg,
			  WM_COMMAND,
			  MPFROM2SHORT(ID_OK4,0),
			  MPFROMP(0)); */
	       WinSendMsg(hwndFrame,
			  WM_COMMAND,
			  MPFROM2SHORT(ID_NEXTV,0),
			  MPFROMP(0));
	       WinSetFocus(HWND_DESKTOP,
			   WinWindowFromID(hwndDlg, ID_ENTRY4));
	      }
	    break;

	 case ID_M :
	    if(ESEARCH || NSEARCH)
	       {
	       /* WinSendMsg(hwndDlg,
			  WM_COMMAND,
			  MPFROM2SHORT(ID_OK4,0),
			  MPFROMP(0)); */
	       WinSendMsg(hwndRFrame,
			  WM_COMMAND,
			  MPFROM2SHORT(ID_LASTV,0),
			  MPFROMP(0));
	       WinSetFocus(HWND_DESKTOP,
			   WinWindowFromID(hwndDlg, ID_ENTRY4));
	       }
	    else
	       {
	       /* WinSendMsg(hwndDlg,
			  WM_COMMAND,
			  MPFROM2SHORT(ID_OK4,0),
			  MPFROMP(0)); */
	       WinSendMsg(hwndFrame,
			  WM_COMMAND,
			  MPFROM2SHORT(ID_LASTV,0),
			  MPFROMP(0));
	       WinSetFocus(HWND_DESKTOP,
			   WinWindowFromID(hwndDlg, ID_ENTRY4));
	       }
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





INT loadNewERec(HWND hwndM, HWND hwndD)
{
INT p, r;
CHAR flr[6];
CHAR szModName[75] = "Edit Record       Module : ";
INT z;


// strcpy(oldRecName, dataRecs.noteName);
WinSendMsg(hwndD, WM_COMMAND, MPFROM2SHORT(ID_CLEARVAR, 0), 0);
WinSendDlgItemMsg(hwndD, ID_ENTRY3,MLM_SETTEXTLIMIT,
		  MPFROM2SHORT(FIXEDLEN,0),NULL);
WinSetWindowText(WinWindowFromID(hwndD, ID_ENTRY3), dataRecs.noteText);
WinSetWindowText(WinWindowFromID(hwndD, ID_RECNAME3), dataRecs.noteName);
z = (FIXEDLEN - 1) - (strlen(dataRecs.noteText) * sizeof(CHAR));
WinSetDlgItemShort(hwndD, ID_BYTESED, (SHORT)z, TRUE);
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
WinSendMsg(WinWindowFromID(hwndD, ID_ENTRY3),
	   MLM_SETSEL,
	   MPFROMP(0),
	   MPFROMP(0));

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
   }

return(1);
}





MRESULT EXPENTRY editRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
INT p, r;
INT z, zz, handle, fHan;
CHAR maxSize[6] = "";
CHAR errMsg2[60] = "";
CHAR flr[6];
CHAR szModName[75] = "Edit Record       Module : ";
SHORT flags, key;
IPT stopPoint = 0;
PFNWP oWinProc;
PFNWP oldRecName3Proc;
SWCNTRL swctle;
PID pide;
INT cz;
INT sLen, zE;
BOOL CONTINUE, FOUND;
CHAR temp[NAMESIZE];
PSWP pOldSize;
PSWP pNewSize;
SWP ctrlpos, ctrlpos2, ctrlpos3;

pNewSize = (PSWP)mp1;
pOldSize = pNewSize + 1;
switch ( msg )
   {
   case WM_INITDLG:
      WinEnableWindow(WinWindowFromID(hwndRes, ID_VIEWF1), FALSE);
      WinEnableWindow(WinWindowFromID(hwndRes, ID_STOPSEARCH), FALSE);
      WinEnableWindow(WinWindowFromID(hwndResn, ID_VIEWF1), FALSE);
      WinEnableWindow(WinWindowFromID(hwndResn, ID_STOPSEARCH), FALSE);
      hwndCurEViewRecord = WinQueryWindow(WinWindowFromID(hwndDlg, ID_RECNAME3), QW_OWNER);
      WinSendDlgItemMsg(hwndDlg, ID_RECNAME3,EM_SETTEXTLIMIT,
		  MPFROM2SHORT(NAMESIZE,0),NULL);
      WinSendDlgItemMsg(hwndDlg, ID_ENTRY3,MLM_SETTEXTLIMIT,
		  MPFROM2SHORT(FIXEDLEN,0),NULL);
      WinSendDlgItemMsg(hwndDlg, ID_ENTRY3,MLM_SETWRAP,
		  MPFROM2SHORT(WW,0),NULL);
      WinSetWindowText(WinWindowFromID(hwndDlg, ID_ENTRY3),
		       dataRecs.noteText);
      WinSetWindowText(WinWindowFromID(hwndDlg, ID_RECNAME3),
		       dataRecs.noteName);
      WinPostMsg(hwndDlg, WM_CONTROL, MPFROM2SHORT(ID_ENTRY3, MLN_CHANGE), 0);
      oWinProc = WinSubclassWindow(WinWindowFromID(hwndDlg, ID_ENTRY3),
				     nCharProc);
      WinSetWindowPtr(WinWindowFromID(hwndDlg, ID_ENTRY3),
		      0, (PVOID) oWinProc);

      oldRecName3Proc = WinSubclassWindow(WinWindowFromID(hwndDlg, ID_RECNAME3),
				     newRecName3Proc);
      WinSetWindowPtr(WinWindowFromID(hwndDlg, ID_RECNAME3),
		      0, (PVOID) oldRecName3Proc);
      WinQueryWindowProcess(hwndDlg, &pide, NULL);
      swctle.hwnd = hwndDlg;
      swctle.hwndIcon = NULLHANDLE;
      swctle.hprog = NULLHANDLE;
      swctle.idProcess = pide;
      swctle.idSession = 0;
      swctle.uchVisibility = SWL_VISIBLE;
      swctle.fbJump = SWL_JUMPABLE;

      strcpy(swctle.szSwtitle, "DataPad Edit Window");
      WinAddSwitchEntry(&swctle);
      if( !pgmData[5] )
	 setParams(hwndDlg, 3, ID_ENTRY3, ID_RECNAME3);
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
      WinSetDlgItemText(hwndDlg, FID_TITLEBAR, szModName);
      WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
      strcpy(oldRecName, dataRecs.noteName);
      hwndEMLEPopup = WinLoadMenu(WinWindowFromID(hwndDlg, ID_OK3),
				  0,
				  ID_EMLEPOPUP);
      if(ESEARCH || NSEARCH)
	 {
	 pgmData[12] = 1;
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
	 WinPostMsg(hwndDlg, WM_COMMAND, (MPARAM)ID_SEARCHED, 0);
	 // WinEnableMenuItem(hwndEMLEPopup, ID_SAVENOWE, FALSE);
	 }
      WinSetWindowPos(hwndDlg, HWND_TOP,
		      pgmData[21], pgmData[22], pgmData[19], pgmData[20],
		      SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ACTIVATE | SWP_ZORDER);
      setRGBColor(WinWindowFromID(hwndDlg, ID_OK3), 204, 0, 0);
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
      WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY3), MLM_RESETUNDO, (MPARAM) 0L, (MPARAM) 0L);
      disableMainScrX(hwndDlg, 2); // NEW
      WinSetOwner(hwndDlg, NULLHANDLE);  // NEW
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_CONTROL:
      switch (SHORT2FROMMP (mp1))
	 {
	 case MLN_CHANGE:
	     if (SHORT1FROMMP (mp1) == ID_ENTRY3)
	       {
	       INT doable;

	       doable = SHORT1FROMMR (WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY3), MLM_QUERYUNDO,
					(MPARAM) 0L, (MPARAM) 0L));
	       if( doable )
		  WinEnableMenuItem(hwndEMLEPopup, ID_UNDOOP, TRUE);
	       else
		  WinEnableMenuItem(hwndEMLEPopup, ID_UNDOOP, FALSE);
	       z = (FIXEDLEN -1) - WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY3));
	       WinSetDlgItemShort(hwndDlg, ID_BYTESED, (SHORT)z, TRUE);
	       }
	       break;

	    default:
	       break;
	 }
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_CHAR :
      {
      ULONG retk;

      flags = SHORT1FROMMP(mp1);
      key = SHORT2FROMMP(mp2);
      if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_ESC)
	 {
	 retk = (ULONG)WinMessageBox(HWND_DESKTOP,
			   hwndDlg,
			   "Press Yes to save, No to discard, or Cancel to abort operation.",
			   "Save File?",
			   0,
			   MB_ICONQUESTION |
			   MB_YESNOCANCEL | MB_DEFBUTTON1 );
	 switch(retk)
	    {
	    case MBID_YES:
	       WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_OK3), 0);
	       return(0);

	    case MBID_NO:
	       WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_CANCEL3), 0);
	       break;

	    case MBID_CANCEL:
	       return(MRESULT) FALSE;
	    }
	 }

      if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_F2)
	 if( !INUSE )
	    {
	    WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_SEARCHED), 0);
	    INUSE = 0;
	    }
      if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_F3)
	 if( !INUSE )
	    {
	    WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_SEARCHEDR), 0);
	    INUSE = 0;
	    }
      if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_F4)
	 if( !INUSE )
	    {
	    WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_STOPEDSEARCH), 0);
	    INUSE = 0;
	    }
      }
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );


   case WM_CLOSE:
      {
      ULONG retk;

      retk = (ULONG)WinMessageBox(HWND_DESKTOP,
			   hwndDlg,
			   "Press Yes to save, No to discard, or Cancel to abort operation.",
			   "Save File?",
			   0,
			   MB_ICONQUESTION |
			   MB_YESNOCANCEL | MB_DEFBUTTON1 );
      switch(retk)
	 {
	 case MBID_YES:
	    WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_OK3), 0);
	    break;

	 case MBID_NO:
	    WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_CANCEL3), 0);
	    break;

	 case MBID_CANCEL:
	    return (MRESULT) FALSE;

	 default:
	    break;
	 }
      }
      break;

   case WM_DESTROY :
      pdest = 0;
      stT = 0;
      m = 0;
      CASE = 0;
      INUSE = 0;
      if( ESEARCH || NSEARCH )
	 strcpy(schText, xtext);
      if( WinQueryWindowULong(hwndDlg, QWL_STYLE) & WS_MAXIMIZED )
	 {
	 pgmData[19] = WinQueryWindowULong(hwndDlg, QWS_CXRESTORE);
	 pgmData[20] = WinQueryWindowULong(hwndDlg, QWS_CYRESTORE);
	 pgmData[21] = WinQueryWindowULong(hwndDlg, QWS_XRESTORE);
	 pgmData[22] = WinQueryWindowULong(hwndDlg, QWS_YRESTORE);
	 }
      else
	 {
	 if( !(WinQueryWindowULong(hwndDlg, QWL_STYLE) & WS_MINIMIZED) )
	    {
	    WinQueryWindowPos(hwndDlg, &ctrlpos);
	    pgmData[19] = ctrlpos.cx;
	    pgmData[20] = ctrlpos.cy;
	    pgmData[21] = ctrlpos.x;
	    pgmData[22] = ctrlpos.y;
	    }
	 }
      enableMainScrX(hwndDlg); /* new */
      nChox = -1;
      strcpy(datFilex, "");
      WinEnableWindow(WinWindowFromID(hwndRes, ID_VIEWF1), TRUE);
      WinEnableWindow(WinWindowFromID(hwndRes, ID_STOPSEARCH), TRUE);
      WinEnableWindow(WinWindowFromID(hwndResn, ID_VIEWF1), TRUE);
      WinEnableWindow(WinWindowFromID(hwndResn, ID_STOPSEARCH), TRUE);
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
	    pti->ptlMinTrackSize.x = scrWid * .454;
	    pti->ptlMinTrackSize.y = scrLen * .435;
	    break;

	 case 800 :
	    pti->ptlMinTrackSize.x = scrWid * .364;
	    pti->ptlMinTrackSize.y = scrLen * .374;
	    break;

	 default :
	    pti->ptlMinTrackSize.x = scrWid * .3747;
	    pti->ptlMinTrackSize.y = scrLen * .36;
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
	 WinQueryWindowPos(WinWindowFromID(hwndDlg, ID_ENTRY3), &ctrlpos2);
	 WinQueryWindowPos(WinWindowFromID(hwndDlg, ID_GBOX1), &ctrlpos3);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_ENTRY3), FALSE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_GBOX1), FALSE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RECNAME3), FALSE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RNAME), FALSE);

	 hei = ctrlpos.cy - (ctrlpos3.y*3.1);
	 wid = ctrlpos.cx - (ctrlpos3.x*2);
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_GBOX1), (HWND)0,
			 0, 0, wid, hei, SWP_SIZE);

	 hei = (ctrlpos.cy-(ctrlpos2.y+(ctrlpos3.y*6.8)));
	 wid = (ctrlpos.cx-(ctrlpos2.x*2));
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_ENTRY3), (HWND)0,
			 0, 0, wid, hei, SWP_SIZE);

	 hei = (ctrlpos3.y*1.9);
	 wid = (ctrlpos.cx-(ctrlpos2.x*7));
	 up = (ctrlpos.cy-(6*ctrlpos3.y));
	 over = (ctrlpos3.x*7.6);
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_RECNAME3), (HWND)0,
			 over, up, wid, hei, SWP_SIZE | SWP_MOVE);

	 up = (ctrlpos.cy-(6*ctrlpos3.y));
	 over = (ctrlpos3.x *2.1);
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_RNAME), (HWND)0,
			 over, up, 0, 0, SWP_MOVE);

	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_ENTRY3), TRUE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_GBOX1), TRUE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RECNAME3), TRUE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RNAME), TRUE);
	 }
      return (WinDefDlgProc(hwndDlg, msg, mp1, mp2));

   case WM_ADJUSTWINDOWPOS :
      if( ((PSWP)mp1)->fl & SWP_MINIMIZE )
	 {
	 WinSetWindowPos(hwndFrame, (HWND)0, 0, 0, 0, 0, SWP_MINIMIZE);
	 WinShowWindow(WinQueryWindow(hwndDlg, QW_OWNER), FALSE);
	 }
      else
	 if( ((PSWP)mp1)->fl & SWP_RESTORE )
	 {
	 WinSetWindowPos(hwndFrame, (HWND)0, 0, 0, 0, 0, SWP_RESTORE);
	 WinShowWindow(WinQueryWindow(hwndDlg, QW_OWNER), TRUE);
	 }
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

    case WM_CONTEXTMENU :
      {
      POINTL ptlMouse;

      WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
      WinPopupMenu(HWND_DESKTOP,
		   hwndDlg,
		   hwndEMLEPopup,
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
	 case ID_OK3 :
	    {
	    PBYTE pText;

	    if( WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY3)) > 0 )
	       {
	       MLEGetItemTextS(hwndDlg, ID_ENTRY3, 0, -1, &pText);
	       strcpy(dataRecs.noteText, pText);
	       free(pText);
	       }
	    else
	       strcpy(dataRecs.noteText, "");
	    }
	    WinQueryWindowText(WinWindowFromID(hwndDlg, ID_RECNAME3),
				sizeof(dataRecs.noteName),
				dataRecs.noteName);
	    if( strlen(dataRecs.noteName) < 3 )
	       {
	       if ( WinMessageBox(HWND_DESKTOP,
		       hwndDlg,
		       "You must enter a Record Name at least 3 characters long to save a Record",
		       "Error ...",
		       0,
		       MB_ICONEXCLAMATION | MB_OKCANCEL) == MBID_CANCEL )
		  {
		  strcpy(schText, xtext);
		  WinDismissDlg(hwndDlg, FALSE);
		  return (MRESULT) FALSE;
		  }
	       else
		  {
		  WinSetFocus(HWND_DESKTOP,
			      WinWindowFromID(hwndDlg, ID_RECNAME3));
		  return (MRESULT) FALSE;
		  }
	       }
	    z = chkName(hwndDlg, dataRecs.noteName);
	    if( z != -1 && z != choice && !ESEARCH && !NSEARCH )
	       {
	       if ( WinMessageBox(HWND_DESKTOP,
		       hwndDlg,
		       "Duplicate Record Name! Please use another name.",
		       "Error ...",
		       0,
		       MB_ICONEXCLAMATION | MB_OKCANCEL) == MBID_CANCEL )
		  {
		  strcpy(schText, xtext);
		  WinDismissDlg(hwndDlg, FALSE);
		  return (MRESULT) FALSE;
		  }
	       else
		  {
		  WinSetFocus(HWND_DESKTOP,
			      WinWindowFromID(hwndDlg, ID_RECNAME3));
		  return (MRESULT) FALSE;
		  }
	       }
	    pdest = 0;
	    stT = 0;
	    m = 0;
	    CASE = 0;
	    strcpy(schText, xtext);
	    if( ESEARCH || NSEARCH )
	       {
	       INT nnCho, mn;

	       // strcpy(oldRecName, dataRecs.noteName);
	       mn = matchAnyName(hwndResults, oldRecName);
	       WinQueryWindowText(WinWindowFromID(hwndDlg, ID_RECNAME3),
				  sizeof(dataRecs.noteName),
				  dataRecs.noteName);
	       nnCho = compIndexName(hwndDlg, oldRecName);
	       // nnCho = compIndexName(hwndDlg, dataRecs.noteName);
	       strcpy(recIndex[nnCho].recName, dataRecs.noteName);
	       // strcpy(recIndex[nnCho].recName, oldRecName);
	       multiWrite(datFile, 1, FIXEDLEN, nnCho);
	       saveIndex(datFile);
	       WinSetLboxItemText(hwndResults, mn, dataRecs.noteName);
	       }
	    WinDismissDlg( hwndDlg, TRUE );
	    break;

	 case ID_CANCEL3 :
	    pdest = 0;
	    stT = 0;
	    m = 0;
	    strcpy(schText, xtext);
	    WinDismissDlg( hwndDlg, FALSE );
	    break;

	 case ID_UNDOOP :
	    {
	    INT doable;

	    doable = SHORT1FROMMR (WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY3), MLM_QUERYUNDO,
					(MPARAM) 0L, (MPARAM) 0L));
	    if( doable )
	       WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY3), MLM_UNDO, (MPARAM) 0L, (MPARAM) 0L);
	    else
	       DosBeep(400, 100);
	    WinEnableMenuItem(hwndEMLEPopup, ID_UNDOOP, FALSE);
	    }
	    break;

	 case ID_SELALL :
	    {
	    INT wlen;

	    wlen = (ULONG)WinSendDlgItemMsg(hwndDlg, ID_ENTRY3,
					    MLM_QUERYFORMATTEXTLENGTH ,
					    MPFROMLONG(0),
					    MPFROMLONG(-1));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3), MLM_SETSEL ,
			      MPFROM2SHORT(0,wlen), MPVOID  ) ;
	    WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwndDlg, ID_ENTRY3));
	    }
	    break;

	 case ID_DTSTAMP :
	    setDateTime(hwndDlg, ID_ENTRY3, pgmData[33], pgmData[34], pgmData[35]);
	    break;

	 case ID_GETLINE :
	    {
	    LONG minT;
	    UINT zP;
	    CHAR lineNum[80] = "Line : ";
	    CHAR static szNum[6];

	    minT = LONGFROMMR(WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY3),
					  MLM_QUERYSEL,
					  (MPARAM) MLFQS_MINSEL, (MPARAM) 0L));
	    zP = (int) LONGFROMMR(WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
					     MLM_LINEFROMCHAR,
					     MPFROMLONG (minT),
					     (MPARAM) 0L));
	    itoa(zP+1,szNum,10);
	    strcat(lineNum, szNum);
	    WinSetWindowText(WinWindowFromID(hwndDlg, ID_MODEDIT), lineNum);
	    }
	    break;

	 case ID_ENDE :
	    zz = WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY3));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
		       MLM_SETSEL,
		       MPFROMP(zz),
		       MPFROMP(zz));
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_HOMEE :
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
		       MLM_SETSEL,
		       MPFROMP(0),
		       MPFROMP(0));
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_SAVENOWE :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    WinQueryDlgItemText(hwndDlg,
				ID_RECNAME3,
				sizeof(dataRecs.noteName),
				dataRecs.noteName);
	    if( strlen(dataRecs.noteName) < 3 )
	       {
	       WinMessageBox(HWND_DESKTOP,
			     hwndDlg,
			     "Record name must be at least 3 characters long ...",
			     "Attention!",
			     0,
			     MB_ICONEXCLAMATION | MB_OK );
	       WinSetFocus(HWND_DESKTOP,
			   WinWindowFromID(hwndDlg, ID_RECNAME3));
	       return(0);
	       }
	    {
	    PBYTE pText;

	    if( WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY3)) > 0 )
	       {
	       MLEGetItemTextS(hwndDlg, ID_ENTRY3, 0, -1, &pText);
	       strcpy(dataRecs.noteText, pText);
	       free(pText);
	       }
	    else
	       strcpy(dataRecs.noteText, "");
	    }
	    if( ESEARCH || NSEARCH )
	       {
	       INT nnCho;
	       INT mn;


	       // strcpy(oldRecName, dataRecs.noteName);
	       mn = matchAnyName(hwndResults, oldRecName);
	       WinQueryWindowText(WinWindowFromID(hwndDlg, ID_RECNAME3),
				  sizeof(dataRecs.noteName),
				  dataRecs.noteName);
	       nnCho = compIndexName(hwndDlg, oldRecName);
	       // nnCho = compIndexName(hwndDlg, dataRecs.noteName);
	       strcpy(recIndex[nnCho].recName, dataRecs.noteName);
	       // strcpy(recIndex[nnCho].recName, oldRecName);
	       multiWrite(datFile, 1, FIXEDLEN, nnCho);
	       saveIndex(datFile);
	       WinSetLboxItemText(hwndResults, mn, dataRecs.noteName);
	       strcpy(oldRecName, dataRecs.noteName);
	       }
	    else
	       {
	       multiWrite(datFile, 1, FIXEDLEN, nCho);
	       strcpy(recIndex[nCho].recName, dataRecs.noteName);
	       saveIndex(datFile);
	       }
	    DosBeep(900, 50);
	    DosBeep(1200, 50);
	    DosBeep(1500, 50);
	    WinSendMsg(hwndDlg, WM_COMMAND, MPFROMP(ID_CLEARVAR), 0);
	    WinSendMsg(hwndFrame, WM_COMMAND, MPFROM2SHORT(ID_LOADINDX, 0), 0);
	    zz = matchName(hwndFrame, recIndex[nCho].recName);
	    WinSendMsg(WinWindowFromID(hwndMainDlg,ID_LISTBOX1),
				       LM_SELECTITEM,
				       MPFROMSHORT(zz),
				       MPFROMSHORT(TRUE));
	    break;

	 case ID_COPY :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3), MLM_COPY, NULL, NULL);
	    break;

	 case ID_PASTE :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3), MLM_PASTE, NULL, NULL);
	    z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY3));
	    WinSetDlgItemShort(hwndDlg, ID_BYTESED, (SHORT)z, TRUE);
	    pdest = 0;
	    stT = 0;
	    m = 0;
	    CASE = 0;
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_CUT :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3), MLM_CUT, NULL, NULL);
	    z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY3));
	    WinSetDlgItemShort(hwndDlg, ID_BYTESED, (SHORT)z, TRUE);
	    pdest = 0;
	    stT = 0;
	    m = 0;
	    CASE = 0;
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_CLEAR :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3), MLM_CLEAR, NULL, NULL);
	    z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY3));
	    WinSetDlgItemShort(hwndDlg, ID_BYTESED, (SHORT)z, TRUE);
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
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
	    WinSendMsg(hwndMainDlg, WM_COMMAND,
		       MPFROM2SHORT(ID_WORDWRAP, 0), NULL);
	    WinSendDlgItemMsg(hwndDlg, ID_ENTRY3,MLM_SETWRAP,
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

	 case ID_PRINTEDIT :
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3), MLM_FORMAT,
		       MPFROMSHORT (MLFIE_WINFMT), (MPARAM) 0L);
	    if( pgmData[13] )
	       printIt(hwndDlg, ID_ENTRY3, TRUE);
	    else
	       {
	       printIt(hwndDlg, ID_ENTRY3, FALSE);
	       DosBeep(300, 50);
	       DosBeep(1000, 70);
	       DosBeep(1500, 90);
	       DosBeep(2200, 130);
	       }
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3), MLM_FORMAT,
		       MPFROMSHORT (MLFIE_CFTEXT), (MPARAM) 0L);
	    break;

	 case ID_IMPORTDROPE :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    z = WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY3));
	    _dos_findfirst(fullFileName,_A_NORMAL,&ffblk);
	    if( ffblk.size >= ( FIXEDLEN - z ) )
	       {
	       if( (FIXEDLEN - (z+1)) == 0 )
		  strcpy(errMsg2, "Record full! Start a new record.");
	       else
		  {
		  itoa(FIXEDLEN - (z+1), maxSize, 10);
		  strcpy(errMsg2, "File size must be equal to or less than ");
		  strcat(errMsg2, maxSize);
		  strcat(errMsg2, " bytes to import!");
		  }
	       WinMessageBox(HWND_DESKTOP,
		       hwndDlg,
		       errMsg2,
		       "Error ...",
		       0,
		       MB_ICONEXCLAMATION | MB_OK);
	       return(0);
	       }
	    zeroDataRecs(dataR.noteText);
	    fHan = open(fullFileName, O_RDONLY | O_BINARY);
	    read(fHan, dataR.noteText, ffblk.size);
	    close(fHan);
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3), MLM_INSERT, PVOIDFROMMP(dataR.noteText), NULL);
	    zeroDataRecs(dataR.noteText);
	    pdest = 0;
	    stT = 0;
	    m = 0;
	    CASE = 0;
	    z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY3));
	    WinSetDlgItemShort(hwndDlg, ID_BYTESED, (SHORT)z, TRUE);
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_IMPORTFILE :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    if( !FindFile(hwndDlg) )
	       return(0);
	    z = WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY3));
	    _dos_findfirst(fullFileName,_A_NORMAL,&ffblk);
	    if( ffblk.size >= ( FIXEDLEN - z ) )
	       {
	       if( (FIXEDLEN - (z+1)) == 0 )
		  strcpy(errMsg2, "Record full! Start a new record.");
	       else
		  {
		  itoa(FIXEDLEN - (z+1), maxSize, 10);
		  strcpy(errMsg2, "File size must be equal to or less than ");
		  strcat(errMsg2, maxSize);
		  strcat(errMsg2, " bytes to import!");
		  }
	       WinMessageBox(HWND_DESKTOP,
		       hwndDlg,
		       errMsg2,
		       "Error ...",
		       0,
		       MB_ICONEXCLAMATION | MB_OK);
	       return(0);
	       }
	    zeroDataRecs(dataR.noteText);
	    fHan = open(fullFileName, O_RDONLY | O_BINARY);
	    read(fHan, dataR.noteText, ffblk.size);
	    close(fHan);
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3), MLM_INSERT, PVOIDFROMMP(dataR.noteText), NULL);
	    zeroDataRecs(dataR.noteText);
	    pdest = 0;
	    stT = 0;
	    m = 0;
	    CASE = 0;
	    z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY3));
	    WinSetDlgItemShort(hwndDlg, ID_BYTESED, (SHORT)z, TRUE);
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_SAVETODISK :
	    {
	    FILE *handS;

	    WinSendMsg(hwndDlg, WM_COMMAND, MPFROMP(ID_CLEARVAR), 0);
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    strcpy(docFlName, "");
	    if( !saveFileRec(hwndDlg) )
	       return(0);
	    {
	    PBYTE pText;

	    if( WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY3)) > 0 )
	       {
	       MLEGetItemTextS(hwndDlg, ID_ENTRY3, 0, -1, &pText);
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
				ID_RECNAME3,
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
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    }
	    break;

	 case ID_NMMATCHESE :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    stopPoint = LONGFROMMR(
			   WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
			   MLM_QUERYSEL,
			   (MPARAM) MLFQS_MAXSEL,
			   (MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
		       MLM_SETSEL,
		       MPFROMP(stopPoint),
		       MPFROMP(stopPoint));
	    DosBeep(2200, 30);
	    DosBeep(1200, 70);
	    DosBeep(800, 40);
	    pdest = 0;
	    stT = 0;
	    return(0);

	 case ID_STOPEDSEARCH :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    stopPoint = LONGFROMMR(
			   WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
			   MLM_QUERYSEL,
			   (MPARAM) MLFQS_MAXSEL,
			   (MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
		       MLM_SETSEL,
		       MPFROMP(stopPoint),
		       MPFROMP(stopPoint));
	    DosBeep(1000, 50);
	    DosBeep(1500, 70);
	    DosBeep(2200, 90);
	    DosBeep(3100, 130);
	    pdest = 0;
	    stT = 0;
	    m = 0;
	    CASE = 0;
	    strcpy(schText, xtext);
	    return(0);

	 case ID_CLEARVAR :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY3));
	    stopPoint = LONGFROMMR(
			   WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
			   MLM_QUERYSEL,
			   (MPARAM) MLFQS_MAXSEL,
			   (MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
		       MLM_SETSEL,
		       MPFROMP(stopPoint),
		       MPFROMP(stopPoint));
	    pdest = 0;
	    stT = 0;
	    m = 0;
	    CASE = 0;
	    strcpy(schText, xtext);
	    return(0);

	 case ID_SEARCHED :
	    if( WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY3) ) <= 0 )
	       break;
	    if( m == 0 )
	       {
	       iz = 0;
	       if( eSearchRec(hwndDlg) == FALSE )
		  return(0);
	       strcpy(xtext, schText);
	       if( CASE == 0  )
		  strupr(schText);
	       m++;
	       }
	    sLen = strlen(schText);
	    min = 0;
	    WinSetFocus(HWND_DESKTOP,
			   WinWindowFromID(hwndDlg, ID_ENTRY3));
	    {
	    PBYTE pText;

	    MLEGetItemTextS(hwndDlg, ID_ENTRY3, 0, -1, &pText);
	    strcpy(dataR.noteText, pText);
	    free(pText);
	    }
	    if( CASE == 0  )
	       strupr(dataR.noteText);
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
		       MLM_SETSEL,
		       MPFROMP(-1),
		       MPFROMP(-1));
	    maxSel = LONGFROMMR(WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
				MLM_QUERYSEL,
				(MPARAM) MLFQS_MAXSEL,
				(MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
		       MLM_SETSEL,
		       MPFROMP(maxSel),
		       MPFROMP(maxSel));
	    min = LONGFROMMR (WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY3),
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
	       m = 0;
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
		     WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
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
	       WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_NMMATCHESE), 0);
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_SEARCHEDR :
	    if( WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY3) ) <= 0 )
	       break;
	    if( m == 0 )
	       {
	       iz = 0;
	       if( eSearchRec(hwndDlg) == FALSE )
		  return(0);
	       strcpy(xtext, schText);
	       if( CASE == 0 )
		  strupr(schText);
	       m++;
	       }
	    sLen = strlen(schText);
	    min = 0;
	    WinSetFocus(HWND_DESKTOP,
			   WinWindowFromID(hwndDlg, ID_ENTRY3));
	    {
	    PBYTE pText;

	    MLEGetItemTextS(hwndDlg, ID_ENTRY3, 0, -1, &pText);
	    strcpy(dataR.noteText, pText);
	    free(pText);
	    }
	    if( CASE == 0 )
	       strupr(dataR.noteText);
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
		       MLM_SETSEL,
		       MPFROMP(-1),
		       MPFROMP(-1));
	    maxSel = LONGFROMMR(WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
				MLM_QUERYSEL,
				(MPARAM) MLFQS_MAXSEL,
				(MPARAM) 0L));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
		       MLM_SETSEL,
		       MPFROMP(maxSel),
		       MPFROMP(maxSel));
	    min = LONGFROMMR (WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY3),
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
	       m = 0;
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
		     WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY3),
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
	       WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_NMMATCHESE), 0);
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


/* view window */
MRESULT EXPENTRY nViewProc( HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2 )
{
PFNWP oWinProc;
ULONG attrFound;
PDRAGINFO pdrginfox;
CHAR szFullDocNameS[MAXNAME];

oWinProc = (PFNWP) WinQueryWindowPtr(hwndWnd, 0);
switch(ulMsg)
   {
    case WM_CONTEXTMENU :
      {
      POINTL ptlMouse;

      WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
      WinPopupMenu(HWND_DESKTOP,
		   WinQueryWindow(hwndWnd, QW_OWNER),
		   hwndVMLEPopup,
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
      if( pgmData[4] )
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
      if( pgmData[4] )
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
      setPPMenuDef(4);
      WinQueryPresParam(hwndWnd, PP_FONTNAMESIZE, 0, &attrFound,
			    sizeof(pValue[0]), &pValue[0], QPF_NOINHERIT);
      WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndWnd, QW_PARENT), ID_RECNAME4 ),
			   PP_FONTNAMESIZE,
			   sizeof(pValue[0]),
			   &pValue[0] ) ;
      WinQueryPresParam(hwndWnd, PP_FOREGROUNDCOLOR, 0, &attrFound,
			    sizeof(pValue[1]), &pValue[1], QPF_NOINHERIT);
      // if( !pgmData[4] )
	 WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndWnd, QW_PARENT), ID_RECNAME4 ),
			      PP_FOREGROUNDCOLOR,
			      sizeof(pValue[1]),
			      &pValue[1] ) ;
      WinQueryPresParam(hwndWnd, PP_BACKGROUNDCOLOR, 0, &attrFound,
			    sizeof(pValue[2]), &pValue[2], QPF_NOINHERIT);
      // if( !pgmData[4] )
	 WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndWnd, QW_PARENT), ID_RECNAME4 ),
			      PP_BACKGROUNDCOLOR,
			      sizeof(pValue[2]),
			      &pValue[2] ) ;
      break;

   case WM_SINGLESELECT :
      WinPostMsg(WinQueryWindow(hwndWnd, QW_PARENT), WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
      break;

   case WM_CHAR :
      {
      // SHORT flags, key;

      // flags = SHORT1FROMMP(mp1);
      // key = SHORT2FROMMP(mp2);
      /* if( !(CHARMSG(&ulMsg)->fs & KC_KEYUP) )
	 break;
      if(CHARMSG(&ulMsg)->chr < 32 || CHARMSG(&ulMsg)->chr > 126)
	 WinPostMsg(WinQueryWindow(hwndWnd, QW_PARENT), WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0); */
      /* if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_RIGHT)
	 {
	 DosBeep(200, 100);
	 }
      if ( flags & KC_VIRTUALKEY && !(flags & KC_KEYUP) && key == VK_LEFT)
	 {
	 DosBeep(200, 100);
	 }   */
      /* if(CHARMSG(&ulMsg)->chr == 155)
	 DosBeep(200, 100);
      if(CHARMSG(&ulMsg)->chr == 157)
	 DosBeep(100, 100); */
      }
      break;

   case WM_BUTTON2MOTIONSTART:
      WinPostMsg(hwndWnd, WM_BEGINDRAG, 0, 0);
      break;

   case WM_BEGINDRAG:
	{
	DRAGITEM drgitem, *pdrgitem ;
	DRAGIMAGE drgimage ;
	CHAR szFullPathName[ MAXNAME] ;
	CHAR fileName[ MAXNAME] ;
	// HPOINTER  standard;
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


	if( standard  != NULLHANDLE )
	   break;
	if( WinQueryDlgItemTextLength(WinQueryWindow(hwndWnd, QW_PARENT),
					     ID_ENTRY4) <= 0 )
	   {
	   standard = NULLHANDLE;
	   break;
	   }
	standard = WinLoadPointer(HWND_DESKTOP, 0L, ID_POINTER2);
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

	if( WinQueryWindowTextLength(WinWindowFromID(WinQueryWindow(hwndWnd, QW_PARENT), ID_ENTRY4)) > 0 )
	   {
	   MLEGetItemTextS(WinQueryWindow(hwndWnd, QW_PARENT), ID_ENTRY4, 0, -1, &pText);
	   strcpy(dataRecs.noteText, pText);
	   free(pText);
	   }
	else
	   strcpy(dataRecs.noteText, "");
	}

	hand = fopen(szFullDocNameS, "w+b");
	addFWHeader(hand, dataRecs.noteName);
	if( hand == NULL )
	   {
	   standard = NULLHANDLE;
	   break;
	   }
	fwrite(&dataRecs.noteText, strlen(dataRecs.noteText), 1, hand);
	fclose(hand);

	pdrginfox = DrgAllocDraginfo( 1L) ;
	pdrginfox -> hwndSource = hwndWnd;

	drgitem.hwndItem = hwndWnd;
	drgitem.ulItemID = 100L ;
	drgitem.hstrType = DrgAddStrHandle( DRT_TEXT) ;
	drgitem.hstrRMF = DrgAddStrHandle( "(DRM_OS2FILE)x(DRF_TEXT,DRF_UNKNOWN)" ) ;
	drgitem.hstrContainerName = DrgAddStrHandle(szFullPathName) ;
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
		       NULL);
	if( dWnd == NULLHANDLE )
	   {
	   standard = NULLHANDLE;
	   DrgFreeDraginfo(pdrginfox);
	   remove(szFullDocNameS);
	   }
	else
	   {
	   dWnd = NULLHANDLE;
	   standard = NULLHANDLE;
	   }
	}
	break;

   case DM_ENDCONVERSATION:
	{
	standard = NULLHANDLE;
	DrgFreeDraginfo (pdrginfox) ;
	remove(szFullDocNameS);
	}
	break ;

   default :
      break;
   }
return( *oWinProc ) (hwndWnd, ulMsg, mp1, mp2);
}


/* edit window */
MRESULT EXPENTRY nCharProc( HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2 )
{
LONG z, len;
PFNWP oWinProc;
HWND hwndFF;
HAB habA;
PCHAR pchStr;
CHAR eMsg[] = "Not enough space in record to import data!";
CHAR eMsgE[] = "Clipboard is empty!";
ULONG attrFound;
CHAR szCont[MAXNAME] = "";
CHAR szSource[MAXNAME] = "";
PDRAGINFO pdrginfox;
CHAR szFullDocNameS[MAXNAME] ;


oWinProc = (PFNWP) WinQueryWindowPtr(hwndWnd, 0);
switch(ulMsg)
   {
    case WM_CONTEXTMENU :
      {
      POINTL ptlMouse;

      WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
      WinPopupMenu(HWND_DESKTOP,
		   WinQueryWindow(hwndWnd, QW_OWNER),
		   hwndEMLEPopup,
		   ptlMouse.x, ptlMouse.y,
		   0,
		   PU_HCONSTRAIN |
		   PU_VCONSTRAIN |
		   PU_KEYBOARD |
		   PU_MOUSEBUTTON2 |
		   PU_MOUSEBUTTON1 );
      }
      break;

/*   case MLM_SETBACKCOLOR :
      if( pgmData[5] )
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
      if( pgmData[5] )
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
   case WM_SINGLESELECT :
      WinPostMsg(WinQueryWindow(hwndWnd, QW_PARENT), WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
      break;

   case WM_PRESPARAMCHANGED :
      setPPMenuDef(5);
      WinQueryPresParam(hwndWnd, PP_FONTNAMESIZE, 0, &attrFound,
			    sizeof(pValue[3]), &pValue[3], QPF_NOINHERIT);
      WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndWnd, QW_PARENT), ID_RECNAME3 ),
			   PP_FONTNAMESIZE,
			   sizeof(pValue[3]),
			   &pValue[3] ) ;
      WinQueryPresParam(hwndWnd, PP_FOREGROUNDCOLOR, 0, &attrFound,
			    sizeof(pValue[4]), &pValue[4], QPF_NOINHERIT);
      // if( !pgmData[5] )
	 WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndWnd, QW_PARENT), ID_RECNAME3 ),
			      PP_FOREGROUNDCOLOR,
			      sizeof(pValue[4]),
			      &pValue[4] ) ;
      WinQueryPresParam(hwndWnd, PP_BACKGROUNDCOLOR, 0, &attrFound,
			    sizeof(pValue[5]), &pValue[5], QPF_NOINHERIT);
      // if( !pgmData[5] )
	 WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndWnd, QW_PARENT), ID_RECNAME3 ),
			      PP_BACKGROUNDCOLOR,
			      sizeof(pValue[5]),
			      &pValue[5] ) ;
      break;

   case WM_CHAR :
      if(CHARMSG(&ulMsg)->fs & KC_CHAR &&
	 CHARMSG(&ulMsg)->chr != '\x08' &&
	 CHARMSG(&ulMsg)->chr != '\x0d' )
	 {
	 hwndFF = WinQueryWindow(hwndWnd, QW_OWNER);
	 z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndFF, ID_ENTRY3));
	 if( z <= 0 )
	    {
	    DosBeep(300, 50);
	    return(0);
	    }
	 }
      if( CHARMSG(&ulMsg)->chr == '\x0d' )
	 {
	 hwndFF = WinQueryWindow(hwndWnd, QW_OWNER);
	 z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndFF, ID_ENTRY3));
	 if( z <= 1 )
	    {
	    DosBeep(300, 50);
	    return(0);
	    }
	 }
      if( !(CHARMSG(&ulMsg)->fs & KC_KEYUP) )
	 break;
      if(CHARMSG(&ulMsg)->chr < 32 || CHARMSG(&ulMsg)->chr > 126)
	 WinPostMsg(WinQueryWindow(hwndWnd, QW_PARENT), WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
      break;

   case MLM_PASTE :
      habA = WinQueryAnchorBlock(hwndWnd);
      WinOpenClipbrd(habA);
      pchStr = (PCHAR) WinQueryClipbrdData(habA, CF_TEXT);
      if( pchStr )
	 len = strlen(pchStr);
      else
	 len = 0;
      WinCloseClipbrd(habA);
      hwndFF = WinQueryWindow(hwndWnd, QW_OWNER);
      if( len <= 0 )
	 {
	 WinMessageBox(HWND_DESKTOP,
		       hwndFF,
		       eMsgE,
		       "Error ...",
		       0,
		       MB_ICONEXCLAMATION | MB_OK);
	 return MRFROMSHORT(TRUE);
	 }
      z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndFF, ID_ENTRY3));
      if( z <= 0 || z < len )
	 {
	 WinMessageBox(HWND_DESKTOP,
		       hwndFF,
		       eMsg,
		       "Error ...",
		       0,
		       MB_ICONEXCLAMATION | MB_OK);
	 return MRFROMSHORT(TRUE);
	 }
      WinPostMsg(WinQueryWindow(hwndWnd, QW_PARENT), WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
      break;

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

      if( pdrginfo->hwndSource == hwndWnd )
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
      WinPostMsg(WinQueryWindow(hwndWnd, QW_PARENT), WM_COMMAND, MPFROMP(ID_IMPORTDROPE), 0L);
      break;

   case WM_BEGINDRAG:
	{
	DRAGITEM drgitem, *pdrgitem ;
	DRAGIMAGE drgimage ;
	CHAR szFullPathName[ MAXNAME] ;
	CHAR fileName[ MAXNAME] ;
	// HPOINTER  standard;
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


	if( standard  != NULLHANDLE )
	   break;
	if( WinQueryDlgItemTextLength(WinQueryWindow(hwndWnd, QW_PARENT),
					     ID_ENTRY3) <= 0 )
	   {
	   standard = NULLHANDLE;
	   break;
	   }
	standard = WinLoadPointer(HWND_DESKTOP, 0L, ID_POINTER2);
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

	if( WinQueryWindowTextLength(WinWindowFromID(WinQueryWindow(hwndWnd, QW_PARENT), ID_ENTRY3)) > 0 )
	   {
	   MLEGetItemTextS(WinQueryWindow(hwndWnd, QW_PARENT), ID_ENTRY3, 0, -1, &pText);
	   strcpy(dataRecs.noteText, pText);
	   free(pText);
	   }
	else
	   strcpy(dataRecs.noteText, "");
	}

	hand = fopen(szFullDocNameS, "w+b");
	addFWHeader(hand, dataRecs.noteName);
	if( hand == NULL )
	   {
	   standard = NULLHANDLE;
	   break;
	   }
	fwrite(&dataRecs.noteText, strlen(dataRecs.noteText), 1, hand);
	fclose(hand);

	pdrginfox = DrgAllocDraginfo( 1L) ;
	pdrginfox -> hwndSource = hwndWnd;

	drgitem.hwndItem = hwndWnd;
	drgitem.ulItemID = 100L ;
	drgitem.hstrType = DrgAddStrHandle( DRT_TEXT) ;
	drgitem.hstrRMF = DrgAddStrHandle( "(DRM_OS2FILE)x(DRF_TEXT,DRF_UNKNOWN)" ) ;
	drgitem.hstrContainerName = DrgAddStrHandle( szFullPathName) ;
	drgitem.hstrSourceName = DrgAddStrHandle( targetName) ;
	drgitem.hstrTargetName = DrgAddStrHandle( targetName) ;
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
	   standard = NULLHANDLE;
	   DrgFreeDraginfo(pdrginfox);
	   remove(szFullDocNameS);
	   }
	else
	   {
	   dWnd = NULLHANDLE;
	   standard = NULLHANDLE;
	   }
	}
	break;

   case DM_ENDCONVERSATION:
	{
	standard = NULLHANDLE;
	DrgFreeDraginfo (pdrginfox) ;
	remove(szFullDocNameS);
	}
	break ;

   default :
      break;
   }
return( *oWinProc ) (hwndWnd, ulMsg, mp1, mp2);
}


MRESULT EXPENTRY newRecName3Proc( HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2 )
{
PFNWP oldRecName3Proc;

oldRecName3Proc = (PFNWP) WinQueryWindowPtr(hwndWnd, 0);
switch(ulMsg)
   {
   case WM_CHAR :
      if( CHARMSG(&ulMsg)->chr == '\x0d' )
	 {
	 WinSetFocus(HWND_DESKTOP,
	    WinWindowFromID(WinQueryWindow(hwndWnd, QW_PARENT), ID_ENTRY3));
	 }
      if( !(CHARMSG(&ulMsg)->fs & KC_KEYUP) )
	 break;
      break;

   default :
      break;
   }
return( *oldRecName3Proc ) (hwndWnd, ulMsg, mp1, mp2);
}






