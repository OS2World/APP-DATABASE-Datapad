.* Information Presentation Facility (IPF) Source File For PADHD ver. 1.1
.*
.*
:userdoc.
:title.PADHD 32bit Notepad
:docprof toc=12.
:h1 res=735. What is PADHD?
:p.
   PADHD ver. 1.1b is a "scratch pad" to jot notes in and organize small 
files into.  While PADHD is NOT a WORD PROCESSOR It has several 
features to help make managing notes a lot easier - such as cut, clear, 
copy, paste, search, save to ASCII, import ASCII file, and an ASCII dump to 
the printer.  At its simplest it is merely a note pad.  At it's most it is a great 
note organizer for students, teachers, programmers ( source code files and 
examples ), and any one with a lot of information to handle.  
:p.
   PADHD will keep up to 1000 records, each 29,999 bytes long, per data file.
There can be an unlimited number of data files, so the amount that can be 
stored is unlimited.  Each data file will grow and/or shrink as you add or 
delete records.  Their size is not static.  Find notes, source code, or any info 
when you need it, then copy and paste into almost any program to use it!  You
can also include up to 25 data files in global searches.
:p.
  Pad can save you hours of time searching your hard drive for files and makes
it easy to organize!

:p.

:h2 res=736 . Program Requirements
:p.
   PADHD is a 32 bit OS/2 program.  Requires OS/2 2.x. with a practical
minimum of 6-8 mb of RAM memory.  This does NOT mean you need an
additional 6-8 mb of memory to run PADHD.  OS/2  needs that amount of
memory to run itself comfortably.
:p.
   Disk space used by PADHD can be considerable.  If you only use one
PDF data file and fill it with 1000 records ( maximum number of records per
data file ) this would take up slightly over 30 mb of disk space.  Of course
that is a HUGE amount of data.  For many home users the number records 
and data files needed will usually result in a lot less space than that being 
used.  Each record will use 30k bytes disk space with each .PDF data file 
having a fixed overhead of 44k bytes.  So for example a .PDF data file with
100 records would use 3,000,044 bytes or slightly over 3 mb.

:h2 res=737 . Registration
:p.
Registration fee is $20.00 if you mail it in before jan. 1 1995.  After Dec. 31
1994 registration is $35.00.

:p.
After sending registration fees you will be sent a CODE word that will 
disable the ugly SHAREWARE screen that comes up every time you start the
program.  You will be registered for ALL future updates or versions of 
PADHD and will NOT have to pay any additional fees. Send all fees and 
inquiries to :

:p.
Gary L. Robinson
.br
600 Apt. D Mumford Dr.
.br
Troy, OH  45373

:p.


:h1 res=738 . Installation
:p.
   PADHD has no installation program.  It consists of four files - PADHD.EXE,
PADHD.DOC, WHATS.NEW, and ORDER.FRM.  After you unzip it, copy 
PADHD.EXE and PADHD.DOC to a utilities directory or where ever you want 
them to reside.  If you don't need the doc files, they may be erased.  The file
ORDER.FRM is an order form to register PADHD and can be easily printed to
make registering an easy and almost painless chore.  The file WHATS.NEW 
is an ASCII text file that summarizes the new features/changes.
:p.
   Migrate PADHD to the desk top or use a program template to put an icon 
on the desktop for it.  Or you could run it from the command line to test it.  
Whatever serves your purpose.  When you run it for the first time it will ask 
you to type in the directory where the program files will be located at.  If, for 
instance, you have put PADHD.EXE and PADHD.DOC in the UTILITIES 
directory - type in the complete path when prompted.  After you run it the 
first time it will create three other files in the UTILITIES directory.  
PADHD.PDF is the default data file that holds the info on your notes.  
PADHD.INI is an initiation data file that holds settings and screen coordinates.
PADHD.DAT is a data file that stores the name of the .PDF files on your search
list.  
:p.
   You probably should run only one copy of PADHD on your system.  When 
you first start PADHD and it prompts you for a path - that path is stored in 
your OS2.INI file.  Every time you start PADHD it reads your OS2.INI file 
and retrieves that path.  That is where the program looks for the 
PADHD.DAT and PADHD.INI file.  If you ran another copy of PADHD in 
another directory or drive it would still look to the path in the OS2.INI file.  
So, if you want to move PADHD from one directory or drive to another be 
sure to erase the 'old' copy AND DON'T COPY THE PADHD.INI AND 
PADHD.DAT files to the new directory.  The program will create new ones 
for you. Then when you start it in it's new home it will prompt you again for 
a new path string.

:h1 res=739 .Quick Start
:p.
   Nothing here yet for this category!



:h1 res=740 . Program Screens
:p.
Explanation of controls and functions on the various program windows.



.*  MAIN SCREEN begin
:h2 res=741 . Main Screen
:p.
:color fc=blue bc=default.
:font facename=Courier size=24x14.
Controls on Main Screen
:font facename=default size=0x0.
:color fc=default bc=default.

:p.
QUIT   :  
Rather obvious function.  It ends the program.

:p.
VIEW   :  
Strictly for viewing - double clicking on the name of the record in the list box 
will also activate the View window.  Copy, Search, Export, andPrint controls 
are available in the View Window.

:p.
EDIT   :  
Edit a note and change it's content.  Notes can be copied or imported from the
clip board, imported from an ASCII file, or typed in.  Copy, Paste, Cut, Clear, 
Print, Search, and Import/Export controls are available in the Edit Window.

:p.
ADD    :  
Use this to start a new note. Up to 1000 notes per data file.  Copy, Paste, Cut,
Clear, and Import controls are available in an Add Window.

:p.
SEARCH : 
Use this to search through the text or name part of the notes for specific words
or phrases.  This global search button will search through all records and find 
any occurrences of your search string.  It can also search through multiple ( up
to 25 ) .PDF modules.

:p.
DELETE :  Destroy the note and remove it from the data file.

:p.
Also read section on :link reftype=hd res=748.Menu Items.:elink.

:p.
   When you first start the program, of course, you have no notes.  So the first
push button you'll use is the ADD function. ( alternately you could use the 
Import File selection on the menu bar to directly import ANY size ASCII 
directly into the .PDF file or use direct manipulation and DROP an ASCII file 
onto the MAIN screen list box )
   When you press the ADD button you'll be presented with the Add Record window.
There you can type in the name of the note that will appear in the initial screen's 
list box.  Then type in the body of the note.  The maximum size for a note is 
29999 characters which includes empty spaces between words.  To the right of the
Record Name field will be a read out of the current number of bytes you have left 
in that record.  Then you have several choices.
:p.
.*  MAIN SCREEN end



.*  VIEW SCREEN begin
:h2 res=742 . View Screen
:p.
:color fc=blue bc=default.
:font facename=Courier size=24x14.
Controls on View Screen
:font facename=default size=0x0.
:color fc=default bc=default.

:p.
COLORS :  
This starts an instance of the system Color Palette which can be used to
Drag and Drop colors on the VIEW entry box - not the record name entry
box.  Hold the CTRL key down while dragging colors to change the TEXT
FOREGROUND color.  ONLY SOME colors will work correctly.  Please read 
section on :link reftype=hd res=749.BUG, NOTES, etc.:elink.



:p.
FONTS  :  
This starts an instance of the system Font Palette which can be used to
Drag and Drop fonts on the VIEW entry box.  NOT ALL font characteristics
are supported.  Please read section on :link reftype=hd res=749.BUGS, NOTES, etc.:elink.

:p.
HOME   :  
Places cursor at the BEGINNING of current record.  Located above Bytes
Available field.  ( CTRL + HOME )

:p.
END     :  
Places cursor at the END of current record.  Located above Bytes Available
field.  ( CTRL + END )

:p.
OK      :  
Closes the View record window.

:p.
COPY   :  
Copies highlighted text to the clip board.  Text can be highlighted in the 
usual ways.  Hold down on Shift button and use arrow keys.  Or hold down
on mouse button 1 and drag the pointer to highlight the text.

:p.
SEARCH + : 
Press + to initiate a forward search.  You'll be presented with a dialog 
entry box to enter a search string.  Press OK to start, Cancel to quit, and 
check the case sensitive box if you need that.  The Default is to be non 
case sensitive.  The program will find your search string and highlight it.  
If It can NOT find it you will hear a musical note.  Continue to press + to 
forward search through the record or - to search in reverse.  ALL 
SEARCHES START AT CURRENT CURSOR POSITION.  ( F2 )

:p.
SEARCH - : 
Reverse search.  ( F3 )

:p.
SEARCH clr : 
Clears the search string.  After using this button, the next time you press
either + or - you will be prompted for a new search string.  ( F4 )

:p.
EXPORT TO FILE    :   
Export From File presents the user with a dialog entry box.
Type in the FULL path and name of the file you want to save
the current record in.  Press OK or Cancel.

:p.
PRINT  :  
This will do a straight, no frills, ASCII dump to the printer.  If you need
formatted printing, save the record to an ASCII file then load into your 
favorite word processor.

.*  VIEW SCREEN end




.*  EDIT SCREEN begin
:h2 res=743 . Edit Screen
:p.
:color fc=blue bc=default.
:font facename=Courier size=24x14.
Controls on Edit Screen
:font facename=default size=0x0.
:color fc=default bc=default.

:p.
COLORS :  
This starts an instance of the system Color Palette which can be used to
Drag and Drop colors on the EDIT entry box - not the record name entry
box.  Hold the CTRL key down while dragging colors to change the TEXT
FOREGROUND color.  ONLY SOME colors will work correctly.  Please read 
section on BUG FIXES AND THINGS.

:p.
FONTS  :  
This starts an instance of the system Font Palette which can be used to
Drag and Drop fonts on the EDIT entry box.  NOT ALL font characteristics
are supported.  Please read section on BUG FIXES AND THINGS.

:p.
SAVE NOW  :  
Saves current record to disk but does NOT close window.  Located
above the Record Name.

:p.
HOME   :  
Places cursor at the BEGINNING of current record.  Located above Bytes
Available field.  ( CTRL + HOME )

:p.
END     :  
Places cursor at the END of current record.  Located above Bytes Available
field.  ( CTRL + END )

:p.
OK      :  
Closes the Edit window and saves the record to disk.

:p.
CANCEL :  
Backs out with out saving or setting anything.

:p.
COPY   :  
Copies highlighted text to the clip board.

:p.
PASTE  :  
This copies whatever is in the clip board and pastes it into the current
record starting at the current cursor position.

:p.
CUT     :  
This deletes any highlighted text from the record but also copies it to the
clip board at the same time.

:p.
CLEAR  :  
This will delete any highlighted text.

:p.
PRINT  :  
This will do a straight, no frills, ASCII dump to the printer.  If you need
formatted printing, save the record to an ASCII file then load into your 
favorite word processor.

:p.
SEARCH + : 
Press + to initiate a forward search.  You'll be presented with a dialog 
entry box to enter a search string.  Press OK to start, Cancel to quit, and 
check the case sensitive box if you need that.  The Default is to be non 
case sensitive.  The program will find your search string and highlight it.  
If It can NOT find it you will hear a musical note.  Continue to press + to 
forward search through the record or - to search in reverse.  ALL 
SEARCHES START AT CURRENT CURSOR POSITION.  ( F2 )

:p.
SEARCH - : 
Reverse search.  ( F3 )

:p.
SEARCH clr : 
Clears the search string.  After using this button, the next time you press
either + or - you will be prompted for a new search string.  ( F4 )

:p.
EXPORT TO FILE    :   
Export From File presents the user with a dialog entry box.
Type in the FULL path and name of the file you want to save
the current record in.  Press OK or Cancel.

:p.
IMPORT FROM FILE  :  
Import From File presents the user with an Open Dialog box
which is used to select a TEXT file and import it into the
current record at the position where the cursor is located at.
Maximum size file that can be imported is 29,999 bytes in a
new record or whatever the Bytes Available field to the right
of the Record name field tells you is available.
.*  EDIT SCREEN end




.*  SEARCH SCREEN begin
:h2 res=744 . Search Screen
:p.

:color fc=blue bc=default.
:font facename=Courier size=24x14.
Search Screens
:p.
:color fc=default bc=default.
:font facename='Tms Rmn' size=24x14.
Global Searches
:font facename=default size=0x0.

:p.
   Global Searches are those searches that are initiated from the search button on the
main program window - not a search conducted while in an Edit or View Window.
When you press the search button for a Global search, you will be presented with a
dialog entry box.  Select either of the two check boxes depending on whether you want
to search the Name or Text fields of the record and whether you need the search to be
case sensitive or not.  The Default is to search the Text field of the record and NOT
be case sensitive.  Enter a search string in the entry field and press OK to begin.
Search strings must at least be 3 characters long.
:p.
   The program will then start searching, one at a time, through every record you have.
If you have 1000 records the search may take 2-3 minutes depending on the speed of
your computer and if what you are looking for is in the last record it looks at.
:p.
   If you have more than one .PDF module designated in your Global Search list it will
load in all of them one after the other until you cancel the search or it reaches the end.   
:p.
   If it finds a match for your search string it will open that record in a Search View 
Record Window and highlight the found string.  You will then be presented with
several choices :

:p.
:color fc=blue bc=default.
:font facename=Courier size=24x14.
Controls on Search Screen
:font facename=default size=0x0.
:color fc=default bc=default.

:p.
COLORS :  
This starts an instance of the system Color Palette which can be used to
Drag and Drop colors on the SEARCH entry box - not the record name entry
box.  Hold the CTRL key down while dragging colors to change the TEXT
foreground color.  ONLY SOME colors will work correctly.  Please read 
section on BUG FIXES AND THINGS.

:p.
FONTS  :  
This starts an instance of the system Font Palette which can be used to
Drag and Drop fonts on the SEARCH entry box.  NOT ALL font characteristics
are supported.  Please read section on BUG FIXES AND THINGS.

:p.
HOME   :  
Places cursor at the BEGINNING of current record.  Located above Bytes
Available field.  ( CTRL + HOME )

:p.
END     :  
Places cursor at the END of current record.  Located above Bytes Available
field.  ( CTRL + END )

:p.
:p.
CANCEL AND RETURN :  
:p.
   CURRENT : 
   Quit the search.  This will close the Search --- View Record window.
   The currently displayed .PDF module ( file ) will stay loaded.
   The record will be highlighted in the list box so you can now perform any
   of the main actions on it. ( Edit, View, Delete )  

:p.
   ORIGINAL : 
   Quit the search.  This will close the Search --- View Record window.
   The original .PDF module ( file ) you started with will be reloaded.

:p.
:p.
CONTINUE GLOBAL  :  
This will close the current window and continue to search
the rest of the records for a match of your search string.  If
another match is found it will open a window and highlight the
string again.  And on and on until it runs out of records and
notifies you that there are no more matches.

:p.
CONTINUE LOCAL + : 
Press + to initiate a forward search.  You'll be presented with a 
dialog entry box to enter a search string.  Press OK to start, Cancel to quit,
and check the case sensitive box if you need that.  The Default is to be 
non case sensitive.  The program will find your search string and highlight 
it.  If It can NOT find it you will hear a musical note.  Continue to press +
to forward search through the record or - to search in reverse.  ( F2 )

:p.
CONTINUE LOCAL - : 
Reverse search.  ( F3 )

:p.
CONTINUE LOCAL clr : 
Clears the search string.  After using this button, the next time
you press either + or - you will be prompted for a new search 
string.  ( F4 )

:p.
COPY   :  
Copies highlighted text to the clip board.  Text can be highlighted in the 
usual ways.  Hold down on Shift button and use arrow keys.  Or hold down
on mouse button 1 and drag the pointer to highlight the text.

.*  SEARCH SCREEN end



.*  ADD SCREEN begin
:h2 res=745 . Add Screen
:p.

:color fc=blue bc=default.
:font facename=Courier size=24x14.
Controls on Add Screen
:font facename=default size=0x0.
:color fc=default bc=default.

:p.
COLORS :  
This starts an instance of the system Color Palette which can be used to
Drag and Drop colors on the ADD entry box - not the record name entry
box.  Hold the CTRL key down while dragging colors to change the TEXT
FOREGROUND color.  ONLY SOME colors will work correctly.  Please read 
section on BUG FIXES AND THINGS.

:p.
FONTS  :  
This starts an instance of the system Font Palette which can be used to
Drag and Drop fonts on the ADD entry box.  NOT ALL font characteristics
are supported.  Please read section on BUG FIXES AND THINGS.

:p.
HOME   :  
Moves cursor to BEGINNING of record.  ( CTRL + HOME )

:p.
END     :  
Moves cursor to END of record.  ( CTRL + END )

:p.
OK      :  
Closes the Add window and saves the record to disk.

:p.
CANCEL :  
Backs out with out saving or setting anything.

:p.
COPY   :  
Copies highlighted text to the clip board.

:p.
PASTE  :  
This button copies whatever is in the clip board and pastes it into the cur-
rent record starting at the current cursor position.

:p.
CUT     :  
This deletes any highlighted text from the record but also copies it to the
clip board at the same time.

:p.
CLEAR  :  
This will delete any highlighted text.

:p.
IMPORT FROM FILE  :  
Import From File presents the user with an Open Dialog box
which is used to select a TEXT file and import it into the
current record at the current cursor location.  The maximum
size file that can be imported is 29,999 bytes in a new record
or whatever the Bytes Available field to the right of the
Record name field tells you is available.

:p.
:p.
After you have saved one or more records then the Edit, View, Search, and Delete
buttons will be usable.  
.*  ADD SCREEN end





.*  SET MODULES SCREEN begin
:h2 res=746 . Set Modules Screen
:p.

:color fc=blue bc=default.
:font facename=Courier size=24x14.
Controls on Set Modules Screen
:font facename=default size=0x0.
:color fc=default bc=default.

:p.
OK :  Accept and Save all changes made.

:p.
CANCEL :  Cancel and do NOT accept changes.

:p.
ADD :  This presents you with a file selection box.  If you have several .PDF files -
any where on the system - select them and add to the search list.  A MAXIMUM
of 25 .PDF files can be on the search list.

:p.
REMOVE :  Removes .PDF file from search list.  It does NOT erase or affect the file
itself.

:p.
NEW :  Create a new .PDF file and give it a name.  This is the ONLY way to start a
new .PDF data file in the program.

:p.
:p.
:color fc=red bc=default.
:font facename=Courier size=24x14.
ATTENTION -- 
:font facename=default size=0x0.
:color fc=default bc=default.
DOUBLE CLICKING on any entry will put it at the top of the search
list.  This will make it the default .PDF file.  The default .PDF file will be the one
listed when the program first starts.  Of course, if you only have one .PDF file in the
search list this is a moot point.  All changes become effective after selecting OK -
which will save all changes and exit this screen.     

.*  SET MODULES SCREEN end





.* MENU ITEMS begin
:h1 res=748 . Menu Items
:color fc=blue bc=default.
:font facename=Courier size=24x14.
:p.
Menu Items
:font facename=default size=0x0.
:color fc=default bc=default.

:p.
OPTIONS :
:p.
About ...    :  Under Options on the main menu bar.  View the noxious Shareware
money grubbing beg screen.
:p.
Import File :  Import any ASCII text file into the current .PDF file.  Theoretically, will
load in ANY SIZE ASCII file.  You will be presented with a Select File
box.  Select an ASCII file then press OK to accept or CANCEL to quit.
If the file can NOT be loaded into one record it will be loaded into as 
many records as needed - as long as there is enough space left in the
.PDF file.  Trying to load a file that is NOT ASCII text may get 
unpredictable results!

:p. 
:p. 
SETTINGS :
:p.
Toggle Word Wrap :  Will turn the word wrap feature on or off.  The title bar of the
main window will say what the current state is.  With word wrap
on you won't need to use the horizontal scroll bar.  This is located 
under the Options selection on the main menu bar.
:p.
Prompt For Record Name :  Prompt for a record name instead of using default name 
when importing ( through menu or drag + drop ) an ASCII 
file into the current .PDF file.
:p.
Default Colors :  Select EDIT, ADD, SEARCH, or VIEW to place a check mark and force
the use the DEFAULT colors.

:p. 
:p. 
SET MODULES :
:p.
Set Modules :  This will open up a whole new screen that is used to set which .PDF
modules you want to be included in a search or to create a new .PDF
file.  See details below under SET MODULES.   

:p. 
:p. 
HELP :
:p.
HELP :  Brings up a scrollable text box to view the PADHD.DOC file - if it is located
in the same directory as PADHD.
.* MENU ITEMS end





.* BUGS ETC begin
:h1 res=749. Bugs, Notes, etc.
:color fc=blue bc=default.
:font facename=Courier size=24x14.
:p.
Notes on PADHD
:font facename=default size=0x0.
:color fc=default bc=default.

:p.
:p.
:p.
:font facename=Helv size=18x8.
:color fc=red bc=default.
VER. 1.0b fixes and additions
:font facename=default size=0x0.
:color fc=default bc=default.
:p.
   Version ( 1.0b ) fixes several bugs that were in earlier versions.  Early versions
would quit and close if you tried to import an ASCII file that was READ ONLY.  This
has been corrected so that PADHD will now import files with the read only attribute 
set on.
:p.
   PADHD also would quit and close down if you attempted to PASTE from an EDIT
or ADD window when the clipboard was EMPTY.  Now PADHD will not shutdown if the
clipboard is empty and you try to paste.  It will, instead, notify you with a dialog box
that the clipboard is empty. 
:p.
   This fixes a bug so that the TAB will work correctly in an Edit of Add box.
:p.
   The ALT-G hot key now works correctly in a View box.
:p.
   DRAG AND DROP SUPPORT is now included as well as ASSOCIATION.  You can
now drop a .PDF object from a DRIVES folder on the PADHD icon.  HOWEVER, if you
try to drop a COPY of a .PDF object on the PADHD icon it may not work.  So if you
want to have a bunch of .PDF objects in a folder or on the desktop make SHADOWS
( CTRL+SHIFT ) of the originals from the drives folder.  The originals or shadows of 
objects can be dropped on or associated with PADHD in the normal OS/2 way.  To 
associate .PDF files to PADHD open the settings menu for PADHD , tab to the 
associations page, and at the bottom add in *.PDF in the New name field.

:p.
:p.
:p.
:font facename=Helv size=18x8.
:color fc=red bc=default.
VER. 1.1 fixes and additions
:font facename=default size=0x0.
:color fc=default bc=default.
:p.
   ASCII text files of ANY size can also be Dropped on the MAIN screen list box.  The
file will automatically loaded into as many records as needed ( if there is room ) and
named appropriately.  You can also Drag the currently highlighted record from the MAIN
screen list box.  It can be dragged to any place that an ASCII file can be dragged to -
such as a word processor, printer, or viewer.  If you drop it on the DESKTOP it will
create a file named TEMP.TXT which can renamed to anything you like that will have
the currently selected record in it.  Any size ASCII file can also be imported into the
current .PDF file using the menu selection Import File - under Options.  
:p.
   Files ( less than 30,000 bytes ) can be dropped directly on an EDIT or ADD entry
box while the record is open.  If there is room it will be inserted at the current 
cursor location.  It acts just like the IMPORT button.
:p.
   Files can be dragged OFF the EDIT, ADD, VIEW, OR SEARCH entry boxes directly
while they are open.  If the file is dropped on the DESKTOP it will be named
TEMP.TXT and will contain the current record.  
:p.
   .PDF files can be dropped directly on the module search list ( set modules ) to add
them to the list.  They CANNOT be dragged off the list.  Use Remove button to take 
them off.
:p.
   View, Edit, Add, and Search View windows CAN now be minimized.  They can be 
retrieved through the window list.  Click on both right and left mouse buttons at the
same time in an empty place on the desktop to bring up the window list.
:p.
   FONTS and COLORS can be dragged and Dropped onto EDIT, VIEW, ADD, and 
SEARCH box Multi Line Entry boxes.  However, not all font characteristics are 
supported.  Italics, bold, underline, or other special characteristics may not work.  Only
PRIMARY colors are supported.  These limitations seem to be built into the Multi Line
Entry boxes.  If there is a work around for this I can't find it yet.  Either I eventually
stumble on a way to work around this ( or I.B.M. 'fixes' this oversight ) or were stuck
with this limitation.  Strangely enough, the single line entry boxes don't seem to have
this limitation.  Anybody have any good suggestions?
:p.
   EDIT, VIEW, ADD, and SEARCH windows can now be HIDDEN.  Click the mouse on
the small box in the upper right hand corner of the window and it will hide it.  To 
retrieve it, bring up the OS/2 window list and select it.  Hopefully, in the future they
will be sizable - but to do so I will have to go to a tool bar type system which
entails a major rewriting of the code.
:p.   
   The current line number is now shown at the top of every EDIT, VIEW, ADD, or
search box.  However, the line number is only updated when keys are released.  For
example - If you hold down on the DOWN arrow key and scroll down through a record,
the line number at the top of the screen won't be updated until you let up on the key.
This was done to keep scrolling performance from degrading badly.  If set to update
for any keystroke, the scrolling rate was atrocious.
:p.
   The currently loaded .PDF modules name is listed at the bottom of the MAIN screen
with the current number of records in that module.  The module name is also listed in
the titlebar of any ADD, EDIT, VIEW, or SEARCH screen.
:p.
   The ONLY way to create a NEW .PDF file is through the Set Modules menu selection
on the MAIN MENU.  This is different than ver. 1.00x.  Select Set Modules on the MAIN
screen menu bar, then press NEW on the Global Search list screen.  Type in a name
for a new .PDF file and press OK.  File name MUST have the .PDF extention.

:p.
:p.
:p.
:font facename=Helv size=18x8.
:color fc=red bc=default.
Additional Notes
:font facename=default size=0x0.
:color fc=default bc=default.
:p.
   A lot of work and a lot of extra code has gone into this release ( 1.1b ) and I have
tested it for quite some time.  However, I am NOT I.B.M.  I don't have the resources to
test extensively.  Be sure to back up ALL data before upgrading to this release.  I 
believe this release to be bug free - but then I also thought Microsoft would continue to
make OS/2 apps in 1991.  Please report any bugs you may find to either my mail ad-
dress or to my GENIE address listed below.
:p.
   One possible bug that has already been reported by one user is an incompatability with
the installable file system HPFS386.  This is NOT the HPFS that ships with OS/2.
Since I do not use or have access to HPFS386 I cannot personally confirm this.  The
user says that when attempting to delete a record the ENTIRE record is deleted.   
:p.
   I have raised the price of PADHD.  Starting January 1, 1995 it will cost $35.00 to
help justify spending all these weekends and extra hours working on it.  All currently
registered users are not affected since they get all updates free and the registration 
code word stays the same.  Anyone who registers before that date still can do so for 
$20.00.
:p.
   Plans for future updates - not counting bug fixes - will hopefully include simple 
Boolean searches, perhaps sizable windows and a tool bar, and any other good suggestions 
that may come from users.  HINT! HINT!
:p.
GENIE mail address is :   G.ROBINSON13
.* BUGS ETC end


:euserdoc.




























