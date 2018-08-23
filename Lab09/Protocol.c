/*
 * File: Protocol.c
 * Author: Bowen Brooks
 * 
 * Created on March 8, 2016, 4:05 PM
 */

#include "protocol.h"
#include "BOARD.h"
//user Library
#include <stdlib.h>
#include <string.h>


#define XOR(x,y) ((x) ^ (y))

#define NOT_VALID_HEX 255

//strcts and constants

enum {
    WAITING,
    RECORDING,
    FIRST_CHECKSUM_HALF,
    SECOND_CHECKSUM_HALF,
    NEWLINE
} STATE = WAITING;

static uint8_t checkSum;
static uint8_t genrateCheckSum(char* message);
static uint8_t hexToInt(char hex);
int Datacounter, dataIndex;
static int counter;

/**
 * Encodes the coordinate data for a guess into the string `message`. This string must be big
 * enough to contain all of the necessary data. The format is specified in PAYLOAD_TEMPLATE_COO,
 * which is then wrapped within the message as defined by MESSAGE_TEMPLATE. The final length of this
 * message is then returned. There is no failure mode for this function as there is no checking
 * for NULL pointers.
 * @param message The character array used for storing the output. Must be long enough to store the
 *                entire string, see PROTOCOL_MAX_MESSAGE_LEN.
 * @param data The data struct that holds the data to be encoded into `message`.
 * @return The length of the string stored into `message`.
 */
int ProtocolEncodeCooMessage(char *message, const GuessData *data)
{
    uint8_t checkSum;
    char internalString[PROTOCOL_MAX_MESSAGE_LEN];
    int length;
    sprintf(internalString, PAYLOAD_TEMPLATE_COO, data->row, data->col);
    checkSum = genrateCheckSum(internalString); //getting the checksum
    sprintf(message, MESSAGE_TEMPLATE, internalString, checkSum); //putting it in one string
    //this is to find the length
    char* temp;
    temp = message;
    length = 0;
    while (*temp != '\0') {
        length++;
        temp++;
    }
    return length;
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeHitMessage(char *message, const GuessData *data)
{
    uint8_t checkSum;
    char internalString[PROTOCOL_MAX_MESSAGE_LEN];
    int length;
    sprintf(internalString, PAYLOAD_TEMPLATE_HIT, data->row, data->col, data->hit);
    checkSum = genrateCheckSum(internalString); //getting the checksum
    sprintf(message, MESSAGE_TEMPLATE, internalString, checkSum); //putting it in one string
    //this is to find the length
    char* temp;
    temp = message;
    length = 0;
    while (*temp != '\0') {
        length++;
        temp++;
    }
    return length;
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeChaMessage(char *message, const NegotiationData *data)
{
    uint8_t checkSum;
    char internalString[PROTOCOL_MAX_MESSAGE_LEN];
    int length;
    sprintf(internalString, PAYLOAD_TEMPLATE_CHA, data->encryptedGuess, data->hash);
    checkSum = genrateCheckSum(internalString); //getting the checksum
    sprintf(message, MESSAGE_TEMPLATE, internalString, checkSum); //putting it in one string
    //this is to find the length
    char* temp;
    temp = message;
    length = 0;
    while (*temp != '\0') {
        length++;
        temp++;
    }
    return length;
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeDetMessage(char *message, const NegotiationData *data)
{
    uint8_t checkSum;
    char internalString[PROTOCOL_MAX_MESSAGE_LEN];
    int length;
    sprintf(internalString, PAYLOAD_TEMPLATE_DET, data->guess, data->encryptionKey);
    checkSum = genrateCheckSum(internalString); //getting the checksum
    sprintf(message, MESSAGE_TEMPLATE, internalString, checkSum); //putting it in one string
    //this is to find the length
    char* temp;
    temp = message;
    length = 0;
    while (*temp != '\0') {
        length++;
        temp++;
    }
    return length;
}

/**
 * This function decodes a message into either the NegotiationData or GuessData structs depending
 * on what the type of message is. This function receives the message one byte at a time, where the
 * messages are in the format defined by MESSAGE_TEMPLATE, with payloads of the format defined by
 * the PAYLOAD_TEMPLATE_* macros. It returns the type of message that was decoded and also places
 * the decoded data into either the `nData` or `gData` structs depending on what the message held.
 * The onus is on the calling function to make sure the appropriate structs are available (blame the
 * lack of function overloading in C for this ugliness).
 *
 * PROTOCOL_PARSING_FAILURE is returned if there was an error of any kind (though this excludes
 * checking for NULL pointers), while
 * 
 * @param in The next character in the NMEA0183 message to be decoded.
 * @param nData A struct used for storing data if a message is decoded that stores NegotiationData.
 * @param gData A struct used for storing data if a message is decoded that stores GuessData.
 * @return A value from the UnpackageDataEnum enum.
 */
ProtocolParserStatus ProtocolDecode(char in, NegotiationData *nData, GuessData *gData)
{
    static char messageArray[PROTOCOL_MAX_MESSAGE_LEN];
    uint8_t value; //this is gonna hold the value of the hextoint 
    static char DATA1[8];
    static char DATA2[8];
    static char DATA3[8];
    switch (STATE) {
    case WAITING:
        if (in != '$') {
            return PROTOCOL_WAITING;
        } else {
            Datacounter = 0;
            counter = 0;
            STATE = RECORDING;
            return PROTOCOL_PARSING_GOOD;
        }
        break;
    case RECORDING:
        if (in != '*') {
            messageArray[counter] = in;
            counter++;
            //this records the data in between the comas and stores them for later
            if (in == ',') {
                Datacounter++;
                dataIndex = 0;
            } else {
                if (Datacounter == 1) {
                    DATA1[dataIndex] = in;

                } else if (Datacounter == 2) {
                    DATA2[dataIndex] = in;
                } else if (Datacounter == 3) {
                    DATA3[dataIndex] = in;
                }
                dataIndex++;
            }
            return PROTOCOL_PARSING_GOOD;
        }
        if (in == '*') {
            STATE = FIRST_CHECKSUM_HALF;
            return PROTOCOL_PARSING_GOOD;
        }
        break;
    case FIRST_CHECKSUM_HALF:
        value = hexToInt(in);
        if (value != NOT_VALID_HEX) {
            checkSum = value;
            checkSum = checkSum << 4; //this puts it into the top 4 bits    
            STATE = SECOND_CHECKSUM_HALF;
            return PROTOCOL_PARSING_GOOD;
        } else {
            STATE = WAITING;
            return PROTOCOL_PARSING_FAILURE;
        }
        break;
    case SECOND_CHECKSUM_HALF:
        value = hexToInt(in);
        if (value == NOT_VALID_HEX) {
            STATE = WAITING;
            return PROTOCOL_PARSING_FAILURE;
        }
        checkSum = checkSum | value; //This gives the lower half bits
        if (checkSum != genrateCheckSum(messageArray)) { //checking if checksum matches
            STATE = WAITING;
            return PROTOCOL_PARSING_FAILURE;
        } else {
            messageArray[counter] = '\0';
            STATE = NEWLINE;
            return PROTOCOL_PARSING_GOOD;
        }
        break;
    case NEWLINE:
        if (in != '\n') {
            STATE = WAITING;
            return PROTOCOL_PARSING_FAILURE;
        } else {
            STATE = WAITING;
            //These check if they are the right messages being transmistted
            if (messageArray[0] == 'D') { //DET
                //NEGOTIATION DATA   
                nData->guess = atoi(DATA1); //getting the number value
                nData->encryptionKey = atoi(DATA2); //getting the number value
                return PROTOCOL_PARSED_DET_MESSAGE;
            } else if (messageArray[0] == 'C') { //CHA
                //NEGOTIATION DATA
                nData->encryptedGuess = atoi(DATA1); //getting the number value
                nData->hash = atoi(DATA2); //getting the number value
                return PROTOCOL_PARSED_CHA_MESSAGE;
            } else if (messageArray[0] == 'H') { //HIT
                //GUESS DATA
                gData->row = atoi(DATA1); //getting the number value;
                gData->col = atoi(DATA2); //getting the number value;
                gData->hit = atoi(DATA3); //getting the number value; 
                return PROTOCOL_PARSED_HIT_MESSAGE;
            } else if (messageArray[1] == '0') { //COO
                //GUESS DATA
                gData->row = atoi(DATA1);
                gData->col = atoi(DATA2);
                return PROTOCOL_PARSED_COO_MESSAGE;
            } else {
                return PROTOCOL_PARSING_FAILURE;
            }
        }
        break;
    }
    return PROTOCOL_PARSING_FAILURE; //if it got here something went wrong!!!
}

/**
 * This function generates all of the data necessary for the negotiation process used to determine
 * the player that goes first. It relies on the pseudo-random functionality built into the standard
 * library. The output is stored in the passed NegotiationData struct. The negotiation data is
 * generated by creating two random 16-bit numbers, one for the actual guess and another for an
 * encryptionKey used for encrypting the data. The 'encryptedGuess' is generated with an
 * XOR(guess, encryptionKey). The hash is simply an 8-bit value that is the XOR() of all of the
 * bytes making up both the guess and the encryptionKey. There is no checking for NULL pointers
 * within this function.
 * @param data The struct used for both input and output of negotiation data.
 */
void ProtocolGenerateNegotiationData(NegotiationData *data)
{
    uint8_t hashValue;
    uint16_t guess, encryptedKey;
    guess = rand() & 0xFFFF;
    encryptedKey = rand() & 0xFFFF;
    data->guess = guess;
    data->encryptionKey = encryptedKey;
    data->encryptedGuess = XOR(guess, encryptedKey);

    //this is just to find the hash
    hashValue = 0;
    hashValue ^= ((guess & 0xFF) << 8);
    hashValue ^= (guess & 0xFF00);

    data->hash = hashValue;

    printf("Guess: %d key: %d EncryptedGuess: %d Hash: %d\n", data->guess, encryptedKey, data->encryptedGuess, hashValue);
}

/**
 * Validates that the negotiation data within 'data' is correct according to the algorithm given in
 * GenerateNegotitateData(). Used for verifying another agent's supplied negotiation data. There is
 * no checking for NULL pointers within this function. Returns TRUE if the NegotiationData struct
 * is valid or FALSE on failure.
 * @param data A filled NegotiationData struct that will be validated.
 * @return TRUE if the NegotiationData struct is consistent and FALSE otherwise.
 */
uint8_t ProtocolValidateNegotiationData(const NegotiationData *data)
{
    uint16_t guess = data->guess;
    uint16_t key = data->encryptionKey;
    uint8_t hashValue;

    hashValue = 0;
    hashValue ^= ((guess & 0xFF) << 8);
    hashValue ^= (guess & 0xFF00);

    //checking to see if the data is correct
    if (data->encryptedGuess == XOR(guess, key)) {
        if (data->hash == hashValue) {
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        return TRUE;
    }
}

/**
 * This function returns a TurnOrder enum type representing which agent has won precedence for going
 * first. The value returned relates to the agent whose data is in the 'myData' variable. The turn
 * ordering algorithm relies on the XOR() of the 'encryptionKey' used by both agents. The least-
 * significant bit of XOR(myData.encryptionKey, oppData.encryptionKey) is checked so that if it's a
 * 1 the player with the largest 'guess' goes first otherwise if it's a 0, the agent with the
 * smallest 'guess' goes first. The return value of TURN_ORDER_START indicates that 'myData' won,
 * TURN_ORDER_DEFER indicates that 'oppData' won, otherwise a tie is indicated with TURN_ORDER_TIE.
 * There is no checking for NULL pointers within this function.
 * @param myData The negotiation data representing the current agent.
 * @param oppData The negotiation data representing the opposing agent.
 * @return A value from the TurnOrdering enum representing which agent should go first.
 */
TurnOrder ProtocolGetTurnOrder(const NegotiationData *myData, const NegotiationData *oppData)
{
    uint16_t xor = XOR(myData->encryptionKey, oppData->encryptionKey);
    uint8_t smallestBit = xor & 0x01;
    if (smallestBit == 1) {
        //I will go first
        if (myData->guess > oppData->guess) {
            return TURN_ORDER_START;
        } else if (myData->guess < oppData->guess) {
            //Oponent goes first
            return TURN_ORDER_DEFER;
        } else {
            return TURN_ORDER_TIE;
        }
    }
    if (smallestBit == 0) {
        //They go first
        if (myData->guess < oppData->guess) {
            return TURN_ORDER_DEFER;
        } else if (myData->guess > oppData->guess) {
            //I go first
            return TURN_ORDER_START;
        } else {
            return TURN_ORDER_TIE;
        }
    }
    //if it gets here they tie
    return TURN_ORDER_TIE;
}

uint8_t genrateCheckSum(char* message)
{
    uint8_t checkSum = 0;

    while (*message) {
        checkSum ^= *message;
        message++;
    }


    return checkSum;
}

uint8_t hexToInt(char hex)
{

    if (hex >= '0' && hex <= '9') {
        return hex - '0';
    }
    if (hex >= 'A' && hex <= 'F') {
        return (10 + (hex - 'A'));
    }

    return NOT_VALID_HEX; //if it gets here it is not a valid number
}