#include "parallaxhome.h"


ParallaxHome::ParallaxHome(QWidget *parent): QGraphicsView(parent) , m_pageOffset(-2)
{
    setupScene();
    setScene(&m_scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setWindowTitle("Parallax Home");
    connect(&m_pageAnimator, SIGNAL(frameChanged(int)), SLOT(shiftPage(int)));
    m_pageAnimator.setDuration(500);
    m_pageAnimator.setFrameRange(0, 100);
    m_pageAnimator.setCurveShape(QTimeLine::EaseInCurve);
    pageChanged(static_cast<int>(m_pageOffset));
}


void ParallaxHome::slideRight()
{
        if (m_pageAnimator.state() != QTimeLine::NotRunning)
            return;
        int edge = -(m_pageOffset - 1);
        if (edge < PAGE_COUNT)
            slideBy(-1);
}
void ParallaxHome::slideLeft()
{
        if (m_pageAnimator.state() != QTimeLine::NotRunning)
            return;
        if (m_pageOffset < 0)
            slideBy(1);
}
void ParallaxHome::slideBy(int dx)
{
        int start = m_pageOffset * 1000;
        int end = (m_pageOffset + dx) * 1000;
        m_pageAnimator.setFrameRange(start, end);
        m_pageAnimator.start();
    }
void ParallaxHome::choosePage(int page)
{
        if (m_pageAnimator.state() != QTimeLine::NotRunning)
            return;
        if (static_cast<int>(-m_pageOffset) == page)
            return;
        slideBy(-page - m_pageOffset);
}

void ParallaxHome::shiftPage(int frame)
{
        int ww = width();
        int hh = height() - m_naviBar->rect().height();
        int oldPage = static_cast<int>(-m_pageOffset);
        m_pageOffset = static_cast<qreal>(frame) / qreal(1000);
        int newPage = static_cast<int>(-m_pageOffset);
        m_naviBar->setPageOffset(-m_pageOffset);
        if (oldPage != newPage)
            emit pageChanged(newPage);
        int ofs = m_pageOffset * ww;
        for (int i = 0; i < m_items.count(); ++i) {
            QPointF pos = m_positions[i];
            QPointF xy(pos.x() * ww, pos.y() * hh);
            m_items[i]->setPos(xy + QPointF(ofs, 0));
        }
        int center = m_wallpaper->pixmap().width() / 2;
        const int parallax = 3;
        int base = center - (ww / 2) - (PAGE_COUNT >> 1) * (ww / parallax);
        int wofs = base - m_pageOffset * ww / parallax;
        m_wallpaper->setPos(-wofs, 0);
}

void ParallaxHome::resizeEvent(QResizeEvent *event)
{
        Q_UNUSED(event);
        layoutScene();
}
void ParallaxHome::keyPressEvent(QKeyEvent *event)
{
        if (event->key() == Qt::Key_Right)
            slideRight();
        if (event->key() == Qt::Key_Left)
            slideLeft();
        event->accept();
}

void ParallaxHome::layoutScene()
{
        int ww = width();
        int hh = height();
        m_scene.setSceneRect(0, 0, PAGE_COUNT * ww - 1, hh - 1);
        centerOn(ww / 2, hh / 2);
        int nw = m_naviBar->rect().width();
        int nh = m_naviBar->rect().height();
        m_naviBar->setPos((ww - nw) / 2, hh - nh);
        shiftPage(m_pageOffset * 1000);
}
void ParallaxHome::setupScene()
{
        qsrand(QTime::currentTime().second());
        QStringList names;
        names << "2238_bg" << "2238_blue_normal" << "2238_blue_passed" << "2238_blue_selected" << "2238_bw_normal";
        names << "2238_bw_passed" << "2238_bw_selected" << "2238_clock" << "2238_file_passed" << "2238_green_selected";
        for (int i = 0; i < PAGE_COUNT * 2; ++i) {
            QString fname = names[i];
            //fname.prepend(":/images/");
            //fname.append(".jpg");
            fname.prepend(":/image/icon/");
            fname.append(".png");

            QPixmap pixmap(fname);
            pixmap = pixmap.scaledToWidth(200);
            QGraphicsPixmapItem *item = m_scene.addPixmap(pixmap);
            m_items << item;
            qreal x = (i >> 1) + (qrand() % 30) / 100.0;
            qreal y = (i & 1) / 2.0  + (qrand() % 20) / 100.0;
            m_positions << QPointF(x, y);
            item->setZValue(1);
        }
        m_naviBar = new NaviBar;
        m_scene.addItem(m_naviBar);
        m_naviBar->setZValue(2);
        connect(m_naviBar, SIGNAL(pageSelected(int)), SLOT(choosePage(int)));
        m_wallpaper = m_scene.addPixmap(QPixmap(":/icons/surfacing.png"));
        m_wallpaper->setZValue(0);
        m_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
}



