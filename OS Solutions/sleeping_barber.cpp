#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

pthread_mutex_t mid;
class semaphore {
   int counter;
public:
   semaphore(int num){
   	  counter = num;
   }
   void wait(){
       pthread_mutex_lock(&mid);         //lock the mutex here
       while(1){
           if(counter>0){                //check for counter value
               counter--;                //decrement counter
               break;                    //break the loop
           }

       }
       pthread_mutex_unlock(&mid);       //unlock mutex here
   }
   void signal(){
       pthread_mutex_lock(&mid);        //lock the mutex here
           counter++;                   //increment counter
           pthread_mutex_unlock(&mid);  //unlock mutex here
       }

};





int freeSeats = 10;
semaphore customers(0), barber(0), seat(1); //seat is a mutex for executing sitting on a chair

void* Barber(void *arg) {
	while(true) {

		cout<<"Sleeping zzzz\n";
		sleep(1);
		customers.wait(); //wait for customers
		
		seat.wait(); //obtain mutex for seat operation
		freeSeats++; //increase free seats number
		seat.signal(); //release mutex on seat operation
		
		barber.signal(); //signal customer barber is free
	}
}

void* Customer(void *arg) {
	while(true) {
		seat.wait(); //obtain mutex for seat operation	
			
		if(freeSeats > 0) {
			
			freeSeats--; //this particular customer has sat down now
			customers.signal(); //add to number of customers	
			seat.signal(); //release seat mutex after sitting
			
			barber.wait(); //wait for the barber
			cout<<"Got the cut\n";
			barber.signal();
			return NULL; //exit the shop after getting the cut
		}	
		else {
			seat.signal();
		}
	}
}


int main() {
   
   pthread_t id[11]; //10 customers 1 barber
   pthread_create(&id[10], NULL, Barber, NULL);
   for(int i=0; i<10; i++)pthread_create(&id[i], NULL, Customer, NULL);

   for(int i=0;i<11;i++)
   pthread_join(id[i],NULL); 

}
