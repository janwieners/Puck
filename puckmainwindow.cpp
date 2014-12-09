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

#include "puckmainwindow.h"

puckMainWindow::puckMainWindow(QWidget *parent) : QMainWindow(parent)
{
    prepareWindow();
}

puckMainWindow::~puckMainWindow()
{
}

void puckMainWindow::loadXCDLbase(void)
{
	filenameXCDLbase=QFileDialog::getOpenFileName(this,
            "Load", ".", "XCDL Files (*.xcdl)");
    if(filenameXCDLbase.isEmpty() == true)
    {
            //QMessageBox::warning(this,"Warning", "You didn't choose an image", "Proceed");
    }
	QString consoleText=">> Processed XCDL ";
	consoleText+=filenameXCDLbase;
	consoleLeft->setText(consoleText);
}

void puckMainWindow::loadXCDLtarget(void)
{
	filenameXCDLtarget=QFileDialog::getOpenFileName(this,
            "Load", ".", "XCDL Files (*.xcdl)");
    if(filenameXCDLtarget.isEmpty() == true)
    {
            //QMessageBox::warning(this,"Warning", "You didn't choose an image", "Proceed");
    }
	QString consoleText=">> Processed XCDL ";
	consoleText+=filenameXCDLtarget;
	consoleRight->setText(consoleText);
}
