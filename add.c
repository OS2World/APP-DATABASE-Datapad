/* add.c  2.1 7-17-95 */

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

MRESULT EXPENTRY addRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY newCharProc( HWND hwndDlg, ULONG msg,
			      MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY newRecNameProc( HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2 );
INT addRec(HWND hwndDlg);

extern VOID setStatus(HWND hwndST, CHAR *pFile);
extern VOID saveIndex(CHAR *dataFileName);
extern INT matchName(HWND hwndM, CHAR *lbSelect);
extern VOID setPPMenuDef(INT num);
// extern VOID setDateTime(HWND hwnd, ULONG ID);
extern VOID setDateTime(HWND hwnd, ULONG ID, INT mode, BOOL AMPM, BOOL DAYNAME);
extern ULONG MLEInsertTextS(HWND hwndDlg, ULONG ulID, LONG iptPosition, PBYTE pText);
extern INT printIt(HWND hwndP, USHORT id, BOOL ASK);
extern INT openFile(CHAR *fileName, CHAR *mode);
extern VOID closeFile(VOID);
extern BOOL FindFile (HWND hwndWnd);
extern VOID zeroDataRecs(CHAR *noteText);
extern INT chkName(HWND hwnd, CHAR *lbSelect);
extern VOID setParams(HWND hwndP, SHORT dx, ULONG IDE, ULONG IDR);
extern VOID addFWHeader(FILE *hand, CHAR *dRec);
extern HFILE openTmp(HFILE hfile, CHAR *szFullPathName, CHAR *szFullDocName, CHAR *fileName);
extern INT multiRead(CHAR *inFile, INT datS, INT sizeF, INT recNum);
extern INT multiWrite(CHAR *inFile, INT datS, INT sizeF, INT recNum);
extern INT setRGBColor(HWND hwndC, ULONG r, ULONG rg, ULONG rb);
extern ULONG MLEGetItemTextS(HWND hwndDlg, ULONG ulID, LONG lIptPosition,
			    LONG lBoundary, PBYTE *ppText);

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
extern FILE *datHan;
extern CHAR fullFileName[MAXNAME];
extern INT WW;
extern struct find_t ffblk;
extern ULONG pValue[16][32];
extern HOBJECT hobjBG;
extern HOBJECT hobjFNT;
extern pgmData[60];
extern CHAR datFile[MAXNAME];
extern HWND hwndAMLEPopup;
extern HWND hwndFrame, hwndMenu;
extern ULONG scrLen, scrWid;
extern UINT FIXEDLEN;
extern HWND hwndMLBox;
extern INT nCho;
extern CHAR EXTENTION[EXTSIZE];
extern HWND hwndMainDlg;

HPOINTER  standardx;


INT addRec(HWND hwndDlg)
{
if ( WinDlgBox(HWND_DESKTOP,
               hwndDlg,
	       addRecProc,
               (HMODULE)0,
               ID_ADDREC,
               NULL) == TRUE )
   return(1);
else
   return(0);
}


MRESULT EXPENTRY addRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
INT p, r;
CHAR flr[6];
// CHAR oRName[NAMESIZE];
CHAR szModName[75] = "Add Record       Module : ";
INT cnt;
INT fHan;
CHAR maxSize[15] = "";
CHAR errMsg2[60] = "";
ULONG z;
PFNWP oldWinProc;
PFNWP oldRecNameProc;
PID pida;
SWCNTRL swctla;
PSWP pOldSize;
PSWP pNewSize;
SWP ctrlpos, ctrlpos2, ctrlpos3;
SWP swp;
PSWP pswp;
SHORT key, flags;


pNewSize = (PSWP)mp1;
pOldSize = pNewSize + 1;
switch ( msg )
   {
   case WM_INITDLG:
      WinSendDlgItemMsg(hwndDlg, ID_RECNAME2,EM_SETTEXTLIMIT,
		  MPFROM2SHORT(NAMESIZE,0),NULL);
      WinSendDlgItemMsg(hwndDlg, ID_ENTRY2,MLM_SETTEXTLIMIT,
		  MPFROM2SHORT(FIXEDLEN,0),NULL);
      WinSendDlgItemMsg(hwndDlg, ID_ENTRY2,MLM_SETWRAP,
		  MPFROM2SHORT(WW,0),NULL);
      zeroDataRecs(dataR.noteText);
      WinPostMsg(hwndDlg, WM_CONTROL, MPFROM2SHORT(ID_ENTRY2, MLN_CHANGE), 0);
      oldWinProc = WinSubclassWindow(WinWindowFromID(hwndDlg, ID_ENTRY2),
				     newCharProc);
      WinSetWindowPtr(WinWindowFromID(hwndDlg, ID_ENTRY2),
		      0, (PVOID) oldWinProc);
      oldRecNameProc = WinSubclassWindow(WinWindowFromID(hwndDlg, ID_RECNAME2),
				     newRecNameProc);
      WinSetWindowPtr(WinWindowFromID(hwndDlg, ID_RECNAME2),
		      0, (PVOID) oldRecNameProc);
      WinQueryWindowProcess(hwndDlg, &pida, NULL);
      swctla.hwnd = hwndDlg;                /* window handle      */
      swctla.hwndIcon = NULLHANDLE;         /* icon handle        */
      swctla.hprog = NULLHANDLE;            /* program handle     */
      swctla.idProcess = pida;               /* process identifier */
      swctla.idSession = 0;                 /* session identifier */
      swctla.uchVisibility = SWL_VISIBLE;   /* visibility         */
      swctla.fbJump = SWL_JUMPABLE;         /* jump indicator     */
      strcpy(swctla.szSwtitle, "DataPad Add Window");
      WinAddSwitchEntry(&swctla);
      if( !pgmData[7] )
	 setParams(hwndDlg, 9, ID_ENTRY2, ID_RECNAME2);
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
      hwndAMLEPopup = WinLoadMenu(WinWindowFromID(hwndDlg, ID_OK2),
				  0,
				  ID_AMLEPOPUP);
      WinSetWindowPos(hwndDlg, HWND_TOP,
		      pgmData[29], pgmData[30], pgmData[27], pgmData[28],
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
      if( pgmData[31] )
	 WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_DTSTAMP), 0);
      WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY2), MLM_RESETUNDO, (MPARAM) 0L, (MPARAM) 0L);
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
	       WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_OK2), 0);
	       return(0);

	    case MBID_NO:
	       WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_CANCEL2), 0);
	       break;

	    case MBID_CANCEL:
	       return(MRESULT) FALSE;
	    }
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
	    WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_OK2), 0);
	    break;

	 case MBID_NO:
	    WinSendMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_CANCEL2), 0);
	    break;

	 case MBID_CANCEL:
	    return (MRESULT) FALSE;

	 default:
	    break;
	 }
      }
      break;

   case WM_DESTROY :
      {
      if( WinQueryWindowULong(hwndDlg, QWL_STYLE) & WS_MAXIMIZED )
	 {
	 pgmData[27] = WinQueryWindowULong(hwndDlg, QWS_CXRESTORE);
	 pgmData[28] = WinQueryWindowULong(hwndDlg, QWS_CYRESTORE);
	 pgmData[29] = WinQueryWindowULong(hwndDlg, QWS_XRESTORE);
	 pgmData[30] = WinQueryWindowULong(hwndDlg, QWS_YRESTORE);
	 }
      else
	 {
	 if( !(WinQueryWindowULong(hwndDlg, QWL_STYLE) & WS_MINIMIZED) )
	    {
	    WinQueryWindowPos(hwndDlg, &ctrlpos);
	    pgmData[27] = ctrlpos.cx;
	    pgmData[28] = ctrlpos.cy;
	    pgmData[29] = ctrlpos.x;
	    pgmData[30] = ctrlpos.y;
	    }
	 }
      }
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
	    pti->ptlMinTrackSize.x = scrWid * .465;
	    pti->ptlMinTrackSize.y = scrLen * .42;
	    break;

	 case 800 :
	    pti->ptlMinTrackSize.x = scrWid * .372;
	    pti->ptlMinTrackSize.y = scrLen * .335;
	    break;

	 default :
	    pti->ptlMinTrackSize.x = scrWid * .383;
	    pti->ptlMinTrackSize.y = scrLen * .32;
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
	 WinQueryWindowPos(WinWindowFromID(hwndDlg, ID_ENTRY2), &ctrlpos2);
	 WinQueryWindowPos(WinWindowFromID(hwndDlg, ID_AGBOX1), &ctrlpos3);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_ENTRY2), FALSE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_AGBOX1), FALSE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RECNAME2), FALSE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RNAME), FALSE);

	 hei = ctrlpos.cy - (ctrlpos3.y*3.1);
	 wid = ctrlpos.cx - (ctrlpos3.x*2);
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_AGBOX1), (HWND)0,
			 0, 0, wid, hei, SWP_SIZE);

	 hei = (ctrlpos.cy-(ctrlpos2.y+(ctrlpos3.y*6.4)));
	 wid = (ctrlpos.cx-(ctrlpos2.x*2));
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_ENTRY2), (HWND)0,
			 0, 0, wid, hei, SWP_SIZE);

	 hei = (ctrlpos3.y*1.9);
	 wid = (ctrlpos.cx-(ctrlpos2.x*7));
	 up = (ctrlpos.cy-(5.6*ctrlpos3.y));
	 over = (ctrlpos3.x*8.7);
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_RECNAME2), (HWND)0,
			 over, up, wid, hei, SWP_SIZE | SWP_MOVE);

	 up = (ctrlpos.cy-(5.8*ctrlpos3.y));
	 over = (ctrlpos3.x *2.3);
	 WinSetWindowPos(WinWindowFromID(hwndDlg, ID_RNAME), (HWND)0,
			 over, up, 0, 0, SWP_MOVE);

	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_ENTRY2), TRUE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_AGBOX1), TRUE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RECNAME2), TRUE);
	 WinEnableWindowUpdate(WinWindowFromID(hwndDlg, ID_RNAME), TRUE);
	 }
      return (WinDefDlgProc(hwndDlg, msg, mp1, mp2));


   case WM_CONTEXTMENU :
      {
      POINTL ptlMouse;

      WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
      WinPopupMenu(HWND_DESKTOP,
		   hwndDlg,
		   hwndAMLEPopup,
		   ptlMouse.x, ptlMouse.y,
		   0,
		   PU_HCONSTRAIN |
		   PU_VCONSTRAIN |
		   PU_KEYBOARD |
		   PU_MOUSEBUTTON2 |
		   PU_MOUSEBUTTON1 );
      }
      break;


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

   case WM_CONTROL:
      switch (SHORT2FROMMP (mp1))
	 {
	 case MLN_CHANGE:
	    if (SHORT1FROMMP (mp1) == ID_ENTRY2)
	       {
	       INT doable;

	       doable = SHORT1FROMMR (WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY2), MLM_QUERYUNDO,
					(MPARAM) 0L, (MPARAM) 0L));
	       if( doable )
		  WinEnableMenuItem(hwndAMLEPopup, ID_UNDOOP, TRUE);
	       else
		  WinEnableMenuItem(hwndAMLEPopup, ID_UNDOOP, FALSE);
	       z = (FIXEDLEN -1) - WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY2));
	       WinSetDlgItemShort(hwndDlg, ID_BYTESADD, (SHORT)z, TRUE);
	       }
	       return 0;

	    default:
	       break;
	 }
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case ID_OK2 :
	    cnt = WinQueryLboxCount(WinWindowFromID(hwndMainDlg, ID_LISTBOX1));
	    WinQueryDlgItemText(hwndDlg,
				ID_RECNAME2,
				sizeof(dataRecs.noteName),
				dataRecs.noteName);
	    {
	    PBYTE pText;

	    if( WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY2)) > 0 )
	       {
	       MLEGetItemTextS(hwndDlg, ID_ENTRY2, 0, -1, &pText);
	       strcpy(dataRecs.noteText, pText);
	       free(pText);
	       }
	    else
	       strcpy(dataRecs.noteText, "");
	    }
	    if( strlen(dataRecs.noteName) < 3 )
	       {
	       if ( WinMessageBox(HWND_DESKTOP,
		       hwndDlg,
		       "You must enter a Record Name at least 3 characters long to add a Record",
		       "Error ...",
		       0,
		       MB_ICONEXCLAMATION | MB_OKCANCEL) == MBID_CANCEL )
		  {
		  WinDismissDlg(hwndDlg, FALSE);
		  return (MRESULT) FALSE;
		  }
	       else
		  {
		  WinSetFocus(HWND_DESKTOP,
			      WinWindowFromID(hwndDlg, ID_RECNAME2));
		  return (MRESULT) FALSE;
		  }
	       }
	    if( chkName(hwndDlg, dataRecs.noteName) != -1 )
	       {
	       if ( WinMessageBox(HWND_DESKTOP,
		       hwndDlg,
		       "Duplicate Record Name! Please use another name.",
		       "Error ...",
		       0,
		       MB_ICONEXCLAMATION | MB_OKCANCEL) == MBID_CANCEL )
		  {
		  WinDismissDlg(hwndDlg, FALSE);
		  return (MRESULT) FALSE;
		  }
	       else
		  {
		  WinSetFocus(HWND_DESKTOP,
			      WinWindowFromID(hwndDlg, ID_RECNAME2));
		  return (MRESULT) FALSE;
		  }
	       }
	    WinDismissDlg( hwndDlg, TRUE );
	    recIndex[cnt].posIndex = cnt;
	    strcpy(recIndex[cnt].recName, dataRecs.noteName);
	    break;

	 case ID_DTSTAMP :
	    setDateTime(hwndDlg, ID_ENTRY2, pgmData[33], pgmData[34], pgmData[35]);
	    break;

	 case ID_CANCEL2 :
	    WinDismissDlg( hwndDlg, FALSE );
	    break;

	 case ID_UNDOOP :
	    {
	    INT doable;

	    doable = SHORT1FROMMR (WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY2), MLM_QUERYUNDO,
					(MPARAM) 0L, (MPARAM) 0L));
	    if( doable )
	       WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY2), MLM_UNDO, (MPARAM) 0L, (MPARAM) 0L);
	    else
	       DosBeep(400, 100);
	    WinEnableMenuItem(hwndAMLEPopup, ID_UNDOOP, FALSE);
	    }
	    break;

	 case ID_SELALL :
	    {
	    INT wlen;

	    wlen = (ULONG)WinSendDlgItemMsg(hwndDlg, ID_ENTRY2,
					    MLM_QUERYFORMATTEXTLENGTH ,
					    MPFROMLONG(0),
					    MPFROMLONG(-1));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY2), MLM_SETSEL ,
			      MPFROM2SHORT(0,wlen), MPVOID  ) ;
	    WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwndDlg, ID_ENTRY2));
	    }
	    break;

	 case ID_COPYADD :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY2));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY2), MLM_COPY, NULL, NULL);
	    break;

	 case ID_PASTEADD :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY2));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY2), MLM_PASTE, NULL, NULL);
	    z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY2));
	    WinSetDlgItemShort(hwndDlg, ID_BYTESADD, (SHORT)z, TRUE);
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_CUTADD :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY2));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY2), MLM_CUT, NULL, NULL);
	    z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY2));
	    WinSetDlgItemShort(hwndDlg, ID_BYTESADD, (SHORT)z, TRUE);
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_PRINTADD :
	    {
	    PBYTE pText;

	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY2), MLM_FORMAT,
		       MPFROMSHORT (MLFIE_WINFMT), (MPARAM) 0L);
	    if( WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY2)) > 0 )
	       {
	       MLEGetItemTextS(hwndDlg, ID_ENTRY2, 0, -1, &pText);
	       strcpy(dataRecs.noteText, pText);
	       free(pText);
	       }
	    else
	       strcpy(dataRecs.noteText, "");
	    }
	    WinQueryDlgItemText(hwndDlg,
				ID_RECNAME2,
				sizeof(dataRecs.noteName),
				dataRecs.noteName);
	    if( pgmData[13] )
	       printIt(hwndDlg, ID_ENTRY2, TRUE);
	    else
	       {
	       printIt(hwndDlg, ID_ENTRY2, TRUE);
	       DosBeep(300, 50);
	       DosBeep(1000, 70);
	       DosBeep(1500, 90);
	       DosBeep(2200, 130);
	       }
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY2), MLM_FORMAT,
		       MPFROMSHORT (MLFIE_CFTEXT), (MPARAM) 0L);
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY2));
	    break;

	 case ID_WWTOG :
	    WinSendMsg(hwndFrame, WM_COMMAND,
		       MPFROM2SHORT(ID_WORDWRAP, 0), NULL);
	    WinSendDlgItemMsg(hwndDlg, ID_ENTRY2,MLM_SETWRAP,
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

	 case ID_CLEARADD :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY2));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY2), MLM_CLEAR, NULL, NULL);
	    z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY2));
	    WinSetDlgItemShort(hwndDlg, ID_BYTESADD, (SHORT)z, TRUE);
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_IMPORTDROP :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY2));
	    z = WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY2));
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
	    fHan = open(fullFileName, O_RDONLY | O_BINARY);
	    read(fHan, dataR.noteText, ffblk.size);
	    close(fHan);
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY2), MLM_INSERT, PVOIDFROMMP(dataR.noteText), NULL);
	    z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY2));
	    WinSetDlgItemShort(hwndDlg, ID_BYTESADD, (SHORT)z, TRUE);
	    zeroDataRecs(dataR.noteText);
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_GETFROMDISKAD :
	    WinSetFocus(HWND_DESKTOP,
			WinWindowFromID(hwndDlg, ID_ENTRY2));
	    if( !FindFile(hwndDlg) )
               return(0);
	    z = WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY2));
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
	    fHan = open(fullFileName, O_RDONLY | O_BINARY);
	    read(fHan, dataR.noteText, ffblk.size);
	    close(fHan);
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY2), MLM_INSERT, PVOIDFROMMP(dataR.noteText), NULL);
	    z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY2));
	    WinSetDlgItemShort(hwndDlg, ID_BYTESADD, (SHORT)z, TRUE);
	    zeroDataRecs(dataR.noteText);
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_GETLINE :
	    {
	    LONG minT;
	    UINT zP;
	    CHAR lineNum[80] = "Line : ";
	    static CHAR szNum[6];

	    minT = LONGFROMMR(WinSendMsg (WinWindowFromID(hwndDlg, ID_ENTRY2),
					  MLM_QUERYSEL,
					  (MPARAM) MLFQS_MINSEL, (MPARAM) 0L));
	    zP = (int) LONGFROMMR(WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY2),
					     MLM_LINEFROMCHAR,
					     MPFROMLONG (minT),
					     (MPARAM) 0L));
	    itoa(zP+1,szNum,10);
	    strcat(lineNum, szNum);
	    WinSetWindowText(WinWindowFromID(hwndDlg, ID_MODADD), lineNum);
	    }
	    break;

	 case ID_ENDA :
	    r = WinQueryWindowTextLength(WinWindowFromID(hwndDlg, ID_ENTRY2));
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY2),
		       MLM_SETSEL,
		       MPFROMP(r),
		       MPFROMP(r));
	    WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	    break;

	 case ID_HOMEA :
	    WinSendMsg(WinWindowFromID(hwndDlg, ID_ENTRY2),
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


MRESULT EXPENTRY newCharProc( HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2 )
{
LONG z, len;
PFNWP oldWinProc;
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

oldWinProc = (PFNWP) WinQueryWindowPtr(hwndWnd, 0);
switch(ulMsg)
   {
   case WM_CONTEXTMENU :
      {
      POINTL ptlMouse;

      WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
      WinPopupMenu(HWND_DESKTOP,
		   WinQueryWindow(hwndWnd, QW_OWNER),
		   hwndAMLEPopup,
		   ptlMouse.x, ptlMouse.y,
		   0,
		   PU_HCONSTRAIN |
		   PU_VCONSTRAIN |
		   PU_KEYBOARD |
		   PU_MOUSEBUTTON2 |
		   PU_MOUSEBUTTON1 );
      }
      break;

   case WM_CHAR :
      if(CHARMSG(&ulMsg)->fs & KC_CHAR &&
	 CHARMSG(&ulMsg)->chr != '\x08' &&
	 CHARMSG(&ulMsg)->chr != '\x0d' )
	 {
	 hwndFF = WinQueryWindow(hwndWnd, QW_OWNER);
	 z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndFF, ID_ENTRY2));
	 if( z <= 0 )
	    {
	    DosBeep(300, 50);
	    return(0);
	    }
	 }
      if( CHARMSG(&ulMsg)->chr == '\x0d' )
	 {
	 hwndFF = WinQueryWindow(hwndWnd, QW_OWNER);
	 z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndFF, ID_ENTRY2));
	 if( z <= 1 )
	    {
	    DosBeep(300, 50);
	    return(0);
	    }
	 }
      if( !(CHARMSG(&ulMsg)->fs & KC_KEYUP) )
	 break;
      if( CHARMSG(&ulMsg)->chr < 32 || CHARMSG(&ulMsg)->chr > 126  )
	 {
	 WinPostMsg(WinQueryWindow(hwndWnd, QW_PARENT), WM_COMMAND, MPFROMSHORT(ID_GETLINE), 0);
	 }
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
      z = (FIXEDLEN - 1) - WinQueryWindowTextLength(WinWindowFromID(hwndFF, ID_ENTRY2));
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
/*
   case MLM_SETBACKCOLOR :
      if( pgmData[7] )
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
      if( pgmData[7] )
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
      setPPMenuDef(7);
      WinQueryPresParam(hwndWnd, PP_FONTNAMESIZE, 0, &attrFound,
			    sizeof(pValue[9]), &pValue[9], QPF_NOINHERIT);
      WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndWnd, QW_PARENT), ID_RECNAME2 ),
			   PP_FONTNAMESIZE,
			   sizeof(pValue[9]),
			   &pValue[9] ) ;
      WinQueryPresParam(hwndWnd, PP_FOREGROUNDCOLOR, 0, &attrFound,
			    sizeof(pValue[10]), &pValue[10], QPF_NOINHERIT);
      // if( !pgmData[7] )
	 WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndWnd, QW_PARENT), ID_RECNAME2 ),
			      PP_FOREGROUNDCOLOR,
			      sizeof(pValue[10]),
			      &pValue[10] ) ;
      WinQueryPresParam(hwndWnd, PP_BACKGROUNDCOLOR, 0, &attrFound,
			    sizeof(pValue[11]), &pValue[11], QPF_NOINHERIT);
      // if( !pgmData[7] )
	 WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndWnd, QW_PARENT), ID_RECNAME2 ),
			      PP_BACKGROUNDCOLOR,
			      sizeof(pValue[11]),
			      &pValue[11] ) ;
      break;

   case WM_SINGLESELECT :
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
      WinPostMsg(WinQueryWindow(hwndWnd, QW_PARENT), WM_COMMAND, MPFROMP(ID_IMPORTDROP), 0L);
      break;

   case WM_BEGINDRAG:
	{
	DRAGITEM drgitem, *pdrgitem ;
	DRAGIMAGE drgimage ;
	HWND hwndTarget;
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


	if( standardx  != NULLHANDLE )
	   break;
	if( WinQueryDlgItemTextLength(WinQueryWindow(hwndWnd, QW_PARENT), ID_ENTRY2) <= 0 ||
	    WinQueryDlgItemTextLength(WinQueryWindow(hwndWnd, QW_PARENT), ID_RECNAME2) <= 0 )
	   {
	   standardx = NULLHANDLE;
	   break;
	   }
	standardx = WinLoadPointer(HWND_DESKTOP, 0L, ID_POINTER2);
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

	WinQueryDlgItemText(WinQueryWindow(hwndWnd, QW_PARENT),
				ID_RECNAME2,
				sizeof(dataRecs.noteName),
				dataRecs.noteName);
	strcpy(targetName, dataRecs.noteName);
	strcpy(szFullDocNameS, szFullPathName);
	strcat(szFullDocNameS, dataRecs.noteName);

	{
	PBYTE pText;

	if( WinQueryWindowTextLength(WinWindowFromID(WinQueryWindow(hwndWnd, QW_PARENT), ID_ENTRY2)) > 0 )
	   {
	   MLEGetItemTextS(WinQueryWindow(hwndWnd, QW_PARENT), ID_ENTRY2, 0, -1, &pText);
	   strcpy(dataRecs.noteText, pText);
	   free(pText);
	   }
	else
	   strcpy(dataRecs.noteText, "");
	}

	hand = fopen(szFullDocNameS, "w+b");
	if( hand == NULL )
	   {
	   standardx= NULLHANDLE;
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
	drgimage.hImage = standardx;
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
	   standardx = NULLHANDLE;
	   DrgFreeDraginfo(pdrginfox);
	   remove(szFullDocNameS);
	   }
	else
	   {
	   dWnd = NULLHANDLE;
	   standardx = NULLHANDLE;
	   }
	}
	break;

   case DM_ENDCONVERSATION:
	{
	standardx = NULLHANDLE;
	DrgFreeDraginfo (pdrginfox) ;
	remove(szFullDocNameS);
	}
	break ;

   default :
      break;
   }
return( *oldWinProc ) (hwndWnd, ulMsg, mp1, mp2);
}



MRESULT EXPENTRY newRecNameProc( HWND hwndWnd, ULONG ulMsg,
			      MPARAM mp1, MPARAM mp2 )
{
PFNWP oldRecNameProc;

oldRecNameProc = (PFNWP) WinQueryWindowPtr(hwndWnd, 0);
switch(ulMsg)
   {
   case WM_CHAR :
      if( CHARMSG(&ulMsg)->chr == '\x0d' )
	 {
	 WinSetFocus(HWND_DESKTOP,
	    WinWindowFromID(WinQueryWindow(hwndWnd, QW_PARENT), ID_ENTRY2));
	 }
      if( !(CHARMSG(&ulMsg)->fs & KC_KEYUP) )
	 break;
      break;

   default :
      break;
   }
return( *oldRecNameProc ) (hwndWnd, ulMsg, mp1, mp2);
}




