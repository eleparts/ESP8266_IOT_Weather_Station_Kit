#include <ESP8266WiFi.h>

#include <Adafruit_ssd1306syp.h>
#define SDA_PIN 14//oled_SDA
#define SCL_PIN 12//oled_SCL
   
#define relay  4
#define relay1 5 

//제조사에서 적용한 와이파이이기에, 사용자의 환경에서 사용 가능한 와이파이와 비밀번호로 설정해야합니다.
const char *ssid     = "Elecrow803";
const char *password = "elecrow2014";

//입력부 코딩 동작에서 출력된 IP address를 사용합니다
const char *host = "192.168.1.184";
WiFiClient client;
//Amended as you build the Server Server port number
const int tcpPort = 8266;

Adafruit_ssd1306syp display(SDA_PIN, SCL_PIN);
   
void setup(){
  
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

 //WiFi 연결을 시작합니다.
  WiFi.begin(ssid, password);

  //WiFi.status() 함수는 WiFi 연결 상태를 확인합니다.
  while (WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());//WiFi. LocalIP () 함수는 8266개의 IP 주소를 반환합니다.

  display.initialize();//Oled 초기화

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Hello,Welcome to Elecrow");
  display.update();
  display.clear();
}
   
   
void loop(){
    // 클라이언트가 연결되어 있는지 여부
    while (!client.connected()){
        if (!client.connect(host, tcpPort)){
            Serial.println("connection....");
            delay(500);
        }
    }
    
    //직렬 포트에 무선으로 데이터에 대한 무선 액세스로 전달됩니다.
while (client.available()){
      uint8_t c = client.read();
      Serial.write(c); 
      
      display.setCursor(0, 51);
      display.print("uv:");//Oled display text


      display.setCursor(0, 0);
      display.print("rain:");//Oled display text
      
      display.setCursor(50,0);
      switch(c){
        case '0': display.print("yes"); break;
        case '1': display.print("no"); break;
      }



      display.setCursor(0, 17);
      display.print("humi:");//Oled display text
      
      display.setCursor(50,17);
      switch(c){
        case 'A': display.print("0"); break;
        case 'B': display.print("1"); break;
        case 'C': display.print("2"); break;
        case 'D': display.print("3"); break;
        case 'E': display.print("4"); break;
        case 'F': display.print("5"); break;
        case 'G': display.print("6"); break;
        case 'H': display.print("7"); break;
        case 'I': display.print("8"); break;
        case 'J': display.print("9"); break;
      }
      display.setCursor(57,17);
      switch(c){
        case 'a': display.print("0"); break;
        case 'b': display.print("1"); break;
        case 'c': display.print("2"); break;
        case 'd': display.print("3"); break;
        case 'e': display.print("4"); break;
        case 'f': display.print("5"); break;
        case 'g': display.print("6"); break;
        case 'h': display.print("7"); break;
        case 'i': display.print("8"); break;
        case 'j': display.print("9"); break;
      }
      
      display.setCursor(64, 17);
      display.print("%");//Oled display text  



      display.setCursor(0, 34);
      display.print("temp:");//Oled display text
      
      display.setCursor(50,34);
      switch(c){
        case 'K': display.print("0"); break;
        case 'L': display.print("1"); break;
        case 'M': display.print("2"); break;
        case 'N': display.print("3"); break;
        case 'O': display.print("4"); break;
        case 'P': display.print("5"); break;
        case 'Q': display.print("6"); break;
        case 'R': display.print("7"); break;
        case 'S': display.print("8"); break;
        case 'T': display.print("9"); break;
      }
      display.setCursor(57,34);
      switch(c){
        case 'k': display.print("0"); break;
        case 'l': display.print("1"); break;
        case 'm': display.print("2"); break;
        case 'n': display.print("3"); break;
        case 'o': display.print("4"); break;
        case 'p': display.print("5"); break;
        case 'q': display.print("6"); break;
        case 'r': display.print("7"); break;
        case 's': display.print("8"); break;
        case 't': display.print("9"); break;
      }

      display.setCursor(64,34);
      display.print("C");//Oled display text 

      display.setCursor(50,51);
      switch(c){
        case '2': display.print("strong"); break;
        case '3': display.print("weak"); break;
      }
       
      display.update();
      if(c=='Z') display.clear();     
    }
   
}
   
