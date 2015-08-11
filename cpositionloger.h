#ifndef CPOSITIONLOGER_H
#define CPOSITIONLOGER_H

#include "cgpspositionprovider.h"

class ILocalDataStorage;
class IRemoteDataStorage;

class CPositionLoger
{
public:
    CPositionLoger();
    ~CPositionLoger();

private:
    CGpsPositionProvider m_oGpsPosProvider;
    ILocalDataStorage * m_pLocalDataStorage;
    IRemoteDataStorage * m_pRemoteDataStorage;
};

#endif // CPOSITIONLOGER_H
