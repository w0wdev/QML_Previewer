#include "util.h"
#include <QPixmap>
#include <QPixmapCache>
#include <QDebug>

QMap<QString, QColor> Util::mAppColors = {
    {"lightestBackground", QColor(80,80,80)},
    {"lightBackground", QColor(66,66,66)},
    {"normalBackground", QColor(48,48,48)},
    {"darkBackground", QColor(39,39,39)},
    {"normalText", QColor(180,180,180)},
    {"lightText", QColor(220,220,220)},
    {"disabledText", QColor(127,127,127)},
    {"lightAccent", QColor(129,212,250)},
    {"darkAccent", QColor(71,117,137)},
    {"pink", QColor(219,98,139)},
    {"red", QColor(200,52,52)},
    {"orange", QColor(206,125,44)},
    {"yellow", QColor(210,210,127)},
    {"green", QColor(127,200,127)},
    {"cyan", QColor(79,203,203)},
    {"blue", QColor(77,127,196)},
    {"magenta", QColor(157,127,210)},
    {"white", QColor(255,255,255)},
    {"black", QColor(0,0,0)},
    {"plot_graph1", QColor(77,127,196)},
    {"plot_graph2", QColor(200,52,52)},
    {"plot_graph3", QColor(127,200,127)},
    {"plot_graph4", QColor(206,125,44)},
    {"plot_graph5", QColor(210,210,127)},
    {"plot_graph6", QColor(79,203,203)},
    {"plot_graph7", QColor(157,127,210)},
    {"plot_graph8", QColor(129,212,250)},
    {"plot_graph9", QColor(180,180,180)},
    {"plot_graph10", QColor(219,98,139)},
    {"plot_graph11", QColor(250,250,200)},
};

Util::Util(QObject* parent)
    : QObject{ parent }
{
}

QString Util::getThemePath()
{
    return ":/res/";
}

QPixmap Util::getIcon(QString path)
{
    while (path.startsWith("/"))
        path.remove(0, 1);

    QPixmap pm;
    if (!QPixmapCache::find(path, &pm)) {
        QString fileName = getThemePath() + path;
        pm.load(getThemePath() + path);
        QPixmapCache::insert(path, pm);
    }

    return pm;
}

QColor Util::getAppQColor(QString colorName)
{
    if (mAppColors.contains(colorName))
        return mAppColors[colorName];

    qDebug() << colorName << " not found in standard colors";
    return Qt::red;
}
