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

#include "config.h"
#include <malloc.h>
#include <string.h>
#include <iostream>

int conf_flag_forward      =1;  // If 1 rules will be inserted first in the forward chain
                                // If 0 no rules are inserted (FORWARD policy must be accept)
char conf_forward_chain[] = "upnp";  // name of forward chain
int conf_debug_mode=0;          // If 1 upnpd will not fork and all Upnp_Action events
                                // will be printed to stdout.
char conf_iptables_command[] = "/sbin/iptables"; // Full path and name of iptables command



// ------------ SECOND PART -------------
#ifdef NOT_DEFINED_AT_ALL
char delim[]={' ','\t'};

/* removes the caracters in delim from the left side of the string */
char *ltrim(char *string)
{
  unsigned int i,j,flag,comp;

  comp=strlen(delim);

  do {
    flag=0;
    for(i=0;i<comp;i++)
      if(string[0]==delim[i]) {
        flag=1;

        for(j=1;j<=strlen(string);j++)
          string[j-1]=string[j];
      }
    }while(flag);

  return string;
}

/* removes the caracters in delim from the right side of the string */
char *rtrim(char *string)
{
  unsigned int i,j,flag,comp;

  comp=strlen(delim);

  j=strlen(string)-1;

  do {
    flag=0;
    for(i=0;i<comp;i++)
      if(string[j]==delim[i]) {
        flag=1;

        string[j]='\0';
        j--;
      }
    }while(flag);


  return string;
}

/*
 * Our first word is our key word that can be separated by spaces and tabs
 * the rest after that is our information
 */
void config_process(char *line)
{
  unsigned int i;
  char *min;

  char *first,*last;

  if(line[0]=='#') // ignore lines started by #
    return;

  ltrim(line);

  min=index(line,delim[0]);

  for(i=1;i<=strlen(delim);i++)
    if(index(line,delim[i])<min && index(line,delim[i])!=null)
      min=index(line,delim[i]);

  if(min==null)
    return;

  first=line;
  last=min;

  last[0]='\0';
  last=min+sizeof(char);

  ltrim(last);
  rtrim(last);

  add_node();
  strncpy(current->key,first,MAX_CONFIG_LINE);
  strncpy(current->info,last,MAX_CONFIG_LINE);

  // Some rules
  if(!strcasecmp(current->key,"port_forward"))
    if(!strcasecmp(current->info,"yes"))
      conf_flag_forward=1;

  // just for debugging
  //cout << "key: " << current->key << endl << "info: " << current->info << endl;
}

/* Find out if we have one particular key in config */
int config_have(char *key)
{
  if(move_first())
    do {
      if(!strcasecmp(current->key,key))
	return 1;
    }while(move_next());

  return 0;
}

/* Returns the info for that key */
char *config_info(char *key)
{
  if(move_first())
    do {
      if(!strcasecmp(current->key,key))
	return current->info;
    }while(move_next());

  return null;
}
#endif
