#include "test.h"

bool readSetting(char *fileContent, int contentLength, const char *settingName, char *returnBuff, int returnLength)
{
    // strncpy(buf, key, l);
    char ContentBuf[contentLength];
    strncpy(ContentBuf, fileContent, sizeof(ContentBuf));
    char *saveptr1;
    char *saveptr2;

    // loop over \n tokens
    char *tok1 = strtok_r(ContentBuf, "\n", &saveptr1);
    while (tok1 != NULL)
    {
        // loop over = token
        char *tok2 = strtok_r(tok1, "=", &saveptr2);
        if (tok2 != NULL)
        {
            if (strcmp(tok2, settingName) == 0)
            {
                tok2 = strtok_r(NULL, "=", &saveptr2);
                if (tok2 != NULL)
                {
                    strncpy(returnBuff, tok2, returnLength - 1);
                }
                return succes;
            }
        }

        tok1 = strtok_r(NULL, "\n", &saveptr1);
    }
    return failed;
}

bool convertToInt(char *Content, int *returnBuff, int len)
{
    // loop over , tokens
    char *tok;
    int i = 0;
    while ((tok = strsep(&Content, ",")) != NULL)
    {
        if (!(i < len))
        {
            return failed;
        }
        if (strlen(tok) == 0)
        {
            returnBuff[i] = ymax + 1;
        }
        else
        {
            returnBuff[i] = atoi(tok);
        }
        i++;
    }
    return succes;
}
