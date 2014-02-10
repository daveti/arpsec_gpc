////////////////////////////////////////////////////////////////////////////////
//
//  File          : arpsecd.c
//  Description   : This the main function for the arpsecd daemon.
//
//  Author  : Patrick McDaniel
//  Created : Tue Mar 26 11:44:08 EDT 2013
//  Dev	    : daveti
//  Modified: Fri Sep 20 10:51:12 PDT 2013
//

// Incliudes	
#include <stdio.h>
#include <unistd.h>
#include <gprolog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

// Project Includes
#include "AsControl.h"
#include "AsLogic.h"
#include "AsLog.h"
#include "AsTMeasure.h"
#include "tpmw.h"

// Definitions
#define ARPSEC_ARGUMENTS "shfcandbetl:"
#define USAGE \
    "\nUSAGE: arpsecd [-h] [-l <logfile>] [-s] [-f] [-a] [-c] [-n] [-d] [-b] [-e] [-t]\n" \
    "\n" \
    "where:\n" \
    "   -h - display this help information\n" \
    "   -l - set the log file (stderr by default), where\n" \
    "        logfile - the path to the file to place log information.\n" \
    "   -f - force the attestation even if the logic approves the mapping.\n" \
    "   -a - allow the binding if no DB entry found during attestation.\n" \
    "	-c - cache the MAC/IP if the attestation succeed.\n" \
    "	-n - no logic layer invoked (always return 'untrusted' from logic layer).\n" \
    "	-d - disable aslFindXXXBindings (only for dev performance debugging).\n" \
    "	-b - disable aslAddBindingsXXX (only for dev performance debugging).\n" \
    "	-e - disable log printing to the standard output.\n" \
    "	-t - disable TPM function (already return success - only for dev performance debugging).\n" \
    "   -s - simulate the kernel and network traffic\n\n"

//
// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : main
// Description  : This is the main function for the arpsecd deamon
//
// Inputs       : mode - simulate or run normally
// Outputs      : 0 if successful, -1 if not

int main(int argc, char **argv) {

    // Local variables
    int ch;
    int simulate = 0;

    //playwithiface();

    // Process the command line parameters
    while ((ch = getopt(argc, argv, ARPSEC_ARGUMENTS)) != -1) {

	// Look at the parameter
	switch (ch) {
	    case 'h': // Help, print usage
		fprintf( stderr, USAGE );
		return( -1 );

	    case 'l': // Set log filename
		setAsLogFilename( optarg );
		break;

	    case 's': // Simulate flag
		simulate = 1;
		break;

	    case 'f': // Force attest flag
		ascForceAttest();
		break;

	    case 'c': // Enable the cache
		ascEnableCache();
		break;

	    case 'n': // Disable the logic layer
		aslDisableLogic();
		break;

	    case 'd': // Disable the aslFindXXXBindings
		ascDisableLogicFindBindings();
		break;

	    case 'b': // Disable the aslAddBindingsXXX
		ascDisableLogicAddBindings();
		break;

	    case 'a': // Allow the binding if no DB entry found
		astAllowBinding();
		break;

	    case 'e': // Disable log printing
		asLogDisable();
		tpmw_disable_log();
		break;

	    case 't': // Disable the TPM function
		astDisableTPM();
		break;

	    default:  // Default (unknown)
		fprintf( stderr, "Unknown command line oition (%c), aborting.", ch );
		return( -1 );
	}
    }

    // Go into the arpsec control loop and process
    ascControlLoop( simulate );

    // Return successfully
    return( 0 );
}
