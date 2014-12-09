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

#include <QImage>

#include "cutcanvas.h"

QImage cutcanvas(QImage *image, QImage *copyimage)
{
	int width, height;
	width=image->width();
	height=image->height();
	QRgb *RGB;

	//QFile data("output.txt");
	//data.open(QFile::WriteOnly | QFile::Truncate);
	//QTextStream out(&data);
	//out << "Image Format: " << image->format();

	int maxWhiteFromTopY=0;
	int maxWhiteFromBottomY=0;
	int maxWhiteFromLeftX=0;
	int maxWhiteFromRightX=0;
	int tempcounter, currentcounter=0;
	bool inProcessing=true;

	tempcounter=0;
	inProcessing=true;
	while(inProcessing == true)
	for(int x=5; x < width-1; x++)
	{
		for(int y=0; y < height-1; y++)
		{
			RGB=(QRgb *)image->scanLine(y)+x;
			if(qRed(*RGB)==255 && qGreen(*RGB)==255 && qBlue(*RGB)==255)
			{
				currentcounter++;
			}
		}
		if(currentcounter >= height*0.75)
		{
			maxWhiteFromLeftX=x;
			inProcessing=false;
			break;
		}
		currentcounter=0;
		if(x == (width/2)-1) inProcessing=false;
	}
	currentcounter=0;
	inProcessing=true;
	while(inProcessing == true)
	for(int x=width-5; x > maxWhiteFromLeftX+1; x--)
	{
		for(int y=0; y < height-1; y++)
		{
			RGB=(QRgb *)image->scanLine(y)+x;
			if(qRed(*RGB)==255 && qGreen(*RGB)==255 && qBlue(*RGB)==255)
			{
				currentcounter++;
			}
		}
		if(currentcounter >= height*0.75)
		{
			maxWhiteFromRightX=x;
			inProcessing=false;
			break;
		}
		currentcounter=0;
		if(x == (width/2)+1) inProcessing=false;
	}
	currentcounter=0;
	inProcessing=true;

	while(inProcessing == true)
	for(int y=1; y < height/2; y++)
	{
		for(int x=maxWhiteFromLeftX; x <= maxWhiteFromRightX; x++)
		{
			RGB=(QRgb *)image->scanLine(y)+x;
			if(qRed(*RGB)==255 && qGreen(*RGB)==255 && qBlue(*RGB)==255)
			{
				currentcounter++;
			}
		}
		if(currentcounter >= (maxWhiteFromRightX-maxWhiteFromLeftX)*0.95 && inProcessing == true)
		{
			maxWhiteFromTopY=y;
			tempcounter=currentcounter;
			currentcounter=0;
			inProcessing=false;
		}
		currentcounter=0;

	}
	tempcounter=0;
	inProcessing=true;

	while(inProcessing == true)
	for(int y=height-1; y > height/2; y--)
	{
		for(int x=maxWhiteFromLeftX; x <= maxWhiteFromRightX; x++)
		{
			RGB=(QRgb *)image->scanLine(y)+x;
			if(qRed(*RGB)==255 && qGreen(*RGB)==255 && qBlue(*RGB)==255)
			{
				currentcounter++;
			}
		}
		if(currentcounter >= width*0.95 && inProcessing == true)
		{
			maxWhiteFromBottomY=y;
			tempcounter=currentcounter;
			currentcounter=0;
			inProcessing=false;
		}

	}

	QRgb *RGB2;
	QImage tempimage(maxWhiteFromRightX-maxWhiteFromLeftX, maxWhiteFromBottomY-maxWhiteFromTopY, QImage::Format_RGB32);
	int tempx=0;
	int tempy=0;
	for(int y=maxWhiteFromTopY; y < maxWhiteFromBottomY; y++)
	{
		tempx=0;
		for(int x=maxWhiteFromLeftX; x<maxWhiteFromRightX; x++)
		{
			RGB=(QRgb *)copyimage->scanLine(y)+x;
			RGB2=(QRgb *)tempimage.scanLine(tempy)+tempx;
			*RGB2=*RGB;
			tempx++;
		}
		tempy++;
	}

	// <Grayscale conversion>
	float gray;
	// Convert Image formally to 32bpp
	if(tempimage.depth()!=32)
	{
		tempimage=tempimage.convertToFormat(QImage::Format_RGB32);
	}
	for (int y=0;y<tempimage.height();y++)
	{
		for (int x=0;x<tempimage.width();x++)
		{
			RGB=(QRgb *)tempimage.scanLine(y) + x;
			gray=(qRed(*RGB)+qGreen(*RGB)+qBlue(*RGB))/3;
			//gray=0.299*qRed(*RGB)+0.587*qGreen(*RGB)+0.144*qBlue(*RGB);
			*RGB=qRgb(gray,gray,gray);
		}
	}
	// </Grayscale conversion>

	return tempimage;
}