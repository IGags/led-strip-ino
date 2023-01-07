void loop(){
  if(irReciver.decode(&results)){
    if(IS_DEBUG){
      Serial.println(results.value, HEX);
    }
    is_frozen = true;
    freeze_time = millis();
    updateSettings();
    irReciver.resume();
  }

  if(is_settings_changed && millis() - prev_settings_time > SETTINGS_AUTOSAVE_DELAY){
    EEPROM.put(1, settings);
    is_settings_changed = false;
    if(IS_DEBUG){
      Serial.println("Settings Changed!");
      Serial.println(settings.speed);
      Serial.println(settings.hue);
      Serial.println(settings.saturation);
      Serial.println(settings.value);
      Serial.println(settings.mode, HEX);
    }
  }
  if(!is_frozen || millis() - freeze_time > FREEZE_TIMEOUT){
    drawEffect();
    freeze_time = millis();
    is_frozen = false;
  }
}