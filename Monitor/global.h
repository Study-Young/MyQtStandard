#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDateTime>

enum EN_MessageType
{
    MESSAGE_ERROR,
    MESSAGE_EVENT,
    MESSAGE_OPERATOR,
};

extern QString g_userName;
extern int g_userLevel;

extern bool g_saveWorkLogFlag;
extern bool g_saveOriDataFlag;

void Crc16Func(unsigned char *ptr, int len, unsigned short *pCrc);
bool isVaildIp(const char* ip);
QString getCurTime();


#endif // GLOBAL_H
