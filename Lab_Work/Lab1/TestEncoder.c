#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define bool int
#define byte unsigned char
#define numChars  32

void initialiseEncoderStateMachine();
void updateEncoderStateMachine();

// Here we are using global variables simply to be compatible with the Arduino program structure
// This is REALLY bad practice so please don't do it otherwise!
long int count = 0;
long int error = 0;

enum states {state1=1, state2, state3, state4};
bool channelAState=0;
bool channelBState=0;

enum states state;
char receivedChars[numChars]="00";   // an array to store the received data

int main(int argc, const char * argv[])
{
    // Replicates the setup in Arduino
    printf("Enter a pair of characters representing initial states of channels A and B\n");
    scanf("%s",receivedChars);
    channelAState = receivedChars[0]!='0';
    channelBState = receivedChars[1]!='0';

    initialiseEncoderStateMachine();
    printf("State %d, count %ld, error %ld\n", state, count, error);
    printf("Enter a pair of characters representing channels A and B, enter 99 to end\n");

    // Replicates the loop in Arduino
    do
    {
        scanf("%s",receivedChars);
        if (strcmp(receivedChars, "99")==0)
        {
            break;
        }
        channelAState = receivedChars[0]!='0';
        channelBState = receivedChars[1]!='0';
        updateEncoderStateMachine();
        printf("State %d, count %ld, error %ld\n", state, count, error);

    }
    while(1);
    return 0;
}
void initialiseEncoderStateMachine()
{

    if (channelAState)
    {
        if(channelBState)
        {
            state = state3;
        } else {
            state = state2;
        }
    } else {
        if(channelBState)
        {
            state = state4;
        } else {
            state = state1;
        }
    }
}

void updateEncoderStateMachine()
{
    switch (state)
    {
    case state1:
        if (channelAState && !channelBState)
        {
            count++;
            state = state2;
        } else if (!channelAState && channelBState) {
            count--;
            state=state4;

        } else if (channelAState && channelBState){
            error++;
            state = state3;
        }
        break;

    case state2:
        if (!channelAState && !channelBState)
        {
            count--;
            state = state1;
        } else if (!channelAState && channelBState) {
            error++;
            state=state4;

        } else if (channelAState && channelBState){
            count++;
            state = state3;
        }
        break;

    case state3:
        if (!channelAState && !channelBState)
        {
            error++;
            state = state1;
        } else if (!channelAState && channelBState) {
            count++;
            state=state4;

        } else if (channelAState && !channelBState){
            count--;
            state = state2;
        }
        break;

    case state4:
        if (!channelAState && !channelBState)
        {
            count++;
            state = state1;
        } else if (channelAState && !channelBState) {
            error++;
            state=state2;

        } else if (channelAState && channelBState){
            count--;
            state = state3;
        }
        break;    
    }
}
