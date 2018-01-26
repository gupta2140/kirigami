/*
 *   Copyright 2016 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "settings.h"

#include <QDebug>
#include <QStandardPaths>
#include <QSettings>
#include <QFile>

#include "libkirigami/tabletmodewatcher.h"

Settings::Settings(QObject *parent)
    : QObject(parent)
{
    m_mobile = Kirigami::TabletModeWatcher::self()->isTabletMode();
    connect(Kirigami::TabletModeWatcher::self(), &Kirigami::TabletModeWatcher::tabletModeChanged,
            this, [this](bool tabletMode) {
                setIsMobile(tabletMode);
            });

    const QString configPath = QStandardPaths::locate(QStandardPaths::ConfigLocation, QStringLiteral("kdeglobals"));
    if (QFile::exists(configPath)) {
        QSettings globals(configPath, QSettings::IniFormat);
        globals.beginGroup(QStringLiteral("KDE"));
        m_scrollLines = qMax(1, globals.value(QStringLiteral("WheelScrollLines"), 3).toInt());
    } else {
        m_scrollLines = 3;
    }
}


Settings::~Settings()
{
}

void Settings::setIsMobile(bool mobile)
{
    if (mobile == m_mobile) {
        return;
    }

    m_mobile = mobile;
    emit isMobileChanged();
}

bool Settings::isMobile() const
{
    return m_mobile;
}

QString Settings::style() const
{
    return m_style;
}

void Settings::setStyle(const QString &style)
{
    m_style = style;
}

int Settings::mouseWheelScrollLines() const
{
    return m_scrollLines;
}

#include "moc_settings.cpp"

