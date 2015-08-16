#include "cpositionloger.h"
#include <QDebug>

#include "cgpspositionprovider.h"
#include "cgpsdatastorage.h"
#include "chttpdatastorage.h"

CPositionLoger::CPositionLoger(QObject * a_pParent = 0)
    : m_pGpsPosProvider(0)
    , m_pLocalDataStorage(0)
    , m_pRemoteDataStorage(0)
{
    initMembers();

    if(0 != m_pGpsPosProvider)
    {
        connect(m_pGpsPosProvider, SIGNAL(positionChanged() ),
                this, SLOT(newPositionDataReady() ) );
    }
    else
    {
        qCritical() << "GPS position provider is not available";
    }
}

CPositionLoger::~CPositionLoger()
{
    deleteMembers();
}

void CPositionLoger::newPositionDataReady()
{
    // TO DO
}

void CPositionLoger::initMembers()
{
    m_pGpsPosProvider = new CGpsPositionProvider(this);
    m_pLocalDataStorage = new CGpsDataStorage(this);
    m_pRemoteDataStorage = new CHttpDataStorage(m_oHttpBrowser);
}

void CPositionLoger::deleteMembers()
{
    if(0 != m_pGpsPosProvider)
    {
        delete m_pGpsPosProvider;
        m_pGpsPosProvider = 0;
    }

    if(0 != m_pLocalDataStorage)
    {
        delete m_pLocalDataStorage;
        m_pLocalDataStorage = 0;
    }

    if(0 != m_pRemoteDataStorage)
    {
        delete m_pRemoteDataStorage;
        m_pRemoteDataStorage = 0;
    }
}

