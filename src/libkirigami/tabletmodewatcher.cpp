/*
 * Copyright 2018 Marco Martin <mart@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "tabletmodewatcher.h"
#include "tabletmodemanager_interface.h"

#ifdef Q_OS_ANDROID
#include <QDBusConnection>
#endif

//TODO: All the dbus stuff should be conditional, optional win32 support

namespace Kirigami {

class TabletModeWatcherSingleton
{
public:
    TabletModeWatcher self;
};

Q_GLOBAL_STATIC(TabletModeWatcherSingleton, privateTabletModeWatcherSelf)


class TabletModeWatcherPrivate
{
public:
    TabletModeWatcherPrivate(TabletModeWatcher *watcher)
        : q(watcher)
    {
#ifndef Q_OS_ANDROID
        m_interface = new OrgKdeKWinTabletModeManagerInterface(QStringLiteral("org.kde.KWin"), QStringLiteral("/org/kde/KWin"), QDBusConnection::sessionBus(), q);

        if (m_interface->isValid()) {
            //NOTE: the initial call is actually sync, because is better a tiny freeze than having the ui always recalculated and changed at the start
            isTabletMode = m_interface->tabletMode();
            QObject::connect(m_interface, &OrgKdeKWinTabletModeManagerInterface::tabletModeChanged,
                    q, [this](bool tabletMode) {
                setIsTablet(tabletMode);
            });
        } else {
            isTabletMode = false;
        }
#else
        isTabletMode = true;
#endif
    }
    ~TabletModeWatcherPrivate() {};
    void setIsTablet(bool tablet);

    TabletModeWatcher *q;
#ifndef Q_OS_ANDROID
    OrgKdeKWinTabletModeManagerInterface *m_interface;
#endif
    bool isTabletMode = false;
};

void TabletModeWatcherPrivate::setIsTablet(bool tablet)
{
    if (isTabletMode == tablet) {
        return;
    }

    isTabletMode = tablet;
    emit q->tabletModeChanged(tablet);
}



TabletModeWatcher::TabletModeWatcher(QObject *parent)
    : QObject(parent),
      d(new TabletModeWatcherPrivate(this))
{
}

TabletModeWatcher::~TabletModeWatcher()
{
    delete d;
}

TabletModeWatcher *TabletModeWatcher::self()
{
    return &privateTabletModeWatcherSelf()->self;
}

bool TabletModeWatcher::isTablet() const
{
    return d->isTabletMode;
}

}

#include "moc_tabletmodewatcher.cpp"
