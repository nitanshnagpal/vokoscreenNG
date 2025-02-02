/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2022 Volker Kohaupt
 *
 * Author:
 *      Volker Kohaupt <vkohaupt@volkoh.de>
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

#include "QvkCameraWindow.h"
#include "global.h"

#include <QDebug>
#include <QMouseEvent>
#include <QBitmap>


QvkCameraWindow::QvkCameraWindow( Ui_formMainWindow *ui_surface, cameraSettingsDialog *settingsDialog )
{
    ui_formMainWindow = ui_surface;
    vkCameraSettingsDialog = settingsDialog;

    setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint );
    setMinimumSize( QSize( 100, 100 ) );
    resize( 100, 100 );
    setMouseTracking( true ); // No function, why?

    connect( ui_formMainWindow->checkBoxCameraMirrorHorizontal, SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraMirrorHorizontal, SLOT( setChecked( bool ) ) );
    connect( ui_formMainWindow->checkBoxCameraMirrorVertical,   SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraMirrorVertical,   SLOT( setChecked( bool ) ) );
    connect( ui_formMainWindow->checkBoxCameraInvert,           SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraInvert,           SLOT( setChecked( bool ) ) );
    connect( ui_formMainWindow->checkBoxCameraGray,             SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraGray,             SLOT( setChecked( bool ) ) );
    connect( ui_formMainWindow->checkBoxCameraMono,             SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraMono,             SLOT( setChecked( bool ) ) );
    connect( ui_formMainWindow->checkBoxCameraWindowFrame,      SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraWindowFrame,      SLOT( setChecked( bool ) ) );
    connect( ui_formMainWindow->comboBoxCameraResolution, SIGNAL( currentIndexChanged( int ) ), vkCameraSettingsDialog->ui->comboBoxCameraResolution,  SLOT( setCurrentIndex( int ) ) );
    connect( ui_formMainWindow->toolButton_camera_view_rectangle, SIGNAL( clicked( bool ) ), vkCameraSettingsDialog->ui->toolButton_camera_view_rectangle, SLOT( setChecked( bool )) );
    connect( ui_formMainWindow->toolButton_camera_view_ellipse,   SIGNAL( clicked( bool ) ), vkCameraSettingsDialog->ui->toolButton_camera_view_ellipse,   SLOT( setChecked( bool ) ) );
    connect( ui_formMainWindow->toolButton_camera_view_circle,    SIGNAL( clicked( bool ) ), vkCameraSettingsDialog->ui->toolButton_camera_view_circle,    SLOT( setChecked( bool ) ) );

    connect( vkCameraSettingsDialog->ui->checkBoxCameraMirrorHorizontal, SIGNAL( toggled( bool ) ), ui_formMainWindow->checkBoxCameraMirrorHorizontal, SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraMirrorVertical,   SIGNAL( toggled( bool ) ), ui_formMainWindow->checkBoxCameraMirrorVertical,   SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraInvert,           SIGNAL( toggled( bool ) ), ui_formMainWindow->checkBoxCameraInvert,           SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraGray,             SIGNAL( toggled( bool ) ), ui_formMainWindow->checkBoxCameraGray,             SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraMono,             SIGNAL( toggled( bool ) ), ui_formMainWindow->checkBoxCameraMono,             SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraWindowFrame,      SIGNAL( toggled( bool ) ), ui_formMainWindow->checkBoxCameraWindowFrame,      SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->comboBoxCameraResolution,  SIGNAL( currentIndexChanged( int ) ), ui_formMainWindow->comboBoxCameraResolution, SLOT( setCurrentIndex( int ) ) );
    connect( vkCameraSettingsDialog->ui->toolButton_camera_view_rectangle, SIGNAL( clicked( bool ) ), ui_formMainWindow->toolButton_camera_view_rectangle, SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->toolButton_camera_view_ellipse,   SIGNAL( clicked( bool ) ), ui_formMainWindow->toolButton_camera_view_ellipse,   SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->toolButton_camera_view_circle,    SIGNAL( clicked( bool ) ), ui_formMainWindow->toolButton_camera_view_circle,    SLOT( setChecked( bool ) ) );

    connect( vkCameraSettingsDialog->ui->buttonBox, SIGNAL( accepted() ), vkCameraSettingsDialog, SLOT( close() ) );
    connect( vkCameraSettingsDialog->ui->pushButtonSwitchToFullscreen, SIGNAL( clicked( bool ) ), this, SLOT( slot_switchToFullscreen() ) );
}


QvkCameraWindow::~QvkCameraWindow()
{
}


void QvkCameraWindow::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event)

    if ( image.format() == QImage::Format_Invalid ) {
        if ( error > "" ) {
            int w = 480;
            int h = 300;
            int pointSize = 14;
            int frameWidth = 8;
            resize( w, h );
            QPainter painter;
            painter.begin( this );
            painter.setRenderHints( QPainter::Antialiasing, true );

            // Background
            QBrush brush( Qt::white );
            painter.fillRect( 0, 0, w, h, brush );

            // Frame
            if ( ui_formMainWindow->checkBoxCameraWindowFrame->isChecked() == true )
            {
                QPen pen;
                pen.setWidth( frameWidth );
                pen.setColor( QString( "#9EBBD8" ) );
                painter.setPen( pen );
                painter.drawRect( frameWidth/2, frameWidth/2, w-frameWidth, h-frameWidth );
            }

            // Text
            QFont font;
            font.setPointSize( pointSize );
            painter.setFont( font );
            QPen pen;
            pen.setColor( Qt::black );
            painter.setPen( pen );
            painter.drawText( QRect( 0, 0, w, h/2 ), Qt::AlignCenter, global::name + " " + global::version );
            painter.drawText( QRectF( 0, 0, w, h ), Qt::AlignCenter, error );

            // Close button
            if ( ui_formMainWindow->checkBoxCameraWindowFrame->isChecked() == true )
            {
                int width = 20;
                int height = 20;
                int distanceToFrame = 3;
                rectCloseButton.setRect( w - frameWidth - distanceToFrame - width, frameWidth + distanceToFrame, width, height );
                QPen pen;
                pen.setWidth( 2 );
                pen.setColor( Qt::red );
                painter.setPen( pen );
                painter.drawLine( rectCloseButton.topRight(), rectCloseButton.bottomLeft() );
                painter.drawLine( rectCloseButton.topLeft(), rectCloseButton.bottomRight() );
            }

            painter.end();
            return;
        }
        return;
    }

    int displayedWidth = 0;
    int displayedHeight = 0;
    if ( isFullScreen() == true ) {
        displayedWidth = width();
        displayedHeight = height();
        image = image.scaled( displayedWidth, displayedHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    } else {
        displayedWidth = image.width();
        displayedHeight = image.height();
    }

    QPixmap pixmap( displayedWidth, displayedHeight );
    if ( ui_formMainWindow->checkBoxCameraWindowFrame->isChecked() == true ) {
        if ( isFullScreen() == true ) {
            pixmap.fill( Qt::black );
        } else {
            pixmap.fill( Qt::transparent );
        }
    } else {
        pixmap.fill( Qt::black );
    }

    QPainter painterPixmap;
    painterPixmap.begin( &pixmap );
    painterPixmap.setRenderHint( QPainter::Antialiasing, true );
    painterPixmap.setRenderHint( QPainter::SmoothPixmapTransform, true );
    painterPixmap.drawImage( ( width() - image.width() ) / 2 , 0, image );
    painterPixmap.end();

    QPainter painter;
    painter.begin( this);
    painter.setRenderHint( QPainter::Antialiasing, true );
    painter.setRenderHint( QPainter::SmoothPixmapTransform, true );
    painter.drawPixmap( QPoint( 0, 0 ), pixmap );
    painter.end();

    if ( ui_formMainWindow->checkBoxCameraWindowFrame->isChecked() == true ) {
        setMask( pixmap.mask() );
    } else {
        clearMask();
    }
}


void QvkCameraWindow::setNewImage( QImage _image )
{
    image = _image;
    repaint();
}


void QvkCameraWindow::closeEvent( QCloseEvent *event )
{
    Q_UNUSED(event);
    emit signal_cameraWindow_close( false );
}


void QvkCameraWindow::slot_switchToFullscreen()
{
    if ( isFullScreen() == true )
    {
        showNormal();
        vkCameraSettingsDialog->close();
    }
    else
    {
        setWindowState( Qt::WindowFullScreen );
        vkCameraSettingsDialog->close();
    }
}


void QvkCameraWindow::resizeEvent( QResizeEvent *event )
{
    Q_UNUSED(event);
    if ( isFullScreen() == true )
    {
        vkCameraSettingsDialog->ui->pushButtonSwitchToFullscreen->setText( tr( "Switch to Window" ) );
    }
    else
    {
        vkCameraSettingsDialog->ui->pushButtonSwitchToFullscreen->setText( tr( "Switch to Fullscreen" ) );
        ui_formMainWindow->labelCameraWindowSize->setText( QString::number( width() ) + "x" + QString::number( height() ) );
        vkCameraSettingsDialog->ui->labelCameraWindowSize->setText( QString::number( width() ) + "x" + QString::number( height() ) );
    }
}


void QvkCameraWindow::mouseDoubleClickEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton )
    {
        if ( isFullScreen() == true )
        {
            showNormal();
        }
        else
        {
            setWindowState( Qt::WindowFullScreen );
            vkCameraSettingsDialog->close();
        }
    }
}


void QvkCameraWindow::keyPressEvent( QKeyEvent *event )
{
    if ( event->key() == Qt::Key_Escape )
    {
        showNormal();
    }

    if ( ( event->key() == Qt::Key_F11 ) or ( event->key() == Qt::Key_F ) )
    {
        if ( isFullScreen() == true )
        {
            showNormal();
        }
        else
        {
            setWindowState( Qt::WindowFullScreen );
        }
    }
}


void QvkCameraWindow::mousePressEvent( QMouseEvent *event )
{
    if ( rectCloseButton.contains( event->pos() ) ) {
        close();
        return;
    }

    if ( event->button() == Qt::RightButton )
    {
        if ( vkCameraSettingsDialog->isVisible() )
        {
            vkCameraSettingsDialog->close();
        }
        else
        {
            vkCameraSettingsDialog->show();
            if ( isFullScreen() == true )
            {
                vkCameraSettingsDialog->ui->widgetCameraWindowSize->hide();
                vkCameraSettingsDialog->ui->checkBoxCameraWindowFrame->hide();
            }
            else
            {
                vkCameraSettingsDialog->ui->widgetCameraWindowSize->show();
                vkCameraSettingsDialog->ui->checkBoxCameraWindowFrame->show();
            }
        }
        return;
    }

    if ( isFullScreen() == true )
    {
        vkCameraSettingsDialog->close();
    }

    if ( ( ui_formMainWindow->checkBoxCameraWindowFrame->isChecked() == true ) and ( event->button() == Qt::LeftButton ) and ( isFullScreen() == false ) )
    {
        QPixmap pixmap( ":/pictures/cursor/size_all.png" );
        QCursor cursor( pixmap );
        setCursor( cursor );
        mousePressed = true;
        mouseLocal_X = event->localPos().x();
        mouseLocal_Y = event->localPos().y();
    }
}


void QvkCameraWindow::mouseReleaseEvent( QMouseEvent *event )
{
    Q_UNUSED(event)
    if ( ( ui_formMainWindow->checkBoxCameraWindowFrame->isChecked() == true ) and ( isFullScreen() == false ) )
    {
        unsetCursor();
        mousePressed = false;
    }
}


void QvkCameraWindow::mouseMoveEvent( QMouseEvent *event )
{
    Q_UNUSED(event)
    if ( ( ui_formMainWindow->checkBoxCameraWindowFrame->isChecked() == true ) and ( mousePressed == true ) )
    {
        move( QCursor::pos().x() - mouseLocal_X, QCursor::pos().y() - mouseLocal_Y );
    }
}
