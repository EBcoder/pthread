#include <iostream>
#include <pthread.h>

#include <chrono>

#ifdef __cplusplus

extern "C" {
    
#endif
    float f1(float x, int intensity);
    
    float f2(float x, int intensity);
    
    float f3(float x, int intensity);
    
    float f4(float x, int intensity);
    
    
    
#ifdef __cplusplus
    
}

#endif

pthread_mutex_t mx;

float sum=0;

struct threadStruct{
    
    int start;
    int end;
    int fnId;
    float a;
    float b;
    float n;
    int intensity;
    int nbthreads;
};

void *myThreadFunction(void *value){
    
    struct threadStruct *th = (struct threadStruct*) value;
    pthread_mutex_lock(&mx);
    for(int i=th->start;i<th->end;i++){
        float x = (th->a + (i + 0.5) * ((th->b-th->a)/th->n));
        if(th->fnId == 1){
            sum += (f1(x,th->intensity)*((th->b-th->a)/th->n));
        }else if(th->fnId == 2){
            sum += (f2(x,th->intensity)*((th->b-th->a)/th->n));
        }else if(th->fnId == 3){
            sum += (f3(x,th->intensity)*((th->b-th->a)/th->n));
        }else if(th->fnId == 4){
            sum += (f4(x,th->intensity)*((th->b-th->a)/th->n));
        }
        pthread_mutex_unlock(&mx);
    }
    pthread_exit(NULL);
}



void* lThread(void *value){
    struct threadStruct *th =(struct threadStruct *)value;
    for(int i=th->start; i<th->end; i++){
        pthread_mutex_lock(&mx);
        float x = (th->a + (i + 0.5) * ((th->b-th->a)/th->n));
        if(th->fnId == 1){
            sum += (f1(x,th->intensity)*((th->b-th->a)/th->n));
        }else if(th->fnId == 2){
            sum += (f2(x,th->intensity)*((th->b-th->a)/th->n));
        }else if(th->fnId == 3){
            sum += (f3(x,th->intensity)*((th->b-th->a)/th->n));
        }else if(th->fnId == 4){
            sum += (f4(x,th->intensity)*((th->b-th->a)/th->n));
        }
        pthread_mutex_unlock(&mx);
    }
    pthread_exit(NULL);
}

int main (int argc, char* argv[]) {
    
    if (argc < 8) {
        
        std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync>"<<std::endl;
        return -1;
    }
    float fnId= atoi(argv[1]);
    
    float a = atoi(argv[2]);
    
    float b = atoi(argv[3]);
    float n = atoi(argv[4]);
    int intensity = atoi(argv[5]);
    int nbthreads = atoi(argv[6]);
    int start=0;
    int end=0;
    //
    pthread_t *threads;
    threads = new pthread_t[nbthreads];
    //instance of a struct
    struct threadStruct *strt;
    strt = new struct threadStruct[nbthreads];
    //start time
    std::chrono::time_point<std::chrono::system_clock> starting = std::chrono::system_clock::now();
    // assign which thread runs when
    for(int i=0;i<nbthreads;i++){
        strt[i].fnId= fnId;
        strt[i].a = a;
        strt[i].b = b;
        strt[i].n = (float)n;
        strt[i].intensity = intensity;
        strt[i].start = i * (n/nbthreads);
        strt[i].end = (i+1) * (n/nbthreads);
        pthread_create(&threads[i],NULL,lThread,(void*) &strt[i]);
    }
    // loop to join threads
    for(int i = 0;(i <nbthreads); i++){
        pthread_join(threads[i], NULL);
    }
    std::cout<<sum;
    //end time
    std::chrono::time_point<std::chrono::system_clock> ending = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = starting-ending;
    std::cerr<<elapsed_seconds.count()<<std::endl;
    pthread_mutex_destroy(&mx);
    pthread_exit(NULL);
    return 0;
    
}


