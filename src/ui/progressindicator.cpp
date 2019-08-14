/*
    This file is part of KNewStuff2.
    Copyright (c) 2007 Josef Spillner <spillner@kde.org>
    Copyright (c) 2007 Jeremy Whiting <jpwhiting@kde.org>
    Copyright (c) 2009-2010 Frederik Gladhorn <gladhorn@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "progressindicator_p.h"

#include <QPushButton>
#include <QLabel>
#include <QVariant>
#include <QHBoxLayout>

#include "kjob.h"

#include <kiconloader.h>
#include <kpixmapsequencewidget.h>

using namespace KNS3;

ProgressIndicator::ProgressIndicator(QWidget *parent)
    : QFrame(parent)
    , m_busyPixmap(KIconLoader::global()->loadPixmapSequence(QStringLiteral("process-working"), 22))
    , m_errorPixmap(KIconLoader::global()->loadPixmapSequence(QStringLiteral("dialog-error"), 22))
{
    setFrameStyle(QFrame::NoFrame);
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setContentsMargins(0, 0, 0, 0);

    //Busy widget
    busyWidget = new KPixmapSequenceWidget(this);
    busyWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    busyWidget->setVisible(false);
    hbox->addWidget(busyWidget);

    m_statusLabel = new QLabel();
    hbox->addWidget(m_statusLabel);
}

void ProgressIndicator::busy(const QString &message)
{
    m_statusLabel->setText(message);
    busyWidget->setVisible(true);
    busyWidget->setSequence(m_busyPixmap);
}

void KNS3::ProgressIndicator::error(const KNSCore::ErrorCode& errorCode, const QString& message, const QVariant& metadata)
{
    if(errorCode == KNSCore::OcsError && metadata.value<int>() == 405) {
        return;
    }
    m_statusLabel->setText(message);
    busyWidget->setVisible(true);
    busyWidget->setSequence(m_errorPixmap);
}

void ProgressIndicator::idle(const QString &message)
{
    m_statusLabel->setText(message);
    busyWidget->setVisible(false);
}

