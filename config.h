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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CONF_FILE          "/etc/upnpd.conf"

#define MAX_CONFIG_LINE 256
#define null NULL

extern int flag_forward;

typedef struct config_node {
  char key[MAX_CONFIG_LINE];
  char info[MAX_CONFIG_LINE];
  struct config_node *next;
  struct config_node *prev;
} config_node;

int move_first();
int move_last();
int move_next();
int move_prev();
void add_node();
void del_node();

void config_process(char *line);
int config_have(char *key);
char *config_info(char *key);

#endif
