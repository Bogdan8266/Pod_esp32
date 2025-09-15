#include <Wire.h>             // Бібліотека для I2C комунікації (потрібна для OLED та BMP180)
#include <Adafruit_GFX.h>     // Базова графічна бібліотека Adafruit
#include <Adafruit_SSD1306.h> // Бібліотека для OLED дисплеїв SSD1306
#include <Adafruit_BMP085.h>  // Бібліотека для датчика BMP180/BMP085 (сумісна з BMP180)
#include <Preferences.h>      // Бібліотека для енергонезалежного зберігання даних на ESP32
#include <Adafruit_NeoPixel.h> // <-- ДОДАЙТЕ ЦЕЙ РЯДОК

const unsigned char epd_bitmap_intro [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xe7, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xe7, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xc7, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xe7, 0xff, 0xf1, 0xf0, 0xf8, 0x7e, 0x1f, 0xcf, 0x87, 0xc3, 0xb3, 0x80, 0x00, 
	0xff, 0xff, 0xff, 0xe7, 0xff, 0xfb, 0xfd, 0xfc, 0xfe, 0x1f, 0xdf, 0xef, 0xe3, 0xb3, 0x80, 0x00, 
	0xff, 0xff, 0xff, 0xe7, 0xff, 0xfb, 0xfd, 0xfe, 0xfe, 0x1f, 0xdf, 0xef, 0xf3, 0xb7, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xc7, 0xfc, 0x1f, 0x9d, 0xce, 0xfe, 0x18, 0xdc, 0xee, 0x73, 0xbf, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xc7, 0xfc, 0x1f, 0x0d, 0x8e, 0xfe, 0x18, 0xd8, 0x6c, 0x73, 0xbe, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xc7, 0xfc, 0x3f, 0x9d, 0xde, 0x7e, 0x18, 0xdc, 0xee, 0xf3, 0xbf, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xc7, 0xff, 0xfb, 0xfd, 0xfe, 0xfe, 0x18, 0xdf, 0xef, 0xf3, 0xb7, 0x80, 0x00, 
	0xff, 0xff, 0xff, 0x83, 0xff, 0xf1, 0xf8, 0xff, 0xee, 0x18, 0xcf, 0xc7, 0xf3, 0xb7, 0x80, 0x00, 
	0xff, 0xff, 0xff, 0x83, 0xf8, 0x00, 0x00, 0xce, 0x00, 0x00, 0x00, 0x06, 0x70, 0x00, 0x00, 0x00, 
	0xff, 0xff, 0xf0, 0x01, 0xf8, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 
	0xff, 0xff, 0x00, 0x01, 0xf8, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 
	0xff, 0xfe, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xff, 0xfe, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xff, 0xfe, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xff, 0xfe, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xff, 0xfe, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xff, 0xfe, 0x00, 0x00, 0xff, 0xef, 0x9e, 0x7c, 0x67, 0xc1, 0xfb, 0xe1, 0xf1, 0xdb, 0x31, 0xc0, 
	0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0xef, 0xfc, 0x6f, 0xf3, 0xff, 0xfb, 0xf8, 0xff, 0x31, 0xc0, 
	0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0xe7, 0xec, 0x6f, 0xf3, 0xff, 0xfb, 0xfc, 0xff, 0x31, 0xc0, 
	0xff, 0xff, 0x00, 0x01, 0xfb, 0x9f, 0xe7, 0xcf, 0xee, 0x73, 0xbf, 0x3b, 0x9c, 0xff, 0x31, 0xc0, 
	0xf0, 0x00, 0x00, 0x01, 0xfb, 0x9f, 0xe3, 0xcf, 0xec, 0x37, 0x3e, 0x1b, 0x1c, 0xff, 0x31, 0xc0, 
	0xe0, 0x00, 0x00, 0x03, 0xfb, 0x9f, 0xe7, 0xef, 0xee, 0x77, 0x3f, 0x3b, 0xbd, 0xff, 0xbb, 0xc0, 
	0xf0, 0x7f, 0xc0, 0x03, 0xfb, 0x9f, 0xcf, 0xfc, 0x6f, 0xf7, 0x3f, 0xfb, 0xfd, 0xdb, 0xbf, 0xc0, 
	0xff, 0xff, 0xd0, 0x03, 0xfb, 0x8f, 0xde, 0x7c, 0x67, 0xe7, 0x3b, 0xf1, 0xfd, 0xdb, 0x9f, 0xc0, 
	0xff, 0xff, 0xfc, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x9c, 0x00, 0x00, 0x00, 
	0xff, 0xff, 0xfe, 0x8f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00
};

// --- Іконки для меню-запускачів (замініть на свої дані) ---
// 'settings_icon', 20x20px
const unsigned char PROGMEM settings_icon_bitmap [] = {
    0xf8, 0xff, 0xf0, 0xc8, 0x9f, 0xf0, 0x80, 0x0f, 0xf0, 0x80, 0x1c, 0x30, 0x00, 0x04, 0x00, 0x07, 
	0x00, 0x00, 0x0f, 0x00, 0x80, 0x8f, 0x00, 0x80, 0x07, 0x08, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 
	0x30, 0xe0, 0x00, 0x70, 0xe2, 0x00, 0x30, 0xf2, 0x80, 0x10, 0xff, 0x86, 0x10, 0xff, 0x86, 0x10, 
	0xff, 0xc0, 0x10, 0xff, 0xc0, 0x30, 0xff, 0xd0, 0x70, 0xff, 0xf2, 0xf0
};

// 'stats_icon', 20x20px
const unsigned char PROGMEM stats_icon_bitmap [] = {
    0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xfc, 0xf0, 0xff, 0xfc, 0xf0, 0xff, 0xf4, 0xf0, 0xff, 
	0xf4, 0xf0, 0xff, 0xf4, 0xf0, 0xff, 0x94, 0xf0, 0xff, 0x94, 0xf0, 0xfe, 0x94, 0xf0, 0xfe, 0x94, 
	0xf0, 0xfe, 0x94, 0xf0, 0xf2, 0x94, 0xf0, 0xf2, 0x94, 0xf0, 0xf2, 0x94, 0xf0, 0xf6, 0x96, 0xf0, 
	0xe0, 0x00, 0x70, 0xe0, 0x00, 0x70, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0
};


#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60         /* Time ESP32 will go to sleep (in seconds) */


// --- Налаштування світлодіода ---
#define LED_PIN 4             // Pin D5 для WS2812
#define NUM_LEDS 1            // У нас один світлодіод
 // Час, коли кнопка була натиснута (для затримки)
unsigned long deviceOffTime = 0;
bool screenNeedsClearing = false;


Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// --- Режими роботи світлодіода ---
enum LedMode {
  LED_OFF,
  LED_RAINBOW,
  LED_RANDOM,
  LED_BATTERY,
  LED_MODE_COUNT
};
LedMode currentLedMode = LED_RAINBOW; // Початковий режим
uint32_t randomPuffColor; // Зберігає колір для режиму LED_RANDOM
float ledBrightness = 0.0f; // Поточна яскравість для плавного ввімкнення/вимкнення

// --- Налаштування дисплея ---
#define SCREEN_WIDTH 128      // Ширина OLED дисплея в пікселях
#define SCREEN_HEIGHT 32      // Висота OLED дисплея в пікселях
#define OLED_RESET -1         // Pin Reset для OLED (використовуємо -1, оскільки він підключений до загального скидання ESP32 або не використовується в I2C режимі)
#define SCREEN_ADDRESS 0x3C   // I2C адреса для 128x32 OLED дисплея (може бути 0x3D для деяких моделей)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Створюємо об'єкт дисплея
Adafruit_BMP085 bmp; // Створюємо об'єкт для датчика BMP180
int settingsMenuIndex = 0;
int nextSettingsMenuIndex = 0; // Для анімації вертикального скролінгу
bool inCurveEditor = false;

// --- Змінні для логіки пробудження ---
bool awaiting_wake_clicks = false;
unsigned long wake_check_start_time = 0;
int wake_click_count = 0;

// --- Змінні для увімкнення/вимкнення та затримки Fire ---
// bool isDeviceOn = true; // Ця змінна більше не потрібна
int fireClickCount = 0; // Залишимо для детекції подвійного кліку
unsigned long lastFireClickTime = 0;
unsigned int fireButtonDelay_ms = 300; // Затримка перед затяжкою
unsigned long firePressStartTime = 0; // Час, коли кнопка була натиснута (для затримки)

// --- Змінні для авто-вимкнення ---
unsigned long lastActivityTime = 0;
unsigned long autoOffDelay_ms = 60000; // 1 хвилина. 0 = вимкнено

// --- Змінні для керування потужністю ---
int currentWattage = 10; // Початкова потужність (1-20)
bool wattageAdjustMode = false;
unsigned long lastWattageAdjustTime = 0;
bool wattageAdjustDirectionUp = true; // true - збільшувати, false - зменшувати
unsigned long lastWattageChangeTime = 0;

// --- Налаштування кнопок ---
const int BUTTON_FIRE_PIN = 15; // Pin для кнопки "Fire" (ЗМІНЕНО НА RTC-СУМІСНИЙ ПІН)
const int BUTTON_NAV_PIN = 19;  // Pin для кнопки навігації / вторинної дії

const int PRESSURE_READING_COUNT = 20; // Кількість зразків для середнього тиску (20 * 100мс = 2с вікно)
float pressureReadings[PRESSURE_READING_COUNT] = {0};
int pressureReadingIndex = 0;
float baselinePressure_Pa = 0;
unsigned long lastPressureReadTime = 0;
int puffPressureThreshold_Pa = 40; // Чутливість (в Паскалях), яку можна буде змінювати
enum PuffDetectorState { PUFF_IDLE, PUFF_ACTIVE };
PuffDetectorState puffDetectorState = PUFF_IDLE;

// --- Глобальні змінні для даних (будуть заповнюватися з датчиків/логіки) ---
unsigned long totalPuffs = 0;       // Загальна кількість затяжок
unsigned long lastPuffDuration_ms = 0; // Тривалість останньої затяжки в мілісекундах
float batteryVoltage = 3.7;         // Напруга на акумуляторі (заглушка)
String currentPreset = "Default";   // Назва поточного пресету

// --- Об'єкт Preferences для збереження даних ---
Preferences preferences;

// --- Управління меню ---
enum MenuState {
  MAIN_SCREEN,        // Головний екран
  SETTINGS_LAUNCHER,  // Екран-запускач для налаштувань
  STATS_LAUNCHER,     // Екран-запускач для статистики
  MENU_COUNT,         // Кількість головних екранів в каруселі

  // Стани для внутрішніх меню (не входять в карусель)
  SETTINGS_SUB_MENU,
  STATS_SUB_MENU
};

MenuState currentMenu = MAIN_SCREEN; // Поточне активне меню
int currentMenuIndex = 0;            // Індекс поточного меню для відображення точок навігації

// --- Змінні для антидребезгу кнопок ---
const long debounceDelay = 50; // Мілісекунди для антидребезгу

// Змінні для кнопки "Fire"
bool fireButton_currentReading;
bool fireButton_lastState = HIGH; // HIGH, коли кнопка не натиснута (через INPUT_PULLUP)
unsigned long fireButton_lastDebounceTime = 0;
bool fireButton_debouncedState = HIGH; // Дебонсований стан кнопки

// Змінні для кнопки навігації
bool navButton_currentReading;
bool navButton_lastState = HIGH; // HIGH, коли кнопка не натиснута
unsigned long navButton_lastDebounceTime = 0;
bool navButton_debouncedState = HIGH; // Дебонсований стан кнопки
bool inMeteoScreen = false;
// --- Змінні для логіки затяжки ---
unsigned long puffStartTime_ms = 0; // Час початку поточної затяжки
bool isPuffing = false;             // Прапорець, що вказує, чи триває зараз затяжка

// --- Прототипи функцій ---
void setupDisplay();
void setupButtons();
void setupSensors();
void setupPreferences();

void drawMainScreen(int x_offset);
void drawSettingsMenu(int x_offset);
void drawStatsMenu(int x_offset);
void drawMeteoScreen(); // Додаємо прототип для нової функції
void drawCurveMenu(); // Додаємо прототип для нової функції
void drawSettingsLauncherScreen(int x_offset); // Новий прототип
void drawStatsLauncherScreen(int x_offset);    // Новий прототип

void handleButtons();
void handleFireButtonPress();
void handleFireButtonRelease(); // Нова функція для обробки відпускання кнопки "Fire"
void handleNavButtonPress();

void readBMP180DataAndDetectPuff(); // Функція для зчитування BMP180 та логіки детекції затяжки
void updateBatteryVoltage();       // Функція для оновлення напруги батареї
void savePreferences();            // Функція для збереження даних
void loadPreferences();            // Функція для завантаження даних

const int PWM_PIN = 2; // D2 для керування MOSFET
const int PWM_CHANNEL = 0;
const int PWM_FREQ = 5000; // Частота PWM, наприклад 5 кГц
const int PWM_RESOLUTION = 8; // 8-бітний PWM (0-255)
const int RAMP_POINTS = 5;
uint8_t rampCurve[RAMP_POINTS] = {0, 64, 128, 192, 255}; // Значення PWM для кожної точки
int rampEditIndex = 0; // Індекс точки для редагування в меню
unsigned long rampDuration_ms = 1000; // Загальний час ramp
bool pwmActive = false;
unsigned long pwmStartTime = 0;

// --- Налаштування підменю ---
enum SettingsMenuState {
  SETTINGS_CURVE_EDITOR,
  SETTINGS_RAMP_TIME,
  SETTINGS_METEO,
  SETTINGS_ANIMATION_TIME,
  SETTINGS_LED_MODE,
  SETTINGS_PUFF_SENSITIVITY,
  SETTINGS_FIRE_DELAY,
  SETTINGS_AUTO_OFF, // <-- Новий пункт
  SETTINGS_COUNT
};

//int settingsMenuIndex = 0;
//bool inCurveEditor = false;

// --- Змінні для анімації пару ---
float vaporAnimProgress = 0.0f; // 0.0 - немає анімації, 1.0 - повністю заповнено
bool vaporAppearing = false;    // true - з’являється, false - зникає
unsigned long vaporAnimLastUpdate = 0;
const float vaporAnimSpeed = 0.02f; // Швидкість анімації (чим менше, тим плавніше)

unsigned int animationDuration_ms = 500; // Початково 500мс, можна змінювати від 0 до 2000
unsigned long animationStartTime = 0;
bool isScreenAnimating = false;
int animationDirection = 0; // -1: вліво, +1: вправо
MenuState nextMenu;

void setup() {
  Serial.begin(115200);

  strip.begin();
  strip.show();

  Wire.begin();
  setupDisplay();

  // --- Перевірка причини пробудження ---
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) {
    // Прокинулись від кнопки, переходимо в режим очікування
    awaiting_wake_clicks = true;
    wake_check_start_time = millis();
    wake_click_count = 1; // Перший клік вже був
  } else {
    // Звичайне ввімкнення (наприклад, після подачі живлення)
    showSplashScreen();
  }

  setupButtons();
  setupSensors();
  setupPreferences();

  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(PWM_PIN, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, 0); // Початково 0

  loadPreferences(); // Завантажуємо збережені дані після ініціалізації
}

void loop() {
  handleButtons();

  if (awaiting_wake_clicks) {
    // --- Режим очікування пробудження ---
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(25, 8);
    display.print("WAKE?");
    display.display();

    // Якщо протягом 2 секунд не було активності, знову засинаємо
    if (millis() - wake_check_start_time > 2000) {
      enterDeepSleep();
    }
  } else {
    // --- Нормальний робочий режим ---
    // Перевіряємо таймер бездіяльності для авто-вимкнення
    if (autoOffDelay_ms > 0 && (millis() - lastActivityTime > autoOffDelay_ms)) {
      Serial.println("Device auto-off due to inactivity.");
      enterDeepSleep();
    }

    // --- Обробка режиму налаштування потужності (Press & Hold) ---
    if (wattageAdjustMode) {
      // Якщо кнопка Fire затиснута
      if (digitalRead(BUTTON_FIRE_PIN) == LOW) {
        lastWattageAdjustTime = millis(); // Скидаємо таймер виходу, поки кнопка затиснута
        if (millis() - lastWattageChangeTime > 100) { // Змінюємо потужність кожні 100 мс
          lastWattageChangeTime = millis();
          if (wattageAdjustDirectionUp) {
            currentWattage++;
            if (currentWattage > 20) currentWattage = 20;
          } else {
            currentWattage--;
            if (currentWattage < 1) currentWattage = 1;
          }
        }
      } else { // Кнопка не затиснута
        // Вихід з режиму по таймауту
        if (millis() - lastWattageAdjustTime > 3000) {
          wattageAdjustMode = false;
          savePreferences(); // Зберігаємо нову потужність
          Serial.println("Wattage adjust mode OFF (timeout).");
        }
      }
    }

    // Перевірка затримки кнопки Fire для початку затяжки
    if (firePressStartTime > 0 && !wattageAdjustMode && digitalRead(BUTTON_FIRE_PIN) == LOW) {
      if (millis() - firePressStartTime >= fireButtonDelay_ms) {
        startPuff();
        firePressStartTime = 0; // Скидаємо, щоб не спрацювало знову
      }
    }

    readBMP180DataAndDetectPuff();
    updatePWM();
    updateVaporAnimation();
    
    updateLed(); // <-- ДОДАЙТЕ ЦЕЙ РЯДОК

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    if (isScreenAnimating && animationDirection != 0) {
      unsigned long elapsed = millis() - animationStartTime;
      float t = 1.0f;
      if (animationDuration_ms > 0) {
        t = min(1.0f, (float)elapsed / animationDuration_ms);
      }
      int offset = (int)(SCREEN_WIDTH * t);

      // Малюємо поточний екран, що з'їжджає вліво
      switch (currentMenu) {
        case MAIN_SCREEN: drawMainScreen(-offset); break;
        case SETTINGS_LAUNCHER: drawSettingsLauncherScreen(-offset); break;
        case STATS_LAUNCHER: drawStatsLauncherScreen(-offset); break;
      }
      // Малюємо наступний екран, що з'являється справа
      switch (nextMenu) {
        case MAIN_SCREEN: drawMainScreen(SCREEN_WIDTH - offset); break;
        case SETTINGS_LAUNCHER: drawSettingsLauncherScreen(SCREEN_WIDTH - offset); break;
        case STATS_LAUNCHER: drawStatsLauncherScreen(SCREEN_WIDTH - offset); break;
      }
      if (t >= 1.0f) {
        isScreenAnimating = false;
        currentMenu = nextMenu;
        currentMenuIndex = (int)currentMenu;
      }
    } else {
      if (inMeteoScreen) {
        drawMeteoScreen();
      } else if (currentMenu == SETTINGS_SUB_MENU && inCurveEditor) {
        drawCurveMenu();
      } else {
        switch (currentMenu) {
          case MAIN_SCREEN: drawMainScreen(0); break;
          case SETTINGS_LAUNCHER: drawSettingsLauncherScreen(0); break;
          case STATS_LAUNCHER: drawStatsLauncherScreen(0); break;
          case SETTINGS_SUB_MENU: drawSettingsMenu(0); break;
          case STATS_SUB_MENU: drawStatsMenu(0); break;
        }
      }
    }

    drawVaporAnimation();
    display.display();
    delay(10);
  }
}

void setupDisplay() {
  // Ініціалізуємо OLED дисплей
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Якщо ініціалізація не вдалася, зупиняємося тут
  }
  Serial.println(F("OLED display initialized"));
  display.display(); // Показуємо початковий логотип Adafruit
  delay(2000);
  display.clearDisplay(); // Очищаємо дисплей

  // --- Підтримка українських символів ---
  // Для відображення українських символів вам знадобиться спеціальний шрифт,
  // оскільки стандартний шрифт Adafruit_GFX (glcdfont.c) не містить кирилиці.
  // Варіанти:
  // 1. Використовувати інструмент "fontconvert" з бібліотеки Adafruit_GFX
  //    для генерації файлу .h з бажаного .ttf шрифта.
  //    Потім включити цей .h файл у ваш скетч і встановити шрифт за допомогою `display.setFont(&YourCustomFont);`
  //    Приклад: https://learn.adafruit.com/custom-fonts-for-adafruit-gfx-library/creating-custom-fonts
  // 2. Шукати готові файли шрифтів з підтримкою кирилиці, сумісні з Adafruit_GFX.
  // Наразі всі текстові рядки залишені англійською, щоб уникнути проблем з відображенням.
}

void setupButtons() {
  // Встановлюємо піни кнопок як вхідні з підтягуючими резисторами (INPUT_PULLUP)
  // Це означає, що піни будуть HIGH, коли кнопка не натиснута, і LOW, коли натиснута
  pinMode(BUTTON_FIRE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_NAV_PIN, INPUT_PULLUP);
  Serial.println(F("Buttons initialized"));
}

void setupSensors() {
  // Ініціалізуємо датчик BMP180
  if (!bmp.begin()) {
    Serial.println(F("BMP180 sensor not found, check wiring or I2C address!"));
    // Для под-системи, можливо, варто продовжувати роботу навіть без датчика,
    // але для відладки краще вивести повідомлення.
  } else {
    Serial.println(F("BMP180 sensor initialized"));
  }
}

void setupPreferences() {
  // Ініціалізуємо Preferences з простором імен "pod-system"
  // Другий параметр false означає режим читання/запису
  preferences.begin("pod-system", false);
  Serial.println(F("Preferences initialized"));
}

void loadPreferences() {
  totalPuffs = preferences.getULong("totalPuffs", 0);
  rampDuration_ms = preferences.getULong("rampDuration", 1000);
  currentWattage = preferences.getInt("wattage", 10); // Завантажуємо потужність
  autoOffDelay_ms = preferences.getULong("autoOff", 60000); // Завантажуємо час авто-вимкнення
  animationDuration_ms = preferences.getUInt("animTime", 500);
  currentLedMode = (LedMode)preferences.getInt("ledMode", LED_RAINBOW);
  puffPressureThreshold_Pa = preferences.getInt("puffSens", 40);
  fireButtonDelay_ms = preferences.getUInt("fireDelay", 300);
  for (int i = 0; i < RAMP_POINTS; i++) {
    String key = "ramp" + String(i);
    rampCurve[i] = preferences.getUChar(key.c_str(), (i * 255) / (RAMP_POINTS - 1));
  }
  Serial.print("Loaded totalPuffs: ");
  Serial.println(totalPuffs);
  Serial.print("Loaded rampDuration: ");
  Serial.println(rampDuration_ms);
  Serial.println("Loaded rampCurve points.");
}

void drawVaporAnimation() {
  if (vaporAnimProgress <= 0.0f) return;

  int vaporEdge = SCREEN_WIDTH * vaporAnimProgress;
  int vaporHeight = SCREEN_HEIGHT;
  int fadeWidth = 30; // ширина розсіювання

  for (int y = 0; y < vaporHeight; y += 2) {
    for (int x = 0; x < vaporEdge; x += 3) {
      int wave = (int)(4 * sin(0.15 * x + 0.5 * y + millis() * 0.002));
      // Ефект fade на межі
      if (x > vaporEdge - fadeWidth) {
        // Зменшуємо щільність точок ближче до краю
        if (random(0, fadeWidth) < vaporEdge - x) continue;
      }
      display.drawPixel(x, y + wave, SSD1306_WHITE);
      display.drawPixel(x, y + wave + 1, SSD1306_WHITE);
    }
  }
}


void savePreferences() {
  preferences.putULong("totalPuffs", totalPuffs);
  preferences.putULong("rampDuration", rampDuration_ms);
  preferences.putInt("wattage", currentWattage); // Зберігаємо потужність
  preferences.putULong("autoOff", autoOffDelay_ms); // Зберігаємо час авто-вимкнення
  preferences.putUInt("animTime", animationDuration_ms);
  preferences.putInt("ledMode", (int)currentLedMode);
  preferences.putInt("puffSens", puffPressureThreshold_Pa);
  preferences.putUInt("fireDelay", fireButtonDelay_ms);
  for (int i = 0; i < RAMP_POINTS; i++) {
    String key = "ramp" + String(i);
    preferences.putUChar(key.c_str(), rampCurve[i]);
  }
  Serial.print("Saved totalPuffs: ");
  Serial.println(totalPuffs);
  Serial.print("Saved rampDuration: ");
  Serial.println(rampDuration_ms);
  Serial.println("Saved rampCurve points.");
}

// --- Функції малювання для різних меню ---
void drawMainScreen(int x_offset) {
  // --- Верхній рядок стану ---
  display.setCursor(x_offset + 0, 0);
  display.print(totalPuffs);

  display.setCursor(x_offset + 40, 0);
  display.print(lastPuffDuration_ms / 1000.0, 1);
  display.print("s");

  display.setCursor(x_offset + 85, 0);
  display.print(batteryVoltage, 1);
  display.print("V");

  int battX = x_offset + 112;
  int battY = 0;
  int battW = 14;
  int battH = 8;
  display.drawRect(battX, battY, battW, battH, SSD1306_WHITE);
  display.drawRect(battX + battW, battY + 2, 2, 4, SSD1306_WHITE);

  int fillWidth = map((int)(batteryVoltage * 10), 32, 42, 0, battW - 2);
  if (fillWidth < 0) fillWidth = 0;
  if (fillWidth > battW - 2) fillWidth = battW - 2;
  display.fillRect(battX + 1, battY + 1, fillWidth, battH - 2, SSD1306_WHITE);

  // --- Відображення потужності ---
  display.setTextSize(2);
  if (wattageAdjustMode) {
    // Мигання для індикації режиму редагування
    if ((millis() / 250) % 2) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Інвертуємо колір
    }
  }
  display.setCursor(x_offset + 45, 16);
  display.print(currentWattage);
  display.print("W");
  display.setTextColor(SSD1306_WHITE); // Повертаємо стандартний колір
  display.setTextSize(1);

  // --- Точки навігації (малюємо тільки на активному екрані) ---
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawSettingsMenu(int x_offset) {
  display.setCursor(x_offset + 0, 0);
  display.print("Settings Menu");

  float y_anim_offset = 0;
  // Обробка анімації вертикального скролінгу
  if (isScreenAnimating && animationDirection == 0) {
    unsigned long elapsed = millis() - animationStartTime;
    float t = 1.0f;
    if (animationDuration_ms > 0) {
      t = min(1.0f, (float)elapsed / animationDuration_ms);
    }
    y_anim_offset = -t * 8; // Анімація зсуву вгору на 8 пікселів

    if (t >= 1.0f) {
      isScreenAnimating = false;
      settingsMenuIndex = nextSettingsMenuIndex; // Оновлюємо стан ПІСЛЯ анімації
    }
  }

  int visibleCount = 2;
  int startIdx = settingsMenuIndex;
  if (settingsMenuIndex > SETTINGS_COUNT - visibleCount) {
    startIdx = SETTINGS_COUNT - visibleCount;
  }

  // Малюємо 3 пункти, щоб було видно той, що "виїжджає" і "в'їжджає"
  for (int i = 0; i < visibleCount + 1; i++) {
    int idx = startIdx + i;
    if (idx >= SETTINGS_COUNT) continue;

    int y = 16 + (i * 8);
    display.setCursor(x_offset + 2, y + y_anim_offset); // Відступ для курсора ">"
    
    if (idx == settingsMenuIndex) {
        display.setCursor(x_offset, y + y_anim_offset);
        display.print(">");
    }

    switch (idx) {
      case SETTINGS_CURVE_EDITOR:
        display.print("Curve editor");
        break;
      case SETTINGS_RAMP_TIME:
        display.print("Ramp time: ");
        display.print(rampDuration_ms);
        display.print("ms");
        break;
      case SETTINGS_METEO:
        display.print("Meteo");
        break;
      case SETTINGS_ANIMATION_TIME:
        display.print("Anim time: ");
        display.print(animationDuration_ms);
        display.print("ms");
        break;
      case SETTINGS_LED_MODE:
        display.print("LED Mode: ");
        switch(currentLedMode) {
          case LED_OFF: display.print("Off"); break;
          case LED_RAINBOW: display.print("Rainbow"); break;
          case LED_RANDOM: display.print("Random"); break;
          case LED_BATTERY: display.print("Battery"); break;
        }
        break;
      case SETTINGS_PUFF_SENSITIVITY:
        display.print("Puff Sens: ");
        display.print(puffPressureThreshold_Pa);
        display.print(" Pa");
        break;
      case SETTINGS_FIRE_DELAY:
        display.print("Fire Delay: ");
        display.print(fireButtonDelay_ms);
        display.print("ms");
        break;
      case SETTINGS_AUTO_OFF:
        display.print("Auto Off: ");
        if (autoOffDelay_ms == 0) {
          display.print("Off");
        } else {
          display.print(autoOffDelay_ms / 60000);
          display.print(" min");
        }
        break;
    }
  }

  // Точки навігації (малюємо тільки на активному екрані)
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawSettingsLauncherScreen(int x_offset) {
  // Малюємо іконку по центру
  display.drawBitmap(x_offset + (SCREEN_WIDTH - 20) / 2, 0, settings_icon_bitmap, 20, 20, SSD1306_WHITE);
  
  // Малюємо підпис
  display.setTextSize(1);
  display.setCursor(x_offset + 40, 24);
  display.print("Settings");

  // Точки навігації
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawStatsLauncherScreen(int x_offset) {
  // Малюємо іконку по центру
  display.drawBitmap(x_offset + (SCREEN_WIDTH - 20) / 2, 0, stats_icon_bitmap, 20, 20, SSD1306_WHITE);

  // Малюємо підпис
  display.setTextSize(1);
  display.setCursor(x_offset + 35, 24);
  display.print("Statistics");

  // Точки навігації
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawStatsMenu(int x_offset) {
  display.setCursor(x_offset + 0, 0);
  display.print("Statistics");
  display.setCursor(x_offset + 0, 16);
  display.print("Total Puffs: ");
  display.print(totalPuffs);
  display.setCursor(x_offset + 0, 24);
  display.print("Last Puff: ");
  display.print(lastPuffDuration_ms / 1000.0, 1);
  display.print("s");

  // Точки навігації (малюємо тільки на активному екрані)
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawMeteoScreen() {
  float pressure = bmp.readPressure() / 100.0; // гПа
  float altitude = bmp.readAltitude();         // метри

  display.setCursor(0, 0);
  display.print("Meteo Station");
  display.setCursor(0, 16);
  display.print("Pressure: ");
  display.print(pressure, 1);
  display.print(" hPa");
  display.setCursor(0, 24);
  display.print("Altitude: ");
  display.print(altitude, 1);
  display.print(" m");
}

void drawCurveMenu() {
  display.setCursor(0, 0);
  display.print("Curve Editor");
  int graphX = 60, graphY = 8, graphW = 60, graphH = 16;
  for (int i = 0; i < RAMP_POINTS; i++) {
    int x = graphX + (i * graphW) / (RAMP_POINTS - 1);
    int y = graphY + graphH - (rampCurve[i] * graphH) / 255;
    if (i < RAMP_POINTS - 1) {
      int x1 = graphX + ((i + 1) * graphW) / (RAMP_POINTS - 1);
      int y1 = graphY + graphH - (rampCurve[i + 1] * graphH) / 255;
      display.drawLine(x, y, x1, y1, SSD1306_WHITE);
    }
    if (i == rampEditIndex)
      display.fillCircle(x, y, 2, SSD1306_WHITE);
    else
      display.drawCircle(x, y, 1, SSD1306_WHITE);
  }
  if (rampEditIndex == RAMP_POINTS) {
    display.setCursor(0, 24);
    display.print("Exit editor");
  } else {
    display.setCursor(0, 24);
    display.print("NAV: point FIRE: value");
  }
}

void drawSettingsLauncherScreen(int x_offset) {
  // Малюємо іконку по центру
  display.drawBitmap(x_offset + (SCREEN_WIDTH - 20) / 2, 0, settings_icon_bitmap, 20, 20, SSD1306_WHITE);
  
  // Малюємо підпис
  display.setTextSize(1);
  display.setCursor(x_offset + 40, 24);
  display.print("Settings");

  // Точки навігації
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawStatsLauncherScreen(int x_offset) {
  // Малюємо іконку по центру
  display.drawBitmap(x_offset + (SCREEN_WIDTH - 20) / 2, 0, stats_icon_bitmap, 20, 20, SSD1306_WHITE);

  // Малюємо підпис
  display.setTextSize(1);
  display.setCursor(x_offset + 35, 24);
  display.print("Statistics");

  // Точки навігації
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawStatsMenu(int x_offset) {
  display.setCursor(x_offset + 0, 0);
  display.print("Statistics");
  display.setCursor(x_offset + 0, 16);
  display.print("Total Puffs: ");
  display.print(totalPuffs);
  display.setCursor(x_offset + 0, 24);
  display.print("Last Puff: ");
  display.print(lastPuffDuration_ms / 1000.0, 1);
  display.print("s");

  // Точки навігації (малюємо тільки на активному екрані)
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawMeteoScreen() {
  float pressure = bmp.readPressure() / 100.0; // гПа
  float altitude = bmp.readAltitude();         // метри

  display.setCursor(0, 0);
  display.print("Meteo Station");
  display.setCursor(0, 16);
  display.print("Pressure: ");
  display.print(pressure, 1);
  display.print(" hPa");
  display.setCursor(0, 24);
  display.print("Altitude: ");
  display.print(altitude, 1);
  display.print(" m");
}

void drawCurveMenu() {
  display.setCursor(0, 0);
  display.print("Curve Editor");
  int graphX = 60, graphY = 8, graphW = 60, graphH = 16;
  for (int i = 0; i < RAMP_POINTS; i++) {
    int x = graphX + (i * graphW) / (RAMP_POINTS - 1);
    int y = graphY + graphH - (rampCurve[i] * graphH) / 255;
    if (i < RAMP_POINTS - 1) {
      int x1 = graphX + ((i + 1) * graphW) / (RAMP_POINTS - 1);
      int y1 = graphY + graphH - (rampCurve[i + 1] * graphH) / 255;
      display.drawLine(x, y, x1, y1, SSD1306_WHITE);
    }
    if (i == rampEditIndex)
      display.fillCircle(x, y, 2, SSD1306_WHITE);
    else
      display.drawCircle(x, y, 1, SSD1306_WHITE);
  }
  if (rampEditIndex == RAMP_POINTS) {
    display.setCursor(0, 24);
    display.print("Exit editor");
  } else {
    display.setCursor(0, 24);
    display.print("NAV: point FIRE: value");
  }
}

void drawSettingsLauncherScreen(int x_offset) {
  // Малюємо іконку по центру
  display.drawBitmap(x_offset + (SCREEN_WIDTH - 20) / 2, 0, settings_icon_bitmap, 20, 20, SSD1306_WHITE);
  
  // Малюємо підпис
  display.setTextSize(1);
  display.setCursor(x_offset + 40, 24);
  display.print("Settings");

  // Точки навігації
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawStatsLauncherScreen(int x_offset) {
  // Малюємо іконку по центру
  display.drawBitmap(x_offset + (SCREEN_WIDTH - 20) / 2, 0, stats_icon_bitmap, 20, 20, SSD1306_WHITE);

  // Малюємо підпис
  display.setTextSize(1);
  display.setCursor(x_offset + 35, 24);
  display.print("Statistics");

  // Точки навігації
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawStatsMenu(int x_offset) {
  display.setCursor(x_offset + 0, 0);
  display.print("Statistics");
  display.setCursor(x_offset + 0, 16);
  display.print("Total Puffs: ");
  display.print(totalPuffs);
  display.setCursor(x_offset + 0, 24);
  display.print("Last Puff: ");
  display.print(lastPuffDuration_ms / 1000.0, 1);
  display.print("s");

  // Точки навігації (малюємо тільки на активному екрані)
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawMeteoScreen() {
  float pressure = bmp.readPressure() / 100.0; // гПа
  float altitude = bmp.readAltitude();         // метри

  display.setCursor(0, 0);
  display.print("Meteo Station");
  display.setCursor(0, 16);
  display.print("Pressure: ");
  display.print(pressure, 1);
  display.print(" hPa");
  display.setCursor(0, 24);
  display.print("Altitude: ");
  display.print(altitude, 1);
  display.print(" m");
}

void drawCurveMenu() {
  display.setCursor(0, 0);
  display.print("Curve Editor");
  int graphX = 60, graphY = 8, graphW = 60, graphH = 16;
  for (int i = 0; i < RAMP_POINTS; i++) {
    int x = graphX + (i * graphW) / (RAMP_POINTS - 1);
    int y = graphY + graphH - (rampCurve[i] * graphH) / 255;
    if (i < RAMP_POINTS - 1) {
      int x1 = graphX + ((i + 1) * graphW) / (RAMP_POINTS - 1);
      int y1 = graphY + graphH - (rampCurve[i + 1] * graphH) / 255;
      display.drawLine(x, y, x1, y1, SSD1306_WHITE);
    }
    if (i == rampEditIndex)
      display.fillCircle(x, y, 2, SSD1306_WHITE);
    else
      display.drawCircle(x, y, 1, SSD1306_WHITE);
  }
  if (rampEditIndex == RAMP_POINTS) {
    display.setCursor(0, 24);
    display.print("Exit editor");
  } else {
    display.setCursor(0, 24);
    display.print("NAV: point FIRE: value");
  }
}

void drawSettingsLauncherScreen(int x_offset) {
  // Малюємо іконку по центру
  display.drawBitmap(x_offset + (SCREEN_WIDTH - 20) / 2, 0, settings_icon_bitmap, 20, 20, SSD1306_WHITE);
  
  // Малюємо підпис
  display.setTextSize(1);
  display.setCursor(x_offset + 40, 24);
  display.print("Settings");

  // Точки навігації
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawStatsLauncherScreen(int x_offset) {
  // Малюємо іконку по центру
  display.drawBitmap(x_offset + (SCREEN_WIDTH - 20) / 2, 0, stats_icon_bitmap, 20, 20, SSD1306_WHITE);

  // Малюємо підпис
  display.setTextSize(1);
  display.setCursor(x_offset + 35, 24);
  display.print("Statistics");

  // Точки навігації
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawStatsMenu(int x_offset) {
  display.setCursor(x_offset + 0, 0);
  display.print("Statistics");
  display.setCursor(x_offset + 0, 16);
  display.print("Total Puffs: ");
  display.print(totalPuffs);
  display.setCursor(x_offset + 0, 24);
  display.print("Last Puff: ");
  display.print(lastPuffDuration_ms / 1000.0, 1);
  display.print("s");

  // Точки навігації (малюємо тільки на активному екрані)
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawMeteoScreen() {
  float pressure = bmp.readPressure() / 100.0; // гПа
  float altitude = bmp.readAltitude();         // метри

  display.setCursor(0, 0);
  display.print("Meteo Station");
  display.setCursor(0, 16);
  display.print("Pressure: ");
  display.print(pressure, 1);
  display.print(" hPa");
  display.setCursor(0, 24);
  display.print("Altitude: ");
  display.print(altitude, 1);
  display.print(" m");
}

void drawCurveMenu() {
  display.setCursor(0, 0);
  display.print("Curve Editor");
  int graphX = 60, graphY = 8, graphW = 60, graphH = 16;
  for (int i = 0; i < RAMP_POINTS; i++) {
    int x = graphX + (i * graphW) / (RAMP_POINTS - 1);
    int y = graphY + graphH - (rampCurve[i] * graphH) / 255;
    if (i < RAMP_POINTS - 1) {
      int x1 = graphX + ((i + 1) * graphW) / (RAMP_POINTS - 1);
      int y1 = graphY + graphH - (rampCurve[i + 1] * graphH) / 255;
      display.drawLine(x, y, x1, y1, SSD1306_WHITE);
    }
    if (i == rampEditIndex)
      display.fillCircle(x, y, 2, SSD1306_WHITE);
    else
      display.drawCircle(x, y, 1, SSD1306_WHITE);
  }
  if (rampEditIndex == RAMP_POINTS) {
    display.setCursor(0, 24);
    display.print("Exit editor");
  } else {
    display.setCursor(0, 24);
    display.print("NAV: point FIRE: value");
  }
}

void drawSettingsLauncherScreen(int x_offset) {
  // Малюємо іконку по центру
  display.drawBitmap(x_offset + (SCREEN_WIDTH - 20) / 2, 0, settings_icon_bitmap, 20, 20, SSD1306_WHITE);
  
  // Малюємо підпис
  display.setTextSize(1);
  display.setCursor(x_offset + 40, 24);
  display.print("Settings");

  // Точки навігації
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawStatsLauncherScreen(int x_offset) {
  // Малюємо іконку по центру
  display.drawBitmap(x_offset + (SCREEN_WIDTH - 20) / 2, 0, stats_icon_bitmap, 20, 20, SSD1306_WHITE);

  // Малюємо підпис
  display.setTextSize(1);
  display.setCursor(x_offset + 35, 24);
  display.print("Statistics");

  // Точки навігації
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawStatsMenu(int x_offset) {
  display.setCursor(x_offset + 0, 0);
  display.print("Statistics");
  display.setCursor(x_offset + 0, 16);
  display.print("Total Puffs: ");
  display.print(totalPuffs);
  display.setCursor(x_offset + 0, 24);
  display.print("Last Puff: ");
  display.print(lastPuffDuration_ms / 1000.0, 1);
  display.print("s");

  // Точки навігації (малюємо тільки на активному екрані)
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawMeteoScreen() {
  float pressure = bmp.readPressure() / 100.0; // гПа
  float altitude = bmp.readAltitude();         // метри

  display.setCursor(0, 0);
  display.print("Meteo Station");
  display.setCursor(0, 16);
  display.print("Pressure: ");
  display.print(pressure, 1);
  display.print(" hPa");
  display.setCursor(0, 24);
  display.print("Altitude: ");
  display.print(altitude, 1);
  display.print(" m");
}

void drawCurveMenu() {
  display.setCursor(0, 0);
  display.print("Curve Editor");
  int graphX = 60, graphY = 8, graphW = 60, graphH = 16;
  for (int i = 0; i < RAMP_POINTS; i++) {
    int x = graphX + (i * graphW) / (RAMP_POINTS - 1);
    int y = graphY + graphH - (rampCurve[i] * graphH) / 255;
    if (i < RAMP_POINTS - 1) {
      int x1 = graphX + ((i + 1) * graphW) / (RAMP_POINTS - 1);
      int y1 = graphY + graphH - (rampCurve[i + 1] * graphH) / 255;
      display.drawLine(x, y, x1, y1, SSD1306_WHITE);
    }
    if (i == rampEditIndex)
      display.fillCircle(x, y, 2, SSD1306_WHITE);
    else
      display.drawCircle(x, y, 1, SSD1306_WHITE);
  }
  if (rampEditIndex == RAMP_POINTS) {
    display.setCursor(0, 24);
    display.print("Exit editor");
  } else {
    display.setCursor(0, 24);
    display.print("NAV: point FIRE: value");
  }
}

void drawSettingsLauncherScreen(int x_offset) {
  // Малюємо іконку по центру
  display.drawBitmap(x_offset + (SCREEN_WIDTH - 20) / 2, 0, settings_icon_bitmap, 20, 20, SSD1306_WHITE);
  
  // Малюємо підпис
  display.setTextSize(1);
  display.setCursor(x_offset + 40, 24);
  display.print("Settings");

  // Точки навігації
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawStatsLauncherScreen(int x_offset) {
  // Малюємо іконку по центру
  display.drawBitmap(x_offset + (SCREEN_WIDTH - 20) / 2, 0, stats_icon_bitmap, 20, 20, SSD1306_WHITE);

  // Малюємо підпис
  display.setTextSize(1);
  display.setCursor(x_offset + 35, 24);
  display.print("Statistics");

  // Точки навігації
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawStatsMenu(int x_offset) {
  display.setCursor(x_offset + 0, 0);
  display.print("Statistics");
  display.setCursor(x_offset + 0, 16);
  display.print("Total Puffs: ");
  display.print(totalPuffs);
  display.setCursor(x_offset + 0, 24);
  display.print("Last Puff: ");
  display.print(lastPuffDuration_ms / 1000.0, 1);
  display.print("s");

  // Точки навігації (малюємо тільки на активному екрані)
  if (x_offset == 0) {
    int dotsX = SCREEN_WIDTH - 15;
    int dotsY = SCREEN_HEIGHT - 5;
    int dotSpacing = 5;
    int dotRadius = 1;
    for (int i = 0; i < MENU_COUNT; i++) {
      if (i == currentMenuIndex) {
        display.fillCircle(dotsX + (i * dotSpacing), dotsY, dotRadius + 1, SSD1306_WHITE);
      } else {
        display.drawCircle(dotsX + (i * dotSpacing), dotsY, dotRadius, SSD1306_WHITE);
      }
    }
  }
}

void drawMeteoScreen() {
  float pressure = bmp.readPressure() / 100.0; // гПа
  float altitude = bmp.readAltitude();         // метри

  display.setCursor(0, 0);
  display.print("Meteo Station");
  display.setCursor(0, 16);
  display.print("Pressure: ");
  display.print(pressure, 1);
  display.print(" hPa");
  display.setCursor(0, 24);
  display.print("Altitude: ");
  display.print(altitude, 1);
  display.print(" m");
}

void drawCurveMenu() {
  display.setCursor(0, 0);
  display.print("Curve Editor");
  int graphX = 60, graphY = 8, graphW = 60, graphH = 16;
  for (int i = 0; i < RAMP_POINTS; i++) {
    int x = graphX + (i * graphW) / (RAMP_POINTS - 1);
    int y = graphY + graphH - (rampCurve[i] * graphH) / 255;
    if (i < RAMP_POINTS - 1) {
      int x1 = graphX + ((i + 1) * graphW) / (RAMP_POINTS - 1);
      int y1 = graphY + graphH - (rampCurve[i + 1] * graphH) / 255;
      display.drawLine(x, y, x1, y1, SSD1306_WHITE);
    }
    if (i == rampEditIndex)
      display.fillCircle(x, y, 2, SSD1306_WHITE);
    else
      display.drawCircle(x, y, 1, SSD1306_WHITE);
  }
  if (rampEditIndex == RAMP_POINTS) {
    display.setCursor(0, 24);
    display.print("Exit editor");
  } else {
    display.setCursor(0, 24);
    display.print("NAV: point FIRE: value");
  }
}

// --- Обробка кнопок з антидребезгом ---
void handleButtons() {
  // --- Обробка кнопки "Fire" ---
  fireButton_currentReading = digitalRead(BUTTON_FIRE_PIN); // Зчитуємо поточний стан кнопки

  // Якщо стан змінився, скидаємо таймер антидребезгу
  if (fireButton_currentReading != fireButton_lastState) {
    fireButton_lastDebounceTime = millis();
  }

  // Якщо пройшов час debounceDelay і стан стабільний
  if ((millis() - fireButton_lastDebounceTime) > debounceDelay) {
    // Якщо поточний стабільний стан відрізняється від попереднього дебонсованого
    if (fireButton_currentReading != fireButton_debouncedState) {
      fireButton_debouncedState = fireButton_currentReading; // Оновлюємо дебонсований стан

      if (fireButton_debouncedState == LOW) { // Кнопка щойно була натиснута
        handleFireButtonPress();
      } else { // Кнопка щойно була відпущена
        handleFireButtonRelease();
      }
    }
  }
  fireButton_lastState = fireButton_currentReading; // Зберігаємо поточний сирий стан для наступної ітерації

  // --- Обробка кнопки навігації ---
  navButton_currentReading = digitalRead(BUTTON_NAV_PIN); // Зчитуємо поточний стан кнопки

  if (navButton_currentReading != navButton_lastState) {
    navButton_lastDebounceTime = millis();
  }

  if ((millis() - navButton_lastDebounceTime) > debounceDelay) {
    if (navButton_currentReading != navButton_debouncedState) {
      navButton_debouncedState = navButton_currentReading;

      if (navButton_debouncedState == LOW) { // Кнопка щойно була натиснута
        handleNavButtonPress();
      }
      // Для кнопки навігації нас поки що цікавить лише натискання, а не відпускання
    }
  }
  navButton_lastState = navButton_currentReading; // Зберігаємо поточний сирий стан
}

void handleFireButtonPress() {
  // --- Логіка для режиму очікування пробудження ---
  if (awaiting_wake_clicks) {
    wake_click_count++;
    wake_check_start_time = millis(); // Скидаємо таймер при кожному кліку
    if (wake_click_count >= 5) {
      awaiting_wake_clicks = false; // Виходимо з режиму очікування
      lastActivityTime = millis();  // Встановлюємо час активності
      showSplashScreen();           // Показуємо заставку
    }
    return; // Більше нічого не робимо в цьому режимі
  }

  lastActivityTime = millis(); // Будь-який клік - це активність

  // --- Логіка підрахунку кліків ---
  if (millis() - lastFireClickTime > 600) { // Скидаємо лічильник, якщо пауза завелика
    fireClickCount = 0;
  }
  lastFireClickTime = millis();
  fireClickCount++;

  // --- Логіка 5-ти натискань для вимкнення ---
  if (fireClickCount >= 5) {
    fireClickCount = 0; // Скидаємо лічильник
    enterDeepSleep();
    return; // Виходимо, щоб не обробляти інші дії
  }

  // --- Логіка 2-х натискань для зміни потужності (ТІЛЬКИ на головному екрані) ---
  if (currentMenu == MAIN_SCREEN && fireClickCount == 2) {
    wattageAdjustMode = true;
    wattageAdjustDirectionUp = !wattageAdjustDirectionUp; // Чергуємо напрямок
    lastWattageAdjustTime = millis();
    firePressStartTime = 0; // Скасовуємо початок затяжки від першого кліку
    Serial.print("Wattage adjust mode ON. Direction: ");
    Serial.println(wattageAdjustDirectionUp ? "UP" : "DOWN");
    return; // Виходимо, щоб не обробляти цей клік як початок затяжки
  }

  // Якщо ми в режимі налаштування потужності, то кліки не обробляються тут
  if (wattageAdjustMode) {
    return;
  }

  Serial.println("Fire button PRESSED!");
  if (currentMenu == MAIN_SCREEN) {
    // Не починаємо затяжку одразу, а лише фіксуємо час натискання
    if (!isPuffing) {
      firePressStartTime = millis();
    }
  } else if (currentMenu == SETTINGS_LAUNCHER) {
    currentMenu = SETTINGS_SUB_MENU;
    settingsMenuIndex = 0; // Починаємо з першого пункту
  } else if (currentMenu == STATS_LAUNCHER) {
    currentMenu = STATS_SUB_MENU;
  } else if (currentMenu == SETTINGS_SUB_MENU && inCurveEditor) {
    if (rampEditIndex < RAMP_POINTS) {
      rampCurve[rampEditIndex] += 32;
      if (rampCurve[rampEditIndex] > 255) rampCurve[rampEditIndex] = 0;
      Serial.print("Ramp point "); Serial.print(rampEditIndex);
      Serial.print(" set to: "); Serial.println(rampCurve[rampEditIndex]);
    }
  } else if (currentMenu == SETTINGS_SUB_MENU && settingsMenuIndex == SETTINGS_CURVE_EDITOR) {
    inCurveEditor = true;
    rampEditIndex = 0;
    Serial.println("Enter Curve Editor");
  } else if (currentMenu == SETTINGS_SUB_MENU && settingsMenuIndex == SETTINGS_RAMP_TIME) {
    rampDuration_ms += 250;
    if (rampDuration_ms > 3000) rampDuration_ms = 0;
    Serial.print("Ramp duration set to: ");
    Serial.println(rampDuration_ms);
    savePreferences();
  } else if (currentMenu == SETTINGS_SUB_MENU && settingsMenuIndex == SETTINGS_METEO) {
    inMeteoScreen = true;
    Serial.println("Enter Meteo Screen");
  } else if (currentMenu == SETTINGS_SUB_MENU && settingsMenuIndex == SETTINGS_ANIMATION_TIME) {
    animationDuration_ms += 250;
    if (animationDuration_ms > 2000) animationDuration_ms = 0;
    Serial.print("Animation duration set to: ");
    Serial.println(animationDuration_ms);
    savePreferences();
  } else if (currentMenu == SETTINGS_SUB_MENU && settingsMenuIndex == SETTINGS_LED_MODE) {
    currentLedMode = (LedMode)((currentLedMode + 1) % LED_MODE_COUNT);
    Serial.print("LED Mode set to: ");
    Serial.println(currentLedMode);
    savePreferences();
  } else if (currentMenu == SETTINGS_SUB_MENU && settingsMenuIndex == SETTINGS_PUFF_SENSITIVITY) {
    puffPressureThreshold_Pa += 10; // Крок зміни 10 Па
    if (puffPressureThreshold_Pa > 100) {
      puffPressureThreshold_Pa = 20; // Циклічно змінюємо від 20 до 100
    }
    Serial.print("Puff sensitivity set to: ");
    Serial.println(puffPressureThreshold_Pa);
    savePreferences();
  } else if (currentMenu == SETTINGS_SUB_MENU && settingsMenuIndex == SETTINGS_FIRE_DELAY) {
    fireButtonDelay_ms += 200; // Крок 200 мс
    if (fireButtonDelay_ms > 1000) {
      fireButtonDelay_ms = 0;
    }
    Serial.print("Fire delay set to: ");
    Serial.println(fireButtonDelay_ms);
    savePreferences();
  } else if (currentMenu == SETTINGS_SUB_MENU && settingsMenuIndex == SETTINGS_AUTO_OFF) {
    if (autoOffDelay_ms == 0) autoOffDelay_ms = 60000;       // Off -> 1 min
    else if (autoOffDelay_ms == 60000) autoOffDelay_ms = 300000; // 1 min -> 5 min
    else if (autoOffDelay_ms == 300000) autoOffDelay_ms = 600000; // 5 min -> 10 min
    else autoOffDelay_ms = 0;                                // 10 min -> Off
    Serial.print("Auto-off set to: ");
    Serial.println(autoOffDelay_ms);
    savePreferences();
  }
}

void handleFireButtonRelease() {
  // Якщо відпустили кнопку до того, як спрацювала затримка, скасовуємо затяжку
  firePressStartTime = 0;
  
  //if (!isDeviceOn) return;

  Serial.println("Fire button RELEASED!");
  if (isPuffing) { // Закінчуємо затяжку, тільки якщо вона була активна
    unsigned long currentPuffDuration = millis() - puffStartTime_ms;
    // Встановлюємо мінімальну тривалість затяжки, щоб уникнути помилкових спрацьовувань від випадкових торкань
    if (currentPuffDuration >= 200) { // Наприклад, мінімум 200 мс
      totalPuffs++;
      lastPuffDuration_ms = currentPuffDuration;
      savePreferences(); // Зберігаємо дані після кожної затяжки
      Serial.print("Puff ended, duration: ");
      Serial.print(currentPuffDuration);
      Serial.println(" ms");
    } else {
        Serial.println("Puff too short, ignored.");
    }
    isPuffing = false; // Скидаємо прапорець активної затяжки
    puffStartTime_ms = 0; // Скидаємо час початку
  }
  pwmActive = false; // Вимикаємо PWM
}

void handleNavButtonPress() {
  lastActivityTime = millis(); // Оновлюємо час активності

  // --- Логіка для виходу з під-меню ---
  if (currentMenu == SETTINGS_SUB_MENU || currentMenu == STATS_SUB_MENU) {
    if (inCurveEditor) { // Логіка для редактора кривої
        rampEditIndex++;
        if (rampEditIndex > RAMP_POINTS) {
          inCurveEditor = false;
          rampEditIndex = 0;
          savePreferences();
        }
    } else if (inMeteoScreen) { // Логіка для метео
        inMeteoScreen = false;
    } else { // Вихід з самого під-меню
        currentMenu = (currentMenu == SETTINGS_SUB_MENU) ? SETTINGS_LAUNCHER : STATS_LAUNCHER;
    }
    return;
  }

  // --- Логіка для гортання головних екранів ---
  if (animationDuration_ms == 0) {
      currentMenu = (MenuState)((currentMenuIndex + 1) % MENU_COUNT);
      currentMenuIndex = (int)currentMenu;
  } else {
      animationDirection = 1;
      nextMenu = (MenuState)((currentMenuIndex + 1) % MENU_COUNT);
      animationStartTime = millis();
      isScreenAnimating = true;
  }
  Serial.print("Switched to menu: ");
  Serial.println(nextMenu);
}




// --- Детектор затяжки по тиску ---
void readBMP180DataAndDetectPuff() {
  // Зчитуємо дані не частіше, ніж раз на 100 мс, щоб не перевантажувати I2C
  if (millis() - lastPressureReadTime < 100) {
    return;
  }
  lastPressureReadTime = millis();

  float currentPressure = bmp.readPressure();

  // Якщо датчик не підключений, виходимо
  if (currentPressure == 0) return;

  // Оновлюємо базовий тиск ТІЛЬКИ, коли немає затяжки
  if (!isPuffing) {
    // Заповнюємо буфер для ковзної середньої
    pressureReadings[pressureReadingIndex] = currentPressure;
    pressureReadingIndex = (pressureReadingIndex + 1) % PRESSURE_READING_COUNT;

    // Розраховуємо середній (базовий) тиск
    float totalPressure = 0;
    for (int i = 0; i < PRESSURE_READING_COUNT; i++) {
      if (pressureReadings[i] == 0) { // Якщо буфер ще не заповнений, чекаємо
        baselinePressure_Pa = currentPressure;
        return;
      }
      totalPressure += pressureReadings[i];
    }
    baselinePressure_Pa = totalPressure / PRESSURE_READING_COUNT;
  }

  // --- Логіка скінченного автомата для детекції ---
  switch (puffDetectorState) {
    case PUFF_IDLE:
      // Якщо тиск впав нижче порогу і затяжка ще не активна
      if (!isPuffing && currentPressure < (baselinePressure_Pa - puffPressureThreshold_Pa)) {
        Serial.print("Puff detected! Pressure drop: ");
        Serial.println(baselinePressure_Pa - currentPressure);
        
        puffDetectorState = PUFF_ACTIVE;
        startPuff(); // Використовуємо централізовану функцію для початку затяжки
      }
      break;

    case PUFF_ACTIVE:
      // Якщо тиск повернувся до норми (з невеликим гістерезисом), порівнюючи з "замороженим" базовим тиском
      if (currentPressure >= (baselinePressure_Pa - (puffPressureThreshold_Pa * 0.5))) {
        Serial.println("Puff ended (pressure restored).");
        
        puffDetectorState = PUFF_IDLE;
        // Завершуємо логіку затяжки, якщо вона була активна
        if (isPuffing) {
            unsigned long currentPuffDuration = millis() - puffStartTime_ms;
            if (currentPuffDuration >= 200) {
              totalPuffs++;
              lastPuffDuration_ms = currentPuffDuration;
              savePreferences();
            }
            isPuffing = false;
            puffStartTime_ms = 0;
            pwmActive = false;
        }
      }
      break;
  }
}


void updateBatteryVoltage() {
  // Ця функція повинна зчитувати реальну напругу батареї,
  // ймовірно, використовуючи аналоговий пін, підключений через дільник напруги.
  // Наприклад:
  // const int BATTERY_ANALOG_PIN = 34; // Приклад GPIO піна для АЦП (ESP32)
  // int rawValue = analogRead(BATTERY_ANALOG_PIN);
  // // Припускаючи дільник напруги з відомим коефіцієнтом, наприклад, 2:1 для 3.3V АЦП
  // // Якщо АЦП 12-бітний (0-4095) і опорна напруга 3.3V
  // const float ADC_REFERENCE_VOLTAGE = 3.3;
  // const int ADC_MAX_VALUE = 4095;
  // const float VOLTAGE_DIVIDER_RATIO = 2.0; // Якщо ви використовуєте дільник напруги 2x (наприклад, резистори 10k і 10k)
  // batteryVoltage = rawValue * (ADC_REFERENCE_VOLTAGE / ADC_MAX_VALUE) * VOLTAGE_DIVIDER_RATIO;
  // Serial.print("Battery Voltage: "); Serial.println(batteryVoltage);
}

void updatePWM() {
  if (pwmActive) {
    unsigned long elapsed = millis() - pwmStartTime;
    int basePwmValue;
    if (elapsed < rampDuration_ms) {
      // Визначаємо, між якими точками ми зараз
      int segment = (elapsed * (RAMP_POINTS - 1)) / rampDuration_ms;
      unsigned long segStart = (segment * rampDuration_ms) / (RAMP_POINTS - 1);
      unsigned long segEnd = ((segment + 1) * rampDuration_ms) / (RAMP_POINTS - 1);
      uint8_t pwmStart = rampCurve[segment];
      uint8_t pwmEnd = rampCurve[segment + 1];
      // Лінійна інтерполяція між точками
      basePwmValue = pwmStart + (pwmEnd - pwmStart) * (elapsed - segStart) / (segEnd - segStart);
    } else {
      basePwmValue = rampCurve[RAMP_POINTS - 1]; // Останнє значення
    }

    // Масштабуємо потужність відповідно до налаштування "ватт"
    int finalPwmValue = (int)(basePwmValue * (currentWattage / 20.0f));
    ledcWrite(PWM_CHANNEL, finalPwmValue);

  } else {
    ledcWrite(PWM_CHANNEL, 0);
  }
}

void updateVaporAnimation() {
  unsigned long now = millis();
  if (now - vaporAnimLastUpdate > 10) { // Оновлюємо кожні 10мс
    vaporAnimLastUpdate = now;
    if (isPuffing) {
      vaporAppearing = true;
      if (vaporAnimProgress < 1.0f) vaporAnimProgress += vaporAnimSpeed;
      if (vaporAnimProgress > 1.0f) vaporAnimProgress = 1.0f;
    } else {
      vaporAppearing = false;
      if (vaporAnimProgress > 0.0f) vaporAnimProgress -= vaporAnimSpeed;
      if (vaporAnimProgress < 0.0f) vaporAnimProgress = 0.0f;
    }
  }
}

void updateLed() {
  // Плавне ввімкнення/вимкнення
  if (isPuffing && ledBrightness < 1.0f) {
    ledBrightness += 0.05f; // Швидкість появи
    if (ledBrightness > 1.0f) ledBrightness = 1.0f;
  } else if (!isPuffing && ledBrightness > 0.0f) {
    ledBrightness -= 0.05f; // Швидкість згасання
    if (ledBrightness < 0.0f) ledBrightness = 0.0f;
  }

  if (ledBrightness <= 0.0f) {
    strip.clear();
    strip.show();
    return;
  }

  uint32_t color;
  static uint16_t rainbowHue = 0;

  switch (currentLedMode) {
    case LED_OFF:
      color = strip.Color(0, 0, 0);
      break;

    case LED_RAINBOW:
      rainbowHue += 256; // Швидкість зміни кольору
      color = strip.ColorHSV(rainbowHue);
      break;

    case LED_RANDOM:
      color = randomPuffColor;
      break;

    case LED_BATTERY:
      if (batteryVoltage >= 3.95) { // 75-100%
        color = strip.Color(0, 255, 0); // Зелений
      } else if (batteryVoltage >= 3.7) { // 50-75%
        color = strip.Color(255, 255, 0); // Жовтий
      } else { // 10-50%
        color = strip.Color(255, 0, 0); // Червоний
      }
      break;
  }

  // Застосовуємо яскравість
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;
  strip.setPixelColor(0, strip.Color(r * ledBrightness, g * ledBrightness, b * ledBrightness));
  strip.show();
}

// Вставте сюди масив з конвертера image2cpp
// 'logo', 128x32px


void showSplashScreen() {
  display.clearDisplay();
  display.drawBitmap(0, 0, epd_bitmap_intro, 128, 32, SSD1306_WHITE);
  display.display();
  delay(1500); // Показуємо заставку 1.5 секунди
}

void startPuff() {
  if (isPuffing) return; // Вже в процесі

  lastActivityTime = millis(); // Затяжка - це активність

  Serial.println("Puff Started!");
  puffStartTime_ms = millis();
  isPuffing = true;
  pwmActive = true;
  pwmStartTime = millis();

  // Логіка для режиму випадкового кольору
  if (currentLedMode == LED_RANDOM) {
    const uint32_t colors[] = {
      strip.Color(255, 0, 0), strip.Color(0, 0, 255), strip.Color(0, 255, 0),
      strip.Color(255, 255, 0), strip.Color(128, 0, 128), strip.Color(255, 165, 0),
      strip.Color(0, 255, 255)
    };
    randomPuffColor = colors[random(0, 7)];
  }
}

void enterDeepSleep() {
  Serial.println("Entering deep sleep mode...");
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(35, 8);
  display.print("OFF");
  display.display();
  delay(1000); // Показуємо напис "OFF" секунду
  display.clearDisplay();
  display.display();

  // Налаштовуємо пробудження по кнопці Fire (пін повинен перейти в LOW)
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_15, 0); // Переконайтесь, що тут ваш RTC-пін

  // Переходимо в глибокий сон
  esp_deep_sleep_start();
}