#ifndef CPOSITIONLOGER_H
#define CPOSITIONLOGER_H

#include <QObject>
#include <chttpbrowsersync.h>

class ILocalDataStorage;
class IRemoteDataStorage;
class CGpsPositionProvider;

class CPositionLoger : public QObject
{
    Q_OBJECT

public:
    explicit CPositionLoger(QObject * a_pParent = 0);
    ~CPositionLoger();

private slots:
    void newPositionDataReady();

private:
    void initMembers();
    void deleteMembers();

    CGpsPositionProvider * m_pGpsPosProvider;
    ILocalDataStorage * m_pLocalDataStorage;
    IRemoteDataStorage * m_pRemoteDataStorage;

    CHttpBrowserSync m_oHttpBrowser;
};

#endif // CPOSITIONLOGER_H
