#define STRIP_DATA_PIN 13 //Цифровой вывод, работающий с лентой
#define REMOTE_PIN 2 //Пин с ик приёмником, обязательно на прерываниях(2,3)
#define STRIP_LENGTH 179 //Длина ленты в контроллерах диодов
#define STRIP_ALIAS WS2812 //Модель ленты

#define MAX_BRIGHTNESS 50 //Максимальная яркаость свечения ленты в процентах

#define IS_DEBUG false //Среда исполнения

#define SETTINGS_AUTOSAVE_DELAY 15000 //Пауза без действий перед автосохранением настроек
#define SETTING_CHANGE_SPEED_COEF 1.3 //Ускорение изменения насироек при зажатии
#define FREEZE_TIMEOUT 1000 //Заморозка ленты для изменения настроек

#define DOT_CHANCE 2 //Шанс появления конфети в процентах