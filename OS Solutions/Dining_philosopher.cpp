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
   semaphore() {
   	  counter =  1;
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
semaphore chopsticks[5], initialize(1), lock(1);
int oddphil = 1, evenphil = 0;

void* odd_philosopher(void *arg) {
    int num;
    initialize.wait();
    num = oddphil;
    oddphil += 2;	
    initialize.signal();
    
    while(true) {
		sleep(2);
    	lock.wait();
		cout<<"Philosopher "<<num<<" thinking"<<endl;
		lock.signal();
		
		cout<<"Philosopher "<<num<<" wants to eat"<<endl;
		
		chopsticks[(num + 1)%5].wait();
		chopsticks[num%5].wait();
		
		cout<<"Philosopher "<<num<<" eating"<<endl;
		
		chopsticks[num%5].signal();
		chopsticks[(num+1) % 5].signal();
		
	}
	
}

void* even_philosopher(void *arg) {
	int num;
	initialize.wait();
	num = evenphil;
	evenphil += 2;
	initialize.signal();
	
	while(true) {

    	sleep(2);
    	lock.wait();
		cout<<"Philosopher "<<num<<" thinking"<<endl;
		lock.signal();
		
		chopsticks[num%5].wait();
		chopsticks[(num+1)%5].wait();
		
		cout<<"Philosopher "<<num<<" eating"<<endl;
		
		chopsticks[(num + 1)%5].signal();
		chopsticks[num % 5].signal();
		
	}

}

void* critical_philosopher(void *arg) {
	
}
int main() {
   
   pthread_t id[5]; //5 philosophers
   for(int i=0; i<3; i++) {
	pthread_create(&id[i], NULL, even_philosopher, NULL);
   }
   for(int i=3; i<5; i++) {
   	pthread_create(&id[i], NULL, odd_philosopher, NULL);
   }
   for(int i=0;i<5;i++)
   pthread_join(id[i],NULL); 

}
