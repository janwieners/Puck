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

#include <iostream>
#include <QImage>
#include <QVector>
#include <QtGui/QApplication>
#include "puckmainwindow.h"

#include "segment.h"
#include "cutcanvas.h"
#include "preprocess.h"
#include "settingsdlg.h"
#include "edgedetection.h"

int main(int argc, char *argv[])
{
	// Start GUI Version of puck
	if(argc<5)
	{
		QApplication a(argc, argv);
		puckMainWindow w;
		w.show();
		return a.exec();
	}

	// With the right count of input arguments: Start Console application
	else
	{
		float segmentationSigma=1;
		if(argv[5]) segmentationSigma=atoi(argv[5]);
		int segmentationK=1000;
		if(argv[6]) segmentationK=atoi(argv[6]);
		int segmentationMin_size=500;
		if(argv[7]) segmentationMin_size=atoi(argv[7]);

		QString filenameLeft=argv[2];
		QString filenameXCDLbase=argv[1];
		QVector <QVector<float>> significantObjectsLeft;
		QString filenameRight=argv[4];
		QString filenameXCDLtarget=argv[3];

		// Input Arguments: puck <inputxcdl> <inputimage> <comparexcdl> <compareimage> 
		// Processing Input Image
		std::cout << "Processing image " << argv[2] << " with XCDL " << argv[1] << ".....\n";
		QImage imageLeft(filenameLeft);
		QImage copyimage=imageLeft;
		imageLeft=cutcanvas(&imageLeft, &copyimage);
		binarize_otsu(&imageLeft);
		imageLeft=segmentImage(&imageLeft, segmentationSigma, segmentationK, segmentationMin_size);

		significantObjectsLeft.clear();
		identifyObjects(&imageLeft, filenameLeft, filenameXCDLbase, &significantObjectsLeft, false);
		imageLeft.save("processed-input-image.png", "PNG");

		// Processing Comparison Image
		std::cout << "Processing comparison image " << argv[4] << " with XCDL " << argv[3] << ".....\n";
		QImage imageRight(filenameRight);
		copyimage=imageRight;
		imageRight=cutcanvas(&imageRight, &copyimage);
		binarize_otsu(&imageRight);
		imageRight=segmentImage(&imageRight, segmentationSigma, segmentationK, segmentationMin_size);

		identifyObjects(&imageRight, filenameRight, filenameXCDLtarget, &significantObjectsLeft, false);
		imageRight.save("processed-cmp-image.png", "PNG");
	}
}
