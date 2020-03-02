#include "ParamMgr.h"
namespace clustering
{
ParamMgr::ParamMgr():
    M (4),
    K (140), 
    MaxClusterSize(80),
    ThreadNum(8),
    Tol (0.0001), 
    Epsilon(5000), 
    MaxDisp(3e+5), 
    MaxBandwidth(1e+5) 
{
    SqrEpsilon = Epsilon * Epsilon;
    SqrMaxDisp = MaxDisp * MaxDisp;
    SqrMaxBandwidth = MaxBandwidth * MaxBandwidth;
}



}
