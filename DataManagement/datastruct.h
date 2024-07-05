#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#define CAL_INTERVAL_CNT 22

enum {
    ADEV = 1,
    TDEV
};

typedef struct FreqAllanStruct
{
    int     dataCnt;
    double  freqAverage;
    int     valueNum;
    double  freqSum;
    double  sigmaSum;
    double  result;
}FreqAllanStruct;

typedef struct TdevStruct
{
    double  tdevValue;
    int     valueNum;
    double  result;
}TdevStruct;

const double g_calKeys[CAL_INTERVAL_CNT] = {1, 2, 4, 8, 10, 20, 40, 80, 100, 200, 400, 800,
                                            1000, 2000, 4000, 8000, 10000, 20000, 40000, 80000,
                                            86400, 100000};

#endif // DATASTRUCT_H
