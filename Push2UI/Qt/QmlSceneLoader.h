#ifndef QML_SCENE_LOADER_H
#define QML_SCENE_LOADER_H

#include <QObject>
#include <QString>

class QuickViewOnFpRenderMedium;

namespace push2
{

class QmlSceneLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString qmlFile READ qmlFile NOTIFY qmlFileChanged)
public:
    QmlSceneLoader(QuickViewOnFpRenderMedium &rQuickViewOnFpRenderMedium,
                   QObject                    *parent = nullptr);
    QString qmlFile() const;
    void loadQmlFile(const QString& qmlFile);
    void setContextProperty(const QString& propertyName, QObject* pObject);
signals:
    void qmlFileChanged();
private:
    QuickViewOnFpRenderMedium &m_rQuickViewOnFpRenderMedium;
    QString m_qmlFile;
};

} // namespace push2


#endif