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
}

PortMap::PortMap(char *RemoteHost, char *Proto, char *ExtIP, int ExtPort, char *IntIP, int IntPort, int Enabled, char *Desc, int LeaseDuration)
{
	
	strcpy (m_RemoteHost, RemoteHost);
	strcpy (m_PortMappingProtocol,Proto);
	m_ExternalPort = ExtPort;
	strcpy (m_InternalClient, IntIP);
	m_InternalPort = IntPort;
	m_PortMappingEnabled = Enabled;
	strcpy (m_PortMappingDescription, Desc);
	m_PortMappingLeaseDuration = LeaseDuration;
}


