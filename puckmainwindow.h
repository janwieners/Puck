/*     * This module is part of the XCL software system
       *
       * Copyright 2010 Jan Gerrit Wieners, Historisch-kulturwissenschatliche Informationsverarbeitung, Universität zu Köln
       *
       * Licensed under the Apache License, Version 2.0 (the "License");
       * you may not use this file except in compliance with the License.
       * You may obtain a copy of the License at
       * 
       * http://www.apache.org/licenses/LICENSE-2.0
       * 
       * Unless required by applicable law or agreed to in writing, software
       * distributed under the License is distributed on an "AS IS" BASIS,
       * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
       * See the License for the specific language governing permissions and
       * limitations under the License.
*/

#ifndef PUCKMAINWINDOW_H
#define PUCKMAINWINDOW_H

#include <QLabel>
#include <QSplitter>
#include <QMainWindow>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QVector>
#include <QtGui>

#include "segment.h"
#include "cutcanvas.h"
#include "preprocess.h"
#include "settingsdlg.h"
#include "edgedetection.h"

class puckMainWindow : public QMainWindow {
    Q_OBJECT
public:
    puckMainWindow(QWidget *parent = 0);
    ~puckMainWindow();

private slots:
	void loadXCDLbase();
	void loadXCDLtarget();
    void loadImageLeft()
    {
        filenameLeft=QFileDialog::getOpenFileName(this,
                "Load", ".", "Image Files (*.png *.xpm *.jpg *.tif *.tiff *.bmp)");
        if(filenameLeft.isEmpty() == true)
        {
                //QMessageBox::warning(this,"Warning", "You didn't choose an image", "Proceed");
        }
        if (imageLeft.load(filenameLeft))
        {
				QString consoleText=">> Processed Image ";
				consoleText+=filenameLeft;
				consoleText+="\nOutput is ";
				consoleText+=filenameLeft;
				consoleText+=".xcdl";
				consoleLeft->setText(consoleText);
				imageLeft=imageLeft.convertToFormat(QImage::Format_RGB32);
				QImage copyimage=imageLeft;
				//imageLeft=cutcanvas(&imageLeft, &copyimage);
				binarize_otsu(&imageLeft);
				imageLeft=TIfilterCC(imageLeft,TIFSobel);
				//imageLeft=segmentImage(&imageLeft, segmentationSigma, segmentationK, segmentationMin_size);

				significantObjectsLeft.clear();
				// MARK: Aktivieren, um Objekte zu kennzeichnen:
				// identifyObjects(&imageLeft, filenameLeft, filenameXCDLbase, &significantObjectsLeft, false);

				pixmapLeft=pixmapLeft.fromImage(imageLeft, 0);

                viewLeft->setPixmap(pixmapLeft);
				viewLeft->setLineWidth(4);
				// !!!
                viewLeft->adjustSize();
        }
        else if(!filenameLeft.isEmpty())
        {
                QMessageBox::warning(this,"Warning",
                                       "This Image seems to be corrupted",
                                       "Proceed");
        }
    }
    void loadImageRight()
    {
        filenameRight=QFileDialog::getOpenFileName(this,
                "Load", ".", "Image Files (*.png *.xpm *.jpg *.tif *.tiff *.bmp)");
        if(filenameRight.isEmpty() == true)
        {
                //QMessageBox::warning(this,"Warning", "You didn't choose an image", "Proceed");
        }
        if (imageRight.load(filenameRight))
        {
				//TODO: Vergleichsmaß definieren
				QString consoleText=">> Processed Image ";
				consoleText+=filenameRight;
				consoleText+="\nOutput is ";
				consoleText+=filenameRight;
				consoleText+=".xcdl";
				consoleRight->setText(consoleText);
				imageRight=imageRight.convertToFormat(QImage::Format_RGB32);

				QImage copyimage=imageRight;
				//imageRight=cutcanvas(&imageRight, &copyimage);
				binarize_otsu(&imageRight);

				// Scale Image
				//if(imageLeft.width()!=0)
				//	imageRight=imageRight.scaledToWidth(imageLeft.width(), Qt::SmoothTransformation);
				//if(imageLeft.height()!=0)
				//	imageRight=imageRight.scaledToHeight(imageLeft.height(), Qt::SmoothTransformation);

				imageRight=segmentImage(&imageRight, segmentationSigma, segmentationK, segmentationMin_size);
				// MARK: Aktivieren, um Objekte zu kennzeichnen:
				// identifyObjects(&imageRight, filenameRight, filenameXCDLtarget, &significantObjectsLeft, true);
				pixmapRight=pixmapRight.fromImage(imageRight, 0);

                viewRight->setPixmap(pixmapRight);
				viewRight->setLineWidth(4);
				// !!!
                viewRight->adjustSize();
        }
        else if(!filenameRight.isEmpty())
        {
                QMessageBox::warning(this,"Warning",
                                       "This Image seems to be corrupted",
                                       "PviewLeftroceed");
        }
    }

private:

    QImage imageLeft;
    QImage imageRight;
    QString filenameLeft;
	QString filenameRight;
	QString filenameXCDLbase;
	QString filenameXCDLtarget;

    QPixmap pixmapLeft;
    QPixmap pixmapRight;
    QSplitter *splitter;
    QLabel *viewLeft;
    QLabel *viewRight;
    QWidget *widgetLeft;
    QWidget *widgetRight;
    QVBoxLayout *layoutLeft;
    QVBoxLayout *layoutRight;
    QScrollArea *scrollAreaLeft;
    QScrollArea *scrollAreaRight;
    QPushButton *leftPushButton;
    QPushButton *rightPushButton;
	QPushButton *leftXCDLButton;
	QPushButton *rightXCDLButton;
	QAction *settingsAction;
	QMenu *fileMenu;

	QLabel *consoleLeft;
	QLabel *consoleRight;

	QVector <QVector<float>> significantObjectsLeft;

	double segmentationSigma;
	float segmentationK;
	int segmentationMin_size;

	class dlgsettings *dlgsettings;

    void prepareWindow(void)
    {
        viewLeft        = new QLabel;
        viewRight       = new QLabel;
        scrollAreaLeft  = new QScrollArea(this);
        scrollAreaRight = new QScrollArea(this);

        scrollAreaLeft->setBackgroundRole(QPalette::Dark);
        scrollAreaLeft->setWidget(viewLeft);

        scrollAreaRight->setBackgroundRole(QPalette::Dark);
        scrollAreaRight->setWidget(viewRight);

        leftPushButton = new QPushButton();
        leftPushButton->setText("Load Image");
		leftXCDLButton = new QPushButton();
        leftXCDLButton->setText("Load XCDL");

        rightPushButton = new QPushButton();
        rightPushButton->setText("Load Image");
		rightXCDLButton = new QPushButton();
        rightXCDLButton->setText("Load XCDL");

        connect(leftPushButton, SIGNAL(clicked()), this, SLOT(loadImageLeft()));
        connect(rightPushButton, SIGNAL(clicked()), this, SLOT(loadImageRight()));
		connect(leftXCDLButton, SIGNAL(clicked()), this, SLOT(loadXCDLbase()));
        connect(rightXCDLButton, SIGNAL(clicked()), this, SLOT(loadXCDLtarget()));

        layoutLeft=new QVBoxLayout;
		layoutLeft->addWidget(leftXCDLButton);
        layoutLeft->addWidget(leftPushButton);        
        layoutLeft->addWidget(scrollAreaLeft);
		consoleLeft=new QLabel(tr(">> Ready for processing XCDL and Image"));
		layoutLeft->addWidget(consoleLeft);
        widgetLeft = new QWidget;
        widgetLeft->setLayout(layoutLeft);

        layoutRight=new QVBoxLayout;
		layoutRight->addWidget(rightXCDLButton);
        layoutRight->addWidget(rightPushButton);
        layoutRight->addWidget(scrollAreaRight);
		consoleRight=new QLabel(tr(">> Ready for processing XCDL and Image"));
		layoutRight->addWidget(consoleRight);
        widgetRight = new QWidget;
        widgetRight->setLayout(layoutRight);

        splitter = new QSplitter;
        splitter->addWidget(widgetLeft);
        splitter->addWidget(widgetRight);

        setCentralWidget(splitter);
        setWindowTitle("Planets :: Puck");
        resize(640, 480);

		settingsAction=new QAction(tr("Settings"), this);
		settingsAction->setIcon(QIcon("icons/wrench.png"));
	
		fileMenu=menuBar()->addMenu(tr("&Settings"));
		fileMenu->addAction(settingsAction);
		connect(settingsAction, SIGNAL(triggered()), this, SLOT(settingshelper()));

		segmentationSigma=1;
		segmentationK=2000;
		segmentationMin_size=750;
    }

private slots:
	void settingshelper(void)
	{
		// Init Settings Dialogue
		dlgsettings=new class dlgsettings(segmentationSigma, segmentationK, segmentationMin_size);
		connect(dlgsettings,
		SIGNAL(printcursettings(double, float, int)),
		this, SLOT(settingssave(double, float, int)));
		dlgsettings->show();
	}
	void settingssave(double sigma, float k, int min_size)
	{
		segmentationSigma=sigma;
		segmentationK=k;
		segmentationMin_size=min_size;
		//expandhistogramrgb=histoexprgb;
		
	}
};

#endif // PUCKMAINWINDOW_H
