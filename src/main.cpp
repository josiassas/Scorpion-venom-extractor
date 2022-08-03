#include <Arduino.h>

 
// Bibliotecas
#include "Keypad.h"
#include "LiquidCrystal.h"   // library for Alphanumeric LCD display (DS1)

/* MAPEAMENTO DO LCD 16x2
    LCD RS pin to digital pin 12
    LCD Enable pin to digital pin 11
    LCD D4 pin to digital pin 5
    LCD D5 pin to digital pin 4
    LCD D6 pin to digital pin 3
    LCD D7 pin to digital pin 2 
*/
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/* MAPEAMENTO DO TECLADO
 * lin1 --> D7
 * lin2 --> D8
 * col1 --> D9
 * col2 --> D10
 */
const byte rows = 2; //duas linhas
const byte cols = 2; //duas colunas

char keys[rows][cols] = {
  {'2','1'},
  {'3','4'}
};

byte rowPins[rows] = {7, 9}; //pinos de conexão das linhas
byte colPins[cols] = {8, 10}; //pinos de conexão das colunas

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

const int extBot = A0;  // Button On/Off connected on pin 30 (estímulo)

const int con = 6;         //  Relay connected on pin 28
const int led = 13;          //  LED connected on pin 13

int ext = 0;

int variable  = 0;           // starting state variable (working time) = 0
int variable1 = 0;           // starting state variable (rest time) = 0

void disp();

void setup()
{
  // initialization of the Inputs / Outputs
  //Serial.begin(9600);
  pinMode(con, OUTPUT);   
  pinMode(led, OUTPUT);
  pinMode(extBot, INPUT);

  lcd.begin(16, 2);           // initialize The LCD 16 Columns / 2 Rows 
  
  lcd.setCursor(2, 0);       // set the cursor Column 6 / Row 1
  lcd.print("Iniciando..."); //
  delay(100);               // 
  
  lcd.clear();               // clear LCD Display
  delay(100);
  lcd.setCursor(4, 0);       // set the cursor Column 6 / Row 1
  lcd.print("Extrator");        // Write "Venom" Column 0 / Row 0
  lcd.setCursor(3, 1);       // set the cursor Column 6 / Row 1
  lcd.print("de  Veneno");    // Write "Extractor" Column 6 / Row 1
  delay(100);               // Waiting 2,5 secondes
  lcd.clear();               // clear LCD Display
  delay(100);
 }

void loop()// loop
{
  char key = keypad.getKey(); //captura a tecla pressionada

  int i = 5;

  disp(); //menu principal
  
  ext = digitalRead(extBot); // Read state of On/Off (HIGH or LOW)

  //incrementa o Ton
  if(key != NO_KEY){ // algum botão pressionado
    //Serial.print("Botão: ");
    //Serial.println(key);
    
    if((key == '1') & (variable < 999 ))
      variable++;
      
    //decrementa o Ton
    if((key == '2') & (variable > 0 ))
      variable--;
      
    //incrementa o Toff
    if((key == '3') & (variable1 < 999))
      variable1++;
    
    //decrementa o Toff
    if((key == '4') & (variable1 > 0))
      variable1--;

//    Serial.print("Ton: ");
//    Serial.print(variable);
//    Serial.print(" | ");
//    Serial.print("Toff: ");
//    Serial.println(variable1);
  } //end if key != no key
  
  //Ciclo para retirada do veneno (envia um ciclo de estímulo no animal)
  if(ext == HIGH){
    while(i > 0){ //sequência de i pulsos
      
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.write("Estimulando...");
      
      //imprime os ciclos restantes
      lcd.setCursor(8,1);
      lcd.print(i);
      
      digitalWrite(con, HIGH); // Relay = "on"
      digitalWrite(led, HIGH); // LED = "on"
      delay(variable * 1000); // Delay of the "Working time"
      //delay(variable1 * 100); // Delay of the "Working time"
      digitalWrite(con, LOW); // Relay = "off"
      digitalWrite(led, LOW); // LED = "off"
      delay(variable1 * 1000); // Delay of the "Rest time"
      //delay(variable1 * 100); // Delay of the "Rest time"
      lcd.clear();
      
      i--;
    } //end for
  }
 else{ //Button on/off state "off"
   digitalWrite(con, LOW); //Relay = "off"
   digitalWrite(led, LOW); // LED ="off"
 }
} //end loop

void disp(){
  //lcd.clear();               // clear LCD Display (DS1)
  lcd.setCursor(0, 0);       // set the cursor Column 0 / Row 0
  lcd.write("Estimulo: ");     // Write "Working:" Column 0 / Row 0
  lcd.setCursor(10, 0);      // set the cursor Column 11 / Row 0
  if(variable < 10)
    lcd.write("  ");
  else if(variable < 100)
    lcd.write(" ");
  lcd.print(variable);       // print "Variable" (corresponding to Working time) Column 11 / Row 0
  lcd.write(" s");           // Write "s" after "Variable"
  
  lcd.setCursor (0, 1);       // set the cursor Column 0 / Row 1
  lcd.write("Descanso: ");        // Write "rest:" Column 0 / Row 1
  lcd.setCursor(10, 1);      // set the cursor Column 11 / Row 1
  if(variable1 < 10)
    lcd.write("  ");
  else if(variable1 < 100)
    lcd.write(" ");
  lcd.print(variable1);       // print "Variable1" (corresponding to Rest time) Column 11 / Row 1
  lcd.write(" s");           // Write "s" after "Variable1"
} //end disp