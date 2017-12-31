#define INTERVAL_SENSOR   17000             //瀹氫箟浼犳劅鍣ㄩ噰鏍锋椂闂撮棿闅�  597000
#define INTERVAL_NET      17000             //瀹氫箟鍙戦�佹椂闂�
//浼犳劅鍣ㄩ儴鍒�================================   
#include <Wire.h>                                  //璋冪敤搴�  
#include "./ESP8266.h"
#include "I2Cdev.h"                                //璋冪敤搴�  
//娓╂箍搴�   
#include "./SHT2x.h"
//鍏夌収
#define  sensorPin_1  A0

#define SSID           "zhonglucen"                   // cannot be longer than 32 characters!
#define PASSWORD       "88888888"

#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) with no data 
                                   // received before closing the connection.  If you know the server
                                   // you're accessing is quick to respond, you can reduce this value.

//WEBSITE     
#define HOST_NAME   "api.heclouds.com"
#define DEVICEID   "20471364"
#define PROJECTID "107064"
#define HOST_PORT   (80)
String apiKey="Xmj4aNDYVHaEZ3d3fsU=v4Jrh2o=";
char buf[10];

#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();

float tempOLED, humiOLED, lightnessOLED;

#define INTERVAL_OLED 1000

String mCottenData;
String jsonToSend;

//3,浼犳劅鍣ㄥ�肩殑璁剧疆 
float sensor_tem, sensor_hum, sensor_lux;                    //浼犳劅鍣ㄦ俯搴︺�佹箍搴︺�佸厜鐓�   
char  sensor_tem_c[7], sensor_hum_c[7], sensor_lux_c[7] ;    //鎹㈡垚char鏁扮粍浼犺緭
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 */
ESP8266 wifi(mySerial);
//ESP8266 wifi(Serial1);                                      //瀹氫箟涓�涓狤SP8266锛坵ifi锛夌殑瀵硅薄
unsigned long net_time1 = millis();                          //鏁版嵁涓婁紶鏈嶅姟鍣ㄦ椂闂�
unsigned long sensor_time = millis();                        //浼犳劅鍣ㄩ噰鏍锋椂闂磋鏃跺櫒

//int SensorData;                                   //鐢ㄤ簬瀛樺偍浼犳劅鍣ㄦ暟鎹�
String postString;                                //鐢ㄤ簬瀛樺偍鍙戦�佹暟鎹殑瀛楃涓�
//String jsonToSend;                                //鐢ㄤ簬瀛樺偍鍙戦�佺殑json鏍煎紡鍙傛暟

void setup(void)     //鍒濆鍖栧嚱鏁�  
{       
  //鍒濆鍖栦覆鍙ｆ尝鐗圭巼  
    Wire.begin();
    Serial.begin(115200);   
    while(!Serial);
    pinMode(sensorPin_1, INPUT);

   //ESP8266鍒濆鍖�
    Serial.print("setup begin\r\n");   

  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print("to station + softap ok\r\n");
  } else {
    Serial.print("to station + softap err\r\n");
  }

  if (wifi.joinAP(SSID, PASSWORD)) {      //鍔犲叆鏃犵嚎缃�
    Serial.print("Join AP success\r\n");  
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }

  if (wifi.disableMUX()) {
    Serial.print("single ok\r\n");
  } else {
    Serial.print("single err\r\n");
  }

  Serial.print("setup end\r\n");
    
  
}
void loop(void)     //寰幆鍑芥暟  
{   
  if (sensor_time > millis())  sensor_time = millis();  
    
  if(millis() - sensor_time > INTERVAL_SENSOR)              //浼犳劅鍣ㄩ噰鏍锋椂闂撮棿闅�  
  {  
    getSensorData();                                        //璇讳覆鍙ｄ腑鐨勪紶鎰熷櫒鏁版嵁
    sensor_time = millis();
  }  

    
  if (net_time1 > millis())  net_time1 = millis();
  
  if (millis() - net_time1 > INTERVAL_NET)                  //鍙戦�佹暟鎹椂闂撮棿闅�
  {                
    updateSensorData();                                     //灏嗘暟鎹笂浼犲埌鏈嶅姟鍣ㄧ殑鍑芥暟
    net_time1 = millis();
  }
  
}

void getSensorData(){  
    sensor_tem = SHT2x.GetTemperature() ;   
    sensor_hum = SHT2x.GetHumidity();   
    //鑾峰彇鍏夌収
    sensor_lux = analogRead(A0);    
    delay(1000);
    dtostrf(sensor_tem, 2, 1, sensor_tem_c);
    dtostrf(sensor_hum, 2, 1, sensor_hum_c);
    dtostrf(sensor_lux, 3, 1, sensor_lux_c);
}
void updateSensorData() {
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //寤虹珛TCP杩炴帴锛屽鏋滃け璐ワ紝涓嶈兘鍙戦�佽鏁版嵁
    Serial.print("create tcp ok\r\n");

jsonToSend="{\"Temperature\":";
    dtostrf(sensor_tem,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"Humidity\":";
    dtostrf(sensor_hum,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"Light\":";
    dtostrf(sensor_lux,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+="}";



    postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";
    postString+="\r\n";
    postString+="\r\n";

  const char *postArray = postString.c_str();                 //灏唖tr杞寲涓篶har鏁扮粍
  Serial.println(postArray);
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send鍙戦�佸懡浠わ紝鍙傛暟蹇呴』鏄繖涓ょ鏍煎紡锛屽挨鍏舵槸(const uint8_t*)
  Serial.println("send success");   
     if (wifi.releaseTCP()) {                                 //閲婃斁TCP杩炴帴
        Serial.print("release tcp ok\r\n");
        } 
     else {
        Serial.print("release tcp err\r\n");
        }
      postArray = NULL;                                       //娓呯┖鏁扮粍锛岀瓑寰呬笅娆′紶杈撴暟鎹�
  
  } else {
    Serial.print("create tcp err\r\n");
  }
  
}

