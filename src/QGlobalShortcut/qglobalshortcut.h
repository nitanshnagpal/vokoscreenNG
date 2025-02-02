#ifndef QGLOBALSHORTCUTPRIVATE_H
#define QGLOBALSHORTCUTPRIVATE_H

#include <QObject>
#include <QAbstractNativeEventFilter>

class QKeySequence;
class QGlobalShortcutPrivate;

class QGlobalShortcut : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit QGlobalShortcut(QObject *parent = nullptr);
    ~QGlobalShortcut();

#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
    // QT5
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;
#else
    // Qt6
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;
#endif

    QKeySequence shortcut();
    bool isEmpty();
    bool isEnabled();

signals:
    void activated();

public slots:
    bool setShortcut(const QKeySequence &keySequence);
    bool unsetShortcut();
    void setEnabled(bool enable);

private:
    QGlobalShortcutPrivate *sPrivate;
};

#endif // QGLOBALSHORTCUTPRIVATE_H
