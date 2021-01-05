#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <DS3231.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
DS3231 Clock;

bool tick = true;

void setup(void) {
  Serial.begin(9600);
  u8g2.begin();
  u8g2.setDisplayRotation(U8G2_MIRROR);
}

void loop(void) {
  u8g2.clearBuffer();

  // hour & minute
  bool h12;
  bool PM;
  int hour = Clock.getHour(h12, PM);
  int minute = Clock.getMinute();
  int second = Clock.getSecond();
  char hour_text[10];
  char minute_text[10];
  
  sprintf(hour_text, "%02d", hour);
  sprintf(minute_text, "%02d", minute);
  
  u8g2.setFont(u8g2_font_osr35_tn);
  u8g2.drawStr(0, 36, hour_text);
  u8g2.drawStr(70, 36, minute_text);
  // u8g2.drawStr(90, 47, String(second).c_str());
  u8g2.drawStr(56, 31, ":");
  
  // date
  bool century;
  int month = Clock.getMonth(century);
  int date = Clock.getDate();
  u8g2.setFont(u8g2_font_crox2c_mn);
  u8g2.drawStr(0, 63, String(Clock.getYear()).c_str());
  u8g2.drawStr(month>=10?40:46, 63, String(month).c_str());
  u8g2.drawStr(month>=10?72:68, 63, String(date).c_str());
  u8g2.drawStr(120, 63, String(Clock.getDoW()).c_str());

  // units
  u8g2.setFont(u8g2_font_wqy13_t_gb2312);
  u8g2.drawUTF8(18, 63, "年");
  u8g2.drawUTF8(month>=10?59:56, 63, "月");
  u8g2.drawUTF8(94, 63, "星期");

  // set brightness
  u8g2.setContrast(255-(float(second)/60*255));

  // ticking indication
  int ti_x = 120;
  int ti_y = 40;
  u8g2.drawCircle(ti_x, ti_y, 1);
  u8g2.drawLine(ti_x, ti_y, tick?ti_x-1:ti_x+1, ti_y+5);
  u8g2.drawCircle(tick?ti_x-2:ti_x+2, ti_y+7, 2);
  

  tick = !tick;

  u8g2.sendBuffer();
  delay(1000);
}
