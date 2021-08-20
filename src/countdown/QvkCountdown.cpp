/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@freenet.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * --End_License--
 */

#include "QvkCountdown.h"

#include <QTimer>
#include <QGuiApplication>
#include <QScreen>
#include <QIcon>
#include <QDebug>
#include <QMouseEvent>
#include <QPushButton>

QvkCountdown::QvkCountdown()
{
}


QvkCountdown::~QvkCountdown()
{
}


void QvkCountdown::init()
{
    createCountdownWindow();

    timer = new QTimer( this );
    timer->setTimerType( Qt::PreciseTimer );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_updateTimer() ) );

    animationTimer = new QTimer( this );
    animationTimer->setTimerType( Qt::PreciseTimer );
    connect( animationTimer, SIGNAL( timeout() ), this, SLOT( slot_updateAnimationTimer() ) );

    hide();
}


void QvkCountdown::createCountdownWindow()
{
    vkCountdownWindow = new QvkCountdownWindow( this );
    vkCountdownWindow->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip ); //With tooltip, no entry in Taskbar

    x = 0;
    y = 0;
    Width = 300;
    Height = 300;;

    vkCountdownWindow->x = 0;
    vkCountdownWindow->y = 0;
    vkCountdownWindow->Width = 300;
    vkCountdownWindow->Height = 300;;
}


void QvkCountdown::startCountdown( int value )
{
    vkCountdownWindow->setGeometry( x, y, Width, Height );
    vkCountdownWindow->show();
    vkCountdownWindow->countValue = value;
    vkCountdownWindow->gradValue = 0;

    timer->start( 1000 );
    animationTimer->start( 25 );
    emit signal_countdownBegin( true );
}


void QvkCountdown::slot_updateTimer()
{
  vkCountdownWindow->gradValue = 0;
  vkCountdownWindow->countValue--;

  if ( vkCountdownWindow->countValue == 0 )
  {
    vkCountdownWindow->setGeometry( x, y, 1, 1 );
    vkCountdownWindow->hide();
    timer->stop();
    animationTimer->stop();
    emit signal_countDownfinish( true );
  }
}

void QvkCountdown::slot_updateAnimationTimer()
{
  vkCountdownWindow->gradValue = vkCountdownWindow->gradValue - 20;
  vkCountdownWindow->update();
}

/*
void QvkCountdown::mousePressEvent( QMouseEvent *event )
{
    if ( rectCancel.contains( event->pos() ) )
    {
        hide();
        timer->stop();
        animationTimer->stop();
        emit signal_countDownCancel( true );
    }
}
*/
