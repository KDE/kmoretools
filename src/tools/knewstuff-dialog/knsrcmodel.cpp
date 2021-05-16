/*
    SPDX-FileCopyrightText: 2020 Dan Leinir Turthra Jensen <admin@leinir.dk>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "knsrcmodel.h"

#include "engine.h"

#include <KConfig>
#include <KConfigGroup>
#include <QDir>

KNSRCModel::KNSRCModel(QObject *parent)
    : QAbstractListModel(parent)
{
    const QStringList files = KNSCore::Engine::availableConfigFiles();
    for (const auto &file : files) {
        KConfig conf(file);
        KConfigGroup group;
        if (conf.hasGroup("KNewStuff3")) {
            group = conf.group("KNewStuff3");
        } else if (conf.hasGroup("KNewStuff2")) {
            group = conf.group("KNewStuff2");
        } else {
            qWarning() << file << " doesn't contain a KNewStuff3 (or KNewStuff2) section.";
            continue;
        }

        Entry *entry = new Entry;
        entry->name = group.readEntry("Name", QFileInfo(file).fileName());
        entry->filePath = file;
        m_entries << entry;
    }
}

KNSRCModel::~KNSRCModel() = default;

QHash<int, QByteArray> KNSRCModel::roleNames() const
{
    static const QHash<int, QByteArray> roleNames{{NameRole, "name"}, {FilePathRole, "filePath"}};
    return roleNames;
}

int KNSRCModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_entries.count();
}

QVariant KNSRCModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if (checkIndex(index)) {
        Entry *entry = m_entries[index.row()];
        switch (role) {
        case NameRole:
            result.setValue(entry->name);
            break;
        case FilePathRole:
            result.setValue(entry->filePath);
            break;
        default:
            break;
        }
    }
    return result;
}
