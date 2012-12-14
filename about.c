/* ABOUT.C  2.1  7-17-95 */

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

VOID about(HWND hwndDlg);
VOID aboutR(HWND hwndDlg);
VOID helpText(HWND hwndDlg);
LONG loadFile (char *fn);
MRESULT EXPENTRY aboutProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY aboutProcR( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY helpProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );

PVOID iobuf;


VOID aboutR(HWND hwndDlg)
{
WinDlgBox(
   HWND_DESKTOP,
   hwndDlg,
   aboutProcR,
   (HMODULE)0,
   ID_ABOUTR,
   NULL);
}

VOID about(HWND hwndDlg)
{
WinDlgBox(
   HWND_DESKTOP,
   hwndDlg,
   aboutProc,
   (HMODULE)0,
   ID_ABOUT,
   NULL);
}

VOID helpText(HWND hwndDlg)
{
WinDlgBox(
   HWND_DESKTOP,
   hwndDlg,
   helpProc,
   (HMODULE)0,
   ID_HELPBOX,
   NULL);
}


MRESULT EXPENTRY aboutProcR( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
INT i;
switch ( msg )
   {
   case WM_INITDLG:
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_CANCEL :
	    WinShowWindow(WinWindowFromID(hwndDlg, DID_CANCEL), FALSE);
	    for( i=0;i<4;i++ )
	       {
	       WinShowWindow(WinWindowFromID(hwndDlg, ID_BEGSCREEN), FALSE);
	       DosSleep(200);
	       WinShowWindow(WinWindowFromID(hwndDlg, ID_BEGSCREEN), TRUE);
	       DosSleep(200);
	       }
	    WinDismissDlg( hwndDlg, TRUE );
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

MRESULT EXPENTRY aboutProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
INT i;
switch ( msg )
   {
   case WM_INITDLG:
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )
	 {
	 case DID_CANCEL :
	    WinDismissDlg( hwndDlg, TRUE );
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

MRESULT EXPENTRY helpProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 )
{
LONG bytes;
IPT ipt;

switch ( msg )
   {
   case WM_INITDLG:
      WinSendMsg (WinWindowFromID(hwndDlg, ID_HELPENTRY),
		  MLM_SETTEXTLIMIT,
		  MPFROMLONG (30000L), (MPARAM) 0L);
      WinSendMsg(WinWindowFromID(hwndDlg, ID_HELPENTRY),
		 MLM_SETTEXTCOLOR,
		 MPFROMLONG (CLR_DARKBLUE), 0L);
      WinSendMsg(WinWindowFromID(hwndDlg, ID_HELPENTRY),
		 MLM_SETBACKCOLOR,
		 MPFROMLONG (CLR_WHITE), 0L);
      bytes = loadFile("DataPad.DOC");
      if( bytes <= 0 )
	 {
	 DosFreeMem(iobuf);
	 WinDismissDlg( hwndDlg, TRUE );
	 }
      WinSendMsg(WinWindowFromID(hwndDlg, ID_HELPENTRY),
		 MLM_SETIMPORTEXPORT,
		 (MPARAM) iobuf,
		 MPFROMLONG(bytes));
      ipt = -1;
      WinSendMsg(WinWindowFromID(hwndDlg, ID_HELPENTRY),
		 MLM_IMPORT,
		 (MPARAM)&ipt,
		 MPFROMLONG(bytes));
      DosFreeMem(iobuf);
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );

   default :
      return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
   }
}


LONG loadFile (char *fn)
{
HFILE file;   /* handle */
ULONG ac;   /* action code (from DosOpen) */
APIRET rc;   /* return code (from API functions) */
ULONG rd;   /* bytes actually read */
ULONG cbSize;   /* file size in bytes */
ULONG dummy;   /* used when seeking back to beginning of file */

/* try to open existing file --  access */
rc = DosOpen (fn, &file, &ac, 0, 0,
	      OPEN_ACTION_OPEN_IF_EXISTS,
	      OPEN_FLAGS_SEQUENTIAL |
	      OPEN_SHARE_DENYNONE |
	      OPEN_ACCESS_READONLY,
	      0);
if (rc != 0)    /* failed */
   return (0);

DosChgFilePtr (file, 0L, 2, &cbSize);   /* determine file size */
if (cbSize > 30000L)
   {
   DosBeep(3700, 100);
   DosClose (file);
   return (-1);
   }

DosChgFilePtr (file, 0L, 0, &dummy);   /* reset pointer to beginning */
rc = DosAllocMem(&iobuf,               /* Allocate memory object         */
		 (INT)cbSize,          /* Size of memory object          */
		 PAG_COMMIT |          /* Commit memory immediately      */
		 PAG_READ   |          /* Allow read access              */
		 PAG_WRITE);           /* Allow write access             */

if (rc != 0)
   return (0);

/* read entire file into buffer */
DosRead (file, iobuf, cbSize, &rd);
DosClose (file);
return (LONG)cbSize;
}


