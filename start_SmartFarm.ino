#include <TimerOne.h> // TimeOne라이브러리 사용
#include <DHT.h> // DHT라이브러리 사용
#define DHTPIN 2 //핀 번호 설정
#define DHTTYPE DHT11 // DHT 사용 번호 설정 (DHT11)
DHT dht(DHTPIN, DHTTYPE); // DHT 설정

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN       6 //NeoPixel LED 핀 번호
#define NUMPIXELS 9 //NeoPixel LED 개수
#define DELAYVAL 1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//-------타이머 인터럽트를 위한 타이머 카운트------------
int time_count = 0;
int hum_count = 0;
int temp_count = 0;
int light_count = 0;
int pump_count = 0;

int Pcount = 0;

//------모듈 사용---------
int pump = 5;
int fan = 4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // 시리얼 모니터 통신속도 9600 설정
  ///////////////////////////////////////////////////////////////
  dht.begin(); // DHT 초기화
  ///////////////////////////////////////////////////////////////
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show();  // Initialize all pixels to 'off'
  ///////////////////////////////////////////////////////////////
  pinMode(pump, OUTPUT);
  pinMode(fan, OUTPUT);
  ///////////////////////////////////////////////////////////////
  Timer1.initialize(1000); //1000000μs = 1s, 1000us = 0.001s, 1000us = 1ms
  Timer1.attachInterrupt(timerIsr);
}

void loop() {
  //--------------온도/습도 값 읽기 및 변수 선언-----------------
  int h = dht.readHumidity(); //습도 값 저장 변수 선언
  int t = dht.readTemperature(); //온도 값 저장 변수 선언
  int l = analogRead(A0);

  //------습도 값 읽기 카운터-------
  if(hum_count == 500){
    hum(h);
    hum_count = 0;
  }

  //------온도 값 읽기 카운터-------
  if(temp_count == 510){
    temp(t);
    temp_count = 0;
  }

  //------조도센서 값 읽기 카운터-------
  if(light_count == 520){
    light(l);
    light_count = 0;
  }

  //------펌프 출력 카운터-------
  if(pump_count == 5000){
    PUMP();
    pump_count = 0;
  }
}

//------------인터럽트를 위한 카운터 호출 함수----------------------
void timerIsr(){
  time_count += 1;
  hum_count += 1;
  temp_count += 1;
  light_count += 1;
  pump_count += 1;
}

void hum(int h){
  Serial.print("습도:");
  Serial.println(h); // 습도 값 출력
}

void temp(int t){
  Serial.print("온도:");
  Serial.println(t); // 온도 값 출력
  if(t >= 25){
    digitalWrite(fan, HIGH);
  } else{
    digitalWrite(fan, LOW);
  }
}

void light(int l){
  Serial.print("조도:");
  Serial.println(l);
  if(l >= 400){
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 150, 0));
      pixels.show();
      delay(DELAYVAL); // Pause before next pass through loop
    }
  } else{
    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      delay(DELAYVAL); // Pause before next pass through loop
    }
  }
}

void PUMP(){
  if(++Pcount >= 10 )
  {
    Count1 = 0;
    digitalWrite(13,HIGH);
  }
  else 
  {
    digitalWrite(13,LOW);
  }
}
