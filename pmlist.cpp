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

#include "pmlist.h"
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream.h>
PortMapList::PortMapList()
{

}

PortMapList::~PortMapList()
{

	for (list<PortMap *>::iterator itr = m_pmap.begin(); itr != m_pmap.end(); itr++)
        {
		delPacketFilter((*itr)->protocol, (*itr)->remote_host,
				(*itr)->external_ip, (*itr)->external_port);
                delPortForward((*itr)->protocol, (*itr)->external_ip,
                                (*itr)->external_port, (*itr)->internal_ip,
                                (*itr)->internal_port);

		delete (*itr);
        }
	m_pmap.clear();
}

int PortMapList::flushPacketFilter()
{

return (1);

}

int PortMapList::PortMapAdd(char *RemoteHost, int Proto, char *ExtIP, int ExtPort,
		char *IntIP, int IntPort, int Enabled, char *Desc, int LeaseDuration)
{

	int fd_socket, fd_proto;
	struct sockaddr_in addr;
	PortMap *temp;
	
	if (Proto==6)
		fd_proto = SOCK_STREAM;
	else
		fd_proto = SOCK_DGRAM;
	if ((fd_socket = socket(AF_INET,fd_proto, 0)) == -1)
		perror("socket");

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(ExtPort);
	if (bind(fd_socket, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		close(fd_socket);
		return (718);
	}

	temp = new PortMap;

	temp->fd_socket = fd_socket;

	if (RemoteHost != NULL)
	{
		temp->remote_host = new char[strlen(RemoteHost)+1];
		strcpy(temp->remote_host, RemoteHost);
	}
       	else temp->remote_host = NULL;

	temp->protocol = Proto;

	if (ExtIP != NULL)
	{
		temp->external_ip = new char[strlen(ExtIP)+1];
		strcpy(temp->external_ip, ExtIP);
	}
	else temp->external_ip = NULL;

	temp->external_port = ExtPort;
	
	if (IntIP != NULL)
	{
		temp->internal_ip = new char[strlen(IntIP)+1];
		strcpy(temp->internal_ip, IntIP);
	}
	else temp->internal_ip = NULL;

	temp->internal_port = IntPort;

	if (Desc != NULL)
	{
		temp->port_mapping_desc = new char[strlen(Desc)+1];
		strcpy(temp->port_mapping_desc, Desc);
	}
	else temp->port_mapping_desc = NULL;
	
	temp->lease_duration = LeaseDuration;

	m_pmap.push_back(temp);
	
	addPacketFilter(Proto, RemoteHost, ExtIP, ExtPort, Enabled, Desc);
	addPortForward(Proto, ExtIP, ExtPort, IntIP, IntPort, Enabled, Desc);
	
	return (1);
}


int PortMapList::PortMapDelete(int Proto, int ExtPort)
{
	
	for (list<PortMap *>::iterator itr = m_pmap.begin(); itr != m_pmap.end(); itr++)
	{
		if (((*itr)->protocol == Proto) && ((*itr)->external_port == ExtPort))
		{
			delPacketFilter((*itr)->protocol, (*itr)->remote_host,
					(*itr)->external_ip, (*itr)->external_port);
			delPortForward((*itr)->protocol, (*itr)->external_ip,
					(*itr)->external_port, (*itr)->internal_ip,
					(*itr)->internal_port);
			
			delete (*itr);
			m_pmap.erase(itr);
			return (1);
		}
	}
	return 0;
}



int PortMapList::addPacketFilter(int Proto, char *SrcIP, char *DestIP, 
	int DestPort,int Enabled, char *Desc)
{

	static char *IpAny = "0/0";
	char prt[4];
	int ret=0;
	
	if (SrcIP == NULL) 
		SrcIP = IpAny;
	if (Proto==6)
		strcpy(prt, "tcp");
	else
		strcpy(prt, "udp");
	
	// sprintf(command, "/sbin/ipchains -I upnp -j ACCEPT -p %s -s %s -d %s %d",
	// prt, SrcIP, DesIP, DestPort);
	
	ret = 1;
	return (1);
}

int PortMapList::addPortForward(int Proto, char *ExtIP, int ExtPort, 
	char *IntIP,int IntPort, int Enabled, char *Desc)
{

	char command[255];
	char prt[4];
	int ret=0;

	if (Proto==6)
		strcpy (prt, "tcp");
	else
		strcpy (prt, "udp");

	sprintf(command,"/usr/sbin/iptables -t nat -A PREROUTING -p %s -d %s --dport %d -j DNAT --to %s:%d", prt, ExtIP, ExtPort, IntIP, IntPort);
	
	system(command);

	ret=1;
	return (ret);
}

int PortMapList::delPacketFilter(int Proto, char *SrcIP, char *DestIP, 
	int DestPort)
{

	static char *IpAny = "0/0";
	char prt[4];
	int ret=0;

	if (SrcIP == NULL) 
		SrcIP = IpAny;
	if (Proto==6)
		strcpy(prt, "tcp");
	else
		strcpy(prt, "udp");

	/*sprintf(command,"/sbin/ipchains -D upnp -j ACCEPT -p %s -s %s -d %s %d"
	, prt, SrcIP, DestIP, DestPort);
	  doCommand(command*/
	
	ret = 1;
	return(ret);
}

int PortMapList::delPortForward(int Proto, char *ExtIP, int ExtPort, 
	char* IntIP, int IntPort)
{

	char command[255];
	char prt[4];
	int ret=0;
	
	if (Proto==6)
		strcpy (prt, "tcp");
	else
		strcpy (prt, "udp");

	sprintf(command, "/usr/sbin/iptables -t nat -D PREROUTING -p %s -d %s --dport %d -j DNAT --to %s:%d", prt, ExtIP, ExtPort, IntIP, IntPort);
	
	system(command);
	ret = 1;

	return (ret);
}

