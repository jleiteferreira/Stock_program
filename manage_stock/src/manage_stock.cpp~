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
#include <cstddef>
#include <ctime>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>


static struct termios old,new2;
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
   char password[100000][10];
   } stock_actions; 

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
   tm *t=gmtime(&secs);
   secs=time(0);      
   t=gmtime(&secs);
   tendencia = 100*data_function->current_value - 100*data_function->daily[t->tm_hour][t->tm_min-10][int (t->tm_sec /10 )];
   return tendencia;
}


int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}


/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new2 = old; /* make new settings same as old settings */
  new2.c_lflag &= ~ICANON; /* disable buffered i/o */
  new2.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new2); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}



bool get_input ( char *buffer, std::size_t size, int timeout )
{
  std::time_t start = std::time ( 0 );
  std::size_t n = 0;

  for ( ; ; ) {
    if ( n == 0 && std::difftime ( std::time ( 0 ), start ) >= timeout )
      return false;

    if ( kbhit() ) {
      if ( n == size - 1 )
        break;

      char ch = (int)getche();

      if ( ch == '\r' ) {
        buffer[n++] = '\n';
        break;
      }
      else
        buffer[n++] = ch;
    }
  }

  buffer[n] = '\0';

  return true;
} 

int input()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 10;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

int main() {
   
 int shmid;
 int shmid_actions;
 int shmid_programas;
 int ultimo;
   bool heartbeat[100000];
   bool reported_dead[100000];
   bool heartbeat_sixhours[100000];
   int heartbeat_hour[100000];
   int activity[100000];
   bool active=0;
   bool exported_father;

   float separate_money[100000];
   int separate_stocks[100000];
   float separate_truemoney[100000];
   int separate_action;
   char separate_password[100000][10];
   char empty[8] ="empty";

   stock_actions *actions;
   stock_history *data;
   programas *dinheiro;

   // TIME VARIRABLES 
   struct tm *current;
   time_t now=time(0);
   char buffer[100];
   long int six_hours = 126000;


   long int i;
   long int to_kill;
   long int f;
   float prev_f = 1;

   float average_money = 10000;

   shmid_actions = shmget(2002,  (sizeof( stock_actions) + 50), 0666  );
   shmid = shmget(2001,  (sizeof( stock_history) + 50), 0666  | IPC_CREAT);
   shmid_actions = shmget(2002,  (sizeof( stock_actions) + 50), 0666  | IPC_CREAT);
   shmid_programas = shmget(2003,  (sizeof( programas) + 50), 0666  | IPC_CREAT);
   
   data = (stock_history *)shmat(shmid, 0, 0);
   actions = (stock_actions *)shmat(shmid_actions, 0, 0);
   dinheiro = (programas *)shmat(shmid_programas, 0, 0);
   float random;
   float tendencia;

   
   string command;
   string command1;
   string command2;
   string command3;
   string garbage;
   string time_stamp;
   string line;

   float temp;

   // SHOW COMMAND VARIABLLES
   bool DISPLAY_TOP = false;
   bool DISPLAY_BOTTOM = false;
   bool DISPLAY_STOCK = false;
   bool DISPLAY_ALL = false;
   bool DISPLAY_FATHER = false;

   for (i = 0 ; i < 100000; i++ ) {
      dinheiro->posicao[i]=i;
      actions->money[i]=1000;
      actions->lock[i]=0;
      actions->stocks[i]=0;
      actions->truemoney[i]=1000;
      actions->action[i] = 0;
      actions->password[i][0] = 0;
      separate_money[i]=1000;
      separate_stocks[i]=0;
      separate_truemoney[i]=1000;
      strcpy (separate_password[i],empty);
      heartbeat[i]=0;
      activity[i]=0;
      heartbeat_sixhours[i]=0;
      heartbeat_hour[i]=0;
      reported_dead[i]=0;
   }

   ultimo = 1000;

   /////////////////////////////////////////
   // OPEN LOG
   /////////////////////////////////////////
   ofstream LOG;
      LOG.open ("/tmp/MEGA_LOG" , ios::out | ios::app );

      current = gmtime(&now);
      sprintf (buffer, "%i/%i/%i %i:%i %i::",current->tm_mday, (current->tm_mon + 1), (current->tm_year + 1900),  current->tm_hour , current->tm_min, current->tm_sec );
      time_stamp = buffer;
      LOG << time_stamp << "TEST STARTED" << endl;

   /////////////////////////////////////////
   // START WHILE LOOP 
   /////////////////////////////////////////

   while (1) {

      //////////////////////////////////////////
      // Sleep for 0.1 seconds
      //////////////////////////////////////////

	usleep(100000);

      /////////////////////////////////////////
      // Set Variables that always reset
      /////////////////////////////////////////
	
      f = 1;
      exported_father = false;

      /////////////////////////////////////////
      // Set a Time variable to time stamp actions
      /////////////////////////////////////////

      now = time(0);
      current = gmtime(&now);
      sprintf (buffer, "%i/%i/%i %i:%i %i::",current->tm_mday, (current->tm_mon + 1), (current->tm_year + 1900),  current->tm_hour , current->tm_min, current->tm_sec );
      time_stamp = buffer;

      /////////////////////////////////////////
      // Set a Time variable to time stamp actions
      /////////////////////////////////////////

      tendencia = Tendencia_10_minutos(data);

      /////////////////////////////////////////
      // Update password list 
      /////////////////////////////////////////
 
      ifstream password_file("/home/joao/password");
      if (password_file.is_open() == true) {
        while ( password_file.good() )
        {
         getline (password_file,line);
         size_t pos;
	 istringstream iss(line);
	 iss >> command3;
	 stringstream ss(command3);
	 //pos = line.find(" ");
	 //i = atoi(line.substr (0,pos));
	 //separate_password[i] = line.substr (pos);
         char password;
	 if ((ss >> i ) && i > 0 && i < 100000) {
		iss >> command3;
                stringstream ss(command3);
		ss >> separate_password[i];
         }
        }
	password_file.close();
      } 

      /////////////////////////////////////////
      // CHECK FOR INPUT
      /////////////////////////////////////////

    if (input()) {
         getline (cin, command);
         cout << "Command entered:" << command << endl;
	 LOG << time_stamp << "Command entered:" << command << endl;
       istringstream iss(command);
       if (iss) { iss >> command1;}
       if (iss) { iss >> command2;}
       if (iss) { iss >> command3;}
       if (iss) { iss >> garbage;}
       if (iss) { cout << "there is a limit of 3 commands" << endl;}
       if (!command1.compare ("exec")) { 
             if (!command2.compare ("live") && !command3.compare ("on") ) { 
                  cout << time_stamp << "Going Live" << endl;
                  LOG << time_stamp <<"Going Live" << endl;
                  active = 1;
                  }
               else if (!command2.compare ("live") && !command3.compare ("off") ) { 
                  cout << time_stamp << "Going OFF" << endl;
                  LOG << time_stamp <<"Going OFF" << endl;
                  active = 0;
                  }
               else if (!command2.compare ("clear")) { system("tput clear\n");}
               else if (!command2.compare ("kill") )  {  
                    stringstream ss(command3);
		    if ((ss >> i ) && i > 0 && i < 10000) {
                      ofstream myfile2; myfile2.open ("/home/joao/tokill.txt", ios::app);
                      LOG << time_stamp << "PROGRAM:" << i << " AT:" << dinheiro->posicao[i] << " Killed manually" << endl; 
                      myfile2 << i ; usleep(500000); myfile2.close();
		    }
               }
               else if (!command2.compare ("reset") )  {  
                    stringstream ss(command3);
		    if ((ss >> i ) && i > 0 && i < 10000) {
                      LOG << time_stamp << "PROGRAM:" << i << " AT:" << dinheiro->posicao[i] << " Reset manually" << endl; 
        	      actions->money[i]=average_money; actions->lock[i]=0; actions->stocks[i]=0; actions->truemoney[i]=average_money; separate_money[i]=average_money;
        	      separate_stocks[i]=0;separate_truemoney[i]=average_money; heartbeat[i]=0; heartbeat_sixhours[i]=0; heartbeat_hour[i]=0;reported_dead[i]=0;activity[i]=0;
		    }
               }
               else if (!command2.compare ("quit")) { return 1;}
               else { cout << "Options\n exec live on\n exec live off\n #clear\n kill\n #quit\n";}
            }
         else if (!command1.compare ("show")) { 
             if (!command2.compare ("top")) { DISPLAY_TOP = true; }
             if (!command2.compare ("all")) { DISPLAY_ALL = true; }
             if (!command2.compare ("stock")) { DISPLAY_STOCK = true; }
             if (!command2.compare ("bottom")) { DISPLAY_BOTTOM = true; }
             if (!command2.compare ("father")) { DISPLAY_FATHER = true; }
             if (!command2.compare ("live")) { cout << time_stamp << "LIVE STATUS:" << active << endl; }
             if (!command2.compare ("average")) { cout << time_stamp << "Average money:" << average_money << endl; }
            }
         else if (!command1.compare ("debug")) { }
         else if (!command1.compare ("?")) { cout << "Commands available \n show\n exec\n debug";}
         else { cout << "This command doesnt exist \n";}
      }

      /////////////////////////////////////////
      // CHECK FOR ACTIONS ON ALL PROGRAMS
      /////////////////////////////////////////

      for (i = 0 ; i < 100000; i++ ) { //MAJOR FOR LOP
      
       if ( actions->lock[i] == 1) {
	   if ( strcmp(separate_password[i],actions->password[i]) != 0) {
		//cout << time_stamp << "Password did not check" << i << endl;
		//cout << "on file" << separate_password[i] << " given:" << actions->password[i] << endl;
               actions->lock[i]  = 0;
               actions->action[i] = 0;
	   } else {
	     // Password checked 
		//cout << time_stamp << "Password checked" << " program:" << i << "Password " <<  separate_password[i] << "Password " << actions->password[i]  << endl;
		actions->password[i][0] = 0;
	   }
            // Debug line
            //cout << "Received action: " << i << ":lock" << actions->lock[i] << ":action" << actions->action[i] << endl;
          if (actions->action[i] != 0) {
               separate_action = actions->action[i];
               actions->stocks[i]= actions->stocks[i] + separate_action;
               separate_stocks[i]= separate_stocks[i] + separate_action;
               heartbeat[i]=1;
               heartbeat_sixhours[i]=1;
               activity[i]++;

	     // IF WE ARE RUNNING LIVE TRANSMITE THE ACTIONS OF TOP GUY
             if (heartbeat[dinheiro->posicao[i]] == 1 && active == 1) {
                if (actions->action[i] > 0) { ofstream myfile3; myfile3.open ("/home/joao/actions"); myfile3 << "buy" << endl; myfile3.close();}
                if (actions->action[i] < 0) { ofstream myfile3;myfile3.open ("/home/joao/actions"); myfile3 << "sell" << endl; myfile3.close();}
             }

             // PROCESS ACTION 
             if (actions->action[i] > 0 && reported_dead[i]==1 ) {
	       heartbeat_hour[i] = 30000;
               actions->money[i] = actions->money[i] - separate_action * (data->current_value + 0.0001);
               separate_money[i] = separate_money[i] - separate_action * (data->current_value + 0.0001);
               LOG << time_stamp << "PROGRAM:" << i << " AT:" << dinheiro->posicao[i] << " BUY:" << separate_action << " MONEY:" << actions->money[i] << "  STOCK: " << (data->current_value + 0.0001) << " NET Value:" << (actions->money[i] + actions->stocks[i]*data->current_value) << endl;
               }
             if (actions->action[i] < 0 && reported_dead[i]== 1 ) {
	       heartbeat_hour[i] = 30000;
               actions->money[i] = actions->money[i] - separate_action * (data->current_value - 0.0001);
               separate_money[i] = separate_money[i] - separate_action * (data->current_value - 0.0001);
               LOG << time_stamp << "PROGRAM:" << i << " AT:" << dinheiro->posicao[i] << " Sell:" << separate_action << " MONEY:" << actions->money[i] << "  STOCK: " << (data->current_value - 0.0001) << " NET Value:" << (actions->money[i] + actions->stocks[i]*data->current_value) << endl;
               }

               // CLEAR LOCK
               actions->lock[i]   = 0;
               actions->action[i]  = 0;
            } 
            
           if( actions->lock[i] == 1) {
               // Program pinged
               actions->lock[i]   = 0;
               actions->action[i]  = 0;
               heartbeat[i]=1;
               reported_dead[i]=1;
	       LOG << time_stamp << "PROGRAM:" << i << " AT:" << dinheiro->posicao[i] << " PINGED" << endl;
            }
            
         } // END OF ACTION PROCESSING

	 // Reduce by one the last time they played 
	 if (heartbeat_hour[i] > 0 ) {
	 	heartbeat_hour[i] = heartbeat_hour[i] - 1;
	 }

         actions->truemoney[i] = actions->money[i] + actions->stocks[i]*data->current_value;

         if (actions->money[i] != separate_money[i] ) {
             ofstream myfile2;
             myfile2.open ("/home/joao/tokill.txt", ios::app);
             LOG << time_stamp << "PROGRAM:" << i << " AT:" << dinheiro->posicao[i] << " Money does not match kill" << endl; 
             myfile2 << i;
             myfile2.close();
            // Reset values of killed fellow
            actions->money[i]=average_money; actions->lock[i]=0; actions->stocks[i]=0; actions->truemoney[i]=average_money; separate_money[i]=average_money;
            separate_stocks[i]=0;separate_truemoney[i]=average_money; heartbeat[i]=0; heartbeat_sixhours[i]=0; heartbeat_hour[i]=0;activity[i]=0;
         }
         if ( (actions->truemoney[i] * 2.5) < actions->money[i] ) {
             ofstream myfile2;
             myfile2.open ("/home/joao/tokill.txt", ios::app);
             LOG << time_stamp << "PROGRAM:" << i << " AT:" << dinheiro->posicao[i] << " True Money:" << actions->truemoney[i] << " Money:" << actions->money[i]   << " Money in game is too high kill" << endl; 
             myfile2 << i;
             myfile2.close();
             // Reset values of killed fellow
            actions->money[i]=average_money; actions->lock[i]=0; actions->stocks[i]=0; actions->truemoney[i]=average_money; separate_money[i]=average_money;
            separate_stocks[i]=0;separate_truemoney[i]=average_money; heartbeat[i]=0; heartbeat_sixhours[i]=0; heartbeat_hour[i]=0;activity[i]=0;
         }
         if (actions->stocks[i] != separate_stocks[i] ) {
              ofstream myfile2;
              myfile2.open ("/home/joao/tokill.txt", ios::app);
              LOG << "PROGRAM:" << i << " AT:" << dinheiro->posicao[i] << " to kill CHANGED HIS STOCKS IN MEMORY" << endl; 
              usleep(500000);
              myfile2 << i;
              myfile2.close();
              // Reset values of killed fellow
              actions->money[i]=average_money; actions->lock[i]=0; actions->stocks[i]=0; actions->truemoney[i]=average_money; separate_money[i]=average_money;
              separate_stocks[i]=0;separate_truemoney[i]=average_money; heartbeat[i]=0; heartbeat_sixhours[i]=0; heartbeat_hour[i]=0;activity[i]=0;
         }

         
	if (i < 99999) {
            if ( actions->truemoney[dinheiro->posicao[i]] < actions->truemoney[dinheiro->posicao[i+1]]) {
               temp = dinheiro->posicao[i];
               dinheiro->posicao[i] = dinheiro->posicao[i+1];
               dinheiro->posicao[i+1] = temp;
            }
         }
      
         if(reported_dead[dinheiro->posicao[i]] == 1 ) { 
            if (f < 10 && DISPLAY_TOP ) { cout << "PROGRAM:" << dinheiro->posicao[i] << " AT:" << f << " with:" << actions->truemoney[dinheiro->posicao[i]] << " Money:" << actions->money[dinheiro->posicao[i]] << " stocks:" << actions->stocks[dinheiro->posicao[i]] <<  " Heartbet:" << heartbeat[dinheiro->posicao[i]] <<  " Last Played:" << heartbeat_hour[dinheiro->posicao[i]] << endl; }
            if (heartbeat_hour[dinheiro->posicao[i]] > 0 && exported_father == false && rand() % 100 > 90 &&  activity[dinheiro->posicao[i]] > 5) {
               ofstream myfile3;
                  myfile3.open ("/home/joao/location.txt");
                  myfile3 << dinheiro->posicao[i];
                 myfile3.close();
                 exported_father = true;
               if (DISPLAY_FATHER) { cout << "PROGRAM:" << dinheiro->posicao[i] << " AT:" << f << " with:" << actions->truemoney[dinheiro->posicao[i]] << " Money:" << actions->money[dinheiro->posicao[i]] << " stocks " << actions->stocks[dinheiro->posicao[i]] <<  " Heartbet:" << heartbeat[dinheiro->posicao[i]] <<  " Last Played:" << heartbeat_hour[dinheiro->posicao[i]] << endl; }
            }
            if (DISPLAY_ALL) { cout << "PROGRAM:" << dinheiro->posicao[i] << " AT:" << f << " with:" << actions->truemoney[dinheiro->posicao[i]] << " Money:" << actions->money[dinheiro->posicao[i]] << " stocks:" << actions->stocks[dinheiro->posicao[i]] <<  " Heartbet:" << heartbeat[dinheiro->posicao[i]] <<  " Last Played:" << heartbeat_hour[dinheiro->posicao[i]] << endl; }
            f++;
            ultimo = dinheiro->posicao[i];
            average_money = ( average_money * ( prev_f - 1 ) / prev_f) + ( actions->truemoney[dinheiro->posicao[i]] / prev_f );
         }  
      }

      ofstream myfile2;
      myfile2.open ("/home/joao/number_of_active.txt");
      myfile2 << f;
      myfile2.close();
      prev_f = f;

      ////////////////////////////////////////////////
      // THIS SECITONS HAS POPULATION CONTROL
      // We will only active population control if it his higher than 100 and stock is has some acitons 
      ////////////////////////////////////////////////

      if( f > 500 && 0 != tendencia ) {
         ofstream myfile2;
         myfile2.open ("/home/joao/location_ultimo.txt");
         myfile2 << dinheiro->posicao[ultimo];
         myfile2.close();
	 to_kill = 0;
	 while ( to_kill == 0 ) {
         	i=rand() % 100000 + 1;
		if (reported_dead[i]==1) {
			to_kill = i;
		} else if ( rand() % 100000 > 99999 ) {
			myfile2.open ("/home/joao/tokill.txt" , ios::app );
			LOG << time_stamp << "To kill because there is suppose to be dead" << to_kill << endl;
			usleep(200000);
			myfile2 << i << endl;
			myfile2.close();
		}
	 }
         random=rand() % 100000;
	 if ( f > 1000) {  random=rand() % 10000; }
	 if ( f > 2000) {  random=rand() % 1000; }
	 if ( f > 3000) {  random=rand() % 600; }
         if (heartbeat[to_kill] == 0 && random < 500 ) {
            myfile2.open ("/home/joao/tokill.txt", ios::app );
            LOG << time_stamp << "To kill porque n jogava ao tempo PROGRAM:" << to_kill << endl;
            usleep(200000);
            myfile2 << to_kill << endl;
            myfile2.close();
         } 
         if (random < 560 && random > 550 ) {
	       myfile2.open ("/home/joao/tokill.txt" , ios::app );
               LOG << time_stamp << "To kill porque e assim a vida PROGRAM:" << to_kill << " Randon value:" << random << endl;
               usleep(200000);
               myfile2 << to_kill << endl;
               myfile2.close();

         }
         if ( ultimo == to_kill ) {
            myfile2.open ("/home/joao/tokill.txt" , ios::app );
            LOG << time_stamp << "To kill porque era o ultimo PROGRAM:" << dinheiro->posicao[ultimo] << endl;
            usleep(200000);
            myfile2 << dinheiro->posicao[ultimo] << endl;
            myfile2.close();
         }
      }

      ////////////////////////////////////////////////
      // END OF POPULATION CONTROL
      ////////////////////////////////////////////////


      ///////////////////////////////////////
      // Find position for new child
      //////////////////////////////////////
      i=rand() % 100000 + 1;
      if (heartbeat[i]==0 && reported_dead[i]==0) {
	 // Write position to file
         ofstream myfile3; myfile3.open ("/home/joao/available.txt");myfile3 << i; myfile3.close();
         // Give child average money
         actions->money[i] = average_money; actions->truemoney[i] = average_money; separate_money[i] = average_money; separate_truemoney[i] = average_money;
      }

      ///////////////////////////////////////
      // Check if someone got killed
      //////////////////////////////////////
      ifstream ifile("/home/joao/killed.txt");
      if (ifile.is_open() == true) {
         ifile >> i; ifile.close();
         LOG << time_stamp << "PROGRAM:" << i << " Killed"<< endl;
         system ("rm /home/joao/killed.txt");
         if (i >0 && i < 100000){
         // Reset values of killed fellow
         actions->money[i]=average_money; actions->lock[i]=0; actions->stocks[i]=0; actions->truemoney[i]=average_money; separate_money[i]=average_money;
         separate_stocks[i]=0;separate_truemoney[i]=average_money; heartbeat[i]=0; heartbeat_sixhours[i]=0; heartbeat_hour[i]=0;reported_dead[i]=0;activity[i]=0;
         }
      }      


      ///////////////////////////////////////
      // Manage the conters for 6 hours
      //////////////////////////////////////
      six_hours = six_hours - 1;
      if (six_hours == 0 ) { six_hours = 126000; }

      if (six_hours == 1 && 0 != tendencia ) {
         LOG << time_stamp << "Six hour counter reached end and there is stock activity" << endl;
         for (i = 0 ; i < 100000; i++ ) {
            if (heartbeat_sixhours[i]==0) { LOG << time_stamp << "PROGRAM:" << i << "Was inactive for very long time 12 hours" << endl ;heartbeat[i]=0;}
            heartbeat_sixhours[i] = 0;
         }
      }

     // OUTPUT SHOW COMMANDS 
     if ( DISPLAY_STOCK ) { cout << time_stamp << "STOCK VALUE:" << data->current_value << " TENDENCY:" << Tendencia_10_minutos(data) << endl; }

     // CANCEL SHOW COMMANDS 
     DISPLAY_TOP = false;
     DISPLAY_BOTTOM = false;
     DISPLAY_STOCK = false;
     DISPLAY_ALL = false;
     DISPLAY_FATHER = false;

   }


}

