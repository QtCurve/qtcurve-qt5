/***************************************************************************
 *   Copyright (C) 2013~2013 by Yichao Yu                                  *
 *   yyc1992@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/

#ifndef __QTCURVE_P_H__
#define __QTCURVE_P_H__

#include "qtcurve.h"
#include <QPainter>
#include <QPushButton>
#include <QToolButton>
#include <QWidget>
#include <QSplitter>
#include <QStatusBar>

class QToolBar;

namespace QtCurve {

extern QString appName;
static const int constMenuPixmapWidth = 22;
static const int constWindowMargin   =  2;
static const int constProgressBarFps = 20;
static const int constTabPad         =  6;

static const QLatin1String constDwtClose("qt_dockwidget_closebutton");
static const QLatin1String constDwtFloat("qt_dockwidget_floatbutton");

// WebKit seems to just use the values from ::pixelMetric to get button sizes.
// So, in pixelMetric we add some extra padding to PM_ButtonMargin
// if we're max rounding - this gives a nicer border. However, dont want
// this on real buttons - so in sizeFromContents we remove this padding
// in CT_PushButton and CT_ComboBox
#define MAX_ROUND_BTN_PAD (ROUND_MAX==opts.round ? 3 : 0)

#define SB_SUB2 ((QStyle::SubControl)(QStyle::SC_ScrollBarGroove << 1))

#define MO_ARROW_X(MO, COL) (state & State_Enabled ?                    \
                             (MO_NONE != opts.coloredMouseOver && (MO) ? \
                              itsMouseOverCols[ARROW_MO_SHADE] :        \
                              palette.color(COL)) :                     \
                             palette.color(QPalette::Disabled, COL))

#define MO_ARROW(COL) MO_ARROW_X(state & State_MouseOver, COL)

#define WINDOWTITLE_SPACER 0x10000000
#define STATE_REVERSE QStyle::StateFlag(0x10000000)
#define STATE_MENU QStyle::StateFlag(0x20000000)
#define STATE_VIEW QStyle::StateFlag(0x40000000)
#define STATE_KWIN_BUTTON QStyle::StateFlag(0x40000000)
#define STATE_TBAR_BUTTON QStyle::StateFlag(0x80000000)
#define STATE_DWT_BUTTON QStyle::StateFlag(0x20000000)
#define STATE_TOGGLE_BUTTON QStyle::StateFlag(0x10000000)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define PIXMAP_DIMENSION 10

// TODO! REMOVE THIS WHEN KDE'S ICON SETTINGS ACTUALLY WORK!!!
#define FIX_DISABLED_ICONS

typedef enum {
    APP_PLASMA,
    APP_KRUNNER,
    APP_KWIN,
    APP_SYSTEMSETTINGS,
    APP_SKYPE,
    APP_KONQUEROR,
    APP_KONTACT,
    APP_ARORA,
    APP_REKONQ,
    APP_OPERA,
    APP_QTDESIGNER,
    APP_QTCREATOR,
    APP_KDEVELOP,
    APP_K3B,
    APP_OPENOFFICE,
    APP_KONSOLE,
    APP_OTHER
} QtcThemedApp;

typedef enum {
    windowsItemFrame      =  2, // menu item frame width
    windowsSepHeight      =  9, // separator item height
    windowsItemHMargin    =  3, // menu item hor text margin
    windowsItemVMargin    =  2, // menu item ver text margin
    windowsRightBorder    = 15, // right border on windows
    windowsCheckMarkWidth = 12, // checkmarks width on windows
    windowsArrowHMargin   =  6  // arrow horizontal margin
} WindowsStyleConsts;

extern QtcThemedApp theThemedApp;
extern QSet<const QWidget*> theNoEtchWidgets;

static inline bool
isOOWidget(const QWidget *widget)
{
    return APP_OPENOFFICE == theThemedApp && !widget;
}

bool blendOOMenuHighlight(const QPalette &pal, const QColor &highlight);
bool isNoEtchWidget(const QWidget *widget);

#ifdef QTC_ENABLE_X11
static inline bool canAccessId(const QWidget *w)
{
    return w && w->testAttribute(Qt::WA_WState_Created) && w->internalWinId();
}
void setOpacityProp(QWidget *w, unsigned short opacity);
void setBgndProp(QWidget *w, unsigned short app, bool haveBgndImage);
void setSbProp(QWidget *w);

#endif

static inline void
setTranslucentBackground(QWidget *widget)
{
    widget->setAttribute(Qt::WA_TranslucentBackground);
#ifdef Q_OS_WIN
    // FramelessWindowHint is needed on windows to make
    // WA_TranslucentBackground work properly
    widget->setWindowFlags(widget->windowFlags() | Qt::FramelessWindowHint);
#endif
}

static inline QList<QStatusBar*>
getStatusBars(QWidget *w)
{
    return w ? w->findChildren<QStatusBar*>() : QList<QStatusBar*>();
}
#if defined FIX_DISABLED_ICONS && defined QTC_QT5_ENABLE_KDE
static inline QPixmap
getIconPixmap(const QIcon &icon, const QSize &size,
              QIcon::Mode mode, QIcon::State)
{
    QPixmap pix=icon.pixmap(size, QIcon::Normal);

    if (QIcon::Disabled == mode) {
        QImage img = pix.toImage();
        KIconEffect::toGray(img, 1.0);
        KIconEffect::semiTransparent(img);
        pix = QPixmap::fromImage(img);
    }
    return pix;
}
#else
static inline QPixmap
getIconPixmap(const QIcon &icon, const QSize &size, QIcon::Mode mode,
              QIcon::State state=QIcon::Off)
{
    return icon.pixmap(size, mode, state);
}
#endif

static inline QPixmap
getIconPixmap(const QIcon &icon, int size, QIcon::Mode mode,
              QIcon::State state=QIcon::Off)
{
    return getIconPixmap(icon, QSize(size, size), mode, state);
}

static inline QPixmap
getIconPixmap(const QIcon &icon, int size, int flags,
              QIcon::State state=QIcon::Off)
{
    return getIconPixmap(icon, QSize(size, size),
                         flags & QStyle::State_Enabled ? QIcon::Normal :
                         QIcon::Disabled, state);
}

static inline QPixmap
getIconPixmap(const QIcon &icon, const QSize &size, int flags,
              QIcon::State state=QIcon::Off)
{
    return getIconPixmap(icon, size, flags & QStyle::State_Enabled ?
                         QIcon::Normal : QIcon::Disabled, state);
}

static inline int numButtons(EScrollbar type)
{
    switch(type)
    {
    default:
    case SCROLLBAR_KDE:
        return 3;
        break;
    case SCROLLBAR_WINDOWS:
    case SCROLLBAR_PLATINUM:
    case SCROLLBAR_NEXT:
        return 2;
        break;
    case SCROLLBAR_NONE:
        return 0;
    }
}

static inline void drawRect(QPainter *p, const QRect &r)
{
    p->drawRect(r.x(), r.y(), r.width()-1, r.height()-1);
}

static inline void drawAaLine(QPainter *p, int x1, int y1, int x2, int y2)
{
    p->drawLine(QLineF(x1+0.5, y1+0.5, x2+0.5, y2+0.5));
}

static inline void drawAaPoint(QPainter *p, int x, int y)
{
    p->drawPoint(QPointF(x+0.5, y+0.5));
}

static inline void drawAaRect(QPainter *p, const QRect &r)
{
    p->drawRect(QRectF(r.x()+0.5, r.y()+0.5, r.width()-1, r.height()-1));
}

static inline bool isMultiTabBarTab(const QAbstractButton *button)
{
    // Check for isFlat fails in KDE SC4.5
    return button && ((::qobject_cast<const QPushButton*>(button) &&
                       // ((QPushButton *)button)->isFlat() &&
                       button->inherits("KMultiTabBarTab")) ||
                      (APP_KDEVELOP == theThemedApp &&
                       ::qobject_cast<const QToolButton *>(button) &&
                       button->inherits("Sublime::IdealToolButton")));
}

QWidget *getParent(QWidget *w, int level);
#ifndef QTC_QT5_ENABLE_KDE
bool parentIs(QWidget *w, int level, const char *className);
#endif

static inline int
toHint(int sc)
{
    switch (sc) {
    case QStyle::SC_TitleBarSysMenu:
        return Qt::WindowSystemMenuHint;
    case QStyle::SC_TitleBarMinButton:
        return Qt::WindowMinimizeButtonHint;
    case QStyle::SC_TitleBarMaxButton:
        return Qt::WindowMaximizeButtonHint;
    case QStyle::SC_TitleBarCloseButton:
        return 0;
    case QStyle::SC_TitleBarNormalButton:
        return 0;
    case QStyle::SC_TitleBarShadeButton:
    case QStyle::SC_TitleBarUnshadeButton:
        return Qt::WindowShadeButtonHint;
    case QStyle::SC_TitleBarContextHelpButton:
        return Qt::WindowContextHelpButtonHint;
    default:
        return 0;
    }
}

void setBold(QWidget *widget);
void unSetBold(QWidget *widget);
QWidget *scrollViewFrame(QWidget *widget);

class QtCurveDockWidgetTitleBar : public QWidget {
public:
    QtCurveDockWidgetTitleBar(QWidget* parent) : QWidget(parent) {}
    virtual ~QtCurveDockWidgetTitleBar() {}
    QSize sizeHint() const {
        return QSize(0, 0);
    }
};

QToolBar *getToolBarChild(QWidget *w);
void setStyleRecursive(QWidget *w, QStyle *s, int minSize);
bool updateMenuBarEvent(QMouseEvent *event, QMenuBar *menu);
QRegion windowMask(const QRect &r, bool full);
const QWidget *getWidget(const QPainter *p);
const QImage *getImage(const QPainter *p);
const QAbstractButton *getButton(const QWidget *w, const QPainter *p);

static inline bool
isKateView(const QWidget *widget)
{
    return (widget && widget->parentWidget() &&
            ::qobject_cast<const QFrame*>(widget) &&
            widget->parentWidget()->inherits("KateView"));
}

static inline bool
isKontactPreviewPane(const QWidget *widget)
{
    return (APP_KONTACT == theThemedApp &&
            widget && widget->parentWidget() &&
            widget->parentWidget()->parentWidget() &&
            widget->inherits("KHBox") &&
            ::qobject_cast<const QSplitter*>(widget->parentWidget()) &&
            widget->parentWidget()->parentWidget()->inherits("KMReaderWin"));
}

static inline QColor
blendColors(const QColor &foreground, const QColor &background, double alpha)
{
#ifndef QTC_QT5_ENABLE_KDE
    return ColorUtils_mix(&background, &foreground, alpha);
#else
    return KColorUtils::mix(background, foreground, alpha);
#endif
}

void drawDots(QPainter *p, const QRect &r, bool horiz, int nLines, int offset,
              const QColor *cols, int startOffset, int dark);
bool isInQAbstractItemView(const QObject *w);
const QToolBar *getToolBar(const QWidget *w);
void drawTbArrow(const QStyle *style, const QStyleOptionToolButton *toolbutton,
                 const QRect &rect, QPainter *painter, const QWidget *widget=0);
void adjustToolbarButtons(const QWidget *widget, const QToolBar *toolbar,
                          int &leftAdjust, int &topAdjust, int &rightAdjust,
                          int &bottomAdjust, int &round);
bool isA(const QObject *w, const char *type);

}

#endif
