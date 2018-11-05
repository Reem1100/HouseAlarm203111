#include <stdio.h>
#include <time.h>
#include <math.h>
#include <wiringPi.h>
#include "ifttt.h"


int status;
extern int ifttt(char* , char* , char* , char* );
status = 1;


void alarmOff(void)
    {
        int buttonValue;
        buttonValue = digitalRead(3);
        while(buttonValue)
            {
            buttonValue = digitalRead(3);
            if(buttonValue == 0)
            {
            status = 2;
            }
            digitalWrite (1, HIGH) ;
            digitalWrite (2, LOW) ;
            digitalWrite (4, LOW) ;
            }
 }

void alarmArming(void)
{
    int i;
    int buttonValue;
    double seconds; 
    double secondsafter10;
    seconds = (double) time(NULL); 
    secondsafter10 = seconds + 10;
    buttonValue = digitalRead(3); 
    digitalWrite (2, LOW) ;
    digitalWrite (4, LOW) ;
    while((seconds <= secondsafter10) && buttonValue)
    {
           buttonValue = digitalRead(3); 
           seconds = (double) time(NULL);
           if(fmod(seconds,2) == 0)
           {
                digitalWrite (1, HIGH);
           }
           else
           {
                digitalWrite (1, LOW) ;
           } 
    }
    digitalWrite (1, LOW) ;
    digitalWrite (2, HIGH) ;
    status = 3;
}

void alarmArmed(void)
{
    
    int buttonValue;
    int motionSignal;
    motionSignal = digitalRead(0);
    buttonValue = digitalRead(3);
    while( !motionSignal && buttonValue)
    {
        buttonValue = digitalRead(3);
        if(buttonValue == 0)
        {
            status = 1;
            break;
        }
        motionSignal = digitalRead(0);
        if(motionSignal)
        {
            status = 4;
            break;
        }
        digitalWrite (1, LOW) ;
        digitalWrite (2, HIGH) ;
        digitalWrite (4, LOW) ;  
    }
}

void alarmTriggerd(void)
    {
        double seconds; 
        double secondsafter10;
        int buttonValue;
        seconds = (double) time(NULL); 
        secondsafter10 = seconds + 10;
        buttonValue = digitalRead(3); 
        while((seconds <= secondsafter10) && buttonValue)
        {
           buttonValue = digitalRead(3); 
           seconds = (double) time(NULL);
           if(fmod(seconds,2) == 0)
           {
              printf("every 2 seconds\n"); 
                digitalWrite (1, HIGH) ;
                digitalWrite (2, HIGH) ;
           }
           else
           {
                digitalWrite (1, LOW) ;
                digitalWrite (2, LOW) ; 
           }
        }
        digitalWrite (1, LOW) ; /* turn LED are off */
        digitalWrite (2, LOW) ; /* turn LED are off */
        if(buttonValue == 0)
        {
            status = 1;
        }
        else
        {
            status = 5;
        }
   } 



void alarmSounding(void)
{
    /* IFFT event should be TRIGGERED*/
   /* ifttt("http://red.eecs.yorku.ca:8080/trigger/event/with/key/Ja3far", "alarm", "is", "sounding"); */
    ifttt("https://maker.ifttt.com/trigger/alarm_triggered/with/key/no1naVILewvTgtj0rMJfY_ZOy81t3fZN_YPwpMGoFHs", "Alarm", "is", "souding");
    int buttonValue;
    double seconds;
    buttonValue = digitalRead(3);
    while(buttonValue)
    {
        buttonValue = digitalRead(3);
        seconds = (double) time(NULL);
        if(fmod(seconds,2) == 0)
        {
          printf("every 2 seconds\n"); 
            digitalWrite (1, HIGH) ;
            digitalWrite (2, HIGH) ;
            digitalWrite (4, HIGH) ;
        }
        else
        {
            digitalWrite (1, LOW) ;
            digitalWrite (2, LOW) ; 
            digitalWrite (4, LOW) ;
        }
        
    }
        status = 1;
    
}


void handlingfunction(void)
{
    while(1)
    {
        if(status == 1)
        {
            alarmOff();
        }
        else if(status == 2)
        {
            alarmArming();
        }
        else if(status == 3)
        {
            alarmArmed();
        }
        else if(status == 4)
        {
            alarmTriggerd();
        }
        else
        {
            alarmSounding();
        }
    }
    
}
int main()
{
    wiringPiSetup () ;
    pinMode(0, INPUT);
    pinMode (1, OUTPUT) ;
    pinMode (2, OUTPUT) ;
    pinMode (3, INPUT) ;
    pullUpDnControl(3, PUD_UP) ;
    pinMode (4, OUTPUT) ;
    handlingfunction();
    

    return 0;
}

