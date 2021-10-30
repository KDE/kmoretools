/*
    SPDX-FileCopyrightText: 2010 Frederik Gladhorn <gladhorn@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KNEWSTUFF3_UI_DownloadManager_H
#define KNEWSTUFF3_UI_DownloadManager_H

#include "entry.h"
#include "knewstuff_export.h"

#if KNEWSTUFF_ENABLE_DEPRECATED_SINCE(5, 29)

namespace KNS3
{
class DownloadManagerPrivate;
/**
 * KNewStuff update checker.
 * This class can be used to search for KNewStuff items
 * without using the widgets and to look for updates of
 * already installed items without showing the dialog.
 * @since 4.5
 * @deprecated Since 5.29, use KNSCore::DownloadManager instead
 */
class KNEWSTUFF_EXPORT DownloadManager : public QObject
{
    Q_OBJECT

public:
    enum SortOrder {
        Newest,
        Alphabetical,
        Rating,
        Downloads,
    };

    /**
     * Create a DownloadManager
     * It will try to find a appname.knsrc file.
     * Appname is the name of your application as provided in the about data->
     *
     * @param parent the parent of the dialog
     * @deprecated Since 5.29, use KNSCore::DownloadManager instead
     */
    KNEWSTUFF_DEPRECATED_VERSION(5, 29, "Use KNSCore::DownloadManager")
    explicit DownloadManager(QObject *parent = nullptr);

    /**
     * Create a DownloadManager. Manually specifying the name of the .knsrc file.
     *
     * @param configFile the name of the configuration file
     * @param parent
     * @deprecated Since 5.29, use KNSCore::DownloadManager instead
     */
    KNEWSTUFF_DEPRECATED_VERSION(5, 29, "Use KNSCore::DownloadManager")
    explicit DownloadManager(const QString &configFile, QObject *parent = nullptr);

    /**
     * destructor
     */
    ~DownloadManager() override;

    /**
      Search for a list of entries. searchResult will be emitted with the requested list.
    */
    void search(int page = 0, int pageSize = 100);

    /**
      Check for available updates.
      Use searchResult to get notified as soon as an update has been found.
      */
    void checkForUpdates();

    /**
      Check for installed resources
      Use searchResult to get notified about installed entries.

      @since 5.28
      */
    void checkForInstalled();

    /**
      Installs or updates an entry
      @param entry
      */
    void installEntry(const KNS3::Entry &entry);

    /**
     * Uninstalls the given entry.
     * @param entry The entry which will be uninstalled.
     * @since 4.7
     */
    void uninstallEntry(const KNS3::Entry &entry);

    /**
      Sets the search term to filter the results on the server.
      Note that this function does not trigger a search. Use search after setting this.
      @param searchTerm
      */
    void setSearchTerm(const QString &searchTerm);

    /**
      Set the sort order of the results. This depends on the server.
      Note that this function does not trigger a search. Use search after setting this.
      @see SortOrder
      @param order
      */
    void setSearchOrder(SortOrder order);

    /**
     * Triggers a search for an entry with @p id as its unique id
     *
     * @see searchResult
     *
     * @since 5.28
     */
    void fetchEntryById(const QString &id);

Q_SIGNALS:
    /**
      Returns the search result.
      This can be the list of updates after checkForUpdates or the result of a search.
      @param entries the list of results. entries is empty when nothing was found.
     */
    void searchResult(const KNS3::Entry::List &entries);

    /**
      The entry status has changed: emitted when the entry has been installed, updated or removed.
      Use KNS3::Entry::status() to check the current status.
      @param entry the item that has been updated.
     */
    void entryStatusChanged(const KNS3::Entry &entry);

    /**
     * Notifies that the engine couldn't be loaded properly and won't be suitable
     */
    void errorFound(const QString &errorMessage);

private:
    Q_PRIVATE_SLOT(d, void _k_slotProvidersLoaded())
    Q_PRIVATE_SLOT(d, void _k_slotEngineError(const QString &error))
    Q_PRIVATE_SLOT(d, void _k_slotEntryStatusChanged(const KNSCore::EntryInternal &entry))
    Q_PRIVATE_SLOT(d, void _k_slotEntriesLoaded(const KNSCore::EntryInternal::List &entries))
    KNS3::DownloadManagerPrivate *const d;
    Q_DISABLE_COPY(DownloadManager)
};

}

#endif // KNEWSTUFF_ENABLE_DEPRECATED_SINCE(5, 29)

#endif
