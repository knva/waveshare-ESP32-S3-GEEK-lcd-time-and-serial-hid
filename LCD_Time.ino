#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

#include "USB.h"
#include "USBHIDMouse.h"
#include "USBHIDKeyboard.h"
USBHIDMouse Mouse;
USBHIDKeyboard Keyboard;

const char* ssid = "weixuedianzi";
const char* password = "weixuedianzi";
const char* ntpServer = "cn.ntp.org.cn";
const long utcOffsetInSeconds = 28800;  // Beijing: UTC +8 - Get the Eastern 8 Zone time (by default, the prime meridian of the Greenwich Observatory is the base line)
                                        // 28800 = 8 * 60 * 60
void task1(void* pvParameters) {
  //任务初始化程序
  while (1) {
    time_t now = time(nullptr);
    char* timeString = ctime(&now);
    removeNewlineCharacters(timeString);

    char date[20];  // Save a buffer for the date, such as "2023 Jan 01 Tue"
    char time[9];   // Save time buffer, such as "12:34:56"

    // Extract date and time
    extractDateAndTime(timeString, date, time);

    // Serial.print("Date: ");
    // Serial.print(date);
    // Serial.print(" ");
    // Serial.print((date + 6));
    // Serial.print(" ");
    // Serial.print((date + 10));
    // Serial.print(" ");
    // Serial.println((date + 14));
    // Serial.print("Time: ");
    // Serial.println(time);

    // Serial.print("Current time is: ");
    // Serial.println(timeString);  /// Print time
    Paint_DrawString_EN(55, 32, time, &Font24, BLACK, GREEN);
    Paint_DrawString_EN(15, 82, date, &Font20, BLACK, GREEN);
    Paint_DrawString_EN(80, 82, (date + 6), &Font20, BLACK, GREEN);
    Paint_DrawString_EN(135, 82, (date + 10), &Font20, BLACK, GREEN);
    Paint_DrawString_EN(175, 82, (date + 14), &Font20, BLACK, GREEN);
    Paint_DrawString_EN(15, 102, "AimBot Started", &Font20, BLACK, GREEN);
    // Convert current time to Unix timestamp
    // long unixTimestamp = static_cast<long>(now);  //Gets a unix timestamp
    // Serial.print("Unix timestamp is: ");
    // Serial.println(unixTimestamp);
    delay(100);
  }
  vTaskDelete(NULL);  //删除自身函数
}
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);
  Mouse.begin();
  Keyboard.begin();
  USB.begin();

  Config_Init();
  LCD_Init();
  LCD_SetBacklight(100);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE);
  Paint_SetRotate(90);
  LCD_Clear(BLACK);
  delay(1000);

  // while (!Serial)
  //   ;
  Paint_DrawString_EN(20, 50, "Wifi Connecting...", &Font20, BLACK, GREEN);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  LCD_Clear(BLACK);
  Paint_DrawString_EN(20, 50, "Wifi Connected", &Font20, BLACK, GREEN);
  Serial.println("Connected to WiFi");
  //Acquisition time
  configTime(utcOffsetInSeconds, 0, ntpServer);
  while (!time(nullptr)) {
    delay(1000);
    Serial.println("Waiting for time sync...");
  }
  LCD_Clear(BLACK);
  Serial.println("Time synced successfully");
  xTaskCreate(task1,
              "printf",
              4096,
              NULL,
              1,
              NULL);
}
int dx;
int dy;
int dxn;
int dyn;
int xindex = 0;
int num_size = 0;

int jump = 127;
void loop() {

  if (Serial.available()) {

    String data = Serial.readString();
    if (data.substring(0, 5)== "shoot") {
      Mouse.click();
    } else if (data.substring(0, 5) == "input") {
      data.remove(0, 5);
      Keyboard.write((const uint8_t*)data.c_str(), data.length());
    } else if (data == "link") {
      Serial.write("ok", 2);
    } else if (data.substring(0, 6) == "silent") {
      data.remove(0, 6);
      xindex = 0;
      num_size = data.indexOf(":", xindex);
      dx = data.substring(xindex, num_size).toInt();
      data.remove(0, num_size + 1);
      dy = data.toInt();
      dxn = dx * -1;
      dyn = dy * -1;

      if (dx > 0) {
        while (dx > 127) {
          dx -= 127;
          Mouse.move(127, 0);
        }
        Mouse.move(dx, 0);
      } else if (dx < 0) {
        while (dx < -127) {
          dx += 127;
          Mouse.move(-127, 0);
        }
        Mouse.move(dx, 0);
      }
      if (dy >= 0) {
        while (dy > 127) {
          dy -= 127;
          Mouse.move(0, 127);
        }
        Mouse.move(0, dy);
      } else if (dy <= 0) {
        while (dy < -127) {
          dy += 127;
          Mouse.move(0, -127);
        }
        Mouse.move(0, dy);
      }
      Mouse.click();
      if (dxn > 0) {
        while (dxn > 127) {
          dxn -= 127;
          Mouse.move(127, 0);
        }
        Mouse.move(dxn, 0);
      } else if (dxn < 0) {
        while (dxn < -127) {
          dxn += 127;
          Mouse.move(-127, 0);
        }
        Mouse.move(dxn, 0);
      }
      if (dyn > 0) {
        while (dyn > 127) {
          dyn -= 127;
          Mouse.move(0, 127);
        }
        Mouse.move(0, dyn);
      } else if (dyn < 0) {
        while (dyn < -127) {
          dyn += 127;
          Mouse.move(0, -127);
        }
        Mouse.move(0, dyn);
      }
    }

    else {
      xindex = 0;
      num_size = data.indexOf(":", xindex);
      dx = data.substring(xindex, num_size).toInt();
      data.remove(0, num_size + 1);
      dy = data.toInt();
      // Serial.println(dx+":"+dy);
      if (dx > 0) {
        while (dx > jump) {
          dx -= jump;
          Mouse.move(jump, 0);
        }
        Mouse.move(dx, 0);
      } else if (dx < 0) {
        while (dx < -jump) {
          dx += jump;
          Mouse.move(-jump, 0);
        }
        Mouse.move(dx, 0);
      }
      if (dy >= 0) {
        while (dy > jump) {
          dy -= jump;
          Mouse.move(0, jump);
        }
        Mouse.move(0, dy);
      } else if (dy <= 0) {
        while (dy < -jump) {
          dy += jump;
          Mouse.move(0, -jump);
        }
        Mouse.move(0, dy);
      }
    }
  }
}

void removeNewlineCharacters(char* str) {
  size_t len = strlen(str);

  // Search forward from the end of the string, dropping the ending '\r' and '\n'
  for (int i = len - 1; i >= 0; --i) {
    if (str[i] == '\r' || str[i] == '\n') {
      str[i] = '\0';  // Replace '\r' or '\n' with '\o'
    } else {
      break;  // Stop after finding the first character that is not '\r' and '\n'
    }
  }
}

void extractDateAndTime(const char* timeString, char* dateTimeStr, char* timeStr) {
  // Use the sscanf function to extract the week, month, date, and year from the string
  sscanf(timeString, "%s %s %s %s %s", dateTimeStr + 14, dateTimeStr + 6, dateTimeStr + 10, timeStr, dateTimeStr);
}