#ifndef __LIBINI_H__
#define __LIBINI_H__




char *trim(char *str);
char* ReadFile(char* pPath);
char* tokenize(char* input, char* output[FILENAME_MAX]);

f_return getSectionParameterValue(char *pFile, char *section, char *parameter, char *value);
float getSPVf(char *pFile, char *section, char *parameter);
int getSPVi(char *pFile, char *section, char *parameter);
char* getSPVc(char *pFile, char *section, char *parameter);

#endif
