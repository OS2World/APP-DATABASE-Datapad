/* IMPORT function  IMPORT.C  2.1 7-17-95 */

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
#include <sys/types.h>
#include <process.h>
#include "paddef.h"

extern VOID setStatus(HWND hwndST, CHAR *pFile);
extern VOID zeroDataRecs(CHAR *noteText);
extern VOID saveIndex(CHAR *dataFileName);
extern BOOL FindFile (HWND hwndWnd);
extern INT matchName(HWND hwndM, CHAR *lbSelect);
extern VOID dBaseFull(HWND hwndDB);
extern INT osName(HWND hwndDlg);
extern VOID parseName(CHAR *name);
extern INT chkDupeName(HWND hwndDlg);
extern INT multiRead(CHAR *inFile, INT datS, INT sizeF, INT recNum);
extern INT multiWrite(CHAR *inFile, INT datS, INT sizeF, INT recNum);
INT importFile(UINT mode);

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
extern CHAR fullFileName[MAXNAME];
extern CHAR datFile[MAXNAME];
extern INT nCho;
extern CHAR ntName[NAMESIZE];
extern struct find_t ffblk;
extern INT fileP;
extern UINT FIXEDLEN;
extern CHAR EXTENTION[EXTSIZE];
extern HWND hwndMainFrame;
extern HWND hwndMainDlg;


// INT importFile(HWND hwndF, INT recCnt, UINT mode)
INT importFile(UINT mode)
{
INT i, x;
CHAR mStr[NAMESIZE] = "";
CHAR rNum[4];
LONG fSize;
INT nob;
INT xtra;
FILE *hand1, *hand2;
LONG pointerPos;
INT flsize;
struct Samp
   {
   SHORT indx;
   CHAR noteName[NAMESIZE];
   }sample;
ULONG oneRecSize;
INT recCnt, ret;


oneRecSize = sizeof(sample) + FIXEDLEN;
flsize = FIXEDLEN;
xtra = 0;
recCnt = WinQueryLboxCount(WinWindowFromID(hwndMainDlg, ID_LISTBOX1));
if( recCnt >= MAXNUMRECS )
   {
   dBaseFull(hwndMainDlg);
   return(0);
   }
zeroDataRecs(dataRecs.noteText);
i = 0;
if( mode == DONTDROPIT )
   {
   if( !FindFile(hwndMainDlg) )
      return(0);
   }
 if( fileP )
   {
   if( osName(hwndMainDlg) == 0 )
      return(0);
   }
else
   {
   parseName(fullFileName);
   do
      {
      ret = chkDupeName(hwndMainDlg);
      }
   while(ret == -1 || ret == 1);

   if( ret == 0 )
      return(0);
   }
 _dos_findfirst(fullFileName,_A_NORMAL,&ffblk);
fSize = ffblk.size;

if ((hand1 = fopen( fullFileName, MRO)) == NULL)
   return(0);
fseek(hand1, 0L, SEEK_SET);
if( fSize <= (flsize - 1) )
   {
   recCnt = WinQueryLboxCount(WinWindowFromID(hwndMainDlg, ID_LISTBOX1));
   if( recCnt >= MAXNUMRECS )
      {
      fclose(hand1);
      dBaseFull(hwndMainDlg);
      nCho = matchName(hwndMainDlg, dataRecs.noteName);
      WinPostMsg(WinWindowFromID(hwndMainDlg, ID_LISTBOX1),
					LM_SELECTITEM,
					MPFROMSHORT(nCho),
					MPFROMSHORT(TRUE));
      return(0);
      }
   nob = fread(&dataRecs.noteText, fSize, 1, hand1);
   fclose(hand1);
   strcpy(dataRecs.noteName, ntName);
   recIndex[recCnt].posIndex = recCnt;
   strcpy(recIndex[recCnt].recName, ntName);
   WinInsertLboxItem(WinWindowFromID(hwndMainDlg, ID_LISTBOX1),
					    LIT_SORTASCENDING,
					    recIndex[recCnt].recName);
   multiWrite(datFile, 1, FIXEDLEN, recCnt);
   saveIndex(datFile);
   zeroDataRecs(dataRecs.noteText);
   }
else
   {
   do
      {
      recCnt = WinQueryLboxCount(WinWindowFromID(hwndMainDlg, ID_LISTBOX1));
      if( recCnt >= MAXNUMRECS )
	 {
	 fclose(hand1);
	 dBaseFull(hwndMainDlg);
	 nCho = matchName(hwndMainDlg, dataRecs.noteName);
	 WinPostMsg(WinWindowFromID(hwndMainDlg, ID_LISTBOX1),
					   LM_SELECTITEM,
					   MPFROMSHORT(nCho),
					   MPFROMSHORT(TRUE));
	 zeroDataRecs(dataRecs.noteText);
	 return(0);
	 }
      if( (fSize - ( (i*(flsize - 200))+xtra)) >= (flsize - 200) )
	 {
	 x = 0;
	 fread(&dataRecs.noteText, (flsize - 200), 1, hand1);
	 while( dataRecs.noteText[(flsize - 201)+x] != '\n' )
	    {
	    fread(&dataRecs.noteText[(flsize - 201)+x+1], 1, 1, hand1);
	    xtra++;
	    x++;
	    if( x > 199 )
	       break;
	    }
	 nob = 1;
	 }
      else
	 {
	 fread(&dataRecs.noteText, fSize - ((i*(flsize - 200))), 1, hand1);
	 nob = -1;
	 }
      strcpy(mStr, ntName);
      itoa(i+1, rNum, 10);
      strcat(mStr, " ");
      strcat(mStr, rNum);
      strcpy(dataRecs.noteName, mStr);
      recIndex[recCnt].posIndex = recCnt;
      strcpy(recIndex[recCnt].recName, mStr);
      WinInsertLboxItem(WinWindowFromID(hwndMainDlg, ID_LISTBOX1),
					       LIT_SORTASCENDING,
					       recIndex[recCnt].recName);

      multiWrite(datFile, 1, FIXEDLEN, recCnt);
      saveIndex(datFile);
      zeroDataRecs(dataRecs.noteText);
      i++;
      }
   while( nob > 0 );
   }
fclose(hand1);
dataRecs.noteText[strlen(dataRecs.noteText)] = '\0';
nCho = matchName(hwndMainDlg, recIndex[recCnt].recName);
WinPostMsg(WinWindowFromID(hwndMainDlg, ID_LISTBOX1),
				  LM_SELECTITEM,
				  MPFROMSHORT(nCho),
				  MPFROMSHORT(TRUE));
zeroDataRecs(dataRecs.noteText);
setStatus(hwndMainDlg, datFile);
return(recCnt);
}


