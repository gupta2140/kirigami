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

#ifndef KIRIGAMI_TABLETMODEWATCHER
#define KIRIGAMI_TABLETMODEWATCHER

#include <QObject>

#ifndef KIRIGAMI_BUILD_TYPE_STATIC
#include <kirigami2_export.h>
#endif

namespace Kirigami {

class TabletModeWatcherPrivate;

#ifdef KIRIGAMI_BUILD_TYPE_STATIC
class TabletModeWatcher : public QObject
#else
class KIRIGAMI2_EXPORT TabletModeWatcher : public QObject
#endif
{
    Q_OBJECT
    Q_PROPERTY(bool tabletMode READ isTablet NOTIFY tabletModeChanged)

public:
    ~TabletModeWatcher();
    static TabletModeWatcher *self();

    bool isTablet() const;

Q_SIGNALS:
    void tabletModeChanged(bool tabletMode);

private:
    TabletModeWatcher(QObject *parent = 0);
    TabletModeWatcherPrivate *d;
    friend class TabletModeWatcherSingleton;
};
}

#endif // KIRIGAMI_TABLETMODEWATCHER
