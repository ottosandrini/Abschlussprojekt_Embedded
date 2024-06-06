#ifndef YOURHEADER_H
#define YOURHEADER_H

typedef enum {
    INIT=0,
    STARTS1,
    STARTS2,
    FIELD,
    PLAY,
    RESULT,
    GAMEEND,
    ERRORt,
    UNEXPECTED,
    MYBAD
    } State_Type;

extern void (*state_table[])(char*, int*, int*, State_Type*);
 

void initializeSM(State_Type*);
void init_(char *, int *, int*,State_Type *); 
void startS1_(char *, int *, int*, State_Type *); 
void startS2_(char *, int *, int*, State_Type *); 
void field_(char *, int *, int*, State_Type *); 
void play_(char *, int *, int*, State_Type *); 
void result_(char *, int *, int*, State_Type *); 
void gameend_(char *, int *, int*, State_Type*); 
void error_(char *, int *, int*, State_Type *); 
void unexpected_(char *, int *, int*, State_Type*); 
void mybad_(char *, int *, int*, State_Type*); 

#endif /* YOURHEADER_H */

