// http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html
// http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/attrib.html
// http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/scanw.html
#include <cstdlib>
#include <ncurses.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <ostream>
#include <iterator>

using namespace std;

unsigned int pos;
int row,col,h,w,x,x3,y,y3,i,lnCount;
bool isFileOpen = false;
bool save = false;
string s = "";
string s2 = "";
WINDOW * worldBorder;
WINDOW * worldWin;
vector<string> textVec;

/*
 *Takes in a file name and prints it out at the top of the window
 *@return world - the window to the user
 */
WINDOW * worldy() { // ()
  WINDOW * world;
  
  
  //clear();
  h = LINES - 4;
  w = COLS - 8;
  y = (LINES - h) / 2;
  x = (COLS - (w - 3)) / 2;
  refresh();
  
  world = newwin(h, w, y, x); // create a new window
  worldBorder = newwin(LINES, COLS, 0, 0); 
  box(worldBorder, 0, 0);
  box(world, 0, 0);  
  
  // arithmetic for center placement
  //int row = (COLS - sizeof(c)) / 2;
  
  wrefresh(worldBorder);
  wrefresh(world); 
  
  return world;
} // world

/*
 * This function prints the menu
 *
 * @return: a window, menu
 */
WINDOW * menu(){
  WINDOW * menu;
  curs_set(0);
  noecho();
  
  h = 10;
  w = 50;
  y = (LINES - h) / 2;
  x = (COLS - (w - 3)) / 2;
  refresh();
  
  // text on menu
  char c[] = "HELLO! These are your options: ";
  char c1[] = "1 - Open";
  char c2[] = "2 - Save As";
  char c3[] = "3 - Save";
  char ca[] = "4 - Close File";
  char c4[] = "! - Exit";
  char c5[] = "Press any key to exit the menu.";
  char c6[] = "By. Melanie Lee and Davic Le";
  
  menu = newwin(h, w, y, x); // create a new window
  box(menu, 0, 0);
  
  // arithmetic for center placement
  row =  (w - sizeof(c)) / 2;
  col = (h - 8) / 2;
  mvwprintw(menu, col, row, c);
  col = (h - 6) / 2;
  mvwprintw(menu, col, row, c1);
  col = (h - 4) / 2;
  mvwprintw(menu, col, row, c2);
  col = (h - 2) / 2;
  mvwprintw(menu, col, row, c3);
  col = h / 2;
  mvwprintw(menu, col, row, ca);
  col = (h + 2) / 2;
  mvwprintw(menu, col, row, c4);
  row =  (w - sizeof(c5)) / 2;
  col = (h + 4) / 2;
  mvwprintw(menu, col, row, c5);
  col = (h + 6) / 2;
  mvwprintw(menu, col, row, c6);
  
  wrefresh(menu);
  curs_set(1);
  
  return menu;
} // menu

/*
 * This function returns a message to the user
 *
 * @param: the 4 int variables (h,w,y, x etc) represent the dementions of the window
 *         and placement
 * @return: messWin - message to the user
 */
WINDOW * message(string mes) {
  WINDOW * messWin;
  curs_set(0);
  
  h = 5;
  w = 50;
  y = (LINES - h) / 2;
  x = (COLS - (w - 3)) / 2;
  refresh();
  const char * fName = mes.c_str();
   
  messWin = newwin(h, w, y, x); // create a new window
  box(messWin, 0, 0);
  
   // arithmetic for center placement
  row =  (sizeof(fName)) / 2;
  col = h / 2;
  mvwprintw(messWin, col, row, fName);
  
  wrefresh(messWin);
  curs_set(1);
  
  if(mes == "Close without saving? y or n?") {
    row =  (sizeof(mes)) / 2;
    col = (h - 2) / 2;
    mvwprintw(messWin, col, row, mes.c_str());
    i = getch();
    if(i == 'y') {
      save = true;
    } else {
      save = false;
      }
    wclear(messWin);    
    wrefresh(messWin);
    wrefresh(worldBorder);
    wrefresh(worldWin);
  }
  return messWin;
} // message

/*
 * This function prints out the contents of the vector
 *
 */
void printVec() {
  wclear(worldWin);
  string HELP = "  ";
  string ME = "\n";
  wprintw(worldWin, ME.c_str());
  
  for(unsigned int i = 0; i < textVec.size(); i++) {
    wprintw(worldWin, HELP.c_str());
    for(unsigned int j = 0; j < textVec[i].length(); j++) {
      
      char & cha = textVec[i].at(j);
      wprintw(worldWin, "%c", cha);
    }
    wprintw(worldWin, ME.c_str());
  }
  box(worldWin, 0, 0);
  box(worldBorder, 0, 0);
  wrefresh(worldBorder);
  wrefresh(worldWin);
  
} //printVec

/*
 * This function saves a file
 *
 */
void saveAs() {
  i = 0;
  bool yes = false;
  bool okName = false;
  char ch[80];
  
  h = 4;
  w = 50;
  y = (LINES - h) / 2;
  x = (COLS - (w - 3)) / 2;
  refresh();
  
  char c [] = "Option 2: Save As";
  char c1[] = "Name of new file: ";
  
  if(s != "" && isFileOpen == true) {
    WINDOW * saveAsing = newwin(h, w, y, x);
    box(saveAsing, 0, 0);
    
    row = (w - sizeof(c)) / 2;
    col = (h - 2) / 2;
    mvwprintw(saveAsing, col, row, c);
    row = (sizeof(c1)) / 2;
    col = h / 2;
    mvwprintw(saveAsing, col, row, c1);
    wrefresh(saveAsing);
    
    while(true) {
    echo();
    wgetstr(saveAsing, ch); // get user input
    s2 = ch;
    noecho();
    clear();
    if(s == s2) {
      message("Can not save as the same file name.");
    }
    if (s2 == "!") {
      break;
    } else {
      okName = true;
      break;
    }
  } // while true
  if(okName) {
    FILE * f1 = fopen(ch, "r");
    if(f1 != NULL) {
      message("File exists. Overwrite? y or n");
      noecho();
      switch(i = getch()) {
      case 'n': // no
	
        message("File not overwritten.");
        break;
      case 'y': // yes
        yes = true;
        break;
      default: // Invalid option
        message("Invalid option. y or n?");
      }
    } // If - same file name
    else {
      yes = true;
    } // else - different file names
    if(yes) {
      h = 4;
      w = 50;
      y = (LINES - h) / 2;
      x = (COLS - (w - 3)) / 2;
      refresh();
      curs_set(0);
      noecho();
      char ch2[] = "New file made.";
      WINDOW * newFiling = newwin(h - 1, w, y, x);
      box(newFiling, 0, 0);
      row = (w - sizeof(ch2)) / 2;
      col = (h - 2) / 2;
      mvwprintw(newFiling, col, row, ch2);
      noecho();  
  
     //wclear(worldWin);
      box(worldBorder, 0 , 0);
      box(worldWin, 0, 0);
      wrefresh(worldBorder);
      wrefresh(worldWin);
      wrefresh(newFiling);
      ifstream in(s); // input file
      ofstream out(s2); // output file
      
      for(unsigned int i = 0; i < textVec.size(); i++) {
       	out << textVec[i] << "\n";
      }
      
      in.close();
      out.close();
    }
  } // while okName
  } else {
    char cErr[] = "No file open.";
    
    if(s != "" && isFileOpen == true) {
      WINDOW * saveAsErr = newwin(h, w, y, x);
      box(saveAsErr, 0, 0);
      
      row = (sizeof(cErr)) / 2;
      col = (h - 2) / 2;
      
      mvwprintw(saveAsErr, col, row, cErr);
    }
  }
  curs_set(1);
  wrefresh(worldWin);
} // Save as

/*
 * This function opens files
 *
 */
void openFile() {
  i = 0;
  string line;
  curs_set(1);
  wclear(worldWin);
  box(worldWin, 0 , 0);
  box(worldBorder, 0 , 0);
  wrefresh(worldBorder);
  wrefresh(worldWin);
  h = 4;
  w = 50;
  y = (LINES - h) / 2;
  x = (COLS - (w - 3)) / 2;
  refresh();
  
  char c[] = "Option 1: open";
  char c1[] = "Enter your file path: ";  
  
  WINDOW * opening = newwin(h, w, y, x);
  box(opening, 0, 0);
  
  row = (w - sizeof(c)) / 2;
  col = (h - 2) / 2;
  echo();
  mvwprintw(opening, col, row, c);
  row = (sizeof(c1)) / 2;
  col = h / 2;
  mvwprintw(opening, col, row, c1);
  wrefresh(opening);
  ifstream fml;
  char str[1000];
  wgetstr(opening, str); // get user input
  wclear(opening);
  s = str;

  clear();
  noecho();
 
  fml.open(str); // user input as file name 
  if(!fml.good()) {
    h = 4;
    w = 50;
    y = (LINES - h) / 2;
    x = (COLS - (w - 3)) / 2;
    refresh();
   
    char c2[] = "Please enter a valid file.";
    WINDOW * notValid = newwin(h - 1, w, y, x);
    
    
    row = (w - sizeof(c2)) / 2;
    col = (h - 2) / 2;
    mvwprintw(notValid, col, row, c2);
    
    isFileOpen = false;
    
    box(worldWin, 0, 0);
    box(worldBorder, 0, 0); 
    box(notValid, 0, 0);
    wrefresh(worldBorder);
    wrefresh(worldWin);
    wrefresh(notValid);
    getch();
    wclear(notValid);
    wrefresh(notValid);
    delwin(notValid);
  }
  else{
    if(isFileOpen == false) {
    textVec.clear();//clear textvec
    }
    while(!fml.eof()) {
      getline(fml, line);
      textVec.push_back(line);
      
      isFileOpen = true;
    } // while - not end of file
    refresh();  // print it on to the real screen
    
    //int tlength;
    
    //tlength = 20; // num of lns in file. Calculate this in a while
    
    printVec();
    box(worldWin, 0, 0);
    box(worldBorder, 0, 0); 
    wrefresh(worldBorder);
    wrefresh(worldWin);
  } // openFile
}

int main(int argc, char * argv[]) {
  bool worldGo = true;
  WINDOW * menuWin;
  initscr(); // start curses mode
  keypad(stdscr, TRUE);
  scrollok(stdscr, TRUE);
  raw();
  refresh();
  noecho();

  
  worldWin = worldy();
  while(worldGo) {
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  //init_pair(3, COLOR_GREEN, COLOR_BLACK);
  wattron(worldWin,COLOR_PAIR(1));
  wattron(worldBorder,COLOR_PAIR(2));
  //wattron(menuWin,COLOR_PAIR(3));
  wbkgd(worldWin, COLOR_PAIR(2));
  wbkgd(worldBorder, COLOR_PAIR(1));
  //wbkgd(menuWin, COLOR_PAIR(3));
  
  if (argc == 2) {
    s = argv[1];
    openFile();
    isFileOpen = true;
    getch();
    clear();
  } // If - file is already given
  
  lnCount = 1;
  for (int j = 3; j < LINES - 3; j ++) {
    mvprintw(j, 1, "%d", lnCount);
    lnCount++;
  } // for rows print line numbers
  const char * fName = s.c_str();
  char f1Men[] = "F1: Menu";
  mvwprintw(worldBorder, 1, 1, f1Men);
  char title[] = "CSCI 1730 Editor!";
  mvwprintw(worldBorder, 1, (COLS - sizeof(title)) / 2, title);
  char fileName[] = "File: %s";
  mvwprintw(worldBorder, LINES - 2, 1, fileName, fName);
  wrefresh(worldBorder);
  wrefresh(worldWin);
  noecho();
  switch(i = getch()) {
  case KEY_F(1):
    menuWin = menu();
    getch();
    wclear(menuWin);
    wrefresh(menuWin);
    delwin(menuWin);
    break;
  case '!': 
    message("Goodbye! ");
    worldGo = false;
    break; 
  case '1':
    textVec.clear();
    echo();
    openFile();
    break;
  case '2': 
    echo();
    saveAs();
    break; 
  case '3':
    save = true;
    message("Save file?");
    wclear(worldWin);
    wrefresh(worldWin);
    wrefresh(worldBorder);
    break;
  case '4':
    if(isFileOpen) {
      message("Close without saving?");
      textVec.clear();
      wclear(worldWin);
      wrefresh(worldWin);
      wrefresh(worldBorder);
    }
    break;
  case KEY_RESIZE:
    noecho();
    worldy(); 
    printVec();
    break; 
  case KEY_UP:{
    if(isFileOpen == TRUE)
      {
	if (pos == (textVec.size()))
	  {
	    break;
	  }
	//wscrl(worldWin,1);                
	pos++;
	getyx(worldWin, y3, x3);
	wmove(worldWin,y3-1, x3);
	wrefresh(worldWin);
        
	//wprintw(worldWin, textVec[pos].c_str());
	box(worldWin, 0, 0);
	box(worldBorder, 0, 0);
	wrefresh(worldBorder);
	wrefresh(worldWin);
	//wmove(worldWin,y3,x3);
      }
    break;
  }
  case KEY_DOWN:{
    if(isFileOpen == TRUE)
      {
	
	//wscrl(worldWin,-1);
	pos--;
	getyx(worldWin, y3, x3);
	wmove(worldWin, y3+1 ,x3);
	wrefresh(worldWin);
	//wprintw(worldWin, textVec[pos-y2].c_str());
	box(worldWin, 0, 0);
	box(worldBorder, 0, 0);
	wrefresh(worldBorder);
	wrefresh(worldWin);
	//wmove(worldWin,y3,x3);
	//refresh();
      }
    break;
  }
  case KEY_LEFT:{
    getyx(worldWin, y3, x3);
    wmove(worldWin, y3 ,x3-1);
    wrefresh(worldWin);
    //wprintw(worldWin, textVec[pos-y2].c_str());
    box(worldWin, 0, 0);
    box(worldBorder, 0, 0);
    wrefresh(worldBorder);
    wrefresh(worldWin);
    break;
  }
  case KEY_RIGHT:{
    getyx(worldWin, y3, x3);
    wmove(worldWin, y3 ,x3+1);
    wrefresh(worldWin);
    //wprintw(worldWin, textVec[pos-y2].c_str());
    box(worldWin, 0, 0);
    box(worldBorder, 0, 0);
    wrefresh(worldBorder);
    wrefresh(worldWin);
    break;
  }
  default:
    noecho();
    
    char fileDescriptorf= i;
    getyx(worldWin, y3, x3);               
    wmove(worldWin, y3, x3);
    winsch(worldWin, i);
    wrefresh(worldWin);
    wmove(worldWin, y3, ++x3);
  }
  }
  
  
  getch(); // wait for user input
  clear();
  endwin(); // end curses mode
  return EXIT_SUCCESS;
} // main
