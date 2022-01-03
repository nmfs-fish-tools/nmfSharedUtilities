/**
 * @file nmfMessageWithDontShowDialog.h
 * @brief Definition for a message box that has a "don't show this message box again" checkbox
 * @date Feb 16, 2018
 *
 * This file contains the definition for a message box widget that ask the user if they want
 * to suppress future such message boxes.
 *
 * @copyright
 * Public Domain Notice\n
 *
 * National Oceanic And Atmospheric Administration\n\n
 *
 * This software is a "United States Government Work" under the terms of the
 * United States Copyright Act.  It was written as part of the author's official
 * duties as a United States Government employee/contractor and thus cannot be copyrighted.
 * This software is freely available to the public for use. The National Oceanic
 * And Atmospheric Administration and the U.S. Government have not placed any
 * restriction on its use or reproduction.  Although all reasonable efforts have
 * been taken to ensure the accuracy and reliability of the software and data,
 * the National Oceanic And Atmospheric Administration and the U.S. Government
 * do not and cannot warrant the performance or results that may be obtained
 * by using this software or data. The National Oceanic And Atmospheric
 * Administration and the U.S. Government disclaim all warranties, express
 * or implied, including warranties of performance, merchantability or fitness
 * for any particular purpose.\n\n
 *
 * Please cite the author(s) in any work or product based on this material.
 */

#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

/**
 * @brief The nmfMessageWithDontShowDialog class the display a message along with
 * a checkbox asking the user if they want to suppress future such message boxes.
 */
class nmfMessageWithDontShowDialog: public QDialog
{
    Q_OBJECT

    bool         m_yes;
    QCheckBox*   m_cbox;
    QPushButton* m_yesBtn;
    QPushButton* m_noBtn;

public:
    /**
     * @brief nmfMessageWithDontShowDialog constructor
     * @param parent : parent widget over which to show this widget
     * @param msg : message to display to the user
     */
    explicit nmfMessageWithDontShowDialog(QWidget* parent,
                                          QString  msg);
    virtual ~nmfMessageWithDontShowDialog() {}

    /**
     * @brief Boolean describing if future such dialogs should be shown
     * @return true if future dialogs are to be shown, false if not
     */
    bool showFutureDialogs();
    /**
     * @brief Boolean signifying if the user clicked yes as an answer to "Don't show this again"
     * @return true if yes, false otherwise
     */
    bool wasYesClicked();

public slots:
    /**
     * @brief Callback invoked when the user clicks the No button
     */
    void callback_noClicked();
    /**
     * @brief Callback invoked when the user clicks the Yes button
     */
    void callback_yesClicked();
};


