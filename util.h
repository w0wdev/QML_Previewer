
#include <QObject>
#include <QMap>
#include <qcolor>
#include <qstring>

class Util : public QObject
{
    Q_OBJECT

public:
    explicit Util(QObject* parent = nullptr);

    static QString getThemePath();
    static QPixmap getIcon(QString path);

    static QColor getAppQColor(QString colorName);

signals:

public slots:

private:
    static QMap<QString, QColor> mAppColors;

};
