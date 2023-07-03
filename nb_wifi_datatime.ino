/*
  nb20230703
  本段提供wifi链接和获取日期时间
  获取时间的方式打开苏宁的一个显示当前日期和时间的网页来截取部分内容实现
  参考
  http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/iot-c/esp8266-nodemcu-web-server/pin-state/
  https://blog.csdn.net/tongyue/article/details/105305280
  http://quan.suning.com/getSysTime.do
  {"sysTime2":"2023-07-03 16:24:47","sysTime1":"20230703162447"}
  ------------------------------------------------------|----| 54-60
  ----------------------------------------------|------|4 6-54
  ------------------------|-------| 24-32
  -------------|--------| 13-23

  调用
  void setup() {
   Serial.begin(115200);
   nb_wifi_datetime();
  }
  void loop() {
  }

*/
//加入wifi信号强度

#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库
#include <ESP8266HTTPClient.h>
//#include <Arduino.h>
HTTPClient http;
void nb_wifi_datetime() {
  ESP8266WiFiMulti wifiMulti;
  wifiMulti.addAP("CQMXY", "08523690"); // 将需要连接的一系列WiFi ID和密码输入这里
  //wifiMulti.addAP("CMCC-NBXX", "14702580"); // ESP8266-NodeMCU再启动后会扫描当前网络
  //wifiMulti.addAP("CMCC-6610", "14702580"); // 环境查找是否有这里列出的WiFi ID。如果有
  Serial.println();
  Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) {  // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
    delay(1000);                             // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
    Serial.print(i++); Serial.print(' ');    // 将会连接信号最强的那一个WiFi信号。
  }                                          // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
  // 此处while循环判断是否跳出循环的条件。
  // WiFi连接成功后将通过串口监视器输出连接成功信息
  Serial.println('\n');                     // WiFi连接成功后
  Serial.print("Connected to ");            // NodeMCU将通过串口监视器输出。
  Serial.println(WiFi.SSID());              // 连接的WiFI名称
  Serial.print("IP address:\t");            // 以及
  Serial.println(WiFi.localIP());           // NodeMCU的IP地址
  delay(3000);
  String GetUrl = "http://quan.suning.com/getSysTime.do";
  http.setTimeout(5000);
  http.begin(GetUrl);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    //读取响应内容
    String response = http.getString();
    Serial.println(response);

    Serial.print(response.substring(13, 23));
    Serial.print(" / ");
    Serial.print(response.substring(24, 32));
    Serial.println();

    Serial.print(response.substring(46, 54));
    Serial.print(" / ");
    Serial.println(response.substring(54, 60));
    Serial.println();

    //{"sysTime2":"2023-07-03 16:53:08","sysTime1":"20230703165308"}
    //2023-07-03 / 16:53:08
    //20230703 / 165308

  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
  delay(10000);
}
