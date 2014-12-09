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

#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QLabel>
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QTextStream>
#include <QScrollArea>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QRadioButton>

class dlgsettings : public QWidget
{
	Q_OBJECT

signals:
	void printcursettings(double sigma, float k, int min_size);

public:
	dlgsettings()
	{
	}
	dlgsettings(double sigma, float k, int min_size);
	void savesettings(void)
	{
		emit printcursettings(sigma, k, min_size);
	}

private slots:
	void accept(void);

private:
	// Control Variables
	double sigma;
	float k;
	int min_size;

	QGroupBox *segmentationgroup;
	QVBoxLayout *segmentationbox;
	QLabel *labelsegmentationSigma;
	QLabel *labelsegmentationK;
	QLabel *labelsegmentationMin_size;
	QLineEdit *editsegmentationSigma;
	QLineEdit *editsegmentationK;
	QLineEdit *editsegmentationMin_size;
};
#endif





