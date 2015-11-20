#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <windows.h>
#include "scope.h"
#include "settingsMotorScope.h"




int main(void)
{
    //Initialize variables to pull in settings
    char dataDir[100], expName[100], expType[100];
    MOTSETTINGS   *mptr, motorSettings;
    SCOPESETTINGS   scopeSettings;
    mptr = &motorSettings;
    // sptr = &scopeSettings;
    char foo[1000];

//    // Read in experiment/scope/motor setting parameters from settings.txt file
    FILE * settingsFile;
    settingsFile = fopen("settings.txt", "r");

    // Read in file storage settings
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "dataDir = %[^\n]s\n", dataDir);
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "expName = %s\n", expName);
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "expType = %s\n", expType);

    // Read in motor settings
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "stepSizeX [mm] = %d", &motorSettings.stepSizeX);//&motorSettings.stepSizeX);
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "stepSizeY [mm] = %d", &motorSettings.stepSizeY);
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "windowSizeX [mm] = %d", &motorSettings.windowSizeX);
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "windowSizeY [mm] = %d", &motorSettings.windowSizeY);
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "velocityX = %d", &motorSettings.velX);
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "velocityY = %d", &motorSettings.velY);
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "pausetime [s] = %d", &motorSettings.pauseTime);

    // Read in scope settings
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "probeAttn = %s", scopeSettings.probeAttn);
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "channel = %s", scopeSettings.channel);
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "coupling = %s", scopeSettings.coupling);
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "numOfPoints = %s", scopeSettings.numOfPoints);
    fgets(foo, 10000, settingsFile);
    sscanf(foo, "numOfAverages = %s", scopeSettings.numOfAverages);
    fclose(settingsFile);

    // Copy this to raster scan later
    int Nx = motorSettings.windowSizeX/motorSettings.stepSizeX;
    int Ny = motorSettings.windowSizeY/motorSettings.stepSizeY;

    // first make directory for storing files
    std::string dirLevel = std::string(dataDir) + std::string(expName) + "\\" ;
    CreateDirectory(dirLevel.c_str(), NULL);
    dirLevel = dirLevel + std::string(expType);
    CreateDirectory(dirLevel.c_str(), NULL);

    // if planar do as follows
    scope SCOPE;
    //SCOPE.initializeScope()
    SCOPE.initializeScope(scopeSettings);
    int i;
    std::ostringstream s;
    if (strcmp(expType, "Planar") == 0)
    {
        for (i = 0; i <= Nx*Ny; i++)
        {
            //std::string s = std::to_string(i);
            s << i;
            std::string filename = dirLevel+"\\"+s.str()+".dat";
            SCOPE.getScopeData(filename.c_str(), scopeSettings);
            s.str("");
            s.clear();
        }
    }
    SCOPE.closeScope();
    //initializeScope();
    //getScopeData();
    //closeScope();
   // return 0;
}