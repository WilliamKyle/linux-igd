#include <upnp/upnp.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <time.h>
#include "config.h"
#include "gatedevice.h"
#include "util.h"
#include "pmlist.h"
#include "globals.h"

int main (int argc, char** argv)
{
	int ret = UPNP_E_SUCCESS;
	int signum;	
	char descDocUrl[50];
	char descDocName[20];
	char xmlPath[50];
	char intIpAddress[16];     // Server internal ip address
	sigset_t sigsToCatch;

	pid_t pid,sid;
	struct rlimit resourceLimit = { 0 };
	int i;
	int status;
	
	if (argc != 3)
   {
      printf("Usage: upnpd <external ifname> <internal ifname>\n");
      printf("Example: upnpd ppp0 eth0\n");
      printf("Example: upnpd eth1 eth0\n");
      exit(0);
   }

	parseConfigFile(&g_forwardRules,&g_debug,g_iptables,
		        g_forwardChainName,g_preroutingChainName,
			g_upstreamBitrate,g_downstreamBitrate,
			descDocName,xmlPath);
	// Save the interface names for later uses
	strcpy(g_extInterfaceName, argv[1]);
	strcpy(g_intInterfaceName, argv[2]);

	// Get the internal ip address to start the daemon on
	GetIpAddressStr(intIpAddress, g_intInterfaceName);	

	// Put igd in the background as a daemon process.
	pid = fork();
	if (pid < 0)
	{
		perror("Error forking a new process.");
		exit(EXIT_FAILURE);
	}
	if (pid > 0)
		exit(EXIT_SUCCESS);

	// become session leader
	if ((sid = setsid()) < 0)
	{
		perror("Error running setsid");
		exit(EXIT_FAILURE);
	}

	// close all file handles
	resourceLimit.rlim_max = 0;
	status = getrlimit(RLIMIT_NOFILE, &resourceLimit);
	if (-1 == status) /* shouldn't happen */
	{
	    perror("error in getrlimit()");
	    exit(1);
	}
	if (0 == resourceLimit.rlim_max)
	{
	    fprintf(stderr, "Max number of open file descriptors is 0!!\n");
	    exit(1);
	}	
	for (i = 0; i < resourceLimit.rlim_max; i++)
	    close(i);
	
	// fork again so child can never acquire a controlling terminal
	pid = fork();
	if (pid < 0)
	{
		perror("Error forking a new process.");
		exit(EXIT_FAILURE);
	}
	if (pid > 0)
		exit(EXIT_SUCCESS);
	
	if ((chdir("/")) < 0)
	{
		perror("Error setting root directory");
		exit(EXIT_FAILURE);
	}
	
	umask(0);

// End Daemon initialization

	// Initialize UPnP SDK on the internal Interface
	if (g_debug) syslog(LOG_DEBUG, "Initializing UPnP SDK ... ");
	if ( (ret = UpnpInit(intIpAddress,0) ) != UPNP_E_SUCCESS)
	{
		syslog (LOG_ERR, "Error Initializing UPnP SDK on IP %s ",intIpAddress);
		syslog (LOG_ERR, "  UpnpInit returned %d", ret);
		UpnpFinish();
		exit(1);
	}
	if (g_debug) syslog(LOG_DEBUG, "UPnP SDK Successfully Initialized.");

	// Set the Device Web Server Base Directory
	if (g_debug) syslog(LOG_DEBUG, "Setting the Web Server Root Directory to %s",xmlPath);
	if ( (ret = UpnpSetWebServerRootDir(xmlPath)) != UPNP_E_SUCCESS )
	{
		syslog (LOG_ERR, "Error Setting Web Server Root Directory to: %s", xmlPath);
		syslog (LOG_ERR, "  UpnpSetWebServerRootDir returned %d", ret); 
		UpnpFinish();
		exit(1);
	}
	if (g_debug) syslog(LOG_DEBUG, "Succesfully set the Web Server Root Directory.");


	// Form the Description Doc URL to pass to RegisterRootDevice
	sprintf(descDocUrl, "http://%s:%d/%s", UpnpGetServerIpAddress(),
				UpnpGetServerPort(), descDocName);

	// Register our IGD as a valid UPnP Root device
	if (g_debug) syslog(LOG_DEBUG, "Registering the root device with descDocUrl %s", descDocUrl);
	if ( (ret = UpnpRegisterRootDevice(descDocUrl, EventHandler, &deviceHandle,
													&deviceHandle)) != UPNP_E_SUCCESS )
	{
		syslog(LOG_ERR, "Error registering the root device with descDocUrl: %s", descDocUrl);
		syslog(LOG_ERR, "  UpnpRegisterRootDevice returned %d", ret);
		UpnpFinish();
		exit(1);
	}

	syslog (LOG_DEBUG, "IGD root device successfully registered.");
	
	// Initialize the state variable table.
	StateTableInit(descDocUrl);
	
	// Record the startup time, for uptime
	startup_time = time(NULL);
	
	// Send out initial advertisements of our device's services with timeouts of 30 minutes
	if ( (ret = UpnpSendAdvertisement(deviceHandle, 1800) != UPNP_E_SUCCESS ))
	{
		syslog(LOG_ERR, "Error Sending Advertisements.  Exiting ...");
		UpnpFinish();
		exit(1);
	}
	syslog(LOG_DEBUG, "Advertisements Sent.  Listening for requests ... ");
	
	// Loop until program exit signals recieved
	sigemptyset(&sigsToCatch);
	sigaddset(&sigsToCatch, SIGINT);
	sigaddset(&sigsToCatch, SIGTERM);
	//sigwait(&sigsToCatch, &signum);
	pthread_sigmask(SIG_SETMASK, &sigsToCatch, NULL);
	sigwait(&sigsToCatch, &signum);
	syslog(LOG_DEBUG, "Shutting down on signal %d...\n", signum);

	// Cleanup UPnP SDK and free memory
	pmlist_FreeList(); 

	UpnpUnRegisterRootDevice(deviceHandle);
	UpnpFinish();

	// Exit normally
	return (1);
}
