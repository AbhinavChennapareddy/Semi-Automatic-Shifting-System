/////////////////////////////////////////////////////               START of SPI Define start
#include <SPI.h>
volatile byte pos;                                                  //used for SPI buffer position, its set to global so it can be reset after populated to RPM array
uint8_t buf[4];
                                                                    //Start of Interrupt routine (ISR)
ISR (SPI_STC_vect)
{
  if (pos < (sizeof (buf)))                                         //Gets SPI data value (RPM) and places in array (buf)
    {
        buf [pos++] = SPDR;                                         //Populates buffer array with bytes at pos less then buff size
    }
}                                                                   // end of interrupt routine SPI_STC_vect

int raw2int()
{
  int data_packet = ((short)buf[1]<<8)+buf[3];
  if(data_packet > 32767)
    data_packet = data_packet - 65536;
    pos = 0;                                                        //Global pos variable set to 0 to reset buffer count for next interrupt buf population
    Serial.print("Converted Data from raw SPI: ");                  //Debugging serial output for raw data conversion
    Serial.println(data_packet);
    return(data_packet);                                            //Returns signed integer value after conversion via PE3's datasheet for CAN messages
}
////////////////////////////////////////////////////////             END of SPI define
  
  const int forwards = 2;
  const int backwards = 4;
  const int up = 0;
  const int down = 1;
  bool shiftup = 0;
  bool shiftdown = 0;
  const int ClutchOutsert = 8;   // HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA best var name ever!!
  const int ClutchInsert = 7;
  const int RPM_Pin;
  const int Position = 2;
  int POS;
  int RPM;

void setup() {

pinMode(forwards, OUTPUT);//Set relay as an output
pinMode(backwards, OUTPUT);//Set relay as an output
pinMode(ClutchInsert, OUTPUT);
pinMode(ClutchOutsert, OUTPUT);
pinMode(up, INPUT);
pinMode(down, INPUT);
pinMode(RPM_Pin, INPUT);
pinMode(Position, INPUT);
Serial.begin(19200);


/////////////////////////////////////////////////////////////////               START of SPI Setup
SPI.beginTransaction(SPISettings(800000, MSBFIRST, SPI_MODE0));
SPCR |= bit (SPE);                                                            // turn on SPI in slave mode
pinMode(MISO, OUTPUT);                                                        // have to send on master in, *slave out*
pos = 0;                                                                      // buffer empty to get ready for interrupts
SPCR |= bit(SPIE);                                                            // now turn on interrupts
/////////////////////////////////////////////////////////////////               END of SPI Setup  
}

void loop() 
{
  POS = analogRead(Position);                                                  // RPM value set to be analog value of RPM given by Simulation POT
  RPM = raw2int();                                                              // RPM value set to be numerical value of ECU given RPM

         
  int num = analogRead(up);
  int num2 = analogRead(down);
  Serial.println(POS);
  if(num2 > 900)
  {
    //digitalWrite(ClutchOutsert, LOW);
    //digitalWrite(ClutchInsert, HIGH);
    //delay(500);
    digitalWrite(forwards,HIGH);
    digitalWrite(backwards,LOW);
    delay(110);
    digitalWrite(forwards,LOW);
    digitalWrite(backwards,HIGH);
    delay(110);
    digitalWrite(forwards,HIGH);
    digitalWrite(backwards,HIGH);
    digitalWrite(ClutchOutsert, HIGH);
    digitalWrite(ClutchInsert, LOW);
    delay(100);
    
  }
  else
   digitalWrite(forwards,HIGH);
   digitalWrite(backwards,HIGH);
   digitalWrite(ClutchOutsert, HIGH);
   digitalWrite(ClutchInsert, HIGH);
  
  if(num > 900)
  {
    digitalWrite(ClutchOutsert, LOW);
    digitalWrite(ClutchInsert, HIGH);
    delay(100);
    digitalWrite(forwards,LOW);
    digitalWrite(backwards,HIGH);
    delay(110);
    digitalWrite(forwards,HIGH);
    digitalWrite(backwards,LOW);
    delay(110);
    digitalWrite(forwards,HIGH);
    digitalWrite(backwards,HIGH);
    //digitalWrite(ClutchOutsert, HIGH);
    //digitalWrite(ClutchInsert, LOW);
    //delay(500);
  }
  else 
   digitalWrite(backwards,HIGH);
   digitalWrite(forwards,HIGH);
   digitalWrite(ClutchInsert,HIGH);
   digitalWrite(ClutchOutsert,HIGH);
}

