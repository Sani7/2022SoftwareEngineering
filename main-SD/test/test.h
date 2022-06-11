#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ymax    100
#ifndef succes
#define succes  0
#endif
#ifndef failed
#define failed    1
#endif

bool readSetting(char *fileContent, int contentLength, const char *settingName, char *returnBuff, int returnLength);
bool convertToInt(char *Content, int *returnBuff, int len);