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


int readCount, writeCount;

string book = "This is a short example with this string being considered as the place where the readers and writers will perform their critical section activity";
int bookLength; //for readers to do something with the book

semaphore readMutex(1), writeMutex(1), readTry(1), resource(1);

void* Reader(void *arg) {
	while(true) {
		sleep(1);
		readTry.wait(); //indicate reader trying to read		
		readMutex.wait(); //lock entry section 
		
		readCount++;
		if(readCount == 1) {
			resource.wait();
		}
		
		readMutex.signal();
		readTry.signal();
		
		cout<<"The read line is "<<book<<"\n";
		
		bookLength = book.length();
	
		cout<<"Book Length "<<bookLength<<"\n";
		
		readMutex.wait();
		
		readCount--;
		if(readCount == 0) {
			resource.signal(); //relaease the locked resource if you are the last reader
		}
		
		readMutex.signal();
		
	}
}

void* Writer(void *arg) {
	while(true) {
		sleep(1);
		
		writeMutex.wait();
		
		writeCount++;
		
		if( writeCount == 1) {
			readTry.wait(); //lock other readers out if you are the first writer
		}
		
		writeMutex.signal();
		
		resource.wait();
		//critical section
		int length = book.length();
		int rindex = rand()%length;
		book = book.replace(rindex, 1, " great ");
		
		resource.signal();
		
		writeMutex.wait();
		
		writeCount--;
		
		if( writeCount == 0) {
			readTry.signal(); //lock other readers out if you are the first writer
		}
		
		writeMutex.signal();
		
	}
}

int main() {
   
   pthread_t id[3]; //2 readers 1 writer
   pthread_create(&id[0], NULL, Reader, NULL);
   pthread_create(&id[1], NULL, Reader, NULL);
   pthread_create(&id[2], NULL, Writer, NULL);


   for(int i=0;i<3;i++)
   pthread_join(id[i],NULL); 

}
