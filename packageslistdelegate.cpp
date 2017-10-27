/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "packageslistdelegate.h"
#include "deblistmodel.h"

#include <QPainter>

PackagesListDelegate::PackagesListDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{
    const QIcon icon = QIcon::fromTheme("application-vnd.debian.binary-package", QIcon::fromTheme("debian-swirl"));
    m_packageIcon = icon.pixmap(32, 32);
    m_removeIcon = QPixmap(":/images/active_tab_close_normal.png");
}

void PackagesListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    painter->fillRect(option.rect, Qt::gray);

    const int content_x = 45;

    // draw top border
    if (index.row())
    {
        const QPoint start(content_x, option.rect.top());
        const QPoint end(option.rect.right() - 10, option.rect.top());

        painter->setPen(QColor(0, 0, 0, 255 * .05));
        painter->setRenderHint(QPainter::Antialiasing, false);
        painter->drawLine(start, end);
    }

    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    // draw package icon
    const int x = 5;
    const int y = option.rect.top() + (option.rect.height() - m_packageIcon.height()) / 2;
    painter->drawPixmap(x, y, m_packageIcon);

    // draw package name
    QRect name_rect = option.rect;
    name_rect.setLeft(content_x);
    name_rect.setHeight(name_rect.height() / 2);

    const QString name = index.data(DebListModel::PackageNameRole).toString();
    const QFont old_font = painter->font();
    QFont f = old_font;
    f.setWeight(QFont::DemiBold);
    painter->setFont(f);
    const QString name_str = painter->fontMetrics().elidedText(name, Qt::ElideRight, 306);
    const QRectF name_bounding_rect = painter->boundingRect(name_rect, name_str, Qt::AlignLeft | Qt::AlignBottom);
    painter->setPen(Qt::black);
    painter->drawText(name_rect, name_str, Qt::AlignLeft | Qt::AlignBottom);
    painter->setFont(old_font);

    // draw package version
    const int version_x = name_bounding_rect.right() + 8;
    QRect version_rect = name_rect;
    version_rect.setLeft(version_x);
    version_rect.setRight(option.rect.right() - 85);
    const QString version = index.data(DebListModel::PackageVersionRole).toString();
    const QString version_str = painter->fontMetrics().elidedText(version, Qt::ElideRight, version_rect.width());
    painter->drawText(version_rect, version_str, Qt::AlignLeft | Qt::AlignBottom);

    // install status
    const int operate_stat = index.data(DebListModel::PackageOperateStatusRole).toInt();
    if (operate_stat != DebListModel::Prepare)
    {
        QRect install_status_rect = option.rect;
        install_status_rect.setRight(option.rect.right() - 15);
        install_status_rect.setLeft(option.rect.right() - 80);

        switch (operate_stat)
        {
        case DebListModel::Operating:
            painter->setPen(QColor(124, 124, 124));
            painter->drawText(install_status_rect, tr("Installing"), Qt::AlignVCenter | Qt::AlignRight);
            break;
        case DebListModel::Success:
            painter->setPen(QColor(65, 117, 5));
            painter->drawText(install_status_rect, tr("Installed"), Qt::AlignVCenter | Qt::AlignRight);
            break;
        default:
            painter->setPen(QColor(255, 109, 109));
            painter->drawText(install_status_rect, tr("Failed"), Qt::AlignVCenter | Qt::AlignRight);
            break;
        }
    } else if (index.data(DebListModel::WorkerIsPrepareRole).toBool() &&
               index.data(DebListModel::ItemIsCurrentRole).toBool()) {
        // draw remove icon
        const int x = option.rect.right() - m_removeIcon.width() - 10;
        const int y = option.rect.top() + (option.rect.height() - m_removeIcon.height()) / 2;
        painter->drawPixmap(x, y, m_removeIcon);
    }

    // draw package info
    QString info_str;
    QRect info_rect = option.rect;
    info_rect.setLeft(content_x);
    info_rect.setTop(name_rect.bottom() + 1 + 3);

    const int install_stat = index.data(DebListModel::PackageVersionStatusRole).toInt();
    if (operate_stat == DebListModel::Failed)
    {
        info_str = index.data(DebListModel::PackageFailReasonRole).toString();
        painter->setPen(QColor(255, 109, 109));
    } else if (install_stat != DebListModel::NotInstalled) {
        if (install_stat == DebListModel::InstalledSameVersion)
        {
            info_str = tr("Same version installed");
            painter->setPen(QColor(65, 117, 5));
        } else {
            info_str = tr("Other version installed: %1").arg(index.data(DebListModel::PackageInstalledVersionRole).toString());
            painter->setPen(QColor(255, 109, 109));
        }
    } else {
        info_str = index.data(DebListModel::PackageDescriptionRole).toString();
        painter->setPen(QColor(90, 90, 90));
    }

    info_str = painter->fontMetrics().elidedText(info_str, Qt::ElideRight, 306);
    painter->drawText(info_rect, info_str, Qt::AlignLeft | Qt::AlignTop);
}

QSize PackagesListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    return index.data(Qt::SizeHintRole).toSize();
}
