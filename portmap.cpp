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

#include "portmap.h"
#include <string.h>

PortMap::PortMap ()
{

}

PortMap::~PortMap ()
{
	if (remote_host)
		delete [] remote_host;
	if (external_ip)
		delete [] external_ip;
	if (internal_ip)
		delete [] internal_ip;
	if (port_mapping_desc)
		delete [] port_mapping_desc;
}

PortMap::PortMap(char *RemoteHost, int Proto, char *ExtIP, int ExtPort, char *IntIP, int IntPort, int Enabled, char *Desc, int LeaseDuration)
{
	remote_host = new char[10];
	if (strlen(RemoteHost) < sizeof (remote_host))
		strcpy (remote_host, RemoteHost);
	protocol = Proto;
	external_ip = new char[12];
	if (strlen(ExtIP) < sizeof(external_ip))
		strcpy (external_ip, ExtIP);
	external_port = ExtPort;
	internal_ip = new char[12];
	if (strlen(IntIP) < sizeof (internal_ip))
		strcpy (internal_ip, IntIP);
	internal_port = IntPort;
	enabled = Enabled;
	port_mapping_desc = new char[255];
	if (strlen(Desc) < sizeof(port_mapping_desc))
		strcpy (port_mapping_desc, Desc);
	lease_duration = LeaseDuration;
}


