/* 
 * File:   ArtificialAgent.c
 * Author: bowenbrooks and Sophia Tacderas
 *
 * Created on March 8, 2016, 4:19 PM
 */

#include "Agent.h"
#include "Field.h"
#include "Leds.h"
#include "FieldOled.h"
#include "Oled.h"

static Field myField, enemyField;
static AgentState state = AGENT_STATE_GENERATE_NEG_DATA;
static AgentEvent event = AGENT_EVENT_NONE;
static NegotiationData MyData;
static NegotiationData EnemyData;
static GuessData myGuess;
static GuessData enemyGuess;
static FieldOledTurn fieldTurn;
static ProtocolParserStatus pStatus;
static TurnOrder turnorder;

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts. This can include things like initialization of the field, placement of the boats,
 * etc. The agent can assume that stdlib's rand() function has been seeded properly in order to
 * use it safely within.
 */
void AgentInit(void)
{
    LEDS_INIT();
    FieldInit(&myField, FIELD_POSITION_EMPTY);
    FieldInit(&enemyField, FIELD_POSITION_UNKNOWN);
    FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_NONE);

    uint8_t row, col, direction;
    int index;
    BoatType boatType = FIELD_BOAT_SMALL;
    for (index = 0; index <= FIELD_BOAT_HUGE; index++) {
        row = rand() & (FIELD_ROWS - 1);
        col = rand() & (FIELD_COLS - 1);
        direction = rand() & 0x03;
        printf("Row: %d Col: %d dir: %d\n", row, col, direction);

        FieldAddBoat(&myField, row, col, direction, boatType);
        boatType++;
        FieldOledDrawScreen(&myField, &enemyField, FIELD_OLED_TURN_NONE);
    }




}

/**
 * The Run() function for an Agent takes in a single character. It then waits until enough
 * data is read that it can decode it as a full sentence via the Protocol interface. This data
 * is processed with any output returned via 'outBuffer', which is guaranteed to be 255
 * characters in length to allow for any valid NMEA0183 messages. The return value should be
 * the number of characters stored into 'outBuffer': so a 0 is both a perfectly valid output and
 * means a successful run.
 * @param in The next character in the incoming message stream.
 * @param outBuffer A string that should be transmit to the other agent. NULL if there is no
 *                  data.
 * @return The length of the string pointed to by outBuffer (excludes \0 character).
 */
int AgentRun(char in, char *outBuffer)
{
    int row, col;
    int length;

    if (event & AGENT_EVENT_MESSAGE_PARSING_FAILED) {
        state = AGENT_STATE_INVALID;
        return 0;
    }
    
    if (in) {
        pStatus = ProtocolDecode(in, &EnemyData, &enemyGuess);
    }

    switch (state) {
    case AGENT_STATE_GENERATE_NEG_DATA:
        ProtocolGenerateNegotiationData(&MyData); //generating the cha message
        state = AGENT_STATE_SEND_CHALLENGE_DATA;
        length = ProtocolEncodeChaMessage(outBuffer, &MyData);
        printf("Generating negotiation data: %s\n", outBuffer);
        return length;
        break;
    case AGENT_STATE_SEND_CHALLENGE_DATA:
        if (pStatus == PROTOCOL_PARSED_CHA_MESSAGE) {
            event = AGENT_EVENT_RECEIVED_CHA_MESSAGE;
            printf("Parsed CHA message\n");
        }
        if (event & AGENT_EVENT_RECEIVED_CHA_MESSAGE) {
            //decoding the incoming challange
            printf("Cha message Guess: %d Hash: %d\n", EnemyData.encryptedGuess, EnemyData.hash);
            state = AGENT_STATE_DETERMINE_TURN_ORDER;
            length = ProtocolEncodeDetMessage(outBuffer, &MyData);
            printf("Encoding DET message: %s\n", outBuffer);
            return length; //generatting the DET message
        }
        break;
    case AGENT_STATE_DETERMINE_TURN_ORDER:
        if (ProtocolValidateNegotiationData(&EnemyData)) {
            turnorder = ProtocolGetTurnOrder(&MyData, &EnemyData); //getting the turn order
            printf("Validated negotiation data\n");
            switch (turnorder) {
            case TURN_ORDER_TIE:
                state = AGENT_STATE_INVALID;
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(AGENT_ERROR_STRING_ORDERING);
                OledUpdate();
                printf("Tie\n");
                break;
            case TURN_ORDER_START:
                state = AGENT_STATE_SEND_GUESS;
                fieldTurn = FIELD_OLED_TURN_MINE; //setting the turn to mine
                FieldOledDrawScreen(&myField, &enemyField, fieldTurn); //updating the screen
                printf("Start\n");
                break;
            case TURN_ORDER_DEFER:
                state = AGENT_STATE_WAIT_FOR_GUESS;
                printf("Defer\n");
                break;
            }
        } else {
            state = AGENT_STATE_INVALID;
            OledClear(OLED_COLOR_BLACK);
            OledDrawString(AGENT_ERROR_STRING_NEG_DATA);
            OledUpdate();
            printf("Invalid\n");
        }
        break;
    case AGENT_STATE_SEND_GUESS:
        row = rand() & (FIELD_ROWS - 1);
        col = rand() & (FIELD_COLS - 1);
        myGuess.col = col;
        myGuess.row = row;
        state = AGENT_STATE_WAIT_FOR_HIT;
        length = ProtocolEncodeCooMessage(outBuffer, &myGuess);
        printf("Sending guess %s\n", outBuffer);
        return length;
        break;
    case AGENT_STATE_WAIT_FOR_HIT:
        pStatus = ProtocolDecode(in, &EnemyData, &myGuess); //decoding the incoming message
        if (pStatus == PROTOCOL_PARSED_HIT_MESSAGE && AgentGetEnemyStatus() == 0) {
            state = AGENT_STATE_WON;
            fieldTurn = FIELD_OLED_TURN_NONE;
            FieldOledDrawScreen(&myField, &enemyField, fieldTurn);
            printf("Last boat hit\n");
        } else {
            state = AGENT_STATE_WAIT_FOR_GUESS;
            FieldUpdateKnowledge(&enemyField, &enemyGuess);
            fieldTurn = FIELD_OLED_TURN_THEIRS;
            FieldOledDrawScreen(&myField, &enemyField, fieldTurn);
            printf("Their turn\n");
        }
        break;
    case AGENT_STATE_WAIT_FOR_GUESS:
        printf("Waiting for guess\n");
        pStatus = ProtocolDecode(in, &EnemyData, &myGuess);
        if (pStatus == PROTOCOL_PARSED_COO_MESSAGE && AgentGetStatus() > 0) {
            state = AGENT_STATE_SEND_GUESS;
            FieldRegisterEnemyAttack(&myField, &enemyGuess);
            fieldTurn = FIELD_OLED_TURN_MINE;
            FieldOledDrawScreen(&myField, &enemyField, fieldTurn);
            printf("Guess was not correct\n");
            return ProtocolEncodeHitMessage(outBuffer, &enemyGuess);
        } else {
            state = AGENT_STATE_LOST;
            fieldTurn = FIELD_OLED_TURN_NONE;
            FieldOledDrawScreen(&myField, &enemyField, fieldTurn);
            printf("I lost this battle\n");
            return ProtocolEncodeHitMessage(outBuffer, &enemyGuess);
        }
        break;
    case AGENT_STATE_INVALID:
        printf("Invalid\n");
        break;
    case AGENT_STATE_LOST:
        printf("lost\n");
        break;
    case AGENT_STATE_WON:
        printf("won\n");
        break;
    }

    return 0;
}

/**
 * StateCheck() returns a 4-bit number indicating the status of that agent's ships. The smallest
 * ship, the 3-length one, is indicated by the 0th bit, the medium-length ship (4 tiles) is the
 * 1st bit, etc. until the 3rd bit is the biggest (6-tile) ship. This function is used within
 * main() to update the LEDs displaying each agents' ship status. This function is similar to
 * Field::FieldGetBoatStates().
 * @return A bitfield indicating the sunk/unsunk status of each ship under this agent's control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetStatus(void)
{
    return FieldGetBoatStates(&myField);
}

/**
 * This function returns the same data as `AgentCheckState()`, but for the enemy agent.
 * @return A bitfield indicating the sunk/unsunk status of each ship under the enemy agent's
 *         control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetEnemyStatus(void)
{
    return FieldGetBoatStates(&enemyField);
}
