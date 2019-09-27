/*
 *   Copyright 2018 Aleix Pol Gonzalez <aleixpol@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 2.11
import QtGraphicalEffects 1.11

import org.kde.kirigami 2.2

LinearGradient {
    id: shadow
    property int edge: Qt.LeftEdge

    width: Units.gridUnit/2
    height: Units.gridUnit/2

    start: Qt.point((edge !== Qt.RightEdge ? 0 : width), (edge !== Qt.BottomEdge ? 0 : height))
    end: Qt.point((edge !== Qt.LeftEdge ? 0 : width), (edge !== Qt.TopEdge ? 0 : height))
    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: Theme.backgroundColor
        }
        GradientStop {
            position: 0.3
            color: Qt.rgba(0, 0, 0, 0.1)
        }
        GradientStop {
            position: 1.0
            color:  "transparent"
        }
    }
}

