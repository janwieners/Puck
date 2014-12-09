/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

/* basic image I/O */

#ifndef PNM_FILE_H
#define PNM_FILE_H

#include <QImage>
#include "image.h"

static image<rgb> *loadQImage(QImage *inputimage)
{
	// Convert QImage
	QRgb *RGB;
	image<rgb> *im = new image<rgb>(inputimage->width(), inputimage->height());
	for(int y=0; y<inputimage->height(); y++)
		for(int x=0; x<inputimage->width(); x++)
		{
			RGB=(QRgb *)inputimage->scanLine(y) + x;
			imRef(im, x, y).r=qRed(*RGB);
			imRef(im, x, y).g=qGreen(*RGB);
			imRef(im, x, y).b=qBlue(*RGB);
		}
	return im;
}

QImage * convertImageToQImage(image<rgb> *inputimage, QImage *outputimage)
{
	for(int y=0; y<inputimage->height(); y++)
		for(int x=0; x<inputimage->width(); x++)
			outputimage->setPixel(x, y, qRgb(imRef(inputimage, x, y).r, imRef(inputimage, x, y).g, imRef(inputimage, x, y).b));

	return outputimage;
}

#endif
