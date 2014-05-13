/***************************************************************************
 *   Copyright (C) 2010 by joao ferreira   *
 *   joao@localhost.localdomain   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <bitset>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Primeira fase
// Fazer load da situacao dos programas
// Analisar recursos do sistema, pequeno programa returna 1 se CPU menos de 50%, Memoria menos de 50%
// Criar ficheiro na localizacao ram do sistema, em cada 100 fazer uma hardcopy para o disco

// Verificar programas a usar mto CPU ou Memoria, matar os mesmos

// Se sistema tiver recursos criar programa,

// A cada 0.8 modificar a cada 0.2 aumentar

using namespace std;

int create_child(char *child, char *parent) {
bool modify=false;
bool increase=false;
bool success=false;
long random;
int random2;
// Decide to modify or increase  
  srand ( time(NULL) );
  random = rand() % 10 + 1;
if ( random >= 8) {
	modify=true;
}
if ( random < 8 ) {
	increase=true;
}
   cout << "parent " << parent << " child " << child << endl;
  string path="/home/joao/memory/";
  string pathchild = path + child;
  string pathparent = path + parent;
  string launch=" 0 test";
  string launchchild= pathchild + launch;
  string launchparent= pathparent + launch;
  string changemod= "chmod 775 ";
  string chamgemodchild = changemod + pathchild;

  char *charparent=new char[pathparent.size()+1];
         charparent[pathparent.size()]=0;
          memcpy(charparent,pathparent.c_str(),pathparent.size());
cout << charparent << endl;
  char *charchild=new char[pathchild.size()+1];
         charchild[pathchild.size()]=0;
          memcpy(charchild,pathchild.c_str(),pathchild.size());

  char *charlaunchchild=new char[launchchild.size()+1];
         charlaunchchild[launchchild.size()]=0;
          memcpy(charlaunchchild,launchchild.c_str(),launchchild.size());

  char *charlaunchparent=new char[launchparent.size()+1];
         charlaunchparent[launchparent.size()]=0;
          memcpy(charlaunchparent,launchparent.c_str(),launchparent.size());

  char *charchamgemodchild=new char[chamgemodchild.size()+1];
         charchamgemodchild[chamgemodchild.size()]=0;
          memcpy(charchamgemodchild,chamgemodchild.c_str(),chamgemodchild.size());

while ( success == false ) {
	//ifstream original;

	// if(ifstream(charparent)) {
	ifstream original(charparent, ios::binary);
	//} else {
	//	cout << "Hey file does not exist" << endl;
	//	break;
	//}
	ofstream child(charchild, ios::binary);
	struct stat results;
		stat( charparent, &results);
	
	// select where file is going to be changed
		cout << "file size is " << results.st_size << endl;
		srand ( time(NULL) );
		random = rand() % results.st_size;
		cout << " file is going to be change at " << random << endl << "Increase " << increase << endl;
		char  * ch;
		char level[4];
		char thetime[4];
		int i;
		i=1;
		if(original.is_open() && child.is_open() )
		{
			while (!original.eof()) {
				//original.get(level);
				ch = new char [1];
				original.read(ch , 1);
				if(!original.eof()){
					if (i == random) {
						if (increase==true) {
						child.write(ch, 1);
						}
						srand ( time(NULL) );
						random2 = rand() % 256;
						sprintf(ch,"%c",random2);
						//cout << " this is the char " << ch << " this is the int " << random2 << endl;
						child.write(ch, 1);
					} else {
					child.write(ch, 1);
					}
				}
				i++;
			}
		}
		original.close();
		child.close();
		int status=1;
		string working ("ID");
		string bla = "bla";
		status = system(charchamgemodchild);
		cout << "Launching new child" << charlaunchchild << endl;
		status = system(charlaunchchild);
		
		if (status == 0) {
			cout << "success" <<endl;
			success = true;
		} else {
			cout << "failure" << endl;
		}
	}
}

int main(int argc, char *argv[])
{
  cout << "Hello, world!" << endl;
   char *child = argv[1];
   char *parent = argv[2];
   if (strcmp (child,parent) == 0) { cout << " kid cannot have the same name as father " << endl; return 0;}
  //cout << "parent " << parent << " child " << child << endl;
  string path="/home/joao/memory/";
  string pathparent = path + parent;
 //cout << charparent << endl;
  char *charparent=new char[pathparent.size()+1];
         charparent[pathparent.size()]=0;
          memcpy(charparent,pathparent.c_str(),pathparent.size());

 cout << charparent << endl;
   if(ifstream(charparent)){ create_child(child,parent);}
  return EXIT_SUCCESS;
}
