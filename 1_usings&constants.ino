#include <FastLED.h>   //Светодиоды
#include <IRremote.h>  //Пульт
#include <EEPROM.h>    //Энергонезваисимая память

CRGB leds[STRIP_LENGTH];
IRrecv irReciver(REMOTE_PIN);
decode_results results;

bool is_frozen = false;
unsigned long freeze_time;

struct {
  byte speed = 120;
  byte hue = 30;
  byte saturation = 127;
  byte value = 127;
  long mode = 0xFFB04F;
} settings;

unsigned long prev_settings_time;
bool is_settings_changed = false;

void setup() {
  if(IS_DEBUG){
    Serial.begin(9600);
  }
  if (EEPROM.read(0) != 'b') {  //Читаем или записываем настройки
    EEPROM.put(1, settings);
    EEPROM.write(0, 'b');

    if(IS_DEBUG){
      Serial.println("first time");
    }
  } else {
    EEPROM.get(1, settings);
    Serial.println("settings applyed");
  }

  pinMode(A1, INPUT);
  randomSeed(analogRead(A1));

  FastLED.addLeds<STRIP_ALIAS, STRIP_DATA_PIN, GRB>(leds, STRIP_LENGTH).setCorrection(TypicalLEDStrip);  //Добавление светодиодной ленты
  FastLED.setBrightness(MAX_BRIGHTNESS);

  if (IS_DEBUG) {
    //Если среда дебажная, то настраиваем общение с компом
    Serial.println(settings.speed);
    Serial.println(settings.hue);
    Serial.println(settings.saturation);
    Serial.println(settings.value);
    Serial.println(settings.mode, HEX);
  }

  irReciver.enableIRIn();  //Включение приёма ИК сигнала
}