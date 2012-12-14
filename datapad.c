/* datapad.C  11-10-97 */

#define INCL_WIN
#define INCL_DOS
#define INCL_SUB
#define INCL_NLS
#define INCL_DOS
#define INCL_GPI
#define INCL_WINENTRYFIELDS
#define INCL_WINSTDDRAG
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS

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
#include "datapad.h"

PMAINFORMINFO pMainFormInfo;

int main(int argc, char *argv[], char *envp[])
{
  HAB   hab;
  QMSG  qmsg;
  HMQ  hmq;
  // HWND  hwndMainFrame;

  /* ##START Form.40 Top of application procedure */
  /* ##END Top of application procedure */

  hab = WinInitialize( 0 );
  if(!hab) {
    exit(1);
  }

  hmq = WinCreateMsgQueue( hab, 0 );
  if(!hmq) {
    WinTerminate(hab);
    exit(1);
  }
  pMainFormInfo = malloc(sizeof(MAINFORMINFO));
  memset((PVOID) pMainFormInfo, 0, sizeof(MAINFORMINFO));

  PrfQueryProfileString(HINI_USERPROFILE, "PAD21", "FILEEXT",
		      ".PDF", EXTENTION, sizeof(EXTENTION));
  if ( argc > 1 )
     {
     if( argc > 2 )
	{
	strcpy(Honor, argv[1]);
	strcpy(dFile, argv[2]);
	}
     else
	{
	if( strnicmp(argv[1]+(strlen(argv[1])-4), EXTENTION, 4) == 0 )
	   {
	   strcpy(dFile, argv[1]);
	   strcpy(Honor, "");
	   }
	else
	   {
	   strcpy(Honor, argv[1]);
	   strcpy(dFile, "");
	   }
	}
     }
  else
     {
     strcpy(Honor, "");
     strcpy(dFile, "");
     }
  // strcpy(Honor, "g...rit");
  // hab = WinInitialize(0);
  // hmq = WinCreateMsgQueue(hab, 80);


  hinit.cb = sizeof( HELPINIT);
  hinit.ulReturnCode = 0L;
  hinit.pszTutorialName = NULL;
  hinit.phtHelpTable = (PHELPTABLE)MAKELONG(HT_MAIN, 0xFFFF);
  hinit.hmodHelpTableModule = (HMODULE) NULL;
  hinit.hmodAccelActionBarModule = (HMODULE)NULL;
  hinit.idAccelTable = 0;
  hinit.idActionBar = 0;
  hinit.pszHelpWindowTitle = "Help for DataPad ver. 3.2";
  hinit.fShowPanelId = CMIC_HIDE_PANEL_ID;
  hinit.pszHelpLibraryName = "datapad.hlp";

  if( (hwndHelp = WinCreateHelpInstance( hab, &hinit)) == NULLHANDLE)
     DosBeep(200, 50);
  else
     {
     if(hinit.ulReturnCode)
	{
	DosBeep(3000, 50);
	WinDestroyHelpInstance( hwndHelp);
	}
     }

  scrWid = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
  scrLen = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);

  /* ##START Form.35 Application initialization */
  /* ##END Application initialization */

  hwndMainFrame = OpenMain(HWND_DESKTOP, pMainFormInfo);
  if (!hwndMainFrame) {
     WinDestroyMsgQueue( hmq );
     WinTerminate( hab );
     exit(1);
  }

  /* Get and dispatch messages from the application message queue  */
  /* until WinGetMsg returns FALSE, indicating a WM_QUIT message.  */
  while( WinGetMsg( hab, &qmsg, (HWND)NULL, 0, 0 ) )
    WinDispatchMsg( hab, &qmsg );

  /* ##START Form.36 Application termination */
  /* ##END Application termination */
  WinDestroyWindow(hwndMainFrame);
  WinDestroyMsgQueue( hmq );            /* and                          */
  WinTerminate( hab );                  /* terminate the application    */
  return(0);
}


MRESULT EXPENTRY MainDlgProc(HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  PMAINFORMINFO pMainFormInfo=(PMAINFORMINFO) WinQueryWindowULong(hwndDlg, QWL_USER);
  HPOINTER hIcon;
  SWP swp;
  ULONG rSize;
  INT xCoor;
  INT yCoor;
  INT xsize;
  INT ysize;
  INT i, cc;
  INT recCnt = 0;
  CHAR errMsg[70] = "";
  CHAR maxRec[5] = "";
  CHAR wwOn[] = "DataPad ver. 3.2  word wrap ON";
  CHAR wwOff[] = "DataPad ver. 3.2   word wrap OFF";
  PFNWP oWinProc;
  CHAR iniPath[MAXNAME] = "";
  INT ic;
  CHAR tempPDF[MAXNAME];
  CHAR tempT[MAXNAME];
  FILE *pHan;
  BOOL dfExist;
  // INT nChox;


 /* ##START Form.37 Top of window procedure */
 /* ##END Top of window procedure */
   switch (msg) {
   /* Form event Opened WM_INITDLG */
   case WM_INITDLG :
     {
     // ULONG ulStyle;
     HWND hwndSysMenu;

     hwndFrame = WinQueryWindow(hwndDlg, QW_PARENT);
     if (mp2==0)
	mp2 = (MPARAM) malloc(sizeof(MAINFORMINFO));
     HandleMessage(hwndFrame, hwndDlg, msg, mp1, mp2);
     pMainFormInfo=(PMAINFORMINFO) WinQueryWindowULong(hwndDlg, QWL_USER);

     // hwndSysMenu = WinWindowFromID(hwndFrame, FID_SYSMENU);
     // WinSetParent(hwndSysMenu, HWND_OBJECT, FALSE);
     // WinUpdateWindow(hwndFrame);

     /* if ( strcmp(Honor, "g...rit") != 0 )
	 {
	 aboutR(hwndDlg);
	 WinShowWindow(WinWindowFromID(hwndDlg, ID_UNREGISTERED), TRUE);
	 }
      else
	 WinShowWindow(WinWindowFromID(hwndDlg, ID_UNREGISTERED), FALSE); */


      // WinShowWindow(WinWindowFromID(hwndDlg, ID_UNREGISTERED), FALSE);
      // hIcon = WinLoadPointer(HWND_DESKTOP, 0L, ID_POINTER);
      // WinSendMsg(hwndDlg, WM_SETICON, MPFROMP(hIcon), 0L);

      addSwEntry(hwndDlg);
      setBasePath();
      strcpy(iniPath, basePath);
      if( iniPath[strlen(iniPath)-1] == '\\' )
	 strcat(iniPath, "DATAPAD.DAT");
      else
	 strcat(iniPath, "\\DATAPAD.DAT");
      if ( _dos_findfirst(iniPath,_A_NORMAL,&ffblk) == 0 )
	 {
	 if( validatePDF(iniPath) )
	    {
	    strcpy(tempDatFile, pdf[0].noteText);
	    dfExist = TRUE;
	    }
	 else
	    {
	    strcpy(tempDatFile, "");
	    dfExist = FALSE;
	    }
	 }
      else
	 {
	 dfExist = FALSE;
	 strcpy(tempDatFile, "");
	 }
      strcpy(iniPath, basePath);
      if( iniPath[strlen(iniPath)-1] == '\\' )
	 strcat(iniPath, "DATAPAD.INI");
      else
	 strcat(iniPath, "\\DATAPAD.INI");
      if ( (hini = PrfOpenProfile(WinQueryAnchorBlock(WinQueryWindow(hwndDlg, QW_OWNER)) , iniPath)) != NULLHANDLE )
	 {
	 PrfQueryProfileSize(hini, "PAD21", "WINPRES", &rSize);
	 if ( rSize == (sizeof(pValue)) )
	    PrfQueryProfileData(hini, "PAD21", "WINPRES",
				&pValue, &rSize);
	 else
	    {
	    pValue[2][0] = 65535;
	    pValue[5][0] = 65535;
	    pValue[8][0] = 65535;
	    pValue[11][0] = 65535;
	    pValue[14][0] = 65535;
	    for( i=1;i<32;i++ )
	       {
	       pValue[2][i] = 0;
	       pValue[5][i] = 0;
	       pValue[8][i] = 0;
	       pValue[11][i] = 0;
	       pValue[14][i] = 0;
	       }
	    }
	 PrfQueryProfileSize(hini, "PAD21", "WINPOS", &rSize);
	 if ( rSize == (sizeof(pgmData)) )
	    {
	    PrfQueryProfileData(hini, "PAD21", "WINPOS",
				&pgmData, &rSize);
	    xCoor = pgmData[0];
	    yCoor = pgmData[1];
	    xsize = pgmData[36];
	    ysize = pgmData[37];
	    WW = pgmData[2];
	    fileP = pgmData[3];
	    }
	 else
	    {
	    xCoor = 100;
	    yCoor = 75;
	    xsize = 460;
	    ysize = 640;
	    WW = 0;
	    fileP = 0;
	    pgmData[4] = 1;
	    pgmData[5] = 1;
	    pgmData[6] = 1;
	    pgmData[7] = 1;
	    pgmData[8] = 0;
	    pgmData[9] = 0;
	    pgmData[10] = 0;
	    pgmData[11] = 0;
	    pgmData[12] = 0;
	    pgmData[13] = 1;
	    pgmData[14] = 1;
	    pgmData[15] = scrWid*.95;
	    pgmData[16] = scrLen*.7;
	    pgmData[17] = scrWid*.025;
	    pgmData[18] = scrLen*.1;
	    pgmData[19] = scrWid*.95;
	    pgmData[20] = scrLen*.7;
	    pgmData[21] = scrWid*.025;
	    pgmData[22] = scrLen*.1;
	    pgmData[23] = scrWid*.95;
	    pgmData[24] = scrLen*.7;
	    pgmData[25] = scrWid*.025;
	    pgmData[26] = scrLen*.1;
	    pgmData[27] = scrWid*.95;
	    pgmData[28] = scrLen*.7;
	    pgmData[29] = scrWid*.025;
	    pgmData[30] = scrLen*.1;
	    pgmData[31] = 0;
	    pgmData[32] = 1;
	    pgmData[33] = 1;
	    pgmData[34] = 1;
	    pgmData[35] = 1;
	    pgmData[38] = scrWid*.319;
	    pgmData[39] = scrLen*.283;
	    pgmData[40] = scrWid*.4;
	    pgmData[41] = scrLen*.4;
	    }
	 if( dfExist )
	    {
	    PrfQueryProfileString(hini, "PAD21", "DEFLOADMOD",
				  "", stickyName, MAXNAME);
	    }
	 else
	    {
	    strcpy(tempDatFile, "");
	    strcpy(stickyName, "");
	    }
	 if( strlen(stickyName) == 0 )
	    {
	    strcpy(stickyName, ndName);
	    }
	 if( strlen(tempDatFile) != 0 )
	    {
	    strcpy(datFile, tempDatFile);
	    }
	 else
	    {
	    setINITFE(hwndDlg);
	    PrfQueryProfileString(HINI_USERPROFILE, "PAD21", "FILEEXT",
				  ".PDF", EXTENTION, sizeof(EXTENTION));
	    setMod(hwndDlg);
	    if( strlen(npdf) == 0 )
	       {
	       strcpy(datFile, "");
	       PrfWriteProfileString(hini, "PAD21", "DATAFILE", datFile);
	       strcpy(stickyName, "");
	       PrfWriteProfileString(hini, "PAD21", "DEFLOADMOD", stickyName);
	       exit(0);
	       }
	    strcpy(datFile, npdf);
	    }
	 }
      else
	 exit(0);
      hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);
      // hwndMenu = WinLoadMenu(hwndDlg, 0, ID_MENU);
      // WinSendMsg(hwndDlg, WM_UPDATEFRAME, (MPARAM)FID_MENU, 0);
      ACCUM = pgmData[8];
      CURMODONLY = pgmData[9];
      CASESENSITIVE = pgmData[10];
      NAMEONLY = pgmData[11];
      chkPDF(dFile);
      if ( initLoad_index(WinWindowFromID(hwndDlg, ID_LISTBOX1), datFile) == -1 )
	 DosBeep(100, 100);
      else
	 WinPostMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
		    LM_SELECTITEM,
		    MPFROMSHORT(0),
		    MPFROMSHORT(TRUE));
      if( fileP == 1 )
	 WinCheckMenuItem(hwndMenu, ID_FPROMPT, TRUE);
      if( pgmData[4] )
	 WinCheckMenuItem(hwndMenu, ID_DEFVIEW, TRUE);
      if( pgmData[5] )
	 WinCheckMenuItem(hwndMenu, ID_DEFEDIT, TRUE);
      if( pgmData[6] )
	 WinCheckMenuItem(hwndMenu, ID_DEFSEARCH, TRUE);
      if( pgmData[7] )
	 WinCheckMenuItem(hwndMenu, ID_DEFADD, TRUE);
      if( pgmData[32] )
	 WinCheckMenuItem(hwndMenu, ID_MAINLIST, TRUE);
      if( pgmData[12] )
	 WinCheckMenuItem(hwndMenu, ID_DEFOPENE, TRUE);
      if( pgmData[13] )
	 WinCheckMenuItem(hwndMenu, ID_PRINTPROMPT, TRUE);
      if( pgmData[14] )
	 WinCheckMenuItem(hwndMenu, ID_PRINTALLPROMPT, TRUE);
      if( pgmData[31] )
	 WinCheckMenuItem(hwndMenu, ID_AUTOSTAMP, TRUE);
      if( pgmData[2] )
	 WinCheckMenuItem(hwndMenu, ID_WORDWRAP, TRUE);
      if( !pgmData[32] )
	 setParams(hwndDlg, 12, ID_LISTBOX1, ID_LISTBOX1);
      switch( pgmData[33] )
	 {
	 case 1 :
	    WinCheckMenuItem(hwndMenu, ID_TIMEANDDATE, TRUE);
	    break;

	 case 2 :
	    WinCheckMenuItem(hwndMenu, ID_TIMEONLY, TRUE);
	    break;

	 case 3 :
	    WinCheckMenuItem(hwndMenu, ID_DATEONLY, TRUE);
	    break;

	 default :
	    WinCheckMenuItem(hwndMenu, ID_TIMEANDDATE, TRUE);
	    break;
	 }
      if( pgmData[34] )
	 WinCheckMenuItem(hwndMenu, ID_AMPM, TRUE);
      if( pgmData[35] )
	 WinCheckMenuItem(hwndMenu, ID_WEEKDAYNAME, TRUE);
      WinSetWindowPos(hwndFrame, HWND_TOP, xCoor, yCoor, xsize, ysize,
		      SWP_ACTIVATE | SWP_MOVE | SWP_SIZE | SWP_SHOW);
      oldPointer = WinQueryPointer(HWND_DESKTOP);
      newPointer = WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE);
      hwnd1 = WinWindowFromID(hwndDlg, ID_LISTBOX1);
      hwnd2 = WinWindowFromID(hwndDlg, ID_EDITVIEW);
      hwnd3 = WinWindowFromID(hwndDlg, ID_VIEW);
      hwnd4 = WinWindowFromID(hwndDlg, ID_ADD);
      hwnd5 = WinWindowFromID(hwndDlg, ID_SEARCH);
      hwnd6 = WinWindowFromID(hwndDlg, ID_DELETE);
      hwnd7 = WinWindowFromID(hwndDlg, ID_QUIT);
      hwnd8 = WinWindowFromID(hwndDlg, ID_PROGSTATUS);
      hwnd9 = WinWindowFromID(hwndDlg, ID_MINUS);
      hwnd10 = WinWindowFromID(hwndDlg, ID_PLUS);
      hwnd11 = WinWindowFromID(hwndFrame, FID_MINMAX);
      hwnd12 = WinWindowFromID(hwndFrame, FID_SYSMENU);

      hwndMainDlg = hwndDlg;
      INUSE = 0;
      if( WW )
	 WinSetWindowText(WinWindowFromID(hwndFrame, FID_TITLEBAR), wwOn);
      else
	 WinSetWindowText(WinWindowFromID(hwndFrame, FID_TITLEBAR), wwOff);
      oWinProc = WinSubclassWindow(WinWindowFromID(hwndDlg, ID_LISTBOX1),
				     nWinProc);
      WinSetWindowPtr(WinWindowFromID(hwndDlg, ID_LISTBOX1),
		      0, (PVOID) oWinProc);
      setStatus(hwndDlg, datFile);

      if( hwndHelp )
	 WinAssociateHelpInstance( hwndHelp, hwndFrame);
      hwndMLBox = WinWindowFromID(hwndDlg, ID_LISTBOX1);
      hwndMLBPopup = WinLoadMenu(WinWindowFromID(hwndDlg, ID_QUIT),
				 0, ID_MLBOXPOPUP);
      // hwndFrame = WinQueryWindow(hwndMLBox, QW_OWNER);

     // ulStyle = WinQueryWindowULong(hwndDlg, QWL_STYLE) ;
     // WinSetWindowULong(hwndDlg, QWL_STYLE, ulStyle &~ FCF_SYSMENU);
     // WinSetWindowBits(hwndDlg, QWL_STYLE, &~FCF_SYSMENU);
     // WinUpdateWindow(hwndDlg);
     nCho = -1;
     strcpy(datFilex, "");
     }
     break;

   case WM_QUIT :
      DosBeep(100, 100);
      break;

   case WM_MOUSEMOVE :
      if ( BUSY )
	 {
	 WinSetPointer(HWND_DESKTOP, newPointer);
	 return(MRESULT)TRUE;
	 }
      else
	 return (WinDefDlgProc(hwndDlg, msg, mp1, mp2));

   /* case WM_SAVEAPPLICATION :
      WinSetWindowPos(hwndDlg,
		      HWND_TOP, 0, 0, 0, 0,
		      SWP_ACTIVATE | SWP_RESTORE);
      WinQueryWindowPos(hwndDlg, (PSWP) &swp);
      pgmData[0] = swp.x;
      pgmData[1] = swp.y;
      pgmData[2] = WW;
      pgmData[3] = fileP;
      pgmData[8] = ACCUM;
      if( ASSOCI )
	 {
	 FILE *mrHanX;
	 CHAR dataFile[MAXNAME] = "";

	 strcpy(dataFile, basePath);
	 if( dataFile[strlen(dataFile)-1] == '\\' )
	    strcat(dataFile, "DATAPAD.DAT");
	 else
	    strcat(dataFile, "\\DATAPAD.DAT");
	 strcpy(datFile, datFileOrg);
	 if ((mrHanX = fopen( dataFile, MRW)) == NULL)
	    mrHanX = fopen( dataFile, "w+b" );
	 fseek(mrHanX, 0L, SEEK_SET);
	 fwrite(&pdf2, sizeof(pdf2), 1, mrHanX);
	 fclose(mrHanX);
	 }
      PrfWriteProfileString(HINI_USERPROFILE, "PAD21", "FILEEXT", EXTENTION);
      PrfWriteProfileString(hini, "PAD21", "DATAFILE", datFile);
      PrfWriteProfileString(hini, "PAD21", "DEFLOADMOD", stickyName);
      PrfWriteProfileData(hini,
			  "PAD21",
			  "WINPOS",
			  &pgmData,
			  sizeof(pgmData));
      PrfWriteProfileData(hini,
			  "PAD21",
			  "WINPRES",
			  &pValue,
			  sizeof(pValue));
      for( i=1;i<NOTMPFILES;i++ )
	 {
	 CHAR sd[3];

	 strcpy(szFullDocName, basePath);
	 if( szFullDocName[strlen(szFullDocName)-1] != '\\' )
	    strcat(szFullDocName, "\\");
	 strcat(szFullDocName, "TMP");
	 itoa(i, sd, 10);
	 strcat(szFullDocName, sd);
	 strcat(szFullDocName, ".TXT");
	 remove(szFullDocName);
	 }
      if ( strcmp(Honor, "g...rit") != 0 )
	 aboutR(hwndDlg);
      DosCloseEventSem(hev);
      DosCloseEventSem(hevS);
      break;   */

   case WM_CONTROL :
      {
      HAB hab;


      if ( SHORT1FROMMP(mp1) == ID_LISTBOX1 )
	 {
	 if (SHORT2FROMMP(mp1) == LN_ENTER )
	    {
	    if( WinIsWindow(hab, hwndCurEViewRecord) || WinIsWindow(hab, hwndCurViewRecord) )
	       {
	       if( WinIsWindow(hab, hwndCurEViewRecord) )
		WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_EDITVIEW), NULL);
	       if( WinIsWindow(hab, hwndCurViewRecord) )
		WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_VIEW), NULL);
	       }
	    else
	       {
	       if( pgmData[12] )
		WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_EDITVIEW), NULL);
	       else
		WinPostMsg(hwndDlg, WM_COMMAND, MPFROMSHORT(ID_VIEW), NULL);
	       }
	    }
	 if (SHORT2FROMMP(mp1) == LN_SELECT )
	    choice = SHORT1FROMMR(WinSendMsg(
				  WinWindowFromID(hwndDlg, ID_LISTBOX1),
				  LM_QUERYSELECTION,
				  MPFROMSHORT(NULL), NULL));
	 }
      }
      break;

   /* case WM_ADJUSTWINDOWPOS :
      if( ((PSWP)mp1)->fl & SWP_MINIMIZE )
	 {
	 WinShowWindow(WinWindowFromID(hwndDlg, ID_GROUPBOX1), FALSE);
	 WinShowWindow(WinWindowFromID(hwndDlg, ID_LISTBOX1), FALSE);
	 }
      else
	 if( ((PSWP)mp1)->fl & SWP_RESTORE )
	    {
	    WinShowWindow(WinWindowFromID(hwndDlg, ID_GROUPBOX1), TRUE);
	    WinShowWindow(WinWindowFromID(hwndDlg, ID_LISTBOX1), TRUE);
	    }
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 ); */

   /* case WM_CLOSE :
     WinPostMsg(hwndDlg, WM_QUIT, 0, 0);
      break;

   case WM_COMMAND :
      switch (SHORT1FROMMP(mp1)) {
      }
      break; */

  /* case WM_INITMENU:
     switch (SHORT1FROMMP(mp1)) {
     }  end switch
  break; */


   case WM_TRANSLATEACCEL:
	if (WinSendMsg(hwndFrame, msg, mp1, mp2 ))
	   return (MRESULT) TRUE;
	return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
	// break;



   case WM_COMMAND :
	  switch ( COMMANDMSG(&msg) -> cmd      )
	     {
	     case ID_SETFILEEXT :
		setFE(hwndFrame);
		// DosBeep(1200, 100);
		break;

	     case ID_EDITVIEW :
		    {
		    HAB hab;


		    removeSwEntry(hwndDlg);
		    recCnt = WinQueryLboxCount(
				WinWindowFromID(hwndDlg, ID_LISTBOX1));
		    if( recCnt <= 0 )
		       {
		       addSwEntry(hwndDlg);
		       return(0);
		       }

		    if( WinIsWindow(hab, hwndCurEViewRecord) )
		       {
		       if( nChox == choice && strcmp(datFile, datFilex) == 0 )
			  {
			  }
		       else
			  {
			  // debugMsgStr("Got this far!", "inside loop.");
			  zeroDataRecs(dataRecs.noteText);
			  nCho = compName(hwndDlg, choice);
			  multiRead(datFile, 1, FIXEDLEN, nCho);
			  nChox = choice;
			  strcpy(datFilex, datFile);


			  if( loadNewERec(hwndDlg, hwndCurEViewRecord) == 1 )
			     {
			     WinSetFocus(HWND_DESKTOP,
				      WinWindowFromID(hwndDlg, ID_LISTBOX1));
			     multiWrite(datFile, 1, FIXEDLEN, nCho);
			     strcpy(recIndex[nCho].recName, dataRecs.noteName);
			     saveIndex(datFile);
			     initLoad_index(WinWindowFromID(hwndDlg, ID_LISTBOX1), datFile);
			     WinPostMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
					  LM_SELECTITEM,
					  MPFROMSHORT(matchName(hwndDlg, dataRecs.noteName)),
					  MPFROMSHORT(TRUE));
			     }
			  }
		       }
		    else
		       {
		       zeroDataRecs(dataRecs.noteText);
		       nCho = compName(hwndDlg, choice);
		       multiRead(datFile, 1, FIXEDLEN, nCho);
		       nChox = choice;
		       strcpy(datFilex, datFile);


		       if( editRec(hwndDlg) )
			  {
			  multiWrite(datFile, 1, FIXEDLEN, nCho);
			  strcpy(recIndex[nCho].recName, dataRecs.noteName);
			  saveIndex(datFile);
			  initLoad_index(WinWindowFromID(hwndDlg, ID_LISTBOX1), datFile);
			  WinPostMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
				     LM_SELECTITEM,
				     MPFROMSHORT(matchName(hwndDlg, dataRecs.noteName)),
				     MPFROMSHORT(TRUE));
			  }
		       }
		    WinSetFocus(HWND_DESKTOP,
			 WinWindowFromID(hwndDlg, ID_LISTBOX1));
		    WinSendMsg(WinWindowFromID(hwndDlg, ID_EDITVIEW),
				   BM_SETDEFAULT, 0, 0);
		    addSwEntry(hwndDlg);
		    }
		    break;

	     case ID_LOADINDX :
		    initLoad_index(WinWindowFromID(hwndDlg, ID_LISTBOX1), datFile);
		    break;

	     case ID_VIEW :
		    {
		    static HWND hwndv;
		    HAB hab;

		    // WinSendMsg(hwndCurViewRecord, WM_DESTROY, 0, 0);
		    // WinPostMsg(hwndv, WM_DESTROY, 0, 0);
		    // disableMainScrX(hwndDlg);
		    removeSwEntry(hwndDlg);
		    recCnt = WinQueryLboxCount(
				WinWindowFromID(hwndDlg, ID_LISTBOX1));
		    if( recCnt <= 0 )
		       {
		       addSwEntry(hwndDlg);
		       return(0);
		       }
		    zeroDataRecs(dataRecs.noteText);
		    nCho = compName(hwndDlg, choice);
		    multiRead(datFile, 1, FIXEDLEN, nCho);
		    if( WinIsWindow(hab, hwndCurViewRecord) )
		       {
		       // numRecs = recCnt;
		       /* WinPostMsg(hwndCurViewRecord, WM_COMMAND,
				  MPFROM2SHORT(ID_OK4, 0), 0); */
		       loadNewRec(hwndDlg, hwndCurViewRecord);
		       WinSetFocus(HWND_DESKTOP,
				   WinWindowFromID(hwndDlg, ID_LISTBOX1));
		       }
		    else
		       {
		       removeSwEntry(hwndCurViewRecord);
		       viewRec(hwndDlg);
		       WinSetFocus(HWND_DESKTOP,
				   WinWindowFromID(hwndDlg, ID_LISTBOX1));
		       WinSendMsg(WinWindowFromID(hwndDlg, ID_VIEW),
				   BM_SETDEFAULT, 0, 0);
		       addSwEntry(hwndDlg);
		       }
		    }
		    break;

	     /* case ID_VIEW :
		    removeSwEntry(hwndDlg);
		    recCnt = WinQueryLboxCount(
				WinWindowFromID(hwndDlg, ID_LISTBOX1));
		    if( recCnt <= 0 )
		       {
		       addSwEntry(hwndDlg);
		       return(0);
		       }
		    zeroDataRecs(dataRecs.noteText);
		    nCho = compName(hwndDlg, choice);
		    multiRead(datFile, 1, FIXEDLEN, nCho);
		    viewRec(hwndDlg);
		    WinSetFocus(HWND_DESKTOP,
			 WinWindowFromID(hwndDlg, ID_LISTBOX1));
		    WinSendMsg(WinWindowFromID(hwndDlg, ID_VIEW),
				   BM_SETDEFAULT, 0, 0);
		    addSwEntry(hwndDlg);
		    break;  */

	     case ID_QUIT :
		    WinSendMsg(hwndDlg, WM_CLOSE, 0, 0);
		    WinSendMsg(hwndDlg, WM_QUIT, 0, 0);
		    WinSendMsg(hwndDlg, WM_DESTROY, 0, 0);
		    WinSendMsg(hwndFrame, WM_CLOSE, 0, 0);
		    WinSendMsg(hwndFrame, WM_QUIT, 0, 0);
		    WinSendMsg(hwndFrame, WM_DESTROY, 0, 0);
		    /* WinSendMsg(hwndMainFrame, WM_CLOSE, 0, 0);
		    WinSendMsg(hwndMainFrame, WM_QUIT, 0, 0);
		    WinSendMsg(hwndMainFrame, WM_DESTROY, 0, 0); */
		    WinDismissDlg(hwndDlg, TRUE);
		    exit(0);
		    break;

	     case ID_ABOUT :
		    // FindFile(HWND_DESKTOP);
		    if ( strcmp(Honor, "g...rit") != 0 )
		       aboutR(hwndDlg);
		    else
		       about(hwndDlg);
		    break;

	     case ID_PRINTPROMPT :
		    if( pgmData[13] )
		       {
		       pgmData[13] = 0;
		       WinCheckMenuItem(hwndMenu, ID_PRINTPROMPT, FALSE);
		       }
		    else
		       {
		       pgmData[13] = 1;
		       WinCheckMenuItem(hwndMenu, ID_PRINTPROMPT, TRUE);
		       }
		    break;

	     case ID_PRINTALLPROMPT :
		    if( pgmData[14] )
		       {
		       pgmData[14] = 0;
		       WinCheckMenuItem(hwndMenu, ID_PRINTALLPROMPT, FALSE);
		       }
		    else
		       {
		       pgmData[14] = 1;
		       WinCheckMenuItem(hwndMenu, ID_PRINTALLPROMPT, TRUE);
		       }
		    break;

	     case ID_DEFOPENE :
		    if( pgmData[12] )
		       {
		       pgmData[12] = 0;
		       WinCheckMenuItem(hwndMenu, ID_DEFOPENE, FALSE);
		       }
		    else
		       {
		       pgmData[12] = 1;
		       WinCheckMenuItem(hwndMenu, ID_DEFOPENE, TRUE);
		       }
		    break;

	     case ID_MCOLOR :
		hobjBG = WinCreateObject("WPColorPalette",
					 "Color Palette",
					 "OPEN=DEFAULT",
					 "<WP_CONFIG>",
					 CO_UPDATEIFEXISTS);
		break;

	     case ID_MFONTSELECT :
		hobjFNT = WinCreateObject("WPFontPalette",
					  "Font Palette",
					  "OPEN=DEFAULT",
					  "<WP_CONFIG>",
					  CO_UPDATEIFEXISTS);
		break;

	     case ID_MAINLIST :
		    if( pgmData[32] )
		       {
		       setParams(hwndDlg, 12, ID_LISTBOX1, ID_LISTBOX1);
		       pgmData[32] = 0;
		       WinCheckMenuItem(hwndMenu, ID_MAINLIST, FALSE);
		       }
		    else
		       {
		       pgmData[32] = 1;
		       WinCheckMenuItem(hwndMenu, ID_MAINLIST, TRUE);
		       WinRemovePresParam(
			     WinWindowFromID(hwndDlg, ID_LISTBOX1),
			     PP_FOREGROUNDCOLOR);
		       WinRemovePresParam(
			     WinWindowFromID(hwndDlg, ID_LISTBOX1),
			     PP_BACKGROUNDCOLOR);
		       WinRemovePresParam(
			     WinWindowFromID(hwndDlg, ID_LISTBOX1),
			     PP_FONTNAMESIZE);
		       pgmData[32] = 1;
		       WinCheckMenuItem(hwndMenu, ID_MAINLIST, TRUE);
		       }
		    break;

	     case ID_TIMEONLY :
		    pgmData[33] = 2;
		    WinCheckMenuItem(hwndMenu, ID_TIMEANDDATE, FALSE);
		    WinCheckMenuItem(hwndMenu, ID_TIMEONLY, TRUE);
		    WinCheckMenuItem(hwndMenu, ID_DATEONLY, FALSE);
		    pgmData[35] = 0;
		    WinCheckMenuItem(hwndMenu, ID_WEEKDAYNAME, FALSE);
		    break;

	     case ID_DATEONLY :
		    pgmData[33] = 3;
		    WinCheckMenuItem(hwndMenu, ID_TIMEANDDATE, FALSE);
		    WinCheckMenuItem(hwndMenu, ID_TIMEONLY, FALSE);
		    WinCheckMenuItem(hwndMenu, ID_DATEONLY, TRUE);
		    break;

	     case ID_TIMEANDDATE :
		    pgmData[33] = 1;
		    WinCheckMenuItem(hwndMenu, ID_TIMEANDDATE, TRUE);
		    WinCheckMenuItem(hwndMenu, ID_TIMEONLY, FALSE);
		    WinCheckMenuItem(hwndMenu, ID_DATEONLY, FALSE);
		    break;

	     case ID_WEEKDAYNAME :
		    if( pgmData[35] )
		       {
		       pgmData[35] = 0;
		       WinCheckMenuItem(hwndMenu, ID_WEEKDAYNAME, FALSE);
		       }
		    else
		       {
		       if( pgmData[33] != 2 )
			  {
			  pgmData[35] = 1;
			  WinCheckMenuItem(hwndMenu, ID_WEEKDAYNAME, TRUE);
			  }
		       }
		    break;

	     case ID_AMPM :
		    if( pgmData[34] )
		       {
		       pgmData[34] = 0;
		       WinCheckMenuItem(hwndMenu, ID_AMPM, FALSE);
		       }
		    else
		       {
		       pgmData[34] = 1;
		       WinCheckMenuItem(hwndMenu, ID_AMPM, TRUE);
		       }
		    break;

	     case ID_DEFVIEW :
		    if( pgmData[4] )
		       {
		       pgmData[4] = 0;
		       WinCheckMenuItem(hwndMenu, ID_DEFVIEW, FALSE);
		       }
		    else
		       {
		       pgmData[4] = 1;
		       WinCheckMenuItem(hwndMenu, ID_DEFVIEW, TRUE);
		       }
		    break;

	     case ID_DEFEDIT :
		    if( pgmData[5] )
		       {
		       pgmData[5] = 0;
		       WinCheckMenuItem(hwndMenu, ID_DEFEDIT, FALSE);
		       }
		    else
		       {
		       pgmData[5] = 1;
		       WinCheckMenuItem(hwndMenu, ID_DEFEDIT, TRUE);
		       }
		    break;

	     case ID_DEFSEARCH :
		    if( pgmData[6] )
		       {
		       pgmData[6] = 0;
		       WinCheckMenuItem(hwndMenu, ID_DEFSEARCH, FALSE);
		       }
		    else
		       {
		       pgmData[6] = 1;
		       WinCheckMenuItem(hwndMenu, ID_DEFSEARCH, TRUE);
		       }
		    break;

	     case ID_DEFADD :
		    if( pgmData[7] )
		       {
		       pgmData[7] = 0;
		       WinCheckMenuItem(hwndMenu, ID_DEFADD, FALSE);
		       }
		    else
		       {
		       pgmData[7] = 1;
		       WinCheckMenuItem(hwndMenu, ID_DEFADD, TRUE);
		       }
		    break;

	     case ID_FPROMPT :
		    if ( WinIsMenuItemChecked(hwndMenu, ID_FPROMPT) )
		       {
		       WinCheckMenuItem(hwndMenu, ID_FPROMPT, FALSE);
		       fileP = 0;
		       }
		    else
		       {
		       WinCheckMenuItem(hwndMenu, ID_FPROMPT, TRUE);
		       fileP = 1;
		       }
		    break;

	     case ID_AUTOSTAMP :
		    if ( pgmData[31] )
		       {
		       pgmData[31] = 0;
		       WinCheckMenuItem(hwndMenu, ID_AUTOSTAMP, FALSE);
		       }
		    else
		       {
		       pgmData[31] = 1;
		       WinCheckMenuItem(hwndMenu, ID_AUTOSTAMP, TRUE);
		       }
		    break;

	     case ID_ADD :
		    removeSwEntry(hwndDlg);
		    recCnt = WinQueryLboxCount(
				WinWindowFromID(hwndDlg, ID_LISTBOX1));
		    if( recCnt >= MAXNUMRECS )
		       {
		       DosBeep(3700, 150);
		       itoa(MAXNUMRECS, maxRec, 10);
		       strcpy(errMsg, "You can only have ");
		       strcat(errMsg, maxRec);
		       strcat(errMsg, " notes in a file. Start a new data file.");
		       strcat(errMsg, "\0");
		       WinMessageBox( HWND_DESKTOP,
				      hwndDlg,
				      errMsg,
				      "Data file is FULL ...",
				      0,
				      MB_OK | MB_ICONEXCLAMATION );
		       addSwEntry(hwndDlg);
		       return(0);
		       }
		    if ( !addRec(hwndDlg) )
		       {
		       WinSetFocus(HWND_DESKTOP,
			    WinWindowFromID(hwndDlg, ID_LISTBOX1));
		       addSwEntry(hwndDlg);
		       return(MRESULT) FALSE;
		       }
		    WinInsertLboxItem(WinWindowFromID(hwndDlg, ID_LISTBOX1),
				      LIT_SORTASCENDING,
				      recIndex[recCnt].recName);
		    WinSetFocus(HWND_DESKTOP,
			 WinWindowFromID(hwndDlg, ID_LISTBOX1));
		    nCho = matchName(hwndDlg, recIndex[recCnt].recName);
		    WinPostMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
			       LM_SELECTITEM,
			       MPFROMSHORT(nCho),
			       MPFROMSHORT(TRUE));
		    WinSendMsg(WinWindowFromID(hwndDlg, ID_ADD),
				   BM_SETDEFAULT, 0, 0);
		    multiWrite(datFile, 1, FIXEDLEN, recCnt);
		    recIndex[recCnt].posIndex = recCnt;
		    strcpy(recIndex[recCnt].recName,
			   dataRecs.noteName);
		    saveIndex(datFile);
		    setStatus(hwndDlg, datFile);
		    addSwEntry(hwndDlg);
		    break;

	     case ID_DELETE :
		    recCnt = WinQueryLboxCount(
				WinWindowFromID(hwndDlg, ID_LISTBOX1));
		    if( recCnt <= 0 )
		       return(0);
		    nCho = compName(hwndDlg, choice);
		    if ( deleteRec(hwndDlg) == -1 )
		       WinPostMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
				  LM_SELECTITEM,
				  MPFROMSHORT(choice),
				  MPFROMSHORT(TRUE));
		    else
		       {
		       multiRead(datFile, 1, FIXEDLEN, recCnt-1);
		       multiWrite(datFile, 1, FIXEDLEN, nCho);
		       truncFile(datFile, recCnt, FIXEDLEN);
		       recIndex[nCho] = recIndex[recCnt - 1];
		       recIndex[nCho].posIndex = nCho;
		       recIndex[nCho].alIndex = FIXEDLEN;
		       zeroMostIndex(recCnt - 1);
		       saveIndex(datFile);
		       initLoad_index(WinWindowFromID(hwndDlg, ID_LISTBOX1), datFile);
		       if( choice == (recCnt - 1) )
			  WinPostMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
				     LM_SELECTITEM,
				     MPFROMSHORT(choice - 1),
				     MPFROMSHORT(TRUE));
		       else
			  WinPostMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
				     LM_SELECTITEM,
				     MPFROMSHORT(choice),
				     MPFROMSHORT(TRUE));
		       }
		    WinSetFocus(HWND_DESKTOP,
			 WinWindowFromID(hwndDlg, ID_LISTBOX1));
		    WinSendMsg(WinWindowFromID(hwndDlg, ID_DELETE),
				   BM_SETDEFAULT, 0, 0);
		    setStatus(hwndDlg, datFile);
		    break;

	     case ID_SEARCH :
		    recCnt = WinQueryLboxCount(WinWindowFromID(hwndDlg, ID_LISTBOX1));
		    if( recCnt <= 0 )
		       return(0);
		    if( searchRec(hwndDlg) )
		       {
		       if( NAMESEARCH )
			  {
			  if( ACCUM )
			     {
			     _beginthread(result2Recs, 16384,(void *)hwndDlg);
			     results2(hwndDlg);
			     }
			  else
			     {
			     _beginthread(searchNames, 16384, (void *)hwndDlg);
			     }
			  }
		       else
			  {
			  if( ACCUM )
			     {
			     _beginthread(resultRecs, 16384,(void *)hwndDlg);
			     results1(hwndDlg);
			     }
			  else
			     {
			     _beginthread(searchRecs, 16384,(void *)hwndDlg);
			     }
			  }
		       }
		    WinSetFocus(HWND_DESKTOP,
			 WinWindowFromID(hwndDlg, ID_LISTBOX1));
		    WinSendMsg(WinWindowFromID(hwndDlg, ID_SEARCH),
				   BM_SETDEFAULT, 0, 0);
		    break;

	     case ID_GETRECCNT :
		    setStatus(hwndDlg, datFile);
		    break;

	     case ID_SETMODULES :
		    setMod(hwndDlg);
		    break;

	     case ID_DEFAULTFILE :
		    setStatus(hwndDlg, datFile);
		    break;

	     case ID_RESIZEMOD :
		    _beginthread(resizePDF, 16384,(void *)hwndDlg);
		    break;

	     case ID_DEFLOADMOD :
		    setDefMod(hwndDlg);
		    break;

	     case ID_OVERSIZED :
		    i = importFile(DONTDROPIT);
		    if( i == 0 )
		       return(0);
		    else
		       recCnt = i;
		    break;

	     case ID_OVERSIZEDX :
		    i = importFile(DODROPIT);
		    if( i == 0 )
		       return(0);
		    else
		       recCnt = i;
		    break;

	     case ID_MINUS :
		    for( ic=0;ic<NUMPDFMODS;ic++ )
		       {
		       if( strcmp(pdf[ic].noteText, "") == 0 )
			  break;
		       }
		    if( ic < 2 )
		       break;
		    if( chkIntegrityPDF(hwndDlg, pdf[ic-1].noteText) == -1 )
		       break;
		    strcpy(tempPDF, pdf[0].noteText);
		    strcpy(pdf[0].noteText, pdf[ic-1].noteText);
		    for( i=ic-1;i>1;i-- )
		       strcpy(pdf[i].noteText, pdf[i-1].noteText);
		    strcpy(pdf[1].noteText, tempPDF);
		    strcpy(datFile, pdf[0].noteText);

		    strcpy(tempT, basePath);
		    if( tempT[strlen(tempT)-1] == '\\' )
		       strcat(tempT, "DATAPAD.DAT");
		    else
		       strcat(tempT, "\\DATAPAD.DAT");
		    if ((pHan = fopen(tempT, MRW)) == NULL)
		       {
		       if ((pHan = fopen(tempT, "w+b" )) == NULL)
			  return(0);
		       }
		    fseek(pHan, 0L, SEEK_SET);
		    fwrite(&pdf, sizeof(pdf), 1, pHan);
		    fclose(pHan);

		    initLoad(WinWindowFromID(hwndDlg, ID_LISTBOX1), pdf[0].noteText);
		    WinPostMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
			       LM_SELECTITEM,
			       MPFROMSHORT(0),
			       MPFROMSHORT(TRUE));
		    WinPostMsg(hwndDlg,
			       WM_COMMAND,
			       MPFROMSHORT(ID_GETRECCNT), 0);
		    break;

	     case ID_PLUS :
		    for( ic=0;ic<NUMPDFMODS;ic++ )
		       {
		       if( strcmp(pdf[ic].noteText, "") == 0 )
			  break;
		       }
		    if( ic < 2 )
		       break;
		    if( chkIntegrityPDF(hwndDlg, pdf[1].noteText) == -1 )
		       break;
		    strcpy(tempPDF, pdf[0].noteText);
		    strcpy(pdf[0].noteText, pdf[1].noteText);
		    for( i=0;i<ic-2;i++ )
		       strcpy(pdf[i+1].noteText, pdf[i+2].noteText);
		    strcpy(pdf[ic-1].noteText, tempPDF);
		    strcpy(datFile, pdf[0].noteText);

		    strcpy(tempT, basePath);
		    if( tempT[strlen(tempT)-1] == '\\' )
		       strcat(tempT, "DATAPAD.DAT");
		    else
		       strcat(tempT, "\\DATAPAD.DAT");
		    if ((pHan = fopen(tempT, MRW)) == NULL)
		       {
		       if ((pHan = fopen(tempT, "w+b" )) == NULL)
			  return(0);
		       }
		    fseek(pHan, 0L, SEEK_SET);
		    fwrite(&pdf, sizeof(pdf), 1, pHan);
		    fclose(pHan);

		    initLoad(WinWindowFromID(hwndDlg, ID_LISTBOX1), pdf[0].noteText);
		    WinPostMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
			       LM_SELECTITEM,
			       MPFROMSHORT(0),
			       MPFROMSHORT(TRUE));
		    WinPostMsg(hwndDlg,
			       WM_COMMAND,
			       MPFROMSHORT(ID_GETRECCNT), 0);
		    break;

	     case ID_SAVEALLTOFILEMOD:
		if( saveAllF(hwndDlg) )
		   _beginthread(saveATFile, 16384,(void *)hwndDlg);
		break;

	     case ID_COPYTOMOD :
		for( ic=0;ic<NUMPDFMODS;ic++ )
		   {
		   if( strcmp(pdf[ic].noteText, "") == 0 )
		      break;
		   }
		if( ic < 2 )
		   break;
		if( getModList(hwndDlg, 1) )
		   {
		   INT km, km2, nch;
		   CHAR orgselName[NAMESIZE];
		   CHAR selName[NAMESIZE];
		   SHORT rsel;
		   UINT tFlrs;

		   rsel = (USHORT) WinSendMsg(
				      WinWindowFromID(hwndDlg, ID_LISTBOX1),
				      LM_QUERYSELECTION, 0L, 0L);
		   WinQueryLboxItemText(WinWindowFromID(hwndDlg, ID_LISTBOX1),
				    rsel, selName, sizeof(selName));
		   strcpy(orgselName, selName);
		   nch = compName(hwndDlg, rsel);
		   multiRead(pdf[0].noteText, 1, FIXEDLEN, nch);
		   tFlrs = getFLRecSize(mcString);
		   if( strlen(dataRecs.noteText) >= tFlrs)
		      {
		      WinMessageBox(HWND_DESKTOP,
				    hwndDlg,
				    "Can Not copy record!",
				    "Record too large to fit into target record",
				    0,
				    MB_OK | MB_ICONEXCLAMATION );
		      return(0);
		      }
		   loadIndex(hwndDlg, mcString);
		   km2 = 0;
		   do
		      {
		      for( km=0;km<MAXNUMRECS;km++ )
			 {
			 if( strlen(recIndex[km].recName) == 0 )
			    km2 = MAXNUMRECS;
			 if( stricmp(selName, recIndex[km].recName) == 0 )
			    {
			    DosBeep(1200, 50);
			    if( dupeName(hwndDlg) )
			       {
			       km2 = MAXNUMRECS + 2;
			       km = MAXNUMRECS;
			       strcpy(selName, dataRecs.noteName);
			       }
			    else
			       {
			       loadIndex(hwndDlg, pdf[0].noteText);
			       return(0);
			       }
			    }
			 else
			    km2 = 0;
			 }
		      }
		   while( km2 == (MAXNUMRECS + 2) );
		   km2 = 0;
		   for( km=0;km<MAXNUMRECS;km++ )
		      {
		      if( strlen(recIndex[km].recName) > 0 )
		      km2++;
		      }
		   if( km2 >= MAXNUMRECS )
		      {
		      WinMessageBox(HWND_DESKTOP,
				    hwndDlg,
				    "Target data file is full!",
				    "Operation can NOT be completed ...",
				    0,
				    MB_OK | MB_ICONEXCLAMATION );
		      loadIndex(hwndDlg, pdf[0].noteText);
		      return(0);
		      }
		   strcpy(recIndex[km2].recName, selName);
		   recIndex[km2].posIndex = -1;
		   saveIndex(mcString);
		   multiWrite(mcString, 1, tFlrs, km2);
		   if( stricmp(pdf[0].noteText, mcString) == 0 )
		      {
		      initLoad(WinWindowFromID(hwndDlg, ID_LISTBOX1), pdf[0].noteText);
		      nch = matchName(hwndDlg, selName);
		      WinPostMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
				 LM_SELECTITEM,
				 MPFROMSHORT(nch),
				 MPFROMSHORT(TRUE));
		      WinPostMsg(hwndDlg,
				 WM_COMMAND,
				 MPFROMSHORT(ID_GETRECCNT), 0);
		      }
		   loadIndex(hwndDlg, pdf[0].noteText);
		   DosBeep(100, 50);
		   DosBeep(300, 60);
		   DosBeep(700, 70);
		   DosBeep(1300, 80);
		   DosBeep(2100, 90);
		   DosBeep(3100, 100);
		   }
		break;

	     case ID_MOVETOMOD :
		for( ic=0;ic<NUMPDFMODS;ic++ )
		   {
		   if( strcmp(pdf[ic].noteText, "") == 0 )
		      break;
		   }
		if( ic < 2 )
		   break;
		if( getModList(hwndDlg, 2) )
		   {
		   INT km, km2, nch;
		   CHAR orgselName[NAMESIZE];
		   CHAR selName[NAMESIZE];
		   SHORT rsel;
		   UINT tFlrs;


		   rsel = (USHORT) WinSendMsg(
				      WinWindowFromID(hwndDlg, ID_LISTBOX1),
				      LM_QUERYSELECTION, 0L, 0L);
		   WinQueryLboxItemText(WinWindowFromID(hwndDlg, ID_LISTBOX1),
				    rsel, selName, sizeof(selName));
		   strcpy(orgselName, selName);
		   nch = compName(hwndDlg, rsel);
		   multiRead(pdf[0].noteText, 1, FIXEDLEN, nch);
		   tFlrs = getFLRecSize(mcString);
		   if( strlen(dataRecs.noteText) >= tFlrs)
		      {
		      WinMessageBox(HWND_DESKTOP,
				    hwndDlg,
				    "Can Not copy record!",
				    "Record too large to fit into target record",
				    0,
				    MB_OK | MB_ICONEXCLAMATION );
		      return(0);
		      }
		   loadIndex(hwndDlg, mcString);
		   km2 = 0;
		   do
		      {
		      for( km=0;km<MAXNUMRECS;km++ )
			 {
			 if( strlen(recIndex[km].recName) == 0 )
			    km2 = MAXNUMRECS;
			 if( stricmp(selName, recIndex[km].recName) == 0 )
			    {
			    DosBeep(1200, 50);
			    if( dupeName(hwndDlg) )
			       {
			       km2 = MAXNUMRECS + 2;
			       km = MAXNUMRECS;
			       strcpy(selName, dataRecs.noteName);
			       }
			    else
			       {
			       loadIndex(hwndDlg, pdf[0].noteText);
			       return(0);
			       }
			    }
			 else
			    km2 = 0;
			 }
		      }
		   while( km2 == (MAXNUMRECS + 2) );
		   km2 = 0;
		   for( km=0;km<MAXNUMRECS;km++ )
		      {
		      if( strlen(recIndex[km].recName) > 0 )
		      km2++;
		      }
		   if( km2 >= MAXNUMRECS )
		      {
		      WinMessageBox(HWND_DESKTOP,
				    hwndDlg,
				    "Target data file is full!",
				    "Operation can NOT be completed ...",
				    0,
				    MB_OK | MB_ICONEXCLAMATION );
		      loadIndex(hwndDlg, pdf[0].noteText);
		      return(0);
		      }
		   strcpy(recIndex[km2].recName, selName);
		   recIndex[km2].posIndex = -1;
		   saveIndex(mcString);
		   multiWrite(mcString, 1, tFlrs, km2);
		   loadIndex(hwndDlg, pdf[0].noteText);
		   km2 = 0;
		   for( km=0;km<MAXNUMRECS;km++ )
		      {
		      if( strlen(recIndex[km].recName) > 0 )
		      km2++;
		      }
		   recCnt = km2;
		   nch = compName2(orgselName, recCnt);
		   multiRead(datFile, 1, FIXEDLEN, recCnt-1);
		   multiWrite(datFile, 1, FIXEDLEN, nch);
		   truncFile(datFile, recCnt, FIXEDLEN);
		   recIndex[nch] = recIndex[recCnt - 1];
		   recIndex[nch].posIndex = nch;
		   recIndex[nch].alIndex = FIXEDLEN;
		   zeroMostIndex(recCnt - 1);
		   saveIndex(datFile);
		   initLoad_index(WinWindowFromID(hwndDlg, ID_LISTBOX1), datFile);
		   if( rsel == (recCnt - 1) )
		      WinPostMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
				    LM_SELECTITEM,
				    MPFROMSHORT(rsel - 1),
				    MPFROMSHORT(TRUE));
		   else
		      WinPostMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
				    LM_SELECTITEM,
				       MPFROMSHORT(rsel),
				       MPFROMSHORT(TRUE));
		   WinSetFocus(HWND_DESKTOP,
			       WinWindowFromID(hwndDlg, ID_LISTBOX1));
		   WinSendMsg(WinWindowFromID(hwndDlg, ID_DELETE),
				   BM_SETDEFAULT, 0, 0);
		   setStatus(hwndDlg, datFile);
		   DosBeep(100, 50);
		   DosBeep(300, 60);
		   DosBeep(700, 70);
		   DosBeep(1300, 80);
		   DosBeep(2100, 90);
		   DosBeep(3100, 100);
		   }
		break;

	     case ID_HELPME :
		    if( hwndHelp )
		       {
		       WinSendMsg( hwndHelp, HM_HELP_CONTENTS,
				   (MPARAM)0, (MPARAM)0);
		       }
		    break;

	     case ID_WORDWRAP :
		    if( WW )
		       {
		       WW = 0;
		       WinCheckMenuItem(hwndMenu, ID_WORDWRAP, FALSE);
		       WinSetWindowText(WinWindowFromID(hwndFrame, FID_TITLEBAR), wwOff);
		       }
		    else
		       {
		       WW = 1;
		       WinCheckMenuItem(hwndMenu, ID_WORDWRAP, TRUE);
		       WinSetWindowText(WinWindowFromID(hwndFrame, FID_TITLEBAR), wwOn);
		       }
		    break;

	     case ID_NEXTV :
		i = WinQueryLboxCount(WinWindowFromID(hwndDlg, ID_LISTBOX1));
		cc = (USHORT) WinSendMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
					 LM_QUERYSELECTION, 0L, 0L);
		if( cc == i - 1 )
		   cc = 0;
		else
		   cc += 1;
		// WinSendMsg(hwndCurViewRecord, WM_COMMAND, MPFROM2SHORT(ID_RESETV, 0), 0);
		WinSendMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
			   LM_SELECTITEM,
			   MPFROMSHORT(cc),
			   MPFROMSHORT(TRUE));
		WinSendMsg(hwndDlg,
			   WM_COMMAND,
			   MPFROM2SHORT(ID_VIEW, 0),
			   MPFROMP(0));
		break;

	     case ID_LASTV :
		i = WinQueryLboxCount(WinWindowFromID(hwndDlg, ID_LISTBOX1));
		cc = (USHORT) WinSendMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
					 LM_QUERYSELECTION, 0L, 0L);
		if( cc == 0 )
		   cc = i - 1;
		else
		   cc -= 1;
		// WinSendMsg(hwndCurViewRecord, WM_COMMAND, MPFROM2SHORT(ID_RESETV, 0), 0);
		WinSendMsg(WinWindowFromID(hwndDlg, ID_LISTBOX1),
			   LM_SELECTITEM,
			   MPFROMSHORT(cc),
			   MPFROMSHORT(TRUE));
		WinSendMsg(hwndDlg,
			   WM_COMMAND,
			   MPFROM2SHORT(ID_VIEW, 0),
			   MPFROMP(0));
		break;


	     default : break;
	     }
	  break;

/***************************************************
***************************************************/
/***************************************************
***************************************************/
/***************************************************
***************************************************/

 /* ##START Form.38 User defined messages */
 /* ##END User defined messages */
   default :
     HandleMessage(hwndFrame, hwndDlg, msg, mp1, mp2);
     return WinDefDlgProc(hwndDlg,msg,mp1,mp2);
   } /* end switch for main msg dispatch */
   return (MRESULT)FALSE;
} /* end dialog procedure */




HWND OpenMain(HWND hwndOwner, PVOID pInfo)
{
   HWND hwndFrame;
   HWND hwndClient;
   FRAMECDATA frameData;
   frameData.cb=sizeof(FRAMECDATA);
   frameData.flCreateFlags= FCF_NOBYTEALIGN | FCF_SIZEBORDER | FCF_TITLEBAR | FCF_SYSMENU | FCF_TASKLIST | FCF_MINBUTTON | FCF_MENU | FCF_ICON ;
   frameData.hmodResources=0;
   frameData.idResources=ID_MAINFORM;
   if (WinQueryWindowUShort(hwndOwner, QWS_ID) == FID_CLIENT)
      hwndOwner = WinQueryWindow(hwndOwner, QW_PARENT);
   hwndFrame=WinCreateWindow(HWND_DESKTOP, WC_FRAME, "", 0,0,0,0,0,hwndOwner,HWND_TOP,ID_MAINFORM,&frameData,0);
   if (hwndFrame) {
      hwndClient =(HWND) WinLoadDlg(hwndFrame,
	      hwndFrame,
	      MainDlgProc,
	      0, ID_MAINFORM,
	      pInfo);
      WinShowWindow(hwndFrame, TRUE);
   }
   return hwndFrame;
}




MRESULT HandleMessage(HWND hwndFrame, HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  INT i;
  HENUM  henum;
  HWND   hwndCtl;
  HWND   hwndHScroll;
  HWND   hwndVScroll;
  LONG   lOffset;
  POINTL pointl;
  PSWP   pCtlSize;
  PSWP   pswp;
  PULONG *pWindowInfo;
  RECTL  rectl;
  SWP    swp;
  ULONG  ulCnt;
  switch (msg) {
  /* Default initialization of window */
  case WM_INITDLG:
     WinSetWindowULong(hwndDlg, QWL_USER, (ULONG) mp2);
     pWindowInfo = (PULONG *) mp2;
     if (pWindowInfo)
	*pWindowInfo=0; /* clear out size data pointer */
     if (hwndFrame == hwndDlg) {
        ULONG  ulPicture;
        HACCEL hAccel;
	WinLoadMenu(hwndFrame, 0, WinQueryWindowUShort(hwndFrame, QWS_ID));
        hAccel = WinLoadAccelTable(0,0,WinQueryWindowUShort(hwndFrame, QWS_ID));
	if (hAccel)
           WinSetAccelTable(0, hAccel, hwndFrame);
        WinQueryWindowRect(hwndDlg, &rectl);
	if (WinWindowFromID(hwndFrame, FID_MENU))
	   rectl.yTop += WinQuerySysValue(HWND_DESKTOP, SV_CYMENU);
	WinQueryTaskSizePos(0, 0, &swp);
        if (swp.y+rectl.yTop>WinQuerySysValue(HWND_DESKTOP,SV_CYSCREEN)) {
           swp.y=WinQuerySysValue(HWND_DESKTOP,SV_CYSCREEN)-rectl.yTop;
        } /* endif */
        if (swp.x+rectl.xRight>WinQuerySysValue(HWND_DESKTOP,SV_CXSCREEN)) {
	   swp.x=WinQuerySysValue(HWND_DESKTOP,SV_CXSCREEN)-rectl.xRight;
	} /* endif */
        WinSetWindowPos(hwndFrame,0,
	   swp.x, swp.y, rectl.xRight, rectl.yTop, SWP_SIZE|SWP_MOVE);
        ulPicture=(ULONG) WinLoadPointer(HWND_DESKTOP, 0,
              WinQueryWindowUShort(hwndFrame, QWS_ID));
        if (!ulPicture)
           ulPicture=(ULONG)WinQuerySysPointer(HWND_DESKTOP,SPTR_APPICON,FALSE);
        WinSendMsg(hwndFrame, WM_SETICON, (MPARAM) ulPicture, (MPARAM) 0);
     } else {
        WinSetWindowUShort(hwndDlg, QWS_ID, FID_CLIENT);

	/* Calculate window positioning and size */
	WinQueryWindowRect(hwndDlg, &rectl);
	WinSendMsg(hwndFrame, WM_QUERYBORDERSIZE, (MPARAM)&pointl, 0);
	rectl.xLeft += pointl.x;
	rectl.xRight += pointl.x;
	rectl.yBottom += pointl.y;
	rectl.yTop += pointl.y;

	if (WinWindowFromID(hwndFrame,FID_HORZSCROLL)) {
	   rectl.yBottom+=WinQuerySysValue(HWND_DESKTOP,SV_CYHSCROLL);
	   rectl.yTop+=WinQuerySysValue(HWND_DESKTOP,SV_CYHSCROLL);
	}
	WinCalcFrameRect(hwndFrame, &rectl, FALSE);

	WinQueryTaskSizePos(0, 0, &swp);
	if (swp.y+rectl.yTop>WinQuerySysValue(HWND_DESKTOP,SV_CYSCREEN)) {
	   swp.y=WinQuerySysValue(HWND_DESKTOP,SV_CYSCREEN)-rectl.yTop;
	} /* endif */
	if (swp.x+rectl.xRight>WinQuerySysValue(HWND_DESKTOP,SV_CXSCREEN)) {
	   swp.x=WinQuerySysValue(HWND_DESKTOP,SV_CXSCREEN)-rectl.xRight;
	} /* endif */

	WinSetWindowPos(hwndFrame,0,
           swp.x, swp.y, rectl.xRight, rectl.yTop, SWP_SIZE|SWP_MOVE);

        /* Initialize information to handle sizing and scrolling windows */
        pWindowInfo = (PULONG *) mp2;
	if (pWindowInfo)
	   *pWindowInfo = 0;

        /* Count number of controls */
        ulCnt=0;
        henum = WinBeginEnumWindows(hwndDlg);
        hwndCtl=WinGetNextWindow(henum);
	while (hwndCtl) {
           ulCnt++;
           hwndCtl=WinGetNextWindow(henum);
        } /* endwhile */
        WinEndEnumWindows(henum);

        pCtlSize = malloc(sizeof(SWP)*(ulCnt+2));

	/* Store client window information first */
        if (pCtlSize) {
           WinQueryWindowPos(hwndDlg, &(pCtlSize[0]));

           /* Query size information on all controls */
	   ulCnt=1;
	   henum = WinBeginEnumWindows(hwndDlg);
           hwndCtl=WinGetNextWindow(henum);
	   while (hwndCtl) {
	      if (WinQueryWindowPos(hwndCtl, &(pCtlSize[ulCnt]))) {
		 if (!pCtlSize[ulCnt].cx || !pCtlSize[ulCnt].cy)
		    pCtlSize[ulCnt].hwnd = 0;
		 else
		    ulCnt++;
              }
              hwndCtl=WinGetNextWindow(henum);
           } /* endwhile */
           WinEndEnumWindows(henum);
           pCtlSize[ulCnt].hwnd = 0;
        }
       /* Store number of controls */       pCtlSize[1].ulReserved1 = ulCnt;
	pWindowInfo = (PULONG *) mp2;
	*pWindowInfo = (PULONG) pCtlSize;

	/* If scrollable, initialize scroll bars */
	if (WinWindowFromID(hwndFrame,FID_HORZSCROLL)) {
	   HandleMessage(hwndFrame, hwndDlg, WM_SIZE, 0, 0);
	}
     }
     break;
  /* Default WM_SIZE for sizeable or scrollable windows */
  case WM_SIZE:
     if (hwndFrame==hwndDlg)
	return 0;
     pWindowInfo = (PULONG *) WinQueryWindowULong(hwndDlg, QWL_USER);
     if (!pWindowInfo)
	return 0;
     pCtlSize = (PSWP) *pWindowInfo;
     if (!pCtlSize)
	return 0;
     if (!WinQueryWindowPos(hwndDlg,&swp))
	return 0;
     if (!swp.cx || !swp.cy)
	return 0;

     /* Handle sizing of scrollable window */
     hwndHScroll=WinWindowFromID(hwndFrame,FID_HORZSCROLL);
     hwndVScroll=WinWindowFromID(hwndFrame,FID_VERTSCROLL);

     if (hwndHScroll && hwndVScroll) {
        /* Get viewable size of client area */
        WinQueryWindowRect(hwndFrame, &rectl);
        WinCalcFrameRect(hwndFrame, &rectl, TRUE);

	if (pCtlSize[0].cx<=rectl.xRight) {
	   pCtlSize[0].x = 0;
	   WinEnableWindow(hwndHScroll,FALSE);
	   WinSendMsg(hwndHScroll,SBM_SETSCROLLBAR,MPFROMSHORT(0),
                      MPFROM2SHORT(0,0));
           WinSendMsg(hwndHScroll,SBM_SETTHUMBSIZE,
		      MPFROM2SHORT(0,0),0);
	} else {
           pCtlSize[0].x = pCtlSize[0].cx-rectl.xRight+rectl.xLeft;
           pCtlSize[0].ulReserved1= 0;
           WinEnableWindow(hwndHScroll,TRUE);
           WinSendMsg(hwndHScroll,SBM_SETSCROLLBAR,
		      MPFROMSHORT(pCtlSize[0].ulReserved1),
                      MPFROM2SHORT(0,pCtlSize[0].x));
           WinSendMsg(hwndHScroll,SBM_SETTHUMBSIZE,
                      MPFROM2SHORT(rectl.xRight,pCtlSize[0].cx),0);
        } /* endif */
	if (pCtlSize[0].cy<=rectl.yTop) {
	   pCtlSize[0].y = 0;
           WinEnableWindow(hwndVScroll,FALSE);
           WinSendMsg(hwndVScroll,SBM_SETSCROLLBAR,MPFROMSHORT(0),
                      MPFROM2SHORT(0,0));
           WinSendMsg(hwndVScroll,SBM_SETTHUMBSIZE,
		      MPFROM2SHORT(0,0),0);
	} else {
	   pCtlSize[0].y = pCtlSize[0].cy-rectl.yTop+rectl.yBottom;
           pCtlSize[0].ulReserved2= pCtlSize[0].y;
           WinEnableWindow(hwndVScroll,TRUE);
           WinSendMsg(hwndVScroll,SBM_SETSCROLLBAR,
                      MPFROMSHORT(pCtlSize[0].ulReserved2),
		      MPFROM2SHORT(0,pCtlSize[0].y));
           WinSendMsg(hwndVScroll,SBM_SETTHUMBSIZE,
		      MPFROM2SHORT(rectl.yTop, pCtlSize[0].cy),0);
        } /* endif */
        ulCnt = 1;
	hwndCtl=pCtlSize[ulCnt].hwnd;
        while (hwndCtl) {
           WinSetWindowPos(hwndCtl, 0, pCtlSize[ulCnt].x,
              pCtlSize[ulCnt].y-pCtlSize[0].y+pCtlSize[0].ulReserved2,
              0, 0, SWP_MOVE);
	   ulCnt++;
           hwndCtl=pCtlSize[ulCnt].hwnd;
        }

     }
     /* Handle sizing of a regular sizeable window */
     else {
        SWP   swpCtl;
	SWP   swpTest;
        ulCnt = 1;
	hwndCtl=pCtlSize[ulCnt].hwnd;
	while (hwndCtl) {
           swpCtl.x = (pCtlSize[ulCnt].x * swp.cx)/pCtlSize[0].cx;
           swpCtl.y = (pCtlSize[ulCnt].y * swp.cy)/pCtlSize[0].cy;
           swpCtl.cx = (pCtlSize[ulCnt].cx * swp.cx)/pCtlSize[0].cx;
           swpCtl.cy = (pCtlSize[ulCnt].cy * swp.cy)/pCtlSize[0].cy;
           WinSetWindowPos(hwndCtl, 0, swpCtl.x, swpCtl.y, swpCtl.cx, swpCtl.cy, SWP_MOVE | SWP_SIZE);

	   /* If needed, adjust the control position */
           WinQueryWindowPos(hwndCtl,&swpTest);
           if (swpTest.x != swpCtl.x || swpTest.y != swpCtl.y) {
              swpCtl.x += (swpCtl.x - swpTest.x);
              swpCtl.y += (swpCtl.y - swpTest.y);
              swpCtl.cx += (swpCtl.cx - swpTest.cx);
              swpCtl.cy += (swpCtl.cy - swpTest.cy);
              WinSetWindowPos(hwndCtl, 0, swpCtl.x, swpCtl.y, swpCtl.cx, swpCtl.cy, SWP_MOVE | SWP_SIZE);
           }
           ulCnt++;
           hwndCtl=pCtlSize[ulCnt].hwnd;
	}
     }
     break;
  /* If position changed, force a WM_SIZE message  */
  case WM_WINDOWPOSCHANGED:
     if (hwndFrame!=hwndDlg) {
	pWindowInfo = (PULONG *) WinQueryWindowULong(hwndDlg, QWL_USER );
	if (!pWindowInfo)
	   return 0;
	pCtlSize = (PSWP) *pWindowInfo;
	if (pCtlSize) {
	   pswp = (PSWP) mp1;
	   if ((pswp[0].cx != pswp[1].cx) || (pswp[0].cy != pswp[1].cy))
	      WinSendMsg(hwndDlg, WM_SIZE, MPFROM2SHORT(pswp[1].cx, pswp[1].cy),
		   MPFROM2SHORT(pswp[0].cx, pswp[0].cy));
	}
     }
     break;
  /* Default handling for WM_CLOSE */
  case WM_CLOSE:
     {
      WinSetWindowPos(hwndFrame,
		      HWND_TOP, 0, 0, 0, 0,
		      SWP_ACTIVATE | SWP_RESTORE);
      WinQueryWindowPos(hwndFrame, (PSWP) &swp);
      pgmData[0] = swp.x;
      pgmData[1] = swp.y;
      pgmData[36] = swp.cx;
      pgmData[37] = swp.cy;
      pgmData[2] = WW;
      pgmData[3] = fileP;
      pgmData[8] = ACCUM;
      if( ASSOCI )
	 {
	 FILE *mrHanX;
	 CHAR dataFile[MAXNAME] = "";

	 strcpy(dataFile, basePath);
	 if( dataFile[strlen(dataFile)-1] == '\\' )
	    strcat(dataFile, "DATAPAD.DAT");
	 else
	    strcat(dataFile, "\\DATAPAD.DAT");
	 strcpy(datFile, datFileOrg);
	 if ((mrHanX = fopen( dataFile, MRW)) == NULL)
	    mrHanX = fopen( dataFile, "w+b" );
	 fseek(mrHanX, 0L, SEEK_SET);
	 fwrite(&pdf2, sizeof(pdf2), 1, mrHanX);
	 fclose(mrHanX);
	 }
      PrfWriteProfileString(HINI_USERPROFILE, "PAD21", "FILEEXT", EXTENTION);
      PrfWriteProfileString(hini, "PAD21", "DATAFILE", datFile);
      PrfWriteProfileString(hini, "PAD21", "DEFLOADMOD", stickyName);
      PrfWriteProfileData(hini,
			  "PAD21",
			  "WINPOS",
			  &pgmData,
			  sizeof(pgmData));
      PrfWriteProfileData(hini,
			  "PAD21",
			  "WINPRES",
			  &pValue,
			  sizeof(pValue));
      for( i=1;i<NOTMPFILES;i++ )
	 {
	 CHAR sd[3];

	 strcpy(szFullDocName, basePath);
	 if( szFullDocName[strlen(szFullDocName)-1] != '\\' )
	    strcat(szFullDocName, "\\");
	 strcat(szFullDocName, "TMP");
	 itoa(i, sd, 10);
	 strcat(szFullDocName, sd);
	 strcat(szFullDocName, ".TXT");
	 remove(szFullDocName);
	 }
      if ( strcmp(Honor, "g...rit") != 0 )
	 aboutR(hwndDlg);
      DosCloseEventSem(hev);
      DosCloseEventSem(hevS);

     WinDismissDlg(hwndFrame, DID_CANCEL);
     WinDestroyWindow(hwndFrame);
     exit(0);
     }
     break;

  case WM_HSCROLL:
     if (hwndFrame==hwndDlg)
	return 0;
     pWindowInfo = (PULONG *) WinQueryWindowULong(hwndDlg, QWL_USER);
     if (!pWindowInfo)
	return 0;
     pCtlSize = (PSWP) *pWindowInfo;
     if (!pCtlSize)
	return 0;
     hwndHScroll=WinWindowFromID(hwndFrame,FID_HORZSCROLL);
     lOffset = pCtlSize[0].ulReserved1;
     switch (SHORT2FROMMP(mp2)) {
     case SB_LINELEFT:
        lOffset--;
        break;
     case SB_LINERIGHT:
        lOffset++;
        break;
     case SB_PAGELEFT:
        WinQueryWindowRect(hwndFrame, &rectl);
        WinCalcFrameRect(hwndFrame, &rectl, TRUE);
        lOffset-=rectl.xRight;
	break;
     case SB_PAGERIGHT:
        WinQueryWindowRect(hwndFrame, &rectl);
        WinCalcFrameRect(hwndFrame, &rectl, TRUE);
        lOffset+=rectl.xRight;
	break;
     case SB_SLIDERTRACK:
     case SB_SLIDERPOSITION:
        lOffset = SHORT1FROMMP(mp2);
	break;
     } /* endswitch */

     if (lOffset < 0)
        lOffset = 0;
     if (lOffset > pCtlSize[0].x)
        lOffset = pCtlSize[0].x;
     if (lOffset > pCtlSize[0].ulReserved1) {
        ulCnt = 1;
        hwndCtl=pCtlSize[ulCnt].hwnd;
        while (hwndCtl) {
           WinSetWindowPos(hwndCtl, 0, pCtlSize[ulCnt].x-lOffset,
              pCtlSize[ulCnt].y-pCtlSize[0].y+pCtlSize[0].ulReserved2,
              0, 0, SWP_MOVE );
           ulCnt++;
           hwndCtl=pCtlSize[ulCnt].hwnd;
        }
        pCtlSize[0].ulReserved1 = lOffset;
        WinSendMsg(hwndHScroll,SBM_SETPOS, MPFROMSHORT(lOffset),0);
     } /* endif */
     else
     if (lOffset < pCtlSize[0].ulReserved1) {
        ulCnt = pCtlSize[1].ulReserved1;
	while (ulCnt >0) {
	   hwndCtl=pCtlSize[ulCnt].hwnd;
           WinSetWindowPos(hwndCtl, 0, pCtlSize[ulCnt].x-lOffset,
              pCtlSize[ulCnt].y-pCtlSize[0].y+pCtlSize[0].ulReserved2,
              0, 0, SWP_MOVE);
           ulCnt--;
        }
        pCtlSize[0].ulReserved1 = lOffset;
        WinSendMsg(hwndHScroll,SBM_SETPOS, MPFROMSHORT(lOffset),0);
     } /* endif */
     break;
  case WM_VSCROLL:
     if (hwndFrame==hwndDlg)
        return 0;
     pWindowInfo = (PULONG *) WinQueryWindowULong(hwndDlg, QWL_USER);
     if (!pWindowInfo)
	return 0;
     pCtlSize = (PSWP) *pWindowInfo;
     if (!pCtlSize)
        return 0;
     hwndVScroll=WinWindowFromID(hwndFrame,FID_VERTSCROLL);
     lOffset = pCtlSize[0].ulReserved2;
     switch (SHORT2FROMMP(mp2)) {
     case SB_LINEUP:
        lOffset--;
        break;
     case SB_LINEDOWN:
        lOffset++;
        break;
     case SB_PAGEUP:
        WinQueryWindowRect(hwndFrame, &rectl);
        WinCalcFrameRect(hwndFrame, &rectl, TRUE);
        lOffset-=rectl.yTop;
        break;
     case SB_PAGEDOWN:
        WinQueryWindowRect(hwndFrame, &rectl);
        WinCalcFrameRect(hwndFrame, &rectl, TRUE);
        lOffset+=rectl.yTop;
        break;
     case SB_SLIDERTRACK:
     case SB_SLIDERPOSITION:
        lOffset = SHORT1FROMMP(mp2);  /* verified should be positive */
        break;
     } /* endswitch */

     if (lOffset < 0)
	lOffset = 0;
     if (lOffset > pCtlSize[0].y)
	lOffset = pCtlSize[0].y;
     if (lOffset < pCtlSize[0].ulReserved2) {
	ulCnt = 1;
        hwndCtl=pCtlSize[ulCnt].hwnd;
        while (hwndCtl) {
           WinSetWindowPos(hwndCtl, 0,
              pCtlSize[ulCnt].x-pCtlSize[0].ulReserved1,
              pCtlSize[ulCnt].y-pCtlSize[0].y+lOffset, 0, 0, SWP_MOVE);
           ulCnt++;
           hwndCtl=pCtlSize[ulCnt].hwnd;
        }
        pCtlSize[0].ulReserved2 = lOffset;
        WinSendMsg(hwndVScroll,SBM_SETPOS, MPFROMSHORT(lOffset),0);
     } /* endif */
    else
    if (lOffset > pCtlSize[0].ulReserved2) {
       ulCnt = pCtlSize[1].ulReserved1;
       while (ulCnt > 0) {
	  hwndCtl=pCtlSize[ulCnt].hwnd;
	  WinSetWindowPos(hwndCtl, 0,
             pCtlSize[ulCnt].x-pCtlSize[0].ulReserved1,
             pCtlSize[ulCnt].y-pCtlSize[0].y+lOffset, 0, 0, SWP_MOVE);
          ulCnt--;
       }
       pCtlSize[0].ulReserved2 = lOffset;
       WinSendMsg(hwndVScroll,SBM_SETPOS, MPFROMSHORT(lOffset),0);
    } /* endif */
     break;
  /* When window is destroyed, deallocate window memory */
  case WM_DESTROY:
     pWindowInfo = (PULONG *) WinQueryWindowULong(hwndDlg, QWL_USER);
     if (pWindowInfo) {
        pCtlSize = (PSWP) *pWindowInfo;
        if (pCtlSize)
           free(pCtlSize);
        free(pWindowInfo);
	WinSetWindowULong(hwndDlg, QWL_USER, 0L);
     }
     break;
  }
  return (MRESULT) FALSE;
}

/***************************************************************/
/* MLEGetItemText                                              */
/*                                                             */
/* Description:                                                */
/*                                                             */
/* This helper function is used easily retrieve all or part    */
/* of the text within an MLE.  There is no limit on the amount */
/* of text to be retrieved.                                    */
/*                                                             */
/* Parameters:                                                 */
/*                                                             */
/*    hwndDlg      - handle to window containing the MLE       */
/*    ulID         - MLE identifier                            */
/*    lIptPosition - Starting IPT position to get value        */
/*                   Use 0 to start at the beginning           */
/*                   Use -1 to start at the cursor position    */
/*    lBoundary    - Number of characters to get               */
/*                   Used -1 to get characters until the end   */
/*    ppText       - Pointer to receive a pointer to text area */
/***************************************************************/
ULONG MLEGetItemText(HWND hwndDlg, ULONG ulID, LONG lIptPosition,
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

/***************************************************************/
/* MLEInsertText                                               */
/*                                                             */
/* Description:                                                */
/*                                                             */
/* This helper function is used easily insert text within the  */
/* MLE.  There is no limit on the amount of text to be inserted*/
/*                                                             */
/* Parameters:                                                 */
/*                                                             */
/*    hwndDlg      - handle to window containing the MLE       */
/*    ulID         - MLE identifier                            */
/*    lIptPosition - IPT to insert text                        */
/*                   Use 0 to start at the beginning           */
/*                   Use -1 to start at the cursor position    */
/*    pText        - Pointer to text area                      */
/***************************************************************/
ULONG MLEInsertText(HWND hwndDlg, ULONG ulID, LONG iptPosition, PBYTE pText)
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



MRESULT EXPENTRY nWinProc( HWND hwndW, ULONG msg,
			   MPARAM mp1, MPARAM mp2 )
{
PFNWP oWinProc;
CHAR szCont[MAXNAME] = "";
CHAR szSource[MAXNAME] = "";
ULONG attrFound;
HAB hab;
PDRAGINFO pdrginfox;
// HWND dWnd;


oWinProc = (PFNWP) WinQueryWindowPtr(hwndW, 0);
if( WinIsWindow(hab, hwndCurEViewRecord) || WinIsWindow(hab, hwndCurViewRecord) )
   return( *oWinProc ) (hwndW, msg, mp1, mp2);
switch(msg)
   {
   case WM_PRESPARAMCHANGED :
      setPPMenuDef(32);
      WinQueryPresParam(hwndW, PP_FONTNAMESIZE, 0, &attrFound,
			    sizeof(pValue[12]), &pValue[12], QPF_NOINHERIT);
      WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndW, QW_PARENT), ID_RECNAME2 ),
			   PP_FONTNAMESIZE,
			   sizeof(pValue[12]),
			   &pValue[12] ) ;
      WinQueryPresParam(hwndW, PP_FOREGROUNDCOLOR, 0, &attrFound,
			    sizeof(pValue[13]), &pValue[13], QPF_NOINHERIT);
      WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndW, QW_PARENT), ID_RECNAME2 ),
			      PP_FOREGROUNDCOLOR,
			      sizeof(pValue[13]),
			      &pValue[13] ) ;
      WinQueryPresParam(hwndW, PP_BACKGROUNDCOLOR, 0, &attrFound,
			    sizeof(pValue[14]), &pValue[14], QPF_NOINHERIT);
      WinSetPresParam ( WinWindowFromID ( WinQueryWindow(hwndW, QW_PARENT), ID_RECNAME2 ),
			      PP_BACKGROUNDCOLOR,
			      sizeof(pValue[14]),
			      &pValue[14] ) ;
      break;

   case WM_CONTEXTMENU :
      {
      POINTL ptlMouse;


      WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
      WinPopupMenu(HWND_DESKTOP,
		   WinQueryWindow(hwndW, QW_OWNER),
		   hwndMLBPopup,
		   ptlMouse.x, ptlMouse.y,
		   0,
		   PU_HCONSTRAIN |
		   PU_VCONSTRAIN |
		   PU_KEYBOARD |
		   PU_MOUSEBUTTON2 |
		   PU_MOUSEBUTTON1 );
      // setParams(hwndMLBPopup, 12, ID_LISTBOX1, ID_LISTBOX1);

      WinSetPresParam(hwndMLBPopup,
		      PP_FONTNAMESIZE,
		      sizeof(pValue[12])+1,
		      &pValue[12]);

      }
      break;

   case WM_BEGINDRAG:
	{
	DRAGITEM drgitem, *pdrgitem ;
	DRAGIMAGE drgimage ;
	HWND hwndList ;
	CHAR szFullPathName[ MAXNAME] ;
	CHAR targetName[ MAXNAME] ;
	HPOINTER  standard;
	SHORT che, inde;
	CHAR cwd[MAXNAME];
	FILE *hand;
	HWND dWnd;
	HOBJECT hobj;
	CHAR rfiles[MAXNAME];
	CHAR rfile[MAXNAME];
	BOOL foundsome;
	FILEFINDBUF3 findbuf;
	HDIR hDir;
	ULONG usSearchCount;
	ULONG ALLFILES;


	ALLFILES = FILE_NORMAL;
	dWnd = NULLHANDLE;
	inde = 0;
	che = 0;
	standard = WinLoadPointer(HWND_DESKTOP, 0L, ID_POINTER2);
	hwndList = WinWindowFromID(WinQueryWindow(hwndW, QW_PARENT), ID_LISTBOX1);

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

	strcpy(szFullDocName, szFullPathName);
	che = SHORT1FROMMR(WinSendMsg(hwndList,
			   LM_QUERYSELECTION,
			   MPFROMSHORT(NULL), NULL));
	inde = compName(WinQueryWindow(hwndW, QW_PARENT), che);
	multiRead(datFile, 1, FIXEDLEN, inde);

	strcpy(targetName, dataRecs.noteName);
	strcat(szFullDocName, targetName);

	hand = fopen(szFullDocName, "w+b");
	if( hand == NULL )
	   break;
	addFWHeader(hand, dataRecs.noteName);
	fwrite(&dataRecs.noteText, strlen(dataRecs.noteText), 1, hand);
	fclose(hand);

	pdrginfox = DrgAllocDraginfo( 1L) ;
	pdrginfox -> hwndSource = hwndW ;

	drgitem.hwndItem = hwndW ;
	drgitem.ulItemID = 100L ;
	drgitem.hstrType = DrgAddStrHandle( DRT_TEXT) ;
	drgitem.hstrRMF = DrgAddStrHandle( "(DRM_OS2FILE)x(DRF_TEXT,DRF_UNKNOWN)" ) ;
	drgitem.hstrContainerName = DrgAddStrHandle( szFullPathName) ;
	drgitem.hstrSourceName = DrgAddStrHandle(targetName);
	drgitem.hstrTargetName = DrgAddStrHandle(targetName);
	drgitem.cxOffset = 0 ;
	drgitem.cyOffset = 0 ;
	drgitem.fsControl = DC_REMOVEABLEMEDIA ;
	// drgitem.fsSupportedOps = DO_MOVEABLE;
	drgitem.fsSupportedOps = DO_COPYABLE;

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
	dWnd = DrgDrag(hwndW,
		       pdrginfox,
		       &drgimage,
		       1L,
		       VK_ENDDRAG,
		       NULL);
	if( dWnd == NULLHANDLE )
	   {
	   DrgFreeDraginfo(pdrginfox);
	   remove(szFullDocName);
	   }
	else
	   dWnd = NULLHANDLE;
	}
	break;

   case DM_ENDCONVERSATION:
	{
	DrgFreeDraginfo(pdrginfox);
	remove(szFullDocName);
	// DosBeep(100, 100);
	}
	break ;

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

      if( pdrginfo->hwndSource == hwndW )
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
      WinPostMsg(WinQueryWindow(hwndW, QW_PARENT), WM_COMMAND, MPFROMP(ID_OVERSIZEDX), 0L);
      break;

   default :
      break;
   }
return( *oWinProc ) (hwndW, msg, mp1, mp2);
}


VOID setStatus(HWND hwndST, CHAR *pFile)
{
CHAR parsedFile[MAXNAME];
CHAR mStatus[80] = "";
CHAR totRecStat[] = " Records";
INT rcCnt = 0;
CHAR cntStr[10] = "";
SHORT i, k;
CHAR flrs[6];

strcpy(parsedFile, "");
for( k=0,i=0;i<strlen(pFile);i++ )
   {
   if( pFile[i] == '\\' )
      k = i;
   }
if( k != 0 )
   {
   strncpy(parsedFile, pFile+(k+1), strlen(pFile) - (k+1));
   parsedFile[strlen(pFile) - (k+1)] = '\0';
   }
else
   strcpy(parsedFile, pFile);
rcCnt = WinQueryLboxCount(WinWindowFromID(hwndST, ID_LISTBOX1));
itoa(rcCnt, cntStr, 10);
itoa(FIXEDLEN*.001, flrs, 10);
strcat(mStatus, parsedFile);
strcat(mStatus, "    " );
strcat(mStatus, cntStr );
strcat(mStatus, "    " );
strcat(mStatus, flrs);
strcat(mStatus, "k");
strcat(mStatus, totRecStat);
WinSetWindowText(WinWindowFromID(hwndST, ID_PROGSTATUS), mStatus);
}


INT chkPDF(CHAR *dFiles)
{
INT i;
FILE *mrHan;
CHAR dataFile[MAXNAME] = "";
BOOL MATCHEDIT;

MATCHEDIT = FALSE;
strcpy(dataFile, basePath);
if( dataFile[strlen(dataFile)-1] == '\\' )
   strcat(dataFile, "DATAPAD.DAT");
else
   strcat(dataFile, "\\DATAPAD.DAT");
for( i=0;i<NUMPDFMODS;i++ )
   pdf[i].noteText[0] = '\0';
if( strlen(dFiles) > 0 )
   {
   if( strnicmp(dFiles+(strlen(dFiles)-4), EXTENTION, 4) == 0 )
      {
      for( i=0;i<NUMPDFMODS;i++ )
	 pdf2[i].noteText[0] = '\0';
      ASSOCI = TRUE;
      mrHan = fopen( dataFile, MRO);
      fseek(mrHan, 0L, SEEK_SET);
      fread(&pdf2, sizeof(pdf2), 1, mrHan);
      fclose(mrHan);
      strcpy(datFileOrg, datFile);
      strcpy(datFile, dFiles);
      strcpy(pdf[0].noteText, dFiles);
      if ((mrHan = fopen( dataFile, MRW)) == NULL)
	 mrHan = fopen( dataFile, "w+b" );
      fseek(mrHan, 0L, SEEK_SET);
      fwrite(&pdf, sizeof(pdf), 1, mrHan);
      fclose(mrHan);
      return(1);
      }
   else
      ASSOCI = FALSE;
   }
else
   ASSOCI = FALSE;
if ( _dos_findfirst(dataFile,_A_NORMAL,&ffblk) == 0 )
   {
   mrHan = fopen( dataFile, MRO);
   fseek(mrHan, 0L, SEEK_SET);
   fread(&pdf, sizeof(pdf), 1, mrHan);
   fclose(mrHan);
   }
else
   {
   strcpy(pdf[0].noteText, datFile);
   }
if(ASSOCI == FALSE )
   {
   INT qn;

   if( strcmp(stickyName, ndName) != 0 )
      {
      for( qn=0;qn<NUMPDFMODS;qn++ )
	 {
	 if( strcmp(stickyName, pdf[qn].noteText) == 0 )
	    {
	    strcpy(datFile, stickyName);
	    strcpy(pdf[qn].noteText, pdf[0].noteText);
	    strcpy(pdf[0].noteText, stickyName);
	    MATCHEDIT = TRUE;
	    }
	 }
      }
   }
if( !MATCHEDIT )
   strcpy(stickyName, ndName);
if ((mrHan = fopen( dataFile, MRW)) == NULL)
   mrHan = fopen( dataFile, "w+b" );
fseek(mrHan, 0L, SEEK_SET);
fwrite(&pdf, sizeof(pdf), 1, mrHan);
fclose(mrHan);
return(1);
}


VOID setPadPath(HWND hwndPP)
{
WinDlgBox(HWND_DESKTOP,
	  hwndPP,
	  pathProc,
	  (HMODULE)0,
	  ID_GETPADPATH,
	  NULL);

}



MRESULT EXPENTRY pathProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
CHAR temp1[MAXNAME];
INT len;

switch ( msg )
   {
   case WM_INITDLG:
      WinSendDlgItemMsg(hwndDlg, ID_PADPATH,EM_SETTEXTLIMIT,
			MPFROM2SHORT(MAXNAME,0),NULL);
      break;

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_OK :
	    WinQueryDlgItemText(hwndDlg, ID_PADPATH,
				sizeof(temp1), temp1);
	    if( existPath(temp1) )
	       {
	       len = strlen(temp1);
	       if(temp1[len - 1] == '\\')
		  temp1[len - 1] = '\0';
	       strcpy(basePath, temp1);
	       WinDismissDlg( hwndDlg, TRUE );
	       }
	    else
	       WinMessageBox(HWND_DESKTOP,
			     hwndDlg,
			     "Invalid Path!",
			     "Attention ...",
			     0,
			     MB_OK | MB_INFORMATION );
	    break;

	 case DID_CANCEL :
	    WinDismissDlg( hwndDlg, FALSE );
	    exit(-1);
	    break;
	 }
      break;
   default :
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
   }
return (MRESULT) FALSE;
}

VOID setBasePath()
{
CHAR buf[MAXNAME];
INT drv;

drv = _getdrive();
if(_getdcwd(drv, buf, sizeof(buf)) == NULL)
   strcpy(basePath, "");
else
   strcpy(basePath, buf);
}


INT validatePDF(CHAR *df)
{
FILE *han;
INT i, nom, mn;

mn = 0;
han = fopen(df, MRO);
fseek(han, 0L, SEEK_SET);
fread(&pdf2, sizeof(pdf2), 1, han);
fclose(han);

for( nom=0;nom<NUMPDFMODS;nom++ )
   {
   pdf[nom].noteText[0] = '\0';
   if( strlen(pdf2[nom].noteText) > 0 )
      mn++;
   }
nom = mn;
for( mn=0;mn<nom;mn++ )
   {
   if ( _dos_findfirst(pdf2[mn].noteText,_A_NORMAL,&ffblk) != 0 )
      strcpy(pdf2[mn].noteText, "");
   }
i = 0;
for( mn=0;mn<nom;mn++ )
   {
   if ( strlen(pdf2[mn].noteText) > 0 )
      {
      strcpy(pdf[i].noteText, pdf2[mn].noteText);
      i++;
      }
   }
if( i == 0 )
   strcpy(pdf[0].noteText, "");
han = fopen(df, MRW);
fseek(han, 0L, SEEK_SET);
fwrite(&pdf, sizeof(pdf), 1, han);
fclose(han);
if( strlen(pdf[0].noteText) > 0 )
   return(1);
else
   return(-1);
}


VOID saveATFile(VOID *hwnd)
{
HAB habT;
HMQ hmqT;
INT b,norc, rNum;
FILE *handOut;

habT = WinInitialize(0);
hmqT = WinCreateMsgQueue(habT, 0);
WinCancelShutdown(hmqT, TRUE);

norc = WinQueryLboxCount(WinWindowFromID((HWND)hwnd, ID_LISTBOX1));
if( norc > 0 )
   {
   handOut = fopen(cwdFound, "w+b");
   if( handOut )
      {
      for( b=0;b<norc;b++ )
	 {
	 zeroDataRecs(dataRecs.noteText);
	 rNum = compName((HWND)hwnd, b);
	 multiRead(datFile, 1, FIXEDLEN, rNum);
	 saveToFile(cwdFound, handOut);
	 }
      fclose(handOut);
      DosBeep(2400, 100);
      }
   else
      {
      msgOK("Error!", "File was NOT saved.");
      }
   }

WinDestroyMsgQueue(hmqT);
WinTerminate(habT);
_endthread();
}


VOID setFE(HWND hwndPP)
{
WinDlgBox(HWND_DESKTOP,
	  hwndPP,
	  setFEProc,
	  (HMODULE)0,
	  ID_SETFILEEXTSCR,
	  NULL);

}


VOID setINITFE(HWND hwndPP)
{
WinDlgBox(HWND_DESKTOP,
	  hwndPP,
	  setFEProcINIT,
	  (HMODULE)0,
	  ID_SETFILEEXTSCR,
	  NULL);

}



MRESULT EXPENTRY setFEProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{

switch ( msg )
   {
   case WM_INITDLG:
      WinSendDlgItemMsg(hwndDlg, ID_ENTRYFEXT,EM_SETTEXTLIMIT,
			MPFROM2SHORT(EXTSIZE-1,0),NULL);
      WinSetDlgItemText(hwndDlg, ID_ENTRYFEXT, EXTENTION);
      WinSendDlgItemMsg(hwndDlg ,ID_ENTRYFEXT, EM_SETSEL ,
		       MPFROM2SHORT(0,EXTSIZE-1), MPVOID  ) ;
      break;

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_OK :
	    {
	    CHAR daFile[MAXNAME];
	    CHAR tempEXT[EXTSIZE];

	    strcpy(daFile, basePath);
	    if( daFile[strlen(daFile)-1] == '\\' )
	       strcat(daFile, "DATAPAD.DAT");
	    else
	       strcat(daFile, "\\DATAPAD.DAT");

	    WinQueryDlgItemText(hwndDlg,
				ID_ENTRYFEXT,
				sizeof(tempEXT),
				tempEXT);

	    if( strlen(tempEXT) != EXTSIZE-1 || tempEXT[0] != '.' ||
		(tempEXT[1] < 21 && tempEXT[1] > 126) ||
		(tempEXT[2] < 21 && tempEXT[1] > 126) ||
		(tempEXT[3] < 21 && tempEXT[1] > 126) )

	       {
	       msgOK("Error!", "File extention not allowed.");
	       break;
	       }
	    else
	       strcpy(EXTENTION, tempEXT);

	    PrfWriteProfileString(HINI_USERPROFILE, "PAD21", "FILEEXT", EXTENTION);
	    remove(daFile);
	    // debugMsgStr(daFile, "daFile");
	    WinDismissDlg(hwndDlg, TRUE );
	    exit(0);
	    }
	    break;

	 case DID_CANCEL :
	    WinDismissDlg(hwndDlg, FALSE);
	    break;
	 }
      break;
   default :
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
   }
return (MRESULT) FALSE;
}


MRESULT EXPENTRY setFEProcINIT( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{

switch ( msg )
   {
   case WM_INITDLG:
      WinSendDlgItemMsg(hwndDlg, ID_ENTRYFEXT,EM_SETTEXTLIMIT,
			MPFROM2SHORT(EXTSIZE-1,0),NULL);
      WinSetDlgItemText(hwndDlg, ID_ENTRYFEXT, EXTENTION);
      WinSendDlgItemMsg(hwndDlg ,ID_ENTRYFEXT, EM_SETSEL ,
		       MPFROM2SHORT(0,EXTSIZE-1), MPVOID  ) ;
      break;

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_OK :
	    {
	    CHAR daFile[MAXNAME];
	    CHAR tempEXT[EXTSIZE];

	    strcpy(daFile, basePath);
	    if( daFile[strlen(daFile)-1] == '\\' )
	       strcat(daFile, "DATAPAD.DAT");
	    else
	       strcat(daFile, "\\DATAPAD.DAT");

	    WinQueryDlgItemText(hwndDlg,
				ID_ENTRYFEXT,
				sizeof(tempEXT),
				tempEXT);

	    if( strlen(tempEXT) != EXTSIZE-1 || tempEXT[0] != '.' ||
		(tempEXT[1] < 21 && tempEXT[1] > 126) ||
		(tempEXT[2] < 21 && tempEXT[1] > 126) ||
		(tempEXT[3] < 21 && tempEXT[1] > 126) )

	       {
	       msgOK("Error!", "File extention not allowed.");
	       break;
	       }
	    else
	       strcpy(EXTENTION, tempEXT);

	    PrfWriteProfileString(HINI_USERPROFILE, "PAD21", "FILEEXT", EXTENTION);
	    // remove(daFile);
	    // debugMsgStr(daFile, "daFile");
	    WinDismissDlg(hwndDlg, TRUE );
	    // EXIT(0);
	    }
	    break;

	 case DID_CANCEL :
	    WinDismissDlg(hwndDlg, FALSE);
	    exit(0);
	    break;
	 }
      break;
   default :
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
   }
return (MRESULT) FALSE;
}


/*
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
*/



VOID enableMainScrX(HWND hwndz)
{
SWP swp;


WinEnableMenuItem(hwndMenu, ID_OPTIONS , TRUE);
WinEnableMenuItem(hwndMenu, ID_HELPME , TRUE);
WinEnableMenuItem(hwndMenu, ID_MISC , TRUE);
WinEnableMenuItem(hwndMenu, ID_SETMODULES , TRUE);
// WinEnableWindow(hwnd1, TRUE);
WinEnableWindow(hwnd2, TRUE);
WinEnableWindow(hwnd3, TRUE);
WinEnableWindow(hwnd4, TRUE);
WinEnableWindow(hwnd5, TRUE);
WinEnableWindow(hwnd6, TRUE);
WinEnableWindow(hwnd7, TRUE);
// WinEnableWindow(hwnd8, TRUE);
// WinEnableWindow(hwnd9, TRUE);
// WinEnableWindow(hwnd10, TRUE);
WinEnableWindow(hwnd11, TRUE);
WinEnableWindow(hwnd12, TRUE);
}

VOID disableMainScrX(HWND hwndz, INT mode)
{
WinEnableMenuItem(hwndMenu, ID_OPTIONS , FALSE);
WinEnableMenuItem(hwndMenu, ID_HELPME , FALSE);
WinEnableMenuItem(hwndMenu, ID_MISC , FALSE);
WinEnableMenuItem(hwndMenu, ID_SETMODULES , FALSE);
// WinEnableWindow(hwnd1, FALSE);
if( mode == 0 )
   {
   WinEnableWindow(hwnd2, FALSE);
   WinEnableWindow(hwnd3, FALSE);
   }
if( mode == 3 )
   WinEnableWindow(hwnd2, FALSE);
if( mode == 2 )
   WinEnableWindow(hwnd3, FALSE);
WinEnableWindow(hwnd4, FALSE);
WinEnableWindow(hwnd5, FALSE);
WinEnableWindow(hwnd6, FALSE);
WinEnableWindow(hwnd7, FALSE);
// WinEnableWindow(hwnd8, FALSE);
// WinEnableWindow(hwnd9, FALSE);
// WinEnableWindow(hwnd10, FALSE);
WinEnableWindow(hwnd11, FALSE);
WinEnableWindow(hwnd12, FALSE);
}



