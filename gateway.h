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

#ifndef _GATEWAY_H_
#define _GATEWAY_H_ 

extern pthread_mutex_t DevMutex;

// Location of your XML and SKL files.  Default is /etc/linuxigd
#define INIT_CONF_DIR      "/etc/linuxigd/"

// Location of your iptables script.  Default is /sbin/iptables
#define IPTABLES	"/sbin/iptables"

// Internal variables, do not modify.
#define GATE_SERVICE_CONFIG	1
#define GATE_SERVICE_CONNECT	2

#endif // _GATEWAY_H_
