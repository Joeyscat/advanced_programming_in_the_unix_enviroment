int 
pthread_barrier_init(pthread_barrier_t *restrict barrier, const pthread_barrierattr_t *restrict attr, unsigned int count){
    if(pthread_mutex_init(&x.balock, (const pthread_mutexattr_t *)attr) != 0)
        return errno;
    x.tcount = count;
    x.flag = 1;
    if(pthread_cond_init((pthread_cond_t *)barrier, (const pthread_condattr_t *)attr)!= 0)
        return errno;
    return 0;
}

pthread_t *pmtid;

struct {
    pthread_mutex_t balock;
    int tcount;
    int flag;
}x;

int 
pthread_barrier_wait(pthread_barrier_t *barrier){
    static tcount = 1;
    pthread_mutex_lock(&x.balock); 
    
    while (tcount != x.tcount){ 
        ++tcount;
        if(pthread_cond_wait((pthread_cond_t *)barrier, &x.balock) != 0)
            return errno;
    }
    
    if(flag){
        flag = 0;
        if(pthread_cond_broadcast((pthread_cond_t *)barrier) != 0)
            return errno;
    }

    pthread_mutex_unlock(&x.balock);

    if (pthread_equal(pthread_self(), *pmtid)) {
        return PTHREAD_BARRIER_SERIAL_THREAD;
    }else{
        return 0;
    }
}

int 
pthread_barrier_destroy(pthread_barrier_t *barrier){
    if(pthread_cond_destroy((pthread_cond_t *)barrier) != 0)
        return errno;
     return 0;
}
