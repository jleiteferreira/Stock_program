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
#include <cstdlib>

using namespace std;

#include <iostream>
#include <fstream>
#include <time.h>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <math.h>
#include <IO.h>
using namespace std;

typedef struct programas { 
	int posicao[100000];
	int activos;
	} programas; 

typedef struct stock_actions { 
	string name; 
	bool lock[100000];
	int action[100000];
	float money[100000];
	int stocks[100000];
	float truemoney[100000];
	} stock_actions; 

typedef struct stock_history { 
	string name; 
	float current_value;
	float day_month_year[32][12][3][3];
	float daily[24][60][6];
	float daily_history[24][60][6];
	long int number_of_days_running;
	} stock_history; 

int GetIntVal(string strConvert) {
  int intReturn;

  // NOTE: You should probably do some checks to ensure that
  // this string contains only numbers. If the string is not
  // a valid integer, zero will be returned.
  
  intReturn = atoi(strConvert.c_str());

  return(intReturn);
}

float GetfloatVal(string strConvert) {
  float floatReturn;

  // NOTE: You should probably do some checks to ensure that
  // this string contains only numbers. If the string is not
  // a valid integer, zero will be returned.
  
  floatReturn = atof(strConvert.c_str());

  return(floatReturn);
}

float Tendencia_10_minutos ( stock_history *data_function )
{
	float tendencia;
	time_t secs=time(0);
	tm *t=localtime(&secs);
	secs=time(0);		
	t=localtime(&secs);
	tendencia = 100*data_function->current_value - 100*data_function->daily[t->tm_hour][t->tm_min-10][int (t->tm_sec /10 )];
	return tendencia;
}

int main() {
	

	while (1) {
		f = 1;
		usleep(1000000);


	cout cout << "Enter names and weight, press 'Done' when finished." << endl;
		while(name.compare ("Done") !=0)
		{
		cin >> name >> weight;
		cout << name << " this is the name" << endl;
		}
	}
}