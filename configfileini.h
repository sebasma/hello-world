//
//  ConfigFileIni.h
//  Master
//
//  Created by J Matthew Sebastian on 5/7/19.
//  Copyright © 2019 J Matthew Sebastian. All rights reserved.
//

#ifndef ConfigFileIni_h
#define ConfigFileIni_h

int GetPrivateProfileString( char *pszAppName, char *pszKeyName, char *pszDefault,
                            char *pszReturnedString, int nSize, char *pszFileName );
int GetPrivateProfileInt   ( char  *pszAppName, char *pszKeyName, int nDefault, int &iReturnVal, char *pszFileName );
int GetPrivateProfileLong  ( char *pszAppName,char *pszKeyName, long nDefault,long &lReturnVal,char *pszFileName);
int GetPrivateProfileDouble( char *pszAppName,char *pszKeyName, double nDefault, double &dReturnVal,char *pszFileName);

#endif /* ConfigFileIni_h */
