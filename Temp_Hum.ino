#include <DHT.h>
#include <DHT_U.h>
#include <avr/pgmspace.h>
#include <GSM.h>

#define ALL_INCOMING_SMS_ENABLED (1)                // 1 - разрешен приём команд со всех входящих/ 0 -только с номера CommandID
#define DHTPIN 7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

char TimeZone = +3;                                 // Ваш часовой пояс относительно GMT+00

const char RemoteID9[] PROGMEM = "+79288711997";    // Шаблон номера, на который отправляем СМС
const char PIN[] PROGMEM = "0000";                  // ПИН-код! Поменяйте на свой!!! Иначе СИМ карта заблокируется!
char IMEI[16];                                      // Буфер для считывания IMEI
char SenderID[16];                                  // Буфер номер отправителя СМС

                                

                            
bool a = false; 
void setup() { 
  delay(4000);                                      // Начальная задержка
  dht.begin();
  while (GSM.Init(PIN)<0);                        // Ждём инициализацию модема
  GSM.NewSMSindic(1);                             // Разрешаем индикацию новых СМС
  GSM.WaitSMS();
  
  Serial.begin(9600); 
  
} 

void loop() { 
  // read the value from the sensor: 
  
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); 
  static uint8_t fir[5] = {0}, i = 0; 
  fir[i] = humidity; 
  static uint8_t fir[5] = {0}, i = 0; 
  fir2[i] = temperature; 
  i++; 
  if (i > 5) i = 0; 
  uint16_t HumadityVal = (fir[0] + fir[1] + fir[2] + fir[3] + fir[4]) / 5; 
  uint16_t TemperatureVal = (fir2[0] + fir2[1] + fir2[2] + fir2[3] + fir2[4]) / 5; 


  
  delay(100); 
  
if (GSM.CheckSMS()== 1) { //Есть новое СМС
  GSM.ReadSMS(NewSMS_index, (char*) SMStxt);
  GSM.DeleteAllSMS(); // Удалить СМСки (накапливать не будем!)
  strSMS = String(SMStxt);
  if(strSMS == "1")  {
   while(GSM.SendSMS(RemoteID9,HumadityVal) < 0);   
   while(GSM.SendSMS(RemoteID9,TemperatureVal) < 0);
 }
  GSM.WaitCall();
  GSM.WaitSMS();
}


}
