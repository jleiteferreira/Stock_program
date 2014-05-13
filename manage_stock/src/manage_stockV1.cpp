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
	
	int shmid;
	int shmid_actions;
	int shmid_programas;
	int ultimo;
	bool heartbeat[100000];
	bool heartbeat_sixhours[100000];
	bool active=0;

	float separate_money[100000];
	int separate_stocks[100000];
	float separate_truemoney[100000];
	int separate_action;

	stock_actions *actions;
	stock_history *data;
	programas *dinheiro;

	time_t secs=time(0);
	tm *t=localtime(&secs);
		long int i;
		long int f;
	shmid_actions = shmget(2002,  (sizeof( stock_actions) + 50), 0666  );
	shmid = shmget(2001,  (sizeof( stock_history) + 50), 0666  | IPC_CREAT);
	shmid_actions = shmget(2002,  (sizeof( stock_actions) + 50), 0666  | IPC_CREAT);
	shmid_programas = shmget(2003,  (sizeof( programas) + 50), 0666  | IPC_CREAT);
	
	data = (stock_history *)shmat(shmid, 0, 0);
	actions = (stock_actions *)shmat(shmid_actions, 0, 0);
	dinheiro = (programas *)shmat(shmid_programas, 0, 0);
	float random;

	for (i = 0 ; i < 100000; i++ ) {
		dinheiro->posicao[i]=i;
		actions->money[i]=1000;
		actions->lock[i]=0;
		actions->stocks[i]=0;
		actions->truemoney[i]=1000;
		actions->action[i] = 0;
		separate_money[i]=1000;
		separate_stocks[i]=0;
		separate_truemoney[i]=1000;
		heartbeat[i]=0;
		heartbeat_sixhours[i]=0;
	}
  	ultimo = 1000;
	while (1) {
		cout << " TOP 10 " << "current value of stock is   " << data->current_value << endl;
		f = 1;
		usleep(100000);
		system("tput clear\n");
		for (i = 0 ; i < 100000; i++ ) {
		
			if ( actions->lock[i] == 1) {
				// Debug line
				//cout << "Received action: " << i << ":lock" << actions->lock[i] << ":action" << actions->action[i] << endl;
				if (actions->action[i] != 0) {
					separate_action = actions->action[i];
					actions->stocks[i]= actions->stocks[i] + separate_action;
					separate_stocks[i]= separate_stocks[i] + separate_action;
					heartbeat[i]=1;
					heartbeat_sixhours[i]=1;
					if (heartbeat[dinheiro->posicao[i]] ==1 && active == 1) {
						if (actions->action[i] > 0) { //buy
							ofstream myfile3;
								myfile3.open ("/home/joao/actions");
								myfile3 << "buy" << endl;
							myfile3.close();
						}
						if (actions->action[i] < 0) { //sell
							ofstream myfile3;
								myfile3.open ("/home/joao/actions");
								myfile3 << "sell" << endl;
							myfile3.close();
						}
					}

					//give him a head of 1 dollar; if he is just starting give him 1 buck
					if ( actions->money[i] == 1000) {
						actions->money[i] = 1001;
						separate_money[i] = 1001;
					}
					//cout << actions->money[i] << endl;
					//cout << actions->action[i] << endl;
					//cout << data->current_value << endl;
					//sleep(10);
					if (actions->action[i] > 0) {
					actions->money[i] = actions->money[i] - separate_action * (data->current_value + 0.0001);
					separate_money[i] = separate_money[i] - separate_action * (data->current_value + 0.0001);
					}
					if (actions->action[i] < 0) {
					actions->money[i] = actions->money[i] - separate_action * (data->current_value - 0.0001);
					separate_money[i] = separate_money[i] - separate_action * (data->current_value - 0.0001);
					}
					//cout << actions->money[i] << endl;
					//cout << actions->action[i] << endl;
					//cout << data->current_value << endl;
					actions->lock[i]   = 0;
					actions->action[i]  = 0;
				} 
				
				if( actions->lock[i] == 1) {
					actions->lock[i]   = 0;
					actions->action[i]  = 0;
					heartbeat[i]=1;
				}
				
			}
			float temp;
			actions->truemoney[i] = actions->money[i] + actions->stocks[i]*data->current_value;

			if (actions->money[i] != separate_money[i] ) {
				ofstream myfile3;
 					 myfile3.open ("/home/joao/tokill.txt");
 					 myfile3 << dinheiro->posicao[i];
  				myfile3.close();
			}
			if (actions->stocks[i] != separate_stocks[i] ) {
				ofstream myfile2;
 					 myfile2.open ("/home/joao/tokill.txt");
 					 myfile2 << dinheiro->posicao[i];
  				myfile2.close();
			}

			if (i < 99999) {
				if ( actions->truemoney[dinheiro->posicao[i]] < actions->truemoney[dinheiro->posicao[i+1]]) {
					temp = dinheiro->posicao[i];
					dinheiro->posicao[i] = dinheiro->posicao[i+1];
					dinheiro->posicao[i+1] = temp;
				}
			}
		
			if(heartbeat[dinheiro->posicao[i]] ==1 ) { if (f < 10) { cout << i << "   " << dinheiro->posicao[i] << " with " << actions->truemoney[dinheiro->posicao[i]] << " Money " << actions->money[dinheiro->posicao[i]] << " stocks " << actions->stocks[dinheiro->posicao[i]] << endl;}
				if (f == 1) {
					ofstream myfile;
						myfile.open ("/home/joao/location.txt");
						myfile << dinheiro->posicao[i];
					myfile.close();
					if (active == 0) {
						if  (actions->truemoney[dinheiro->posicao[i]] > 1004 && actions->stocks[dinheiro->posicao[i]] ==0) {
							active == 1;
						}
					}
				}
				f++;
				ultimo = i;
			}
		}
		
		cout << "Last one at " << ultimo << "   " << dinheiro->posicao[ultimo] << " with " << actions->truemoney[dinheiro->posicao[ultimo]] <<  endl;
		cout << "There are " << f-1 << " number of programs running" << " Active:"<< active << endl;


		if( f > 1000 ) {

			ofstream myfile2;
			myfile2.open ("/home/joao/location_ultimo.txt");
			myfile2 << dinheiro->posicao[ultimo];
			myfile2.close();

			i=rand() % 100000 + 1;
			random=rand();
			if (heartbeat[i]==0 && random < 0.20) {
				myfile2.open ("/home/joao/tokill.txt");
				myfile2 << i;
				myfile2.close();
			} else if (random > 0.99) {
				if (dinheiro->posicao[1]!=i && dinheiro->posicao[2]!=i  && dinheiro->posicao[3]!=i  && dinheiro->posicao[4]!=i && dinheiro->posicao[5]!=i) {
					myfile2.open ("/home/joao/tokill.txt");
					myfile2 << i;
					myfile2.close();
				}
			} else if ( random < 0.90 && random > 0.80 ) {
				myfile2.open ("/home/joao/tokill.txt");
				myfile2 << dinheiro->posicao[ultimo];
				myfile2.close();
			}
		}
		i=rand() % 100000 + 1;
		if (heartbeat[i]==0) {
			ofstream myfile3;
			myfile3.open ("/home/joao/available.txt");
		 	myfile3 << i;
  			myfile3.close();
		}
		ifstream ifile("/home/joao/killed.txt");
		if (ifile.is_open() == true) {
 			ifile >> i;
			cout << "o que morreu foi " << i << endl;
			ifile.close();
			if (i>0){
			actions->money[i]=1000;
			actions->lock[i]=0;
			actions->stocks[i]=0;
			actions->truemoney[i]=1000;
			separate_money[i]=1000;
			separate_stocks[i]=0;
			separate_truemoney[i]=1000;
			heartbeat[i]=0;
			heartbeat_sixhours[i]=0;
			}
		}		



	       // time_t twelve_hours;
  		//twelve_hours = time (NULL) % (41600);
		//if (twelve_hours == 1 && 0 != Tendencia_10_minutos ( data )) {
		//	cout << " asdsadsad " << endl;
		//	for (i = 0 ; i < 100000; i++ ) {
		//		if (heartbeat_sixhours[i]==0) {heartbeat[i]=0;}
		//		heartbeat_sixhours[i]=0;
		//		if (heartbeat_sixhours[i]==0 &&  heartbeat[i]==0 ) { //this baby is dead. find a way to handle it
		//			ofstream myfile2;
		//			myfile2.open ("/home/joao/tokill.txt");
		//			myfile2 << i;
		//	     		myfile2.close();
		//			usleep(100000);
		//		}
		//	}

		//}
		
	}

}

