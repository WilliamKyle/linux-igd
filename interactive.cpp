/******************************************************************************
*  Copyright (c) 2002 Linux UPnP Internet Gateway Device Project              *    
*  All rights reserved.                                                       *
*                                                                             *   
*  This file is part of The Linux UPnP Internet Gateway Device (IGD).         *
*                                                                             *
*  The Linux UPnP IGD is free software; you can redistribute it and/or modify *
*  it under the terms of the GNU General Public License as published by       *
*  the Free Software Foundation; either version 2 of the License, or          *
*  (at your option) any later version.                                        *
*                                                                             *    
*  The Linux UPnP IGD is distributed in the hope that it will be useful,      *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of             *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
*  GNU General Public License for more details.                               *
*                                                                             *   
*  You should have received a copy of the GNU General Public License          * 
*  along with Foobar; if not, write to the Free Software                      *
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  *
*                                                                             *  
*                                                                             *  
******************************************************************************/

#include "interactive.h"
#include <iostream>
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>

int max_y,max_x;
int draw_time=1;

void interactive_mode()
{
  if((initscr() == NULL))       // if initscr() returns a NULL 
    {
      cout << "Error initializing ncurses!" << endl;  // print error message 
      exit(1);                           
    }

  signal(SIGWINCH, resizeHandler); // If the screen gets resized we receive this signal

  getmaxyx(stdscr,max_y,max_x); // get screen resolution  

  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  while(getch()!='\n') {
    if(draw_time) {
      redraw();
      draw_time=0;
    }
  }
  endwin();
}

void redraw()
{
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_CYAN);
  init_pair(2, COLOR_BLUE, COLOR_CYAN);

  attron(COLOR_PAIR(1));
  for(int i=0;i<max_x*max_y;i++)
    printw(" ");

  mvprintw(6,((max_x - 9)/2),"Linux IGD");

  attron(COLOR_PAIR(2));
  box(4,((max_x - 9)/2)-2,13,5);
  
  attroff(COLOR_PAIR(1));
  refresh();
}

void resizeHandler(int sig)
{
  getmaxyx(stdscr,max_y,max_x); // get the new screen resolution 
  draw_time=1;
}

void box(int x,int y, int width, int height)
{
  int i;

  mvprintw(x,y,"+");
  for(i=0;i<width-2;i++)
    printw("-");
  printw("+");

  for(i=1;i<height-1;i++){
    mvprintw(x+i,y,"|");
    mvprintw(x+i,y+width-1,"|");
  }

  mvprintw(x+height-1,y,"+");
  for(i=0;i<width-2;i++)
    printw("-");
    printw("+");
}
