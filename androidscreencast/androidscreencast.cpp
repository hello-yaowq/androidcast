/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
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
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
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

#include <QtGui>
#include "androidscreencast.h"


//! [0]
AndroidScreenCast::AndroidScreenCast()
{
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    createActions();
    createMenus();
    imageWidth = 480;
    imageHeight = 800;
    setWindowTitle(tr("AndroidScreenCast-Powered by ywq"));
    resize(330, 450);
    isSwipe=false;

    exec = new ControlThread();
    exec->start();
    connect(exec, SIGNAL(capChanged()), this, SLOT(updateImage()));

}

//! [9]
void AndroidScreenCast::zoomIn()
//! [9] //! [10]
{
    scaleImage(0.75);
}

void AndroidScreenCast::zoomOut()
{
    scaleImage(0.5);
}

//! [10] //! [11]
void AndroidScreenCast::normalSize()
//! [11] //! [12]
{
    imageLabel->adjustSize();

    resize(QSize(imageLabel->size().width()+6, imageLabel->size().height()+23));
    scaleFactor = 1.0;
}
//! [12]

//! [13]
void AndroidScreenCast::fitToWindow()
//! [13] //! [14]
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}
//! [14]


//! [15]
void AndroidScreenCast::about()
//! [15] //! [16]
{
    QMessageBox::about(this, tr("About AndroidScreenCas"),
            tr("<p>The software is powered by yaoweiqing, email:82900868@qq.com</p>"));

}

void AndroidScreenCast::execute(QString cmd)
{
    qDebug()<<"cmd:"<<cmd;
    exec->pushCmd(cmd);
}

//! [16]
void AndroidScreenCast::home()
{
    execute("adb shell input keyevent 3");

}

void AndroidScreenCast::back()
{
    execute("adb shell input keyevent 4");
}

void AndroidScreenCast::menu()
{
    execute("adb shell input keyevent 82");
}

void AndroidScreenCast::camera()
{
    execute("adb shell input keyevent 27");
}

void AndroidScreenCast::volup()
{
    execute("adb shell input keyevent 24");
}

void AndroidScreenCast::voldown()
{
    execute("adb shell input keyevent 25");
}

void AndroidScreenCast::power()
{
    execute("adb shell input keyevent 26");
}

void AndroidScreenCast::reboot()
{
    execute("adb shell reboot");
}

//! [17]
void AndroidScreenCast::createActions()
//! [17] //! [18]
{
    backAct = new QAction(tr("&Back"), this);
    backAct->setShortcut(tr("Ctrl+B"));
    connect(backAct, SIGNAL(triggered()), this, SLOT(back()));

    homeAct = new QAction(tr("&Home"), this);
    homeAct->setShortcut(tr("Ctrl+H"));
    connect(homeAct, SIGNAL(triggered()), this, SLOT(home()));

    menuAct = new QAction(tr("&Menu"), this);
    menuAct->setShortcut(tr("Ctrl+M"));
    connect(menuAct, SIGNAL(triggered()), this, SLOT(menu()));

    cameraAct = new QAction(tr("Camera"), this);
    //cameraAct->setShortcut(tr("Ctrl+M"));
    connect(cameraAct, SIGNAL(triggered()), this, SLOT(camera()));

    volupAct = new QAction(tr("Voluem Up"), this);
    //volupAct->setShortcut(tr("Ctrl+M"));
    connect(volupAct, SIGNAL(triggered()), this, SLOT(volup()));

    voldownAct = new QAction(tr("Volume Down"), this);
    //voldownAct->setShortcut(tr("Ctrl+M"));
    connect(voldownAct, SIGNAL(triggered()), this, SLOT(voldown()));

    powerAct = new QAction(tr("Power"), this);
    //powerAct->setShortcut(tr("Ctrl+M"));
    connect(powerAct, SIGNAL(triggered()), this, SLOT(power()));

    rebootAct = new QAction(tr("&Reboot"), this);
    rebootAct->setShortcut(tr("Ctrl+R"));
    connect(rebootAct, SIGNAL(triggered()), this, SLOT(reboot()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(true);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}
//! [18]

//! [19]
void AndroidScreenCast::createMenus()
//! [19] //! [20]
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    //viewMenu->addAction(zoomInAct);
    //viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);    

    controlMenu = new QMenu(tr("&Control"), this);
    controlMenu->addAction(backAct);
    controlMenu->addAction(homeAct);
    controlMenu->addAction(menuAct);
    controlMenu->addAction(cameraAct);
    controlMenu->addAction(volupAct);
    controlMenu->addAction(voldownAct);
    controlMenu->addAction(powerAct);
    controlMenu->addSeparator();
    controlMenu->addAction(rebootAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(controlMenu);
    menuBar()->addMenu(helpMenu);

}
//! [20]

//! [21]
void AndroidScreenCast::updateActions()
//! [21] //! [22]
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}
//! [22]

//! [23]
void AndroidScreenCast::scaleImage(double factor)
//! [23] //! [24]
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);
    resize(QSize(imageLabel->size().width()+6, imageLabel->size().height()+23));

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);

}
//! [24]

//! [25]
void AndroidScreenCast::adjustScrollBar(QScrollBar *scrollBar, double factor)
//! [25] //! [26]
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
//! [26]
AndroidScreenCast::~AndroidScreenCast()
{
    if (exec)
    {
        exec->terminate();
        exec->wait(100);
        delete exec;
    }
}

void AndroidScreenCast::mousePressEvent(QMouseEvent *event)
{
   isSwipe=false;
   startX=event->x();
   startY=event->y();
}

void AndroidScreenCast::mouseMoveEvent(QMouseEvent *event)
{
    isSwipe=true;
}

void AndroidScreenCast::mouseReleaseEvent(QMouseEvent *event)
{
    QString cmd;
    if (isSwipe){
        cmd.sprintf("adb shell input swipe %d %d %d %d",startX*480/width(),startY*800/height(), event->x()*480/width(), event->y()*800/height());
        isSwipe=false;
    }else{
        cmd.sprintf("adb shell input tap %d %d",event->x()*480/width(), event->y()*800/height());
    }
    execute(cmd);
}


void AndroidScreenCast::updateImage()
{
    QImage image("c:\\cap.png");
    if (!image.isNull()) {
        imageWidth = image.width();
        imageHeight = image.height();
        imageLabel->setPixmap(QPixmap::fromImage(image));

        imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());
        //scrollArea->resize(imageLabel->size());
//! [3] //! [4]

        fitToWindow();
    }
}
