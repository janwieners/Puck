#ifndef SEGMENT_H
#define SEGMENT_H

QImage segmentImage(QImage *inputimage, float inputsigma, float inputk, int inputmin_size);
QImage identifyObjects(QImage *inputimage, QString filename, QString xcdlFile, QVector <QVector<float>> *significantObjectsVector, bool processingRightImage);
#endif // SEGMENT_H
