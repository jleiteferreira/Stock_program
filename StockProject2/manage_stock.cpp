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

	stock_actions *actions;
	stock_history *data;
	programas dinheiro;

	time_t secs=time(0);
	tm *t=localtime(&secs);
		long int i;
	
	shmid = shmget(2001,  (sizeof( stock_history) + 50), 0666  | IPC_CREAT);
	shmid_actions = shmget(2002,  (sizeof( stock_actions) + 50), 0666  | IPC_CREAT);
	
	data = (stock_history *)shmat(shmid, 0, 0);
	actions = (stock_actions *)shmat(shmid_actions, 0, 0);

	for (i = 0 ; i < 100000; i++ ) {
		dinheiro.posicao[i]=i;
		actions->money[i]=1000;
		actions->lock[i]=0;
		actions->stocks[i]=0;
		actions->truemoney[i]=1000;
	}
  
	while (1) {
		sleep(1);

		for (i = 0 ; i < 100000; i++ ) {
		
			if( actions->lock[i] == 1) {
				if (actions->action[i] != 0) {
					actions->stocks[i]= actions->stocks[i] + actions->action[i];
					cout << actions->money[i] << endl;
					cout << actions->action[i] << endl;
					cout << data->current_value << endl;
					actions->money[i] = actions->money[i] - actions->action[i] * data->current_value;
					cout << actions->money[i] << endl;
					cout << actions->action[i] << endl;
					cout << data->current_value << endl;
					actions->lock[i]   = 0;
					actions->action[i]  = 0;
				} 
				
				if( actions->lock[i] == 1) {
					actions->lock[i]   = 0;
				}
				
			}
			float temp;
			actions->truemoney[i] = actions->money[i] + actions->stocks[i]*data->current_value;
			if (i < 99999) {
				if ( actions->truemoney[dinheiro.posicao[i]] < actions->truemoney[dinheiro.posicao[i+1]]) {
					temp = dinheiro.posicao[i];
					dinheiro.posicao[i] = dinheiro.posicao[i+1];
					dinheiro.posicao[i+1] = temp;
				}
			}
		}
		
		
		cout << " TOP 10 " << endl;
		cout << " 0 " << dinheiro.posicao[0] << " with " << actions->truemoney[dinheiro.posicao[0]] <<  endl;
		cout << " 1 " << dinheiro.posicao[1] << " with " << actions->truemoney[dinheiro.posicao[1]] <<  endl;
		cout << " 2 " << dinheiro.posicao[2] << " with " << actions->truemoney[dinheiro.posicao[2]] <<  endl;
		cout << " 3 " << dinheiro.posicao[3] << " with " << actions->truemoney[dinheiro.posicao[3]] <<  endl;
		cout << " 4 " << dinheiro.posicao[4] << " with " << actions->truemoney[dinheiro.posicao[4]] <<  endl;
		cout << " 5 " << dinheiro.posicao[5] << " with " << actions->truemoney[dinheiro.posicao[5]] <<  endl;
		cout << " 6 " << dinheiro.posicao[6] << " with " << actions->truemoney[dinheiro.posicao[6]] <<  endl;
		cout << " 7 " << dinheiro.posicao[7] << " with " << actions->truemoney[dinheiro.posicao[7]] <<  endl;
		cout << " 8 " << dinheiro.posicao[8] << " with " << actions->truemoney[dinheiro.posicao[8]] <<  endl;
		cout << " 9 " << dinheiro.posicao[9] << " with " << actions->truemoney[dinheiro.posicao[9]] <<  endl;
		cout << " 10 " << dinheiro.posicao[10] << " with " << actions->truemoney[dinheiro.posicao[10]] <<  endl;
		
		cout << " Botton 10 " << endl;
		cout << " 1 " << dinheiro.posicao[100000-1] << " with " << actions->truemoney[dinheiro.posicao[100000-1]] <<  endl;
		cout << " 2 " << dinheiro.posicao[100000-2] << " with " << actions->truemoney[dinheiro.posicao[100000-2]] <<  endl;
		cout << " 3 " << dinheiro.posicao[100000-3] << " with " << actions->truemoney[dinheiro.posicao[100000-3]] <<  endl;
		cout << " 4 " << dinheiro.posicao[100000-4] << " with " << actions->truemoney[dinheiro.posicao[100000-4]] <<  endl;
		cout << " 5 " << dinheiro.posicao[100000-5] << " with " << actions->truemoney[dinheiro.posicao[100000-5]] <<  endl;
		cout << " 6 " << dinheiro.posicao[100000-6] << " with " << actions->truemoney[dinheiro.posicao[100000-6]] <<  endl;
		cout << " 7 " << dinheiro.posicao[100000-7] << " with " << actions->truemoney[dinheiro.posicao[100000-7]] <<  endl;
		cout << " 8 " << dinheiro.posicao[100000-8] << " with " << actions->truemoney[dinheiro.posicao[100000-8]] <<  endl;
		cout << " 9 " << dinheiro.posicao[100000-9] << " with " << actions->truemoney[dinheiro.posicao[100000-9]] <<  endl;
		cout << " 10 " << dinheiro.posicao[100000-10] << " with " << actions->truemoney[dinheiro.posicao[100000-10]] <<  endl;
		
	}

}
