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

int main(int argc, char *argv[]) {
	
	
	int ID;
	ID = atoi(argv[1]);
	
	cout << " MY ID IS " << ID << endl;
	
	int shmid;
	int shmid_actions;

	stock_actions *actions;
	stock_history *data;
	float the_ultimate_funtion;
	int state = 0;
	int stocks_to_buy;
	time_t secs=time(0);
	tm *t=localtime(&secs);

	key_t key; /* key that will be used for buying and selling */ 
	int msgflg; /* msgflg to be passed to msgget() */ 
	int msqid; /* return value from msgget() */ 

	
	shmid = shmget(2001,  (sizeof( stock_history) + 50), 0666 );
	shmid_actions = shmget(2002,  (sizeof( stock_actions) + 50), 0666 );
	
	data = (stock_history *)shmat(shmid, 0, 0);
	actions = (stock_actions *)shmat(shmid_actions, 0, 0);
	
	while (1) {
		sleep(10);
		cout << " My real money is " << actions->truemoney[ID] << endl;
		cout << " I have this many stocks " << actions->stocks[ID] << endl;
		cout << " My money is  " << actions->money[ID] << endl;
		cout << data->current_value <<  " value from data " <<  endl;
		
		// Funcao se decide comprar, esperar ou vender, tem de ser um conjunto de formulas. 
		// As formulas funcionaram como multiplicacao. Se o resultado for superior a um compra
		// se o resultado for inferior a 1 venda
		
		cout << " Deciding if should buy, sell, wait or hold stock " << endl;
		secs=time(0);		
		t=localtime(&secs);
		if ( state == 0 ) {
			//calcular variavel ultimos 10 min
			//1 determinal que horas sao
			// determine if we buy or sell
			// buy if better than 1.1 sell if lower than 0.9
			cout << Tendencia_10_minutos ( data ) << " Tendencia " << endl;
			the_ultimate_funtion = exp ( Tendencia_10_minutos ( data ) ) ;
			cout << the_ultimate_funtion  << " Ultimate value " << endl;
			
			if ( the_ultimate_funtion > 1.02 ) {
				// lets buy
				// Calculate the number of stocks you can by
				cout << " let's buy " << endl;
				stocks_to_buy =  int ( actions->money[ID] / (data->current_value));
				cout << " we are going to buy " << stocks_to_buy << endl;
				cout << " send msg " << endl;
				actions->action[ID]=stocks_to_buy;
				actions->lock[ID]=1;
				while ( actions->lock[ID]==1 ) {
					
					sleep(1);
					state=1;
				}
			}
		}
		
		if ( state == 1 ) {
			
			cout << Tendencia_10_minutos ( data ) << " Tendencia " << endl;
			the_ultimate_funtion = exp ( Tendencia_10_minutos ( data ) ) ;
			cout << the_ultimate_funtion  << " Tendencia " << endl;
			
			if ( the_ultimate_funtion < 1 ) {
				// lets sell
				stocks_to_buy =  int ( actions->money[ID] / (data->current_value));

				actions->lock[ID]=1;
				actions->action[ID]= -actions->stocks[ID] ;
				while ( actions->lock[ID]==1 ) {
					
					sleep(1);
					state=0;
				}
			}
			
			
			
			
			
		}
		
		
	}

}
