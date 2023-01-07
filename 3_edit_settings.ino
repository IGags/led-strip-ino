int prev_action;
byte setting_counter;

void updateSettings() {
  bool was_changed = false;

  if(!is_settings_changed){
    is_settings_changed = true;
    prev_settings_time = millis();
    was_changed = true;

    if(IS_DEBUG){
      Serial.println("start changeSettings");
    }
  }

  switch (results.value) {
    case 0xFFA25D:  //Включение-выключение
      prev_action = 0x000000;
      settings.mode = results.value;
      break;
    case 0xFF629D:  //Скорость вперёд
    case 0xFFE21D:  //Скорость назад
    case 0xFF22DD:  //Цвет вперёд
    case 0xFF02FD:  //Насыщенность вперёд
    case 0xFFC23D:  //Яркость вперёд
    case 0xFFE01F:  //Цвет назад
    case 0xFFA857:  //Насыщенность назад
    case 0xFF906F:  //Яркость назад
      prev_action = results.value;
      setting_counter = 0;
      updateByteSettings();
      break;
    case 0xFF6897:
    case 0xFF9867:
    case 0xFFB04F:
    case 0xFF30CF:
    case 0xFF18E7:
    case 0xFF7A85:
    case 0xFF10EF:
    case 0xFF38C7:
    case 0xFF5AA5:
    case 0xFF42BD:
    case 0xFF4AB5:
    case 0xFF52AD:
      prev_action = 0x000000;
      settings.mode = results.value;
      break;
    case 0xFFFFFFFF:  //Затянутое нажатие
      updateByteSettings();
      break;
    default:
      if(was_changed){
        is_settings_changed = false;

        if(IS_DEBUG){
          Serial.println("Changes aborted!");
        }
      }
  }
}

void updateByteSettings(){
  if(setting_counter < 20){
    setting_counter = (byte)ceil(((double)setting_counter)*SETTING_CHANGE_SPEED_COEF + 0.01);
  }
  switch (prev_action) {
    case 0xFF629D:  //Скорость вперёд
      settings.speed = saveIncrease(settings.speed, setting_counter);
      break;
    case 0xFFE21D:  //Скорость назад
      settings.speed = saveDecrease(settings.speed, setting_counter);
      break;    
    case 0xFF22DD:  //Цвет вперёд
      settings.hue = saveIncrease(settings.hue, setting_counter);
      break;
    case 0xFF02FD:  //Насыщенность вперёд
      settings.saturation = saveIncrease(settings.saturation, setting_counter);
      break;
    case 0xFFC23D:  //Яркость вперёд
      settings.value = saveIncrease(settings.value, setting_counter);
      break;
    case 0xFFE01F:  //Цвет назад
      settings.hue = saveDecrease(settings.hue, setting_counter);
      break;
    case 0xFFA857:  //Насыщенность назад
      settings.saturation = saveDecrease(settings.saturation, setting_counter);
      break;
    case 0xFF906F:  //Яркость назад    
      settings.value = saveDecrease(settings.value, setting_counter);
      break;
  }
}

byte saveIncrease(byte value, byte incriment){
  if(255 - value <= incriment) return 255;
  return value + incriment;
}

byte saveDecrease(byte value, byte decriment){
  if(decriment > value) return 0;
  return value - decriment;
}