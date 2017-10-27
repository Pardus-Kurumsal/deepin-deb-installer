/**
 * Copyright (C) 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef MULTIPLEINSTALLPAGE_H
#define MULTIPLEINSTALLPAGE_H

#include "infocontrolbutton.h"

#include <QWidget>

#include <QPushButton>
#include <QProgressBar>
#include <QTextEdit>
#include <QPropertyAnimation>

class PackagesListView;
class DebListModel;
class MultipleInstallPage : public QWidget
{
    Q_OBJECT

public:
    explicit MultipleInstallPage(DebListModel *model, QWidget *parent = 0);

signals:
    void back() const;
    void requestRemovePackage(const int index) const;

private slots:
    void onWorkerFinshed();
    void onOutputAvailable(const QString &output);
    void onProgressChanged(const int progress);
    void onItemClicked(const QModelIndex &index);

    void showInfo();
    void hideInfo();

private:
    DebListModel *m_debListModel;
    PackagesListView *m_appsView;
    QTextEdit *m_infoArea;
    InfoControlButton *m_infoControlButton;
    QProgressBar *m_installProgress;
    QPropertyAnimation *m_progressAnimation;
    QPushButton *m_installButton;
    QPushButton *m_acceptButton;
    QPushButton *m_backButton;
};

#endif // MULTIPLEINSTALLPAGE_H
