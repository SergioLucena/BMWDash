/*--------------------------------------------------------------
  Programa:     Display monitor de parâmetros para BMW K1200RS

  Descrição:  Monitor para nível combustível, tensão da bateria, 
              temperatura do motor e canal auxiliar
 
  Hardware:     Arduino UNO com divisor de tensão em A0 a A3.
                TFT LCD ST 7735
               
  Software:     Desenvolvido usando software Arduino 1.6.12 
               

  Data:         10 /02/2017

  Author:      Sergio de Lucena
--------------------------------------------------------------*/
#define sclk 13
#define mosi 11
#define cs   10
#define dc   9
#define rst  8  //  reset

#include   <Adafruit_GFX.h>  // Core graphics library
#include   <Adafruit_ST7735.h>// Hardware-specific library
#include   <SPI.h>
Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);
// number of analog samples to take per reading, per channel
#define NSAMP 100 // number of samples to take befor displaying
// voltage divider calibration values
#define Dv1    100.00
#define Dv2    11.001
#define Dv3    110.00
#define Dv4    10.985
// ADC reference voltage / calibration value
#define VREF    5.00

float sum[4] = {0};                // sums of samples taken
unsigned char Scount = 0;  // current sample number
float AVvolts[4] = {0};        // calculated voltages
char cnt1 = 0;                  // used in 'for' loops



void setup()
{
  
   tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
   tft.fillScreen(ST7735_BLUE); //  blue screen
   tft.setTextSize(3);
   tft.setCursor(39,40);
   tft.println("BMW");
   delay(1000);

   tft.setCursor(1,80);
   tft.println("K1200RS");
   delay(1500);
   tft.fillScreen(ST7735_BLACK); //  clear screen
   tft.setTextColor(ST7735_RED);
   tft.drawRoundRect(2, 20, 120, 110, 5,ST7735_WHITE);
   tft.drawRoundRect(36, 0, 53, 19, 5,ST7735_RED);
   tft.setTextSize(2);
     tft.setCursor(46,2);
   tft.println("BMW");
   tft.setTextSize(1);
   tft.setTextColor(ST7735_CYAN,ST7735_BLACK);
   tft.setCursor(0,140);
   tft.println("    System Monitor");
}

void loop()
{
  
    // take a number of analog samples and add them up
    while (Scount < NSAMP) {
        // sample each channel A0 to A3
        for (cnt1 = 0; cnt1 < 4; cnt1++) {
            sum[cnt1] += analogRead(A0 + cnt1);
        }
        Scount++;
        delay(10);
    }
    // calculate the voltage for each channel
    for (cnt1 = 0; cnt1 < 4; cnt1++) {
        AVvolts[cnt1] = ((float)sum[cnt1] / (float)NSAMP * VREF) / 1024.0;
    }
    // display voltages on TFT LCC Display
   
  
    // voltage 1 - V1(pin A0  
    tft.setTextColor(ST7735_YELLOW,ST7735_BLACK); // set color for V1
    tft.setTextSize(2);
    if(AVvolts[0]*Dv1 < 20)//Ajusta limite do aviso de baixo nível de combustível
    { tft.setTextColor(ST7735_BLACK,ST7735_WHITE);

    }
    
     

   
    tft.setCursor(10, 28);
    tft.print("Fuel ");
   // tft.setCursor(65, 28);
    tft.print(AVvolts[0] * Dv1, 0);
    tft.setCursor(105, 28);
    tft.print("%");
    tft.setCursor(70,28);
    if(AVvolts[0]*Dv1 > 99)
        {tft.print("100");
        }
        tft.setCursor(93,28);
        if(AVvolts[0]*Dv1 < 99){
        tft.print(" ");
    }  
      
    // voltage 2 - V2(pin A1)
    tft.setTextColor(ST7735_GREEN,ST7735_BLACK);// set color for V2
    tft.setCursor(10, 55);
    tft.print("Bat ");
    tft.setCursor(55, 55);
    tft.print(AVvolts[1] * Dv2, 1);

    tft.setCursor(90,55);
        if(AVvolts[1]*Dv2 < 10){
        tft.print(" ");
        }
    tft.setCursor(105, 55);
    tft.print("V");
    // voltge 3 - V3(pin A2)
    
    tft.setTextColor(ST7735_CYAN,ST7735_BLACK);// set color for V3
    if(AVvolts[2]*Dv3 > 100)//Ajusta limite do aviso de TEMPERATURA ALTA
     {tft.setTextColor(ST7735_YELLOW,ST7735_RED);
      }
    tft.setCursor(10, 82);
    tft.print("Temp ");
    tft.setCursor(60, 82);
    tft.print(AVvolts[2] * Dv3, 0);
     tft.setCursor(84,82);
        if(AVvolts[2]*Dv3 < 99){
        tft.print(" ");
        }
    tft.setTextSize(1);
    tft.setCursor(97, 82);
    tft.print("o");
    tft.setTextSize(2);
    tft.setCursor(105, 82);
    tft.print("C");
    // voltage 4 - V4(pin A3)
    tft.setTextColor(ST7735_WHITE,ST7735_BLACK);// set color for V4
    tft.setCursor(10, 108);
      if(AVvolts[2]*Dv3 > 99){
        tft.print(" ATENCAO ");
        }
        if(AVvolts[2]*Dv3 < 99){
        tft.print("         ");
        }
         tft.setCursor(10, 108);
        if(AVvolts[0]*Dv1 < 20){
        tft.print(" ATENCAO ");
        }
        delay(200);
        if(AVvolts[0]*Dv1 > 20){
        tft.print("         ");
        }
    //tft.setCursor(65, 110);
    //tft.print(AVvolts[3] * Dv4, 0);
    //tft.setCursor(105, 110);
   // tft.print("V");
    tft.drawRoundRect(2, 20, 120, 110, 5,ST7735_WHITE);
    tft.drawLine(4,48,120,48,ST7735_WHITE);
    tft.drawLine(4,75,120,75,ST7735_WHITE);
    tft.drawLine(4,103,120,103,ST7735_WHITE);
   
      
    // reset count and sums
    Scount = 0;
    for (cnt1 = 0; cnt1 < 4; cnt1++) {
        sum[cnt1] = 0;
    }
}

