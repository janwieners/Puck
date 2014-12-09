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

#include "settingsdlg.h"

dlgsettings::dlgsettings(double sigma_input, float k_input, int min_size_input)
{
	// Initialization
	sigma=sigma_input;
	k=k_input;
	min_size=min_size_input;
	
	// + Kohonen Map
	segmentationgroup=new QGroupBox("Graph Based Segmentation (Huttenlocher-Felzenszwalb)");
	labelsegmentationSigma=new QLabel("Sigma");
	labelsegmentationK=new QLabel("K");
	labelsegmentationMin_size=new QLabel("Min Size");
	
	editsegmentationSigma=new QLineEdit;
	editsegmentationK=new QLineEdit;
	editsegmentationMin_size=new QLineEdit;

	segmentationbox=new QVBoxLayout;
	segmentationbox->addWidget(labelsegmentationSigma);
	segmentationbox->addWidget(editsegmentationSigma);
	segmentationbox->addWidget(labelsegmentationK);
	segmentationbox->addWidget(editsegmentationK);
	segmentationbox->addWidget(labelsegmentationMin_size);
	segmentationbox->addWidget(editsegmentationMin_size);	

	// Set
	editsegmentationSigma->setText(QString::number(sigma));
	editsegmentationK->setText(QString::number(k));
	editsegmentationMin_size->setText(QString::number(min_size));

	segmentationgroup->setLayout(segmentationbox);
	// - Kohonen

	// Finalize
	QGridLayout *tmpBox=new QGridLayout;
	tmpBox->addWidget(segmentationgroup);

	QPushButton *acceptbutton=new QPushButton("Save Settings");
	connect(acceptbutton, SIGNAL(clicked()), this, SLOT(accept()));

	tmpBox->addWidget(acceptbutton);

	setLayout(tmpBox);
}

void dlgsettings::accept(void)
{
	QString tempstring="";

	tempstring=editsegmentationSigma->text();
	sigma=tempstring.toDouble(); //toFloat()

	tempstring=editsegmentationK->text();
	k=tempstring.toFloat();

	tempstring=editsegmentationMin_size->text();
	min_size=tempstring.toInt();

	savesettings();	
	close();
}


	
