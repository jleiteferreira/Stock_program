#include <iostream>
#include <fstream>
#include <time.h>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>


using namespace std;


typedef struct stock_history { 
	string name; 
	float current_value;
	float day_month_year[32][12][10][3];
	float daily[24][60][6];
	float daily_history[24][60][6];
	long int number_of_days_running;
	} stock_history; 

int GetIntVal(string strConvert) {
  int intReturn;

  intReturn = atoi(strConvert.c_str());

  return(intReturn);
}

float GetfloatVal(string strConvert) {
  float floatReturn;

  floatReturn = atof(strConvert.c_str());

  return(floatReturn);
}


int main() {
	
	int dayi;
	int monthi;
	int yeari;
	float value;
	int shmid;
	
	time_t secs=time(0);
	tm *t=localtime(&secs);

	stock_history *Euro_info;
	
	shmid = shmget(2001, (sizeof( stock_history) + 50), 0666);
	shmctl(shmid, IPC_RMID, 0);
	shmid = shmget(2001,  (sizeof( stock_history) + 50), 0666 | IPC_CREAT);
	Euro_info = (stock_history *) shmat( shmid, NULL, 0 );
	int i;
	i = 0;
	
	Euro_info->number_of_days_running = 0;

	int f;
	for (i = 0 ; i < 32; i++ ) {
		for (f = 0 ; f < 12; f++ ) {
			for (yeari = 0 ; yeari < 10; yeari++ ) {
				day_month_year[i][d][yeari][0]=0
				day_month_year[i][d][yeari][1]=0
				day_month_year[i][d][yeari][2]=0
			}
		}
	}
	int =0;
	while (1) {
		i++;
		sleep(10);
		string line;
		ifstream myfile ("stockvalue.txt");
		if (myfile.is_open())
		{
			while (! myfile.eof() )
			{
				getline (myfile,line);
				std::stringstream ss;
				ss << line;
				cout << line << endl;
				ss >> Euro_info->current_value;
				//eurodollar.current_value = eurodollar.current_value*100;
				cout << Euro_info->current_value << endl;


				secs=time(0);		
				t=localtime(&secs);
				printf("%04d-%02d-%02d %02d:%02d:%02d\n",
				t->tm_year+1900,t->tm_mon+1,t->tm_mday,
				t->tm_hour,t->tm_min,t->tm_sec);
				Euro_info->daily[t->tm_hour][t->tm_min][int (t->tm_sec /10 )]= Euro_info->current_value;
						
				Euro_info->daily_history[t->tm_hour][t->tm_min][int (t->tm_sec /10 )] = float ( (Euro_info->current_value + Euro_info->daily_history[t->tm_hour][t->tm_min][int (t->tm_sec /10 )] * float ( Euro_info->number_of_days_running ))/( float ( Euro_info->number_of_days_running )  + 1) );
						
				printf("%04d-%02d-%02d %02d:%02d:%02d\n",
							t->tm_year+1900,t->tm_mon+1,t->tm_mday,
							t->tm_hour,t->tm_min,int (t->tm_sec /10 ));
				cout << "The value is " << Euro_info->current_value << " and the average value for the day is " << Euro_info->daily_history[t->tm_hour][t->tm_min][int (t->tm_sec /10 )] << endl;
			}
			myfile.close();
			
			if ( int (t->tm_hour) == 0 && int (t->tm_min) == 0 && (int (t->tm_sec /10 )) == 0 ) {
				
				
				Euro_info->number_of_days_running++;
				cout << " This script is running for " << Euro_info->number_of_days_running << endl;
			}
		}
		if (i == 300)
		{
			i = 1;
			ifstream myfile ("stockvaluehistory.txt");
			while (! myfile.eof() )
			{
			getline (myfile,line);
			cout << line << endl;
			if ( line.size() > 52 ) {
				string year;
				string month;
				string day;
				string opening;
				string high;
				string low;
				year = line.substr (7,4);
				month = line.substr (12,2);
				day = line.substr (15,2);
				opening = line.substr (20,8);
				high = line.substr (31,8);
				low = line.substr (42,8);

			 
				yeari = GetIntVal(year);
				yeari = yeari - 2007;	
				
				dayi = GetIntVal(day);
				
				monthi = GetIntVal(month);
				
				value = GetfloatVal(opening);
				Euro_info->day_month_year[dayi][monthi][yeari][1] = value;
				value = GetfloatVal(high);
				Euro_info->day_month_year[dayi][monthi][yeari][2] = value;
				value = GetfloatVal(low);
				Euro_info->day_month_year[dayi][monthi][yeari][0] = value;
				}
			}
			myfile.close();
			


		}

		i++;
	}
}
