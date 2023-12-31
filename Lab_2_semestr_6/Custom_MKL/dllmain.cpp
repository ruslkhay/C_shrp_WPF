﻿// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "mkl.h"
#include "mkl_df_types.h"

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}




extern "C" _declspec(dllexport) double Interpolate(const int length, const double* points, const double* func, double* res, double* der, int gridlen, double* grid, const double* limits, double* integrals)
{
    DFTaskPtr task;
    int i = 3;
    double* scoeff = new double[(length - 1) * DF_PP_CUBIC];
    MKL_INT dorder[] = { 1, 1, 1 };
    double* limit1 = new double[1];
    double* limit2 = new double[1];
    limit1[0] = limits[0];
    limit2[0] = limits[1];

    int status = dfdNewTask1D(&task, length, points, DF_NON_UNIFORM_PARTITION, 1, func, DF_NO_HINT);
    status = dfdEditPPSpline1D(task, DF_PP_CUBIC, DF_PP_NATURAL, DF_BC_1ST_LEFT_DER + DF_BC_1ST_RIGHT_DER, der, DF_NO_IC, NULL, scoeff, DF_NO_HINT);
    status = dfdConstruct1D(task, DF_PP_SPLINE, DF_METHOD_STD);
    status = dfdInterpolate1D(task, DF_INTERP, DF_METHOD_PP, gridlen, grid, DF_UNIFORM_PARTITION, i, dorder, NULL, res, DF_NO_HINT, NULL);
    status = dfdIntegrate1D(task, DF_METHOD_PP, 1, limit1, DF_UNIFORM_PARTITION, limit2, DF_UNIFORM_PARTITION, NULL, NULL, integrals, DF_NO_HINT);
    status = dfDeleteTask(&task);
    return DF_BC_1ST_LEFT_DER + DF_BC_1ST_RIGHT_DER;
}

