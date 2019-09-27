/*
 *   Copyright (C) 2012 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library/Lesser General Public License
 *   version 2, or (at your option) any later version, as published by the
 *   Free Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library/Lesser General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 2.11
import QtQuick.Layouts 1.11

import org.kde.kirigami 2.0 as Kirigami

RowLayout
{
    id: view
    property bool editable: false
    property int max: 10
    property int rating: 0
    property real starSize: Kirigami.Units.gridUnit

    clip: true
    spacing: 0

    readonly property var ratingIndex: (theRepeater.count/view.max)*view.rating

    Repeater {
        id: theRepeater
        model: 5
        delegate: Kirigami.Icon {
            Layout.minimumWidth: view.starSize
            Layout.minimumHeight: view.starSize
            Layout.preferredWidth: view.starSize
            Layout.preferredHeight: view.starSize

            width: height
            source: "rating"
            opacity: (view.editable && mouse.item.containsMouse ? 0.7
                        : index>=view.ratingIndex ? 0.2
                        : 1)

            ConditionalLoader {
                id: mouse

                anchors.fill: parent
                condition: view.editable
                componentTrue: MouseArea {
                    hoverEnabled: true
                    onClicked: rating = (max/theRepeater.model*(index+1))
                }
                componentFalse: null
            }
        }
    }
}
