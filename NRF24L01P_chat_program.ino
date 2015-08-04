#include<SPI.h>
#include"nRf24L01.h"
#include"RF24.h"
#include"printf.h"
RF24 radio(9, 10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
bool go_to_tx_mode = false;
char input[32] = "";
void setup() {
  Serial.begin(57600);
  printf_begin();
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);
  radio.startListening();
  radio.printDetails();
  // put your setup code here, to run once:
 
}
 
void loop() {
  if (go_to_tx_mode)
  {
    radio.stopListening();
    bool ok = radio.write(input, 32);
    if (ok)
    {
      printf("ME : %s\n", input);
      go_to_tx_mode = false;
      radio.startListening();
    }
    else
      printf("could not write....\n");
  }
  if (radio.available())
  {
    char payload[32] = "";
    bool done = false;
    while (!done)
    {
      done = radio.read( payload , 32 );
      printf("HIM : %s\n", payload);
    }
 
  }
  if (Serial.available())
  { 
    int len = Serial.readBytesUntil('\n', input, 31);
    input[len] = '\0';
    go_to_tx_mode = true;
  }
 
}
