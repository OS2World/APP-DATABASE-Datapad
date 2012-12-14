/******************************************************************************/
/*                                                                            */
/* Program: PADHD.EXE                                                         */
/*                                                                            */
/* Description: Description                                                   */
/*                                                                            */
/* File Name : Main.H                                                         */
/*                                                                            */
/* Author : GARY L. ROBINSON                                                  */
/*                                                                            */
/* Copyright 1997 Copyright notice                                            */
/*                                                                            */
/* Generated using VisPro/C serial number VPC5013754                          */
/*                                                                            */
/******************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

 /* ##START Form.39 Form header definitions */
 /* ##END Form header definitions */
/* Helper routines */
MRESULT HandleMessage(HWND hwndFrame, HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2);
ULONG MLEGetItemText(HWND hwndDlg, ULONG ulID, LONG lIptPosition,
      LONG lBoundary, PBYTE *ppText);
ULONG MLEInsertText(HWND hwndDlg, ULONG ulID, LONG iptPosition, PBYTE pText);
HWND OpenMain(HWND hwndOwner, PVOID pInfo);

typedef struct _MAINFORMINFO {
   PSWP pControlSize;   /* Used to handle control sizing */
   /* ##START Form.33 Window structure members */
   /* ##END Window structure members */
} MAINFORMINFO;
typedef MAINFORMINFO * PMAINFORMINFO;
#define ID_FORMTIMER 100   /* Used for timer events */
#endif

MRESULT EXPENTRY MainWinProc(HWND hwndF, ULONG msg,
				      MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY nWinProc( HWND hwndW, ULONG msg,
				   MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY pathProc( HWND hwndDlg, ULONG msg,
				   MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY setFEProc( HWND hwndDlg, ULONG msg,
				   MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY setFEProcINIT( HWND hwndDlg, ULONG msg,
				   MPARAM mp1, MPARAM mp2 );
extern MRESULT EXPENTRY gmlProc1( HWND hwndDlg, ULONG msg,
				  MPARAM mp1, MPARAM mp2 );
extern MRESULT EXPENTRY gmlProc2( HWND hwndDlg, ULONG msg,
				  MPARAM mp1, MPARAM mp2 );
extern MRESULT EXPENTRY setModProc( HWND hwndDlg, ULONG msg,
                                     MPARAM mp1, MPARAM mp2 );
extern MRESULT EXPENTRY addRecProc(HWND hwndDlg, ULONG msg,
			     MPARAM mp1, MPARAM mp2);
extern MRESULT EXPENTRY editRecProc(HWND hwndDlg, ULONG msg,
			     MPARAM mp1, MPARAM mp2);
extern MRESULT EXPENTRY searchRecProc(HWND hwndDlg, ULONG msg,
			     MPARAM mp1, MPARAM mp2);
extern MRESULT EXPENTRY aboutProc(HWND hwndDlg, ULONG msg,
			     MPARAM mp1, MPARAM mp2);
extern MRESULT EXPENTRY aboutProcR(HWND hwndDlg, ULONG msg,
			     MPARAM mp1, MPARAM mp2);
extern MRESULT EXPENTRY searchViewProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
extern MRESULT EXPENTRY searchNameProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
extern MRESULT EXPENTRY viewRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
extern MRESULT EXPENTRY eSearchRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
extern MRESULT EXPENTRY vSearchRecProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
extern MRESULT EXPENTRY saveFileProc( HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
extern MRESULT EXPENTRY findProc( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 );
extern MRESULT EXPENTRY dupeNameProc( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 );
extern MRESULT EXPENTRY stickyProc( HWND hwndDlg, ULONG msg,
			   MPARAM mp1, MPARAM mp2 );
extern EXPENTRY saveALLFProc(HWND hwndDlg, ULONG msg,
			    MPARAM mp1, MPARAM mp2 );
extern INT removeSwEntry(HWND hwnd);
extern INT addSwEntry(HWND hwnd);
extern INT setDefMod(HWND hwndD);
extern INT dupeName(HWND hwndML);
extern getModList(HWND hwndML, INT mode);
extern INT addRec(HWND hwndDlg);
extern INT eSearchRec(HWND hwndDlg);
extern INT vSearchRec(HWND hwndDlg);
extern INT searchRec(HWND hwndDlg);
extern INT editRec(HWND hwndDlg);
extern HWND viewRec(HWND hwndDlg);
// extern VOID viewRec(HWND hwndDlg);
extern VOID aboutR(HWND hwndDlg);
extern VOID about(HWND hwndDlg);
extern VOID zeroRecs(VOID);
extern INT initLoad_index(HWND hwndList, CHAR *dataFileName);
extern INT deleteRec(HWND hwndMain);
extern VOID searchRecs(VOID *hwnd);
extern VOID searchNames(VOID *hwnd);
extern INT loadDefFile(HWND hwndList);
extern INT openFile(CHAR *fileName, CHAR *mode);
extern VOID closeFile(VOID);
extern INT truncFile(CHAR *fileName, INT noRecs, INT flrs);
extern INT closeFileTrunc(CHAR *fileName, INT noRecs, INT flrs);
// extern INT closeFileTrunc(CHAR *fileName, INT noRecs);
extern INT readFile(INT index);
extern INT writeFile(INT index);
extern VOID zeroIndex(INT index);
extern VOID zeroAllIndex(VOID);
extern VOID saveIndex(CHAR *dataFileName);
extern INT searchView(HWND hwndDlg);
extern VOID unSubClassWin(VOID);
extern VOID subClassWin(VOID);
extern VOID enableButtons(VOID);
extern VOID disableButtons(VOID);
extern INT chkRet(VOID);
extern INT saveFileRec(HWND hwndDlg);
extern INT printRec(HWND hwndDlg);
extern BOOL FindFile (HWND hwndWnd);
extern VOID zeroDataRecs(CHAR *noteText);
extern INT chkName(HWND hwnd, CHAR *lbSelect);
extern INT compName(HWND hwndM, INT cho);
extern INT compName2(CHAR *sName, INT nrec);
extern INT matchName(HWND hwndM, CHAR *lbSelect);
extern INT copyData(INT c);
extern VOID helpText(HWND hwndDlg);
extern INT wFile(FILE *datHan, INT index);
extern INT osName(HWND hwndDlg);
extern VOID dBaseFull(HWND hwndDB);
extern VOID parseName(CHAR *name);
extern VOID getPath(VOID);
extern VOID setMod(HWND hwndD);
extern INT chkDupeName(HWND hwndDlg);
extern INT importFile(UINT mode);
extern INT readFiles(CHAR *fileName, LONG index);
extern INT writeFiles(CHAR *fileName, LONG index);
// extern INT truncFile(CHAR *fileName, INT noRecs);
extern INT existPath(CHAR *pathStr);
extern INT initLoad(HWND hwndList, CHAR *fileName);
extern INT results1(HWND hwndR);
extern INT results2(HWND hwndR);
extern VOID FAR resultRecs(VOID *hwnd);
extern VOID FAR result2Recs(VOID *hwnd);
extern INT chkIntegrityPDF(HWND hwndQ, CHAR *PDFName);
extern VOID addFWHeader(FILE *hand, CHAR *dRec);
extern HFILE openTmp(HFILE hfile, CHAR *szFullPathName, CHAR *szFullDocName, CHAR *fileName);
extern INT loadIndex(HWND hwndI, CHAR *dataFileName);
extern INT createPDF(HWND hwndD);
extern INT multiRead(CHAR *inFile, INT datS, INT sizeF, INT recNum);
extern INT multiWrite(CHAR *inFile, INT datS, INT sizeF, INT recNum);
extern INT getFLRecSize(CHAR *inFile);
extern VOID zeroMostIndex(INT index);
extern VOID strdelc( CHAR *s, CHAR c);
extern VOID FAR resizePDF(VOID *hwnd);
extern VOID FAR resizePrompt(VOID *hwnd);
extern VOID setParams(HWND hwndP, SHORT dx, ULONG IDE, ULONG IDR);
extern VOID setPPMenuDef(INT num);
extern INT saveToFile(CHAR *flName, FILE *hdl);
extern INT compIndexName(HWND hwndM, CHAR *sName);
extern INT saveAllF(HWND hwndDlg);
extern VOID msgOK(CHAR *title, CHAR *message);
extern VOID loadNewRec(HWND hwndM, HWND hwndD);
extern INT loadNewERec(HWND hwndM, HWND hwndD);

extern HEV hevrs;
extern HEV hev, hevS;

extern HWND hwndCurViewRecord, hwndCurEViewRecord;

VOID FAR saveATFile(VOID *hwnd);
VOID setStatus(HWND hwndST, CHAR *pFile);
INT chkPDF(CHAR *dFiles);
VOID setPadPath(HWND hwndPP);
VOID setBasePath(VOID);
INT validatePDF(CHAR *df);
VOID setFE(HWND hwndPP);
VOID setINITFE(HWND hwndPP);
VOID enableMainScrX(HWND hwndz);
VOID disableMainScrX(HWND hwndz, INT mode);



extern VOID msgOK(CHAR *title, CHAR *message);
extern VOID debugMsgCH(CHAR ch, CHAR *varName);
extern VOID debugMsgStr(CHAR *str, CHAR *varName);
extern VOID debugMsgULong(ULONG num, CHAR *varName);
extern VOID debugMsgInt(INT num, CHAR *varName);



HWND  hwndMainFrame;
INT pgmData[60];
DATETIME dt;
struct DAT
   {
   SHORT posIndex;
   SHORT alIndex;
   CHAR recName[NAMESIZE];
   } recIndex[MAXNUMRECS];
struct DATA
   {
   SHORT indx;
   CHAR noteName[NAMESIZE];
   CHAR noteText[NOTESIZE];
   } dataRecs;
struct DATAS
   {
   SHORT indx;
   CHAR noteName[NAMESIZE];
   CHAR noteText[SPECIALDATA];
   }dataR;
struct DATAMOD
   {
   CHAR noteText[MAXNAME];
   }pdf[NUMPDFMODS];
struct DATAMODX
   {
   CHAR noteText[MAXNAME];
   }pdf2[NUMPDFMODS];
INT numRecs = 0;
SHORT choice = 0;
SHORT continueSearch = 1;
CHAR datFile[MAXNAME];
CHAR datFilex[MAXNAME];
CHAR datFileOrg[MAXNAME];
CHAR dFile[MAXNAME];
CHAR Honor[NAMESIZE];
HINI hini;
FILE *datHan, *datHan2;
CHAR schText[NAMESIZE] = "";
CHAR xtext[NAMESIZE] = "";
HPOINTER  newPointer;
HPOINTER  oldPointer;
INT BUSY = 0;
PFNWP oldProc1;
PFNWP oldProc2;
PFNWP oldProc3;
PFNWP oldProc4;
PFNWP oldProc5;
PFNWP oldProc6;
PFNWP oldProc7;
PFNWP oldProc8;
PFNWP oldProc9;
PFNWP oldProc10;
HWND hwnd1, hwnd2, hwnd3, hwnd4, hwnd5,
     hwnd6, hwnd7, hwnd8, hwnd9, hwnd10,
     hwnd11, hwnd12, hwnd13, hwnd14, hwnd15;
HWND hwndMenu;
HWND hwndView;
HWND hwndMainDlg;
INT CASE = 0;
CHAR *pdest;
CHAR *pdestT;
CHAR *pT;
INT eols[MAXLINES];
INT stT;
CHAR fullFileName[MAXNAME] = "";
CHAR initFSpec[MAXNAME];
INT WW;
FILEDLG fdFileDlg;
INT INUSE;
INT nCho;
CHAR ntName[NAMESIZE];
PFNWP oWinProc;
ULONG dn;
INT fileP;
struct find_t ffblk;
CHAR szFullDocName[MAXNAME] ;
ULONG pValue[16][32];
HOBJECT hobjBG;
HOBJECT hobjFNT;
SHORT presFlag[4];
CHAR basePath[MAXNAME] = "";
HELPINIT hinit;
HWND hwndHelp;
HWND hwndFrame;
HWND hwndRFrame;
HWND hwndMLBox;
HWND hwndMLBPopup, hwndEMLEPopup, hwndVMLEPopup, hwndAMLEPopup;
BOOL ACCUM;
BOOL ESEARCH = FALSE;
BOOL NSEARCH = FALSE;
BOOL CURMODONLY;
BOOL ASSOCI;
BOOL CASESENSITIVE;
BOOL NAMEONLY;
BOOL NAMESEARCH;
CHAR mcString[MAXNAME];
CHAR stickyName[MAXNAME];
CHAR ndName[] = "No default ...";
ULONG scrLen, scrWid;
CHAR npdf[260];
UINT FIXEDLEN;
CHAR tempDatFile[MAXNAME] = "";
CHAR cwdFound[MAXNAME+NAMESIZE];
CHAR EXTENTION[EXTSIZE];
INT nChox;


