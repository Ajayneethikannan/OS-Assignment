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

semaphore lock(1), paper_matches(0), tobacco_matches(0), tobacco_paper(0), agent(0);

void* Agent(void *arg) {
while(true) {
	lock.wait();
	int random = rand() % 3;
	switch(random) {
		case 0:
			paper_matches.signal();
			break;
		case 1:
			tobacco_matches.signal();
			break;
		case 2:
			tobacco_paper.signal();
			break;
		default:
			break;
	}
	lock.signal();
	agent.wait();
}	
}

void* tobacco_guy(void *arg) {
	while(true) {
		paper_matches.wait();
	    lock.wait();
	    sleep(1); //take your time to make the cigarette
	    cout<<"tobacco guy smokes !\n";
	    agent.signal();
	    lock.signal();
	}
}

void* paper_guy(void *arg) {
	while(true) {
		tobacco_matches.wait();
	    lock.wait();
	    sleep(1); //take your time to make the cigarette
	    cout<<"paper guy smokes !\n";
	    agent.signal();
	    lock.signal();
	}
}

void* matches_guy(void *arg) {
	while(true) {
		tobacco_paper.wait();
	    lock.wait();
	    sleep(1); //take your time to make the cigarette
	    cout<<"matches guy smokes !\n";
		agent.signal();
	    lock.signal();
	}
}


int main() {
   
   pthread_t id[4]; //3 smokers 1 agent
   pthread_create(&id[0], NULL, Agent, NULL);
   pthread_create(&id[1], NULL, matches_guy, NULL);
   pthread_create(&id[2], NULL, paper_guy, NULL);
   pthread_create(&id[3], NULL, tobacco_guy, NULL);

   for(int i=0;i<4;i++)
   pthread_join(id[i],NULL); 

}

