//
// Created by mirac on 2022/10/4.
//
#include "util.h"
#include <stdio.h>
#include <cstdlib>
void error_if(bool condition, const char* msg){
    if(condition){
        perror(msg);
        exit(EXIT_FAILURE);
    }
}
