#include <QtCore>
#include <QtGui>
#include <QtSvg>
#include <QGraphicsRectItem>
/*程序中用到了svg格式的图片，所以需包含QtSvg*/
#define PAGE_COUNT 5
/*定义滑动窗体的窗体数*/


class NaviBar : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    NaviBar();
    void setPageOffset(qreal ofs);
signals:
    void pageSelected(int page);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
    QList<QGraphicsSvgItem*> m_icons;
    QGraphicsRectItem *m_cursor;
};
