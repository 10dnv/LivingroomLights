/* ############### INCLUDES ############### */
#include <functions.h>
#include <math.h>

/* ############### GLOBAL VARIABLES ############### */
IRrecv irrecv(PIN_IR_RECEIVER);

WiFiServer server(80);
const char* ssid = "xxxxx";
const char* password = "xxxx";

decode_results results;

uint8 activeFunction = SM_Function_Idle;

/* ############### PROTOTYPES ############### */
static void Function_IRLearn();
static void Function_Idle();
static void connectWifi();

/* Function name: void Btn_MainFunction()
** Description  : Main logic for button state
** Params       : param_btnStruct - pointer to button entity
** Return       : -
*/
 static void Btn_MainFunction( struct_button * param_btnStruct)
{
    param_btnStruct->button_CountUp++;

    /* Update button state */
 
    if (digitalRead(param_btnStruct->button_pin) == LOW)
    {
        param_btnStruct->button_CountUp++;

        if (param_btnStruct->button_CountUp > BUTTON_DEBOUNCE_COUNTER_LONG_PRESS)
        {
            param_btnStruct->button_state = Button_LongPressed;
        } else if (param_btnStruct->button_CountUp > BUTTON_DEBOUNCE_COUNTER_PRESS)
        {
            param_btnStruct->button_state = Button_Pressed;
            param_btnStruct->button_CountDown = 0;
        }
    }else if (digitalRead(param_btnStruct->button_pin) == HIGH)
    {
        if((param_btnStruct->button_state == Button_Pressed) || (param_btnStruct->button_state == Button_LongPressed))
        {
            param_btnStruct->button_state = Button_Released;
        }

        param_btnStruct->button_CountDown++;
        if (param_btnStruct->button_CountDown > BUTTON_DEBOUNCE_COUNTER_NOT_PRESSED)
        {
            param_btnStruct->button_CountUp = 0;
            param_btnStruct->button_state = Button_NotPressed;
        }
    }
}

/* Function name: void Btn_Cyclic()
** Description  : Button heartbeat function. Cycles trough all the buttons and catches any state change
** Params       : -
** Return       : -
*/
void Btn_Cyclic()
{
    for(uint8 i = 0; i < NUMBER_OF_BUTTONS; i++ )
    {
        Btn_MainFunction(&BtnArray[i]);
    }
}

/* Function name: void Relay_Cyclic()
** Description  : Relay heartbeat function. Cycles trough all the relays and updates its state
** Params       : -
** Return       : -
*/
void Relay_Cyclic()
{
    for(uint8 i = 0; i < NUMBER_OF_RELAYS; i++ )
    {
        digitalWrite(RelayArray[i].relay_pin, RelayArray[i].relay_state);
    }
}

/* Function name: void Function_Init()
** Description  : Initial Function; Used to start local server;
** Params       : -
** Return       : -
*/
void Function_Init()
{
    irrecv.enableIRIn();
    WiFi.begin(ssid, password);
    server.begin();

#ifdef DEBUG_INFO
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("Server started");

    /* Print the IP address */
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
#endif /* #ifdef DEBUG_INFO */
}

/* Function name: void Function_IRLearn()
** Description  : This function enables the user to register new IR Remote (ex. TV remote) buttons.
** Params       : -
** Return       : -
*/
static void Function_IRLearn()
{
    static uint8 index = 1;

    if (index == 1)
    {
        for(uint8 i = 0; i < NUMBER_OF_RELAYS; i++ )
        {
            /* Turn off all the relays */
            RelayArray[i].relay_state = STD_RELAY_OFF;
        }
    }
#ifdef DEBUG_INFO
    Serial.println("Waiting for codes...");
#endif /* #ifdef DEBUG_INFO */
    if (irrecv.decode(&results)) 
    {
        switch(index)
        {
            
            case 1:
            {
                btnCodesIR_buffer.code_button1 = results.value;
#ifdef DEBUG_INFO
                Serial.print("Code #1: ");
                Serial.println(btnCodesIR_buffer.code_button1, HEX);
#endif /* #ifdef DEBUG_INFO */
                index = 2;
                delay(500);
                break;
            }
            case 2:
            {
                btnCodesIR_buffer.code_button2 = results.value;
#ifdef DEBUG_INFO
                Serial.print("Code #2: ");
                Serial.println(btnCodesIR_buffer.code_button2, HEX);
#endif /* #ifdef DEBUG_INFO */
                index = 3;
                delay(500);
                break;
            }
            case 3:
            {
                btnCodesIR_buffer.code_button3 = results.value;
#ifdef DEBUG_INFO
                Serial.print("Code #3: ");
                Serial.println(btnCodesIR_buffer.code_button3, HEX);
#endif /* #ifdef DEBUG_INFO */
                delay(500);
                
                /* Save new codes in EEPROM in case of power outage */
                EEPROM.put(CODES_STRUCT_ADDR, btnCodesIR_buffer);
                EEPROM.commit();
                
                index = 1;
                /* Jump to idle function */
                activeFunction = SM_Function_Idle;
                break;
            }
        }

        irrecv.resume(); /* Receive the next IR value */
    }
}

/* Function name: void MainAppStateMachine()
** Description  : Main application state machine. Enables transitioning through different functions.
** Params       : -
** Return       : -
*/
void MainAppStateMachine()
{
    switch (activeFunction)
    {
        case SM_Function_Idle:
        {
             Function_Idle();
            break;
        }
        case SM_Function_Learn:
        {
            Function_IRLearn();
            break;
        }
        default:
        break;
    }
}

/* Function name: void Function_Idle()
** Description  : When this function is active, system is waiting for commands.
** Params       : -
** Return       : -
*/
static void Function_Idle()
{
    static uint8 lastBtnState[3] = { Button_NotPressed, Button_NotPressed, Button_NotPressed};
    static int counter = 0;
    counter++;
    
    connectWifi();

    /* Check if learning mode is being requested; This consists of buttons 0 & 1 to be long pressed*/
    if ((BtnArray[0].button_state == Button_LongPressed) && (BtnArray[1].button_state == Button_LongPressed))
    {
        activeFunction = SM_Function_Learn;
    }

    /* Check if memorized remote control buttons are pressed; Every 300 cycles */
    if(counter % 300 == 0)
    {
        if (irrecv.decode(&results)) 
            {
#ifdef DEBUG_INFO
                serialPrintUint64(results.value, HEX);
#endif /* #ifdef DEBUG_INFO */
                
                if(btnCodesIR_buffer.code_button1 == results.value)
                {
                    if(RelayArray[0].relay_state == STD_RELAY_OFF)
                    {
                        RelayArray[0].relay_state = STD_RELAY_ON;
                    }
                    else
                    {
                        RelayArray[0].relay_state = STD_RELAY_OFF;
                    }
                }

                if(btnCodesIR_buffer.code_button2 == results.value)
                {
                    if(RelayArray[1].relay_state == STD_RELAY_OFF)
                    {
                        RelayArray[1].relay_state = STD_RELAY_ON;
                    }
                    else
                    {
                        RelayArray[1].relay_state = STD_RELAY_OFF;
                    }
                }

                if(btnCodesIR_buffer.code_button3 == results.value)
                {
                    if(RelayArray[2].relay_state == STD_RELAY_OFF)
                    {
                        RelayArray[2].relay_state = STD_RELAY_ON;
                    }
                    else
                    {
                        RelayArray[2].relay_state = STD_RELAY_OFF;
                    }
                }
                irrecv.resume();
            }
            counter = 0;
    }else
    {
        /* Check physical buttons input */
        for(uint8 i = 0; i < NUMBER_OF_BUTTONS; i++ )
        {
            if ((BtnArray[i].button_state == Button_Pressed) && (lastBtnState[i] == Button_NotPressed))
            {
                if(RelayArray[i].relay_state == STD_RELAY_OFF)
                {
                    RelayArray[i].relay_state = STD_RELAY_ON;
                }
                else
                {
                    RelayArray[i].relay_state = STD_RELAY_OFF;
                }
            }
            lastBtnState[i] = BtnArray[i].button_state;
        }
    }
}

/* Function name: void connectWifi()
** Description  : This function handles the web requests and renders the webpage interface.
** Params       : -
** Return       : -
*/
static void connectWifi()
{
    /* Check if a client has connected */
    WiFiClient client = server.available();
    if (!client)
    {
        return;
    }

    /* Read the first line of the request */
    String request = client.readStringUntil('\r');
#ifdef DEBUG_INFO
    Serial.println(request);
#endif /* #ifdef DEBUG_INFO */
    client.flush();

    /* Match the request */
    String partON  ="";
    String partOFF ="";

    for (uint8 i = 0; i < NUMBER_OF_RELAYS; i++)
    {
        partON = "/light";
        partON += i;
        partON +="=ON";

        partOFF = "/light";
        partOFF += i;
        partOFF +="=OFF";

        if (request.indexOf(partON) != -1)
        {
            RelayArray[i].relay_state = STD_RELAY_ON;
        }

        if (request.indexOf(partOFF) != -1)
        {
            RelayArray[i].relay_state = STD_RELAY_OFF;
        }
 
    }

        /* Return the response */
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("");
        client.println("<!DOCTYPE HTML>");
        client.println("<html>");

        client.println("<div width = \" 100%\">");
        client.println("<br><br>");
        client.println("<a href=\"/light0=ON\"\"><button>Light 1 ON </button></a>");
        client.println("<a href=\"/light0=OFF\"\"><button>Light 1 OFF </button></a>");

        if(RelayArray[0].relay_state == STD_RELAY_ON)
        {
            client.println("<button style=\"background-color: green\">STATE</button>");
        }
        else
        {
            client.println("<button style=\"background-color: red\">STATE</button>");
        }
        client.println("<br>");

        client.println("<a href=\"/light1=ON\"\"><button>Light 2 ON </button></a>");
        client.println("<a href=\"/light1=OFF\"\"><button>Light 2 OFF </button></a>");

        if(RelayArray[1].relay_state == STD_RELAY_ON)
        {
            client.println("<button style=\"background-color: green\">STATE</button>");
        }
        else
        {
            client.println("<button style=\"background-color: red\">STATE</button>");
        }

        client.println("<br>");

        client.println("<a href=\"/light2=ON\"\"><button>Light 3 ON </button></a>");
        client.println("<a href=\"/light2=OFF\"\"><button>Light 3 OFF </button></a>");
        if(RelayArray[2].relay_state == STD_RELAY_ON)
        {
            client.println("<button style=\"background-color: green\">STATE</button>");
        }
        else
        {
            client.println("<button style=\"background-color: red\">STATE</button>");
        }

        client.println("</div>");
        client.println("</html>");
}