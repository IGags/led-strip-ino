//Выключение
void empty() {
  for (int i = 0; i < STRIP_LENGTH; i++) {
    leds[i] = CHSV(0, 0, 0);
  }
  FastLED.show();
}

//Однотонный
void solid() {
  for (int i = 0; i < STRIP_LENGTH; i++) {
    leds[i] = CHSV(settings.hue, settings.saturation, settings.value);
  }
  FastLED.show();
}

//Пульсация
void pulse() {
  for (int i = 0; i < STRIP_LENGTH; i++) {
    float spd = map((float)settings.speed, 0.0, 255.0, 0.3, 3.0);
    spd = constrain(spd, 0.3, 3.0);
    leds[i] = CHSV(settings.hue, settings.saturation, sin8(spd * 128 * millis() / 1024));
  }
  FastLED.show();
}

//Конфети
void dots() {
  byte spd = map(settings.speed, 0, 255, 1, 40);
  fadeToBlackBy(leds, STRIP_LENGTH, spd);
  int pos = random16(STRIP_LENGTH);
  leds[pos] += CHSV(settings.hue, settings.saturation, settings.value);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.show();
}

void coloredDots() {
  static byte baza = 0;
  byte spd = map(settings.speed, 0, 255, 1, 40);
  fadeToBlackBy(leds, STRIP_LENGTH, spd);
  int pos = random16(STRIP_LENGTH);
  leds[pos] += CHSV(baza++ + random8(64), settings.saturation, settings.value);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.show();
}

void rainbow() {
  static byte counter;
  static unsigned long prev_measure;
  if (millis() - prev_measure > 5) {
    for (int i = 0; i < STRIP_LENGTH; i++) {
      leds[i] = CHSV(counter + i * (10 - map(settings.speed, 0, 255, 1, 10)), 255, 255);
    }
    counter++;
    FastLED.show();
    prev_measure = millis();
  }
}

void slightFill() {
  static byte baza = 0;
  fadeToBlackBy(leds, STRIP_LENGTH, 2);
  int pos = beatsin16(13, 0, STRIP_LENGTH - 1);
  leds[pos] += CHSV(baza++, 255, 192);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.show();
}

void cyclone() {
  static byte baza = 0;
  static unsigned long prev_measure;
  if (millis() - prev_measure > 30) {
    for (int i = 0; i < STRIP_LENGTH; i++) {
      leds[i].nscale8(250);
    }
    for (int i = 0; i < STRIP_LENGTH; i++) {
      leds[i] = CHSV(baza++, 255, 255);
      FastLED.setBrightness(MAX_BRIGHTNESS);
      FastLED.show();
    }
    prev_measure = millis();
  }
}

#define HUE_GAP 21      // заброс по hue
#define FIRE_STEP 15    // шаг огня
#define HUE_START 0     // начальный цвет огня (0 красный, 80 зелёный, 140 молния, 190 розовый)
#define MIN_BRIGHT 70   // мин. яркость огня
#define MAX_BRIGHT 255  // макс. яркость огня
#define MIN_SAT 245     // мин. насыщенность
#define MAX_SAT 255     // макс. насыщенность

void fire() {
  static unsigned long prev_time = 0;
  static uint32_t counter;
  if (millis() - prev_time > 20) {
    for (int i = 0; i < STRIP_LENGTH; i++) {
      getFireColor((inoise8(i * FIRE_STEP, counter)), i);
    }
    FastLED.show();
    prev_time = millis();
    counter += map(settings.speed, 0, 255, 10, 50);
  }
}

void getFireColor(int val, int i) {
  leds[i] = CHSV(
    settings.hue + map(val, 0, 255, 0, HUE_GAP),
    constrain(map(val, 0, 255, MAX_SAT, MIN_SAT), 0, 255),
    constrain(map(val, 0, 255, MIN_BRIGHT, MAX_BRIGHT), 0, 255));
}

CRGBPalette16 gPal = CRGBPalette16(CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
;
#define COOLING 55
#define SPARKING 120

void fireTick() {
  static uint32_t prevTime;

  if (millis() - prevTime > 30) {
    prevTime = millis();
    random16_add_entropy(random());
    Fire2012WithPalette();
    FastLED.show();
  }
}

void Fire2012WithPalette() {
  static byte heat[STRIP_LENGTH];
  for (int i = 0; i < STRIP_LENGTH; i++) {
    heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / STRIP_LENGTH) + 2));
  }
  for (int k = STRIP_LENGTH - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
  if (random8() < SPARKING) {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }

  for (int j = 0; j < STRIP_LENGTH; j++) {
    byte colorindex = scale8(heat[j], 240);
    leds[j] = ColorFromPalette(gPal, colorindex);
  }
}

void sides() {
  static unsigned long prev_measure;
  if (millis() - prev_measure > map(settings.speed, 0, 255, 100, 15)) {
    byte idex = random(0, STRIP_LENGTH);
    byte ihue = random(0, 255);
    leds[idex] = CHSV(ihue, settings.saturation, settings.value);
    FastLED.show();
    prev_measure = millis();
  }
}

void fill() {
  static unsigned long prev_measure;
  if (millis() - prev_measure > 20) {
    if (random(100) < DOT_CHANCE) {
      leds[0] = CHSV(random(256), settings.saturation, settings.value);
    }
    for (int i = STRIP_LENGTH - 1; i > 0; i--) {
      leds[i] = leds[i - 1];
    }
    FastLED.show();
    prev_measure = millis();
  }
}

struct flash{
  int position = -1;
  bool direction;
  CRGB color;
};

flash flashes[STRIP_LENGTH/10];

void cells(){
  static unsigned long prev_measure;
  if(millis() - prev_measure > 20){
    fadeToBlackBy(leds, STRIP_LENGTH, 10);
    for(int i = 0; i < STRIP_LENGTH/10; i++){
      flashes[i].position = flashes[i].direction ? ++flashes[i].position : --flashes[i].position;
      if(flashes[i].position >= STRIP_LENGTH || flashes[i].position < 0){
        flashes[i].position = random(0, STRIP_LENGTH);
        flashes[i].direction = (bool)random(0,2);
        flashes[i].color = CHSV(random(0, 256), settings.saturation, settings.value);
      }
      leds[flashes[i].position] =blend(leds[flashes[i].position], flashes[i].color, 127);
    }
    FastLED.setBrightness(MAX_BRIGHTNESS);
    FastLED.show();
    prev_measure = millis();
  }
}