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

#ifndef EDGEDETECTION_H
#define EDGEDETECTION_H


enum TIFilterType { TIFLowpass1, TIFLowpass2, TIFHighpass1, TIFHighpass2,
                    TIFWMean1, TIFWMean2, TIFLaplace1, TIFLaplace2, TIFLaplace3,
                    TIFPrewitt, TIFRoberts, TIFSobel };

QImage TIfilterCC(QImage image, enum TIFilterType type);

static unsigned int TIfilter8CC(QImage image,int x,
       int *filter, int divisor, int absolute,unsigned char *baseline);
static unsigned int TIfilter24(QImage image,int x,
       int *filter, int divisor, int absolute,QRgb *basergbline);
static QImage TIrefilter(QImage image,QImage result,enum TIFilterType type);



#endif // EDGEDETECTION_H
