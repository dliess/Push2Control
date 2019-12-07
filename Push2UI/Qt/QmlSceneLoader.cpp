#include "QmlSceneLoader.h"
#include "QuickViewOnFpRenderMedium.h"
#include <QQmlContext>

push2::QmlSceneLoader::QmlSceneLoader(QuickViewOnFpRenderMedium &rQuickViewOnFpRenderMedium,
                                      QObject                    *parent) :
    QObject(parent),
    m_rQuickViewOnFpRenderMedium(rQuickViewOnFpRenderMedium)
{
    m_rQuickViewOnFpRenderMedium.fboQuickView.rootContext()->setContextProperty("qmlSceneLoader", this);
    m_rQuickViewOnFpRenderMedium.setSource("qrc:///qmls/main.qml");
}

QString push2::QmlSceneLoader::qmlFile() const
{
    return m_qmlFile;
}

void push2::QmlSceneLoader::loadQmlFile(const QString& qmlFile)
{
    if(m_qmlFile != qmlFile)
    {
        m_qmlFile = qmlFile;
        emit qmlFileChanged();
    }
}

void push2::QmlSceneLoader::setContextProperty(const QString& propertyName, QObject* pObject)
{
    m_rQuickViewOnFpRenderMedium.fboQuickView.rootContext()->setContextProperty(propertyName, pObject);
}
