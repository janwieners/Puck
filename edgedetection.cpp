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
#include "edgedetection.h"

QImage TIfilterCC(
QImage          image,
enum TIFilterType     type)
{
static int filter[15][9]=  { 1, 1, 1, 1, 1, 1, 1, 1, 1, /*  0 = low pass 1 */
                            0, 1, 0, 1, 1, 1, 0, 1, 0, /*  1 = low pass 2 */
                            0,-1, 0,-1, 5,-1, 0,-1, 0, /*  2 = high pass 1 */
                           -1,-1,-1,-1, 9,-1,-1,-1,-1, /*  3 = high pass 2 */
                            1, 2, 1, 2, 4, 2, 1, 2, 1, /*  4 = W.Mean 1 */
                            0, 1, 0, 1, 2, 1, 0, 1, 0, /*  5 = W.Mean 2 */
                            0, 1, 0, 1,-4, 1, 0, 1, 0, /*  6 = Laplace 1 */
                           -1,-1,-1,-1, 8,-1,-1,-1,-1, /*  7 = Laplace 2 */
                            0,-1, 0,-1, 7,-1, 0,-1, 0, /*  8 = Laplace 3 */
                           -1,-1,-1, 0, 0, 0, 1, 1, 1, /*  9 = Prewitt A */
                           -1, 0, 0, 0, 0, 0, 0, 0, 1, /* 10 = Roberts A */
                           -1,-2,-1, 0, 0, 0, 1, 2, 1, /* 11 = Sobel  A */
                            1, 0,-1, 1, 0,-1, 1, 0,-1, /* 12 = Prewitt B */
                            0, 0,-1, 0, 0, 0, 1, 0, 0, /* 13 = Roberts B */
                           -1, 0, 1,-2, 0, 2,-1, 0, 1  /* 14 = Sobel B  */
                          };
static int divisor[15] = {9,5,1,1,16,6,1,1,3,1,1,1,1,1,1};
static int absolute[15] = {0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1};
unsigned char *baseline;
QRgb *basergbline, *resultline;

QImage result=QImage(image.width(),image.height(),QImage::Format_RGB32);

if (image.depth() == 8)
   {
   for (int y=1;y<result.height()-1;y++)
       {
       baseline=image.scanLine(y);
       resultline=(QRgb *)result.scanLine(y);
       for (int x=1;x<result.width()-1;x++)
           *(resultline + x) =
              TIfilter8CC(image,x,filter[type],divisor[type],absolute[type],baseline);
       }

   if (type < TIFPrewitt || type > TIFSobel) return result;
   return TIrefilter(image,result,(enum TIFilterType)(type+3));
   }

if (image.depth() == 32)
   {
   for (int y=1;y<result.height()-1;y++)
       {
       basergbline=(QRgb *)image.scanLine(y);
       resultline=(QRgb *)result.scanLine(y);
       for (int x=1;x<result.width()-1;x++)
           *(resultline + x) =
              TIfilter24(image,x,filter[type],divisor[type],absolute[type],basergbline);
       }

   if (type < TIFPrewitt || type > TIFSobel) return result;
   return TIrefilter(image,result,(enum TIFilterType)(type+3));
   }

return result;
}

static unsigned int TIfilter8CC(
QImage                          image,
int                                   x,
int                                    *filter,
int                                            divisor,
int                                                    absolute,
unsigned char                                                  *baseline)
{
unsigned int result, candidate, width;
const unsigned int *table;
int collectRed, collectGreen, collectBlue;
int Xoffset[] = { -1, 0, 1, -1, 0, 1, -1, 0, 1};
int Yoffset[] = { -1, -1, -1, 0, 0, 0, 1, 1, 1};

width=image.width();
table=image.colorTable().constData();
collectRed=collectGreen=collectBlue=0;
for (int i=0;i < 9; i++)
    {
    //candidate = image.colorTable()[*(baseline + (width*Yoffset[i]) + x + Xoffset[i])];
    candidate = table[*(baseline + (width*Yoffset[i]) + x + Xoffset[i])];
    collectRed += (int)(qRed(candidate)) * filter[i];
    collectGreen += (int)(qGreen(candidate)) * filter[i];
    collectBlue += (int)(qBlue(candidate)) * filter[i];
    }
collectRed /= divisor;
if (collectRed < 0) collectRed *= absolute;
else if (collectRed > 255) collectRed = 255;
collectGreen /= divisor;
if (collectGreen < 0) collectGreen *= absolute;
else if (collectGreen > 255) collectGreen = 255;
collectBlue /= divisor;
if (collectBlue < 0) collectBlue *= absolute;
else if (collectBlue > 255) collectBlue = 255;

result=qRgb(collectRed,collectGreen,collectBlue);

return result;
}

static unsigned int TIfilter24(
QImage                         image,
int                                  x,
int                                   *filter,
int                                           divisor,
int                                                   absolute,
QRgb                                                          *basergbline)
{
unsigned int result, candidate, width;
int collectRed, collectGreen, collectBlue;
int Xoffset[] = { -1, 0, 1, -1, 0, 1, -1, 0, 1};
int Yoffset[] = { -1, -1, -1, 0, 0, 0, 1, 1, 1};

width=image.width();
collectRed=collectGreen=collectBlue=0;
for (int i=0;i < 9; i++)
    {
    candidate = *(basergbline + (width*Yoffset[i]) + x + Xoffset[i]);
    collectRed += (int)(qRed(candidate)) * filter[i];
    collectGreen += (int)(qGreen(candidate)) * filter[i];
    collectBlue += (int)(qBlue(candidate)) * filter[i];
    }
collectRed /= divisor;
if (collectRed < 0) collectRed *= absolute;
else if (collectRed > 255) collectRed = 255;
collectGreen /= divisor;
if (collectGreen < 0) collectGreen *= absolute;
else if (collectGreen > 255) collectGreen = 255;
collectBlue /= divisor;
if (collectBlue < 0) collectBlue *= absolute;
else if (collectBlue > 255) collectBlue = 255;

result=qRgb(collectRed,collectGreen,collectBlue);

return result;
}

static QImage TIrefilter(
QImage                   image,
QImage                         result,
enum TIFilterType                     type)
{
QRgb a,b;
int collectRed, collectGreen, collectBlue;
QImage part2 = TIfilterCC(image,type);
QRgb *resultline, *part2line;

for (int y=0;y<result.height();y++)
    {
    resultline=(QRgb *)result.scanLine(y);
    part2line=(QRgb *)part2.scanLine(y);
    for (int x=0;x<result.width();x++)
        {
        a=*(resultline + x);
        b=*(part2line + x);
        if ((collectRed = qRed(a) + qRed(b)) > 255) collectRed = 255;
        if ((collectGreen = qGreen(a) + qGreen(b)) > 255) collectGreen = 255;
        if ((collectBlue = qBlue(a) + qBlue(b)) > 255) collectBlue = 255;
        *(resultline + x)=
           qRgb(collectRed,collectGreen,collectBlue);
        }
    }

return result;
}