#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;

pthread_mutex_t mid;

int BUFFER_SIZE = 10;
int BUFFER[10] = {1 , 2, 3, 4, 5, 6, 7, 8, 9, 10};

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

semaphore full(10), empty(0), lock(1);


void* producer(void *id) {
	while(true) {
		sleep(1);
		int product = rand() % 100; //random number between 0 to 99
		
		empty.wait();
		lock.wait();
		
		BUFFER[BUFFER_SIZE] = product;
		BUFFER_SIZE++;
	
		lock.signal();
		full.signal();
		
		lock.wait(); //locking and signal for print statements for their atomicity
		cout<<"product: "<<product<<" BUFFER_SIZE: "<<BUFFER_SIZE<<endl;
		lock.signal();
		 
	}	
}

void* consumer(void *id) {
	while(true) {
		int consume;
		
		full.wait();
		lock.wait();
		
		consume = BUFFER[BUFFER_SIZE - 1];
		BUFFER_SIZE--;
		
		lock.signal();
		empty.signal();
		
		lock.wait(); //locking and signal for print statements for their atomicity
		cout<<"consume: "<<consume<<" BUFFER_SIZE: "<<BUFFER_SIZE<<endl;
		lock.signal();
		 
	}	
}


int main() {
   
   pthread_t id[2];
   pthread_create(&id[1], NULL, consumer, NULL);
   pthread_create(&id[0], NULL, producer, NULL);

   for(int i=0;i<2;i++)
   pthread_join(id[i],NULL); 

}
