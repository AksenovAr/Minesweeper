/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "MineSweeperModel.h"
#include <QRandomGenerator>

MineSweeperModel::MineSweeperModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    BOOM = false;
    WIN = false;
    FlagsCount = Mines;
    init();
}

int MineSweeperModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return height;
}

int MineSweeperModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return width;
}

QVariant MineSweeperModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != CellRole)
        return QVariant();

    return calculateImage( index.column(), index.row() );
}

QVariant MineSweeperModel::calculateImage( int column, int row ) const
{
    QVariant var;
    if ( m_currentState[two2one(column,row)].m_isFlag == true )
    {
        var = QVariant( QString("images/Minesweeper_flag.jpg") );
    }
    else if ( m_currentState[two2one(column,row)].m_isOpen == false )
    {
        return QVariant( QString("images/Minesweeper_close.png") );
    }
    else if ( m_currentState[two2one(column,row)].m_isBoom == true )
    {
        var = QVariant( QString("images/Minesweeper_boom.png") );
    }
    else if ( m_currentState[two2one(column,row)].m_isMine == true )
    {
        var = QVariant( QString("images/Minesweeper_mine.jpg") );
    }
    else if ( m_currentState[two2one(column,row)].m_isOpen == true && m_currentState[two2one(column,row)].m_value == 0 )
    {
        var = QVariant( QString("images/Minesweeper_open.jpg") );
    }
    else if ( m_currentState[two2one(column,row)].m_value == 1 )
    {
        var = QVariant( QString("images/Minesweeper_1.png") );
    }
    else if ( m_currentState[two2one(column,row)].m_value == 2 )
    {
        var = QVariant( QString("images/Minesweeper_2.png") );
    }
    else if ( m_currentState[two2one(column,row)].m_value == 3 )
    {
        var = QVariant( QString("images/Minesweeper_3.png") );
    }
    else if ( m_currentState[two2one(column,row)].m_value == 4 )
    {
        var = QVariant( QString("images/Minesweeper_4.png") );
    }
    else if ( m_currentState[two2one(column,row)].m_value == 5 )
    {
        var = QVariant( QString("images/Minesweeper_5.png") );
    }
    else if ( m_currentState[two2one(column,row)].m_value == 6 )
    {
        var = QVariant( QString("images/Minesweeper_6.png") );
    }
    else if ( m_currentState[two2one(column,row)].m_value == 7 )
    {
        var = QVariant( QString("images/Minesweeper_7.png") );
    }
    else if ( m_currentState[two2one(column,row)].m_value == 8 )
    {
        var = QVariant( QString("images/Minesweeper_8.png") );
    }
    return var;
}

bool MineSweeperModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != CellRole )
        return false;

    if ( value.toInt() == 0 )
    {
        openPlace( index.column(), index.row() );
    }
    if ( value.toInt() == 1)
    {
        setFlag( index.column(), index.row() );
    }

    emit dataChanged(index, index, {role});
    return true;
}

void  MineSweeperModel::timeElapsed()
{
    m_isGameActive = false;
    BOOM = true;
    emit BOOMChanged();
    openAll();
}

void  MineSweeperModel::openPlace( int column, int row )
{
    if (false == m_isGameActive)
        return;
    CellData &t = m_currentState[two2one(column,row)];
    if (t.m_isFlag)
        return;
    if (t.isOpen())
        return;
    if (t.isMine())
    {
        t.setBoom();
        m_isGameActive = false;
        BOOM = true;
        emit BOOMChanged();
        openAll();
        return;
    }

    openCell(t.i(), t.j());

    if (is_win())
    {
        m_isGameActive = false;
        WIN = true;
        emit WINChanged();
        openAll();
    }

    emit dataChanged(index(0, 0), index(height - 1, width - 1), {});
}

void MineSweeperModel::setFlag( int column, int row )
{
    if (false == m_isGameActive) return;
    CellData &t = m_currentState[two2one(column,row)];
    if (t.isOpen()) return;

    if (t.m_isFlag == true)
    {
        t.m_isFlag = false;
        FlagsCount++;
    }
    else if (t.m_isFlag == false && FlagsCount)
    {
        t.m_isFlag = true;
        FlagsCount--;
    }

    emit dataChanged(index(0, 0), index(height - 1, width - 1), {});
}

Qt::ItemFlags MineSweeperModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

void MineSweeperModel::clear()
{
    BOOM = false;
    WIN = false;
    FlagsCount = Mines;
    emit BOOMChanged();
    init();
    emit dataChanged(index(0, 0), index(height - 1, width - 1), {});
}

void MineSweeperModel::init()
{
    m_currentState.clear();
    m_isGameActive = true;
    // Заполняем ячейками
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            m_currentState.push_back( CellData(i, j) );
        }
    }
    // Заполняем минами
    int tempMins = Mines;
    do
    {
        size_t random = static_cast<size_t> (QRandomGenerator::global()->bounded(width*height));
        if (m_currentState[random].isMine() == false)
        {
            m_currentState[random].setMine();
            --tempMins;
        }

    } while (tempMins);

    // высчитываем соседей
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (m_currentState[two2one(i,j)].isMine()) {
                if (isValidCoord(i - 1, j - 1)) m_currentState[two2one(i - 1,j - 1)].incValue();
                if (isValidCoord(i - 1, j)) m_currentState[two2one(i - 1,j)].incValue();
                if (isValidCoord(i - 1, j + 1)) m_currentState[two2one(i - 1,j + 1)].incValue();
                if (isValidCoord(i, j - 1)) m_currentState[two2one(i,j - 1)].incValue();
                if (isValidCoord(i, j + 1)) m_currentState[two2one(i,j + 1)].incValue();
                if (isValidCoord(i + 1, j - 1)) m_currentState[two2one(i + 1,j - 1)].incValue();
                if (isValidCoord(i + 1, j)) m_currentState[two2one(i + 1,j)].incValue();
                if (isValidCoord(i + 1, j + 1)) m_currentState[two2one(i + 1,j + 1)].incValue();
            }
        }
    }
}

size_t MineSweeperModel::two2one(int x, int y ) const
{
    return static_cast<size_t> (width * x + y);
}

bool MineSweeperModel::is_win()
{
    int n = width * height - Mines;
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
            n -= m_currentState[two2one(i,j)].isOpen();
    return 0 == n;
}

bool MineSweeperModel::isValidCoord(int i, int j)
{
    return i >= 0 && j >= 0 && i < width && j < height;
}

void MineSweeperModel::openCell(int i, int j)
{
    if (false == isValidCoord(i, j)) return;
    CellData &obj = m_currentState[two2one(i,j)];
    if (obj.isOpen())
        return;
    obj.setOpen();
    if (obj.value()) return;
    openCell(i - 1, j); openCell(i + 1, j);
    openCell(i, j - 1); openCell(i, j + 1);
}

void MineSweeperModel::openAll()
{
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
            m_currentState[two2one(i,j)].setOpen();

    emit dataChanged(index(0, 0), index(height - 1, width - 1), {});
}

int MineSweeperModel::getBOOM()const
{
    return BOOM;
}

void MineSweeperModel::setBOOM(const bool &i)
{
    BOOM = i;
}

int MineSweeperModel::getWIN()const
{
    return WIN;
}

void MineSweeperModel::setWIN(const bool &i)
{
    WIN = i;
}
