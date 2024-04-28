#include "global.h"

QString g_userName;
int g_userLevel;   // 0:测试  1:管理  2:研发  3:超级管理

bool g_saveWorkLogFlag;
bool g_saveOriDataFlag;

// 生成CRC校验码
void Crc16Func(unsigned char *ptr, int len, unsigned short *pCrc)
{
    ushort crc = 0XFFFF;
    for(int i=0; i<len; i++)
    {
        crc = (ushort)(crc ^ ptr[i]);
        for(int j=0; j<8; j++)
        {
            crc = (crc & 1) != 0 ? (ushort)((crc >> 1) ^ 0xA001) : (ushort)(crc >> 1);
        }
    }
    *pCrc = (crc & 0XFF00) | (crc & 0x00FF);
}

bool isVaildIp(const char* ip)
{
    int dots = 0;                                       //字符 . 的个数
    int setions = 0;                                    //ip每一部分总和（0-255）
    int strnum = 0;                                     //字符串长度记录

    if (NULL == ip || *ip == '.')                       //排除输入参数为NULL, 或者第一个字符为'.'的字符串
        return false;                                   //格式错误

    while (*ip)                                         //字符串不为空
    {
        if (*ip == '.')                                 //如果当前字符为点.则开始判断上一段是否合法在0~255之间
        {
            dots++;                                     //字符 . 的个加一
            if (setions >= 0 && setions <= 255)         //检查ip是否合法
                setions = 0;                            //对上一段IP总和进行清零
            else
                return false;                           //格式错误，某一段数值不在0~255之间
        }
        else if (*ip >= '0' && *ip <= '9')              //判断是不是数字
            setions = setions * 10 + (*ip - '0');       //求每一段总和
        else
            return false;                               //该字符既不是数字也不是点
        ip++;                                           //指向下一个字符
        if (++strnum>15)                                //3*4+3=15,IP地址字符串非结束符字符数最多15个
            return false;                               //防止出现“1234567890987654321.”这种前面全是数字的字符串输入进来。
    }
    /*判断IP最后一段是否合法*/
    if (setions >= 0 && setions <= 255)
    {
        if (dots == 3)                                  //防止出现：“192.168.123”这种不是三个点的情况
            return true;                                //IP地址合法正确
    }
    return false;
}

QString getCurTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}
