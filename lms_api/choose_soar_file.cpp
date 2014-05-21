/*
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * The Initial Developer of this code is John Hansen.
 * Portions created by John Hansen are Copyright (C) 2009-2013 John Hansen.
 * All Rights Reserved.
 *
 */

#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//#include "ev3_lcd.h"
//#include "ev3_command.h"
//

string getExtension(string filename){
	int dotIndex = filename.rfind('.');
	if(dotIndex == -1){
		return "none";
	} else if(dotIndex == filename.size()-1){
		return "none";
	}
	return filename.substr(dotIndex+1);
}

void clearScreen(void* pLcd){
	memset(pLcd, 0, LCD_BUFFER_SIZE);
}

int main()
{
	DIR *dir;
	struct dirent *ent;


  //LcdInit();
	
	vector<string> soarFiles;

	dir = opendir("./");
	if(dir != NULL){
		ent = readdir(dir);
		while(ent != NULL){
			string filename(ent->d_name);
			string ext = getExtension(filename);
			if(ext == "h"){
				soarFiles.push_back(filename);
			}
			ent = readdir(dir);
		}
		closedir(dir);
	}

	int font = FONTTYPE_NORMAL;
	int dispFile = open(LMS_LCD_DEVICE_NAME, O_RDWR);
	byte* pFB0;
	byte displayBuf[LCD_BUFFER_SIZE];
	byte* pLcd = displayBuf;

	if(dispFile != -1){
		pTmp = (byte*)mmap(NULL, LCD_BUFFER_LENGTH, PROT_READ + PROT_WRITE, MAP_SHARED, dispFile, 0);
		if(pTmp == MAP_FAILED){
			cout << "FAILED TO MAP" << endl;
		}

	for(int i = 0; i < soarFiles.size(); i++){
		dLcdDrawText(pLcd, 1, 0, 10, FONTTYPE_NORMAL, 
	}



	
 // printf("LcdText(1, 0, 100, \"hello world\");\n");
  //LcdText(1, 0, 100, "hello world");
  
  //LcdExit();
}
