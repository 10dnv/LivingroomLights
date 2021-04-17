/* ############### INCLUDES ############### */
#include <functions.h>
#include <math.h>

/* ############### VARIABILE GLOBALE ############### */
IRrecv irrecv(PIN_IR_RECEIVER);

WiFiServer server(80);
const char* ssid = "xxxxx";
const char* password = "xxxx";

decode_results results;

uint8 functieActiva = SM_Function_Idle;

/* ############### PROTOTIPURI ############### */
static void Function_IRLearn();
static void Function_Idle();
static void connectWifi();

/* Numele functiei: void set_luminozitate()
** Descriere      : Seteaza intensitatea luminoasa pentru banda LED
** Parametrii     : uint8 intensitate  [0:100]%
** Return         : -
*/


/* Numele functiei: uint8 get_luminozitate()
** Descriere      : Furnizeaza nivelul de luminozitate
** Parametrii     : -
** Return         : Procentul de luminozitate
*/
// uint8 get_luminozitate()
// {
//     uint16 suma = 0;
//     uint8     i = 0;
//     uint16 media_aritmetica = 0;
//     uint8 val_procentuala = 0;

//     for (i = 0; i < 30; i++)
//     {   
//         suma = suma + analogRead(PIN_PHOTORESISTOR);
//     }

//     media_aritmetica = suma / i;
//     val_procentuala = map(media_aritmetica, 0, 720, 0, 300);

//     return (val_procentuala);
// }



/* Numele functiei: uint8 encoder()
** Descriere      : Functia care citeste valoarea encoder-ului si starea butonului
** Parametrii     : -
** Return         : Directia de rotatie 1,0,2
*/
 static void Btn_MainFunction( struct_button * param_btnStruct)
{
    param_btnStruct->button_CountUp++;

     
    /* Actualizez starea butonului */
 
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

void Btn_Cyclic()
{
    for(uint8 i = 0; i < NUMBER_OF_BUTTONS; i++ )
    {
        Btn_MainFunction(&BtnArray[i]);
    }
}

void Relay_Cyclic()
{
    for(uint8 i = 0; i < NUMBER_OF_RELAYS; i++ )
    {
        digitalWrite(RelayArray[i].relay_pin, RelayArray[i].relay_state);
    }
}


void Function_Init()
{
    irrecv.enableIRIn(); 
    WiFi.begin(ssid, password);
     server.begin();
//      while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
  Serial.println("");
  Serial.println("WiFi connected");
    Serial.println("Server started");

    // Print the IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
}

static void Function_IRLearn()
{

    static uint8 index = 1;

    if (index == 1)
    {
        for(uint8 i = 0; i < NUMBER_OF_RELAYS; i++ )
        {
            RelayArray[i].relay_state = STD_RELAY_OFF;
        }
    }
    Serial.println("Astept cod");
    if (irrecv.decode(&results)) 
    {
        switch(index)
        {
            
            case 1:
            {

                Serial.print("codul 1: ");
                btnCodesIR_buffer.code_button1 = results.value;
                Serial.println(btnCodesIR_buffer.code_button1, HEX);
                index = 2;
               // RelayArray[0].relay_state = STD_RELAY_ON;
                delay(500);
                break;
            }
            case 2:
            {
                Serial.print("codul 2: ");
                btnCodesIR_buffer.code_button2 = results.value;
                Serial.println(btnCodesIR_buffer.code_button2, HEX);
                index = 3;
                //RelayArray[1].relay_state = STD_RELAY_ON;
                delay(500);
                break;
            }
            case 3:
            {
                Serial.print("codul 3: ");
                btnCodesIR_buffer.code_button3 = results.value;
                Serial.println(btnCodesIR_buffer.code_button3, HEX);
                //RelayArray[2].relay_state = STD_RELAY_ON;
                delay(500);
                
                  //salvez in EEPROM
                EEPROM.put(ADRESA_STRUCTURA_CODURI, btnCodesIR_buffer);
                EEPROM.commit();
                
                
                index = 1;
                functieActiva = SM_Function_Idle;
                break;
            }

        }

        irrecv.resume(); // Receive the next value
    }
        
}

void MainAppStateMachine()
{
    switch (functieActiva)
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
        case 3:
        {
            
            break;
        }
        case 4:
        {
            
            break;
        }
       
        
    
    default:
        break;
    }
}

static void Function_Idle()
{
    static uint8 lastBtnState[3] = { Button_NotPressed, Button_NotPressed, Button_NotPressed};
    static int counter = 0;
    counter++;
    
     connectWifi();
Serial.println(btnCodesIR_buffer.code_button1);
//Verific daca am combinatia de butoane pt learning
if ((BtnArray[0].button_state == Button_LongPressed) && (BtnArray[1].button_state == Button_LongPressed))
{
    functieActiva = SM_Function_Learn;
}


 if(counter % 300 ==0)/* Verific inputul de la telecomanda */
 {
     if (irrecv.decode(&results)) 
        {
            serialPrintUint64(results.value, HEX);
            
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
     /* verific inputul de la butoane */
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

static void connectWifi()
{
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) 
    {
    return;
    }

    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Match the request
    String partON  ="";
    String partOFF ="";
    for (uint8 i = 0; i < NUMBER_OF_RELAYS; i++)
    {
        partON = "/releu" ;
        partON += i;
        partON +="=ON";

        partOFF = "/releu" ;
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

         // Return the response
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println(""); //  do not forget this one
        client.println("<!DOCTYPE HTML>");
        client.println("<html>");

        client.println("<div width = \" 100%\">");
        client.println("<br><br>");
        client.println("<a href=\"/releu0=ON\"\"><button>Releu 1 ON </button></a>");
        client.println("<a href=\"/releu0=OFF\"\"><button>Releu 1 OFF </button></a>");
        if(RelayArray[0].relay_state == STD_RELAY_ON)
        {
            client.println("<button style=\"background-color: green\">STARE</button>");
        }
        else
        {
            client.println("<button style=\"background-color: red\">STARE</button>");
        }
        client.println("<br>");

        client.println("<a href=\"/releu1=ON\"\"><button>Releu 2 ON </button></a>");
        client.println("<a href=\"/releu1=OFF\"\"><button>Releu 2 OFF </button></a>");
        if(RelayArray[1].relay_state == STD_RELAY_ON)
        {
            client.println("<button style=\"background-color: green\">STARE</button>");
        }
        else
        {
            client.println("<button style=\"background-color: red\">STARE</button>");
        }
        
        
        client.println("<br>");

        client.println("<a href=\"/releu2=ON\"\"><button>Releu 3 ON </button></a>");
        client.println("<a href=\"/releu2=OFF\"\"><button>Releu 3 OFF </button></a>");
        if(RelayArray[2].relay_state == STD_RELAY_ON)
        {
            client.println("<button style=\"background-color: green\">STARE</button>");
        }
        else
        {
            client.println("<button style=\"background-color: red\">STARE</button>");
        }

        client.println("</div>");
        client.println("</html>");


}
