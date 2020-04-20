//
//  ConfigFileIni.cpp
//  Master
//
//  Created by J Matthew Sebastian on 5/7/19.
//  Copyright © 2019 J Matthew Sebastian. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <memory.h>

#ifdef WINDOWS
#include "..\include\configfileini.h"
#else
#include "../include/configfileini.h"
#endif
/**********************************************************************
 ; ini Example
 ; last modified 1 April 20019
 [owner]
 name=John Doe
 organization=Acme Widgets Inc.
 
 [database]
 ; use IP address in case network name resolution is not working
 server=192.0.2.62
 port=143
 file="payroll.dat"
***********************************************************************/
/*
 ;  config file for d2pdb
 [log]
 name=text.log
 
 [image files]
 ; use IP address in case network name resolution is not working
 directories=c:\testdata
 */




int GetPrivateProfileString( char *pszAppName, char *pszKeyName, char *pszDefault, char *pszReturnString, int nSize, char *pszFileName )
{
    int iRc = 0;
    FILE *fp;
    char *pszFileContents;
    struct stat filestatus;
    char szBuffer[1024] = "";
    char szSection[200] = "";
    char *pStartKey = NULL;
    
    
    memset( (void*)szBuffer, '\0', sizeof(szBuffer));
    memset( (void *)pszReturnString, '\0', nSize);
    
    
    iRc = stat( pszFileName, &filestatus );
    
    if(iRc)
    {
        strcpy( pszReturnString, pszDefault);
        printf( "Error: Configuration file [%s] does not exist. rc = %d\n", pszFileName, iRc );
        return -1;
    }
    
    pszFileContents = (char *)malloc( filestatus.st_size+1 );
    
    fp = fopen(pszFileName, "r");
    if (fp)
    {
        iRc = (int)fread(pszFileContents, 1, filestatus.st_size, fp );
        
        fclose(fp);
        if( iRc != filestatus.st_size)
        {
            strcpy( pszReturnString, pszDefault);
            printf( "Error: Configuration file [%s] does not exist. rc = %d\n", pszFileName, iRc );
            return -1;
        }
    }
    else
    {
        printf("Error - cannot open file [%s]\n", pszFileName);
    }
    
    // ignore comments
    int iComments = 0;
    for( int z = 0; z < filestatus.st_size+1; z++ )
    {
        if( pszFileContents[z] == ';')
            iComments = 1;
        if( pszFileContents[z] == '\n')
            iComments = 0;
        if( iComments == 1)
            pszFileContents[z] = ' ';
    }
    
    // build search string out of section name
    sprintf(szSection, "[%s]", pszAppName );
    int iSection = (int)strlen(szSection);
    
    // search for the section name
    char *pStartSection = strstr( pszFileContents, szSection);
    if( pStartSection == NULL )
    {
        // section does not exist in the configuraiton file
        free(pszFileContents);
        strcpy( pszReturnString, pszDefault);
        pszFileContents = NULL;
        return -1;
    }
    else
    {
        // skip over the section name
        pStartSection += iSection;
    }


    // search for the beginning of the next section if it exists
//    char *pEndSection = strstr( pStartSection, "[" );
    
    // Build the Key Lable
    char szKeyLabel[100] = "";
    sprintf( szKeyLabel, "%s=", pszKeyName );
    int iKeyLabel = 0;
    iKeyLabel = (int)strlen( szKeyLabel );

    // search for the keyname
    char *pStartKeyLine = strstr( pStartSection, szKeyLabel );

    // if Key Label isn't found return default
    if( pStartKeyLine == NULL )
    {
        // key  does not exist in the configuraiton file
        strcpy( pszReturnString, pszDefault);
        free(pszFileContents);
        pszFileContents = NULL;
        return -2;
    }
    else
    {
        // skip over the key name
        pStartKeyLine += iKeyLabel;
        pStartKey = pStartKeyLine;
    }

    // got the beginning of the key value
    char *pEndKey =  NULL;
    pEndKey = pStartKey;

    // get the end of the key value
    while (( *pEndKey != '\n' ) && ( *pEndKey != '\0' ))
        pEndKey += 1;

    // copy the value to the return string
    int iKey = (int)(pEndKey - pStartKey);

    // check if their is enough room in the return buffer
    if( iKey > nSize )
    {
        iRc = -3;
        strncpy( pszReturnString, pStartKey, nSize-1);
    }
    else
    {
        iRc = 0;
        strncpy(pszReturnString, pStartKey, iKey );
    }
    free(pszFileContents);

    //return 0 for sucess -# for error
    return(iRc);
}

int GetPrivateProfileInt   ( char  *pszAppName, char *pszKeyName, int nDefault, int &iReturnVal, char *pszFileName )
{
    int irc = 0;
    char szDefault[100] = "unknown";
    char szReturnString[100] = "";

    irc = GetPrivateProfileString( pszAppName, pszKeyName, szDefault, szReturnString, sizeof(szReturnString), pszFileName);
    
    if ( irc == 0 )
    {
        iReturnVal = atoi(szReturnString);
        
    }
    else
        iReturnVal = nDefault;
    
    return irc;
}


int GetPrivateProfileLong  ( char *pszAppName,char *pszKeyName,long nDefault,long &lReturnVal,char *pszFileName)
{
    int irc = 0;
    char szDefault[100] = "unknown";
    char szReturnString[100] = "";

    irc = GetPrivateProfileString( pszAppName, pszKeyName, szDefault, szReturnString, sizeof(szReturnString), pszFileName);
    
    if ( irc == 0 )
    {
        lReturnVal = atol(szReturnString);
        
    }
    else
        lReturnVal = nDefault;
    
    return irc;
}

int GetPrivateProfileDouble( char *pszAppName, char *pszKeyName, double nDefault, double &dReturnVal, char *pszFileName)
{
    int irc = 0;
    char szDefault[100] = "unknown";
    char szReturnString[100] = "";

    irc = GetPrivateProfileString( pszAppName, pszKeyName, szDefault, szReturnString, sizeof(szReturnString), pszFileName);
       
    if ( irc == 0 )
    {
        dReturnVal = atof(szReturnString);
    }
    else
        dReturnVal = nDefault;
       
    return irc;
}
