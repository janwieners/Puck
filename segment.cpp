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

/* Efficient Graph-Based Image Segmentation - Pedro F. Felzenszwalb and Daniel P. Huttenlocher
   International Journal of Computer Vision, Volume 59, Number 2, September 2004

   Implementation from http://people.cs.uchicago.edu/~pff/segment/
*/

#include <QList>
#include <QFile>
#include <QImage>
#include <QTextStream>
#include <QPainter>

#include "3rdparty/segment/misc.h"
#include "3rdparty/segment/pnmfile.h"
#include "3rdparty/segment/segment-image.h"
#include "3rdparty/segment/image.h"

QImage segmentImage(QImage *inputimage, float inputsigma, float inputk, int inputmin_size)
{
    double sigma = inputsigma; // 1
    float k = inputk; // 2000
    int min_size = inputmin_size; // 1500

	image<rgb> *input = loadQImage(inputimage);
    int num_ccs;

    image<rgb> *seg = segment_image(input, sigma, k, min_size, &num_ccs);

	inputimage=convertImageToQImage(seg, inputimage);

	// extract background from segmentation result
	// TODO: Find the "right" way to extract background information
	QRgb *RGB;
	RGB=(QRgb *)inputimage->scanLine(0) + 0;
	int backgroundColor=qBlue(*RGB);
	for(int y=0; y<inputimage->height(); y++)
		for(int x=0; x<inputimage->width(); x++)
		{
			RGB=(QRgb *)inputimage->scanLine(y) + x;
			if(qBlue(*RGB) == backgroundColor)
			{
				
				*RGB = qRgb(255,255,255);
			}
			else
			{
				//*RGB = qRgb(0,0,0);
			}
		}

	return *inputimage;
}

QImage identifyObjects(QImage *inputimage, QString filename, QString xcdlFile, QVector <QVector<float>> *significantObjectsVector, bool processingRightImage)
{
	QString outstring=filename;
	outstring+=".xcdl";
	QFile data(outstring);
	data.open(QFile::WriteOnly | QFile::Truncate);
	QTextStream fileOut(&data);
	//fileOut << "<?xml version='1.0' encoding='utf-8'?>" << "\n";

	int currentobject=255;
	int lefttopX=0, lefttopY=0;
	int rightbottomX=0, rightbottomY=0;
	bool stopprocessing=false;
	QRgb *RGB;
	QRgb *RGB2;
	int objectCounter=0;

	QList <int> hitlist;
	QString searchstring="</object>";
	QString tmpstring="";
	bool stopProcessing=false;
	if(xcdlFile!="")
	{
		QFile file(xcdlFile);
		if(file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream in(&file);
			while(!in.atEnd() && stopProcessing==false)
			{
				tmpstring = in.readLine();
				//if(tmpstring.contains("</object>", Qt::CaseInsensitive)) stopProcessing=true;
				if(tmpstring.contains("</normData>", Qt::CaseInsensitive))
				{
					fileOut << tmpstring << "\n";
					stopProcessing=true;
				}
				else fileOut << tmpstring << "\n";
			}

			fileOut << "<property id=\"p15381\" source=\"raw\" cat=\"descr\">" << "\n"
				<< "<name id=\"id9998\">significantCoordinates</name>" << "\n";
				
			bool relevantobject=true;
			// Now process objects
			for(int y=1; y<inputimage->height()-1; y++)
			for(int x=1; x<inputimage->width()-1; x++)
			{
				RGB=(QRgb *)inputimage->scanLine(y) + x;
				if((qRed(*RGB) != currentobject) && (qGreen(*RGB) != currentobject) && (qBlue(*RGB) != currentobject) && qBlue(*RGB) != 255)
				{
					// New object found
					objectCounter++;
					currentobject=qBlue(*RGB);
					// Found top left point of the object
					lefttopX=x;
					lefttopY=y;

					for(int ycoordinate=inputimage->height()-1; ycoordinate > 1; ycoordinate--)
					{
						for(int xcoordinate=inputimage->width()-1; xcoordinate > 1; xcoordinate--)
						{
							RGB2=(QRgb *)inputimage->scanLine(ycoordinate) + xcoordinate;
							if(qBlue(*RGB2) == currentobject && stopprocessing == false)
							{
								rightbottomX=xcoordinate;
								rightbottomY=ycoordinate;

								stopprocessing=true;
							}
						}
					}
					float lefttopXrelative=(float)lefttopX/inputimage->width();
					float lefttopYrelative=(float)lefttopY/inputimage->height();
					float rightbottomXrelative=(float)rightbottomX/inputimage->width();
					float rightbottomYrelative=(float)rightbottomY/inputimage->height();

					relevantobject=true;
					if(processingRightImage == false)
					{
						QVector <float> tempvector;
						tempvector.append(lefttopXrelative);
						tempvector.append(lefttopYrelative);
						tempvector.append(rightbottomXrelative);
						tempvector.append(rightbottomYrelative);
						significantObjectsVector->append(tempvector);
					}
					else
					{
						relevantobject=false;
						// Check, if Object of left image file is in right image file
						if(objectCounter <= significantObjectsVector->size())
						for(int i = 0; i < significantObjectsVector->size(); ++i)
						{
								// REFACTOR ME (PLEASE)!
								if(significantObjectsVector->at(i).at(0) <= lefttopXrelative+0.2
									&& significantObjectsVector->at(i).at(0) >= lefttopXrelative-0.2

									&& significantObjectsVector->at(i).at(1) <= lefttopYrelative+0.2
									&& significantObjectsVector->at(i).at(1) >= lefttopYrelative-0.2

									&& significantObjectsVector->at(i).at(2) <= rightbottomXrelative+0.2
									&& significantObjectsVector->at(i).at(2) >= rightbottomXrelative-0.2
									
									&& significantObjectsVector->at(i).at(3) <= rightbottomYrelative+0.2
									&& significantObjectsVector->at(i).at(3) >= rightbottomYrelative-0.2)
								{
									relevantobject=true;
								}
						}
					}
					if(relevantobject == true)
					{
						fileOut << "<valueSet id=\"i_i1_i2xx_s1_" << objectCounter << "\">" << "\n"
							<< "<labValue>" << "\n"
							<< "<val>" << lefttopXrelative << " "
							<< lefttopYrelative << " "
							<< rightbottomXrelative << " " << rightbottomYrelative << "</val>" << "\n"
							<< "<type>rational</type>" << "\n"
							<< "</labValue>" << "\n"
							<< "</valueSet>" << "\n";

						// Finalize processing: Paint markers -- REFACTOR ME!
						// a) Left Top Marker
						for(int x=lefttopX-8; x < lefttopX+9; x++)
						{
							if(x > 0 && x!=lefttopX)
							{
								if(x < inputimage->width()) 
								{
									RGB=(QRgb *)inputimage->scanLine(lefttopY) + x;
									*RGB = qRgb(255,0,0);
								}
								if(x < inputimage->width() && lefttopY+1 < inputimage->height()-1) 
								{
									RGB=(QRgb *)inputimage->scanLine(lefttopY+1) + x;
									*RGB = qRgb(255,0,0);
								}
								if(x < inputimage->width() && lefttopY-1 > 1) 
								{
									RGB=(QRgb *)inputimage->scanLine(lefttopY-1) + x;
									*RGB = qRgb(255,0,0);
								}
							}
						}
						for(int y=lefttopY-8; y < lefttopY+9; y++)
						{
							if(y > 0 && y!=lefttopY)
							{
								if(y < inputimage->height()-1)
								{
									RGB=(QRgb *)inputimage->scanLine(y) + lefttopX;
									*RGB = qRgb(255,0,0);
								}
								if(y < inputimage->height()-1 && lefttopX+1 < inputimage->width())
								{
									RGB=(QRgb *)inputimage->scanLine(y) + lefttopX+1;
									*RGB = qRgb(255,0,0);
								}
								if(y < inputimage->height()-1 && lefttopX-1 > 1)
								{
									RGB=(QRgb *)inputimage->scanLine(y) + lefttopX-1;
									*RGB = qRgb(255,0,0);
								}
							}
						}
						// b) Right Bottom Marker
						for(int x=rightbottomX-8; x < rightbottomX+9; x++)
						{
							if(x > 0 && x!=rightbottomX)
							{
								if(x < inputimage->width()) 
								{
									RGB=(QRgb *)inputimage->scanLine(rightbottomY) + x;
									*RGB = qRgb(255,0,0);
								}
								if(x < inputimage->width() && rightbottomY+1 < inputimage->height()-1) 
								{
									RGB=(QRgb *)inputimage->scanLine(rightbottomY+1) + x;
									*RGB = qRgb(255,0,0);
								}
								if(x < inputimage->width() && rightbottomY-1 > 1) 
								{
									RGB=(QRgb *)inputimage->scanLine(rightbottomY-1) + x;
									*RGB = qRgb(255,0,0);
								}
							}
						}
						for(int y=rightbottomY-8; y < rightbottomY+9; y++)
						{
							if(y > 0 && y!=rightbottomY)
							{
								if(y < inputimage->height()-1)
								{
									RGB=(QRgb *)inputimage->scanLine(y) + rightbottomX;
									*RGB = qRgb(255,0,0);
								}
								if(y < inputimage->height()-1 && rightbottomX+1 < inputimage->width())
								{
									RGB=(QRgb *)inputimage->scanLine(y) + rightbottomX+1;
									*RGB = qRgb(255,0,0);
								}
								if(y < inputimage->height()-1 && rightbottomX-1 > 1)
								{
									RGB=(QRgb *)inputimage->scanLine(y) + rightbottomX-1;
									*RGB = qRgb(255,0,0);
								}
							}
						}
						// Now delete marked object to process next objects
						for(int y=lefttopY-1; y<=rightbottomY; y++)
							for(int x=1; x < inputimage->width()-1; x++)
							{
								RGB=(QRgb *)inputimage->scanLine(y) + x;
								if(qBlue(*RGB) == currentobject)
								{
									*RGB = qRgb(0, 0, 255);										
								}
							}
						stopprocessing=false;
						currentobject=255;
					} // if(relevantobject != true) -- PLEASE REFACTOR ME!
					else
					{
						if(objectCounter <= significantObjectsVector->size())
						{
							fileOut << "<valueSet id=\"i_i1_i2xx_s1_" << objectCounter << "\">" << "\n"
								<< "<labValue>" << "\n"
								<< "<val>" << 0 << " "
								<< 0 << " "
								<< 0 << " " << 0 << "</val>" << "\n"
								<< "<type>rational</type>" << "\n"
								<< "</labValue>" << "\n"
								<< "</valueSet>" << "\n";

							// Now delete marked object to process next objects
							for(int y=lefttopY-1; y<=rightbottomY; y++)
								for(int x=1; x < inputimage->width()-1; x++)
								{
									RGB=(QRgb *)inputimage->scanLine(y) + x;
									if(qBlue(*RGB) == currentobject)
									{
										*RGB = qRgb(0, 0, 255);										
									}
								}
						}
						else
						{
							// Now delete marked object to process next objects
							for(int y=lefttopY-1; y<=rightbottomY; y++)
								for(int x=1; x < inputimage->width()-1; x++)
								{
									RGB=(QRgb *)inputimage->scanLine(y) + x;
									if(qBlue(*RGB) == currentobject)
									{
										*RGB = qRgb(0, 0, 255);										
									}
								}
						}
					}
					stopprocessing=false;
					currentobject=255;
				}
			}
			if(objectCounter < significantObjectsVector->size())
			{
				while(objectCounter < significantObjectsVector->size())
				{
					objectCounter++;
					fileOut << "<valueSet id=\"i_i1_i2xx_s1_" << objectCounter << "\">" << "\n"
								<< "<labValue>" << "\n"
								<< "<val>" << 0 << " "
								<< 0 << " "
								<< 0 << " " << 0 << "</val>" << "\n"
								<< "<type>rational</type>" << "\n"
								<< "</labValue>" << "\n"
								<< "</valueSet>" << "\n";
				}
			}

			//fileOut << "</property>" << "\n" << "</object>" << "\n";
			fileOut << "</property>" << "\n";

			// Finalize Processing
			while(!in.atEnd())
			{
				fileOut << in.readLine() << "\n";
			}
		}
     }

	 // Delete irrelevant (marked as green) objects
		//for(int y=lefttopY-1; y<=rightbottomY; y++)
		//	for(int x=1; x < inputimage->width()-1; x++)
		//	{
		//		RGB=(QRgb *)inputimage->scanLine(y) + x;
		//		if(qGreen(*RGB) == 255 && qRed(*RGB) == 0)
		//		{
		//			*RGB = qRgb(255, 255, 255);										
		//		}
		//	}
	data.close();
	return *inputimage;
}