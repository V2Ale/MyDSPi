#ifndef __LIBINI_H__
#define __LIBINI_H__

char *trim(char *str);
f_return getSectionParameterValue(char *pPath, char *section, char *parameter, char *value);
float getSPVf(char *pPath, char *section, char *parameter);
int getSPVi(char *pPath, char *section, char *parameter);
char* getSPVc(char *pPath, char *section, char *parameter);

#endif
