#include "common.hpp"

long minInterArrival(long* interArrival, int num){
    long temp = interArrival[0];
    for(int i = 1; i < num; i++){
        if(temp > interArrival[i])
            temp = interArrival[i];
    }
    return temp/1000;
}

long maxInterArrival(long* interArrival, int num){
    long temp = 0;
    for(int i = 0; i < num; i++){
        if(interArrival[i] > temp)
            temp = interArrival[i];
    }
    return temp/1000;
}

long meanInterArrival(long* interArrival, int num){
    long temp = interArrival[0];
    for(int i = 1; i < num; i++){
        temp += interArrival[i];
    }
    temp = temp / num;
    return temp/1000;
}
