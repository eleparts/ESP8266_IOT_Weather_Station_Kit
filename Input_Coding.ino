#include <ESP8266WiFi.h>
#define MAX_SRV_CLIENTS 3   //Maximum number of connection at the same time, the number of devices you want to access, 8266 tcpserver access only five

//제조사에서 적용한 와이파이이기에, 사용자의 환경에서 사용 가능한 와이파이와 비밀번호로 설정해야합니다.
const char *ssid = "raspberry_net5"; 
const char *password = "elezzang75969116";
 
WiFiServer server(8266);//Your port, modify, the range of 0-65535
WiFiClient serverClients[MAX_SRV_CLIENTS];
WiFiClient client;

//핀 번호 및 변수 지정
#define uv_PIN A0
#define water_PIN 4
#define tem_PIN 5
#define board_LED 16

int light_sensor;
int temp_print,temp_print_1,temp_print_10;
int humi_print,humi_print_1,humi_print_10;

void setup(){
  Serial.begin(115200);
  delay(10);
  pinMode(board_LED, OUTPUT);
  digitalWrite(board_LED, 0);
  //wifi 연결 및 연결 확인과 IP 주소 출력
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());//WiFi. LocalIP () return 8266 IP addresses

  server.begin();
  server.setNoDelay(true);  //Combined with normal only after some

  pinMode(water_PIN, INPUT);
  pinMode(uv_PIN, INPUT);
}
 
void loop(){
  int temp;
  int humi;
  unsigned int loopCnt;
  // 40비트를 저장하는 데 사용되는 숫자 배열을 생성합니다.
  int chr[40] = {0};
  unsigned long time1;
  bgn:
  delay(2000);
  //출력 로우 레벨 20ms
  //A 40 mu s output level
  pinMode(tem_PIN,OUTPUT);
  digitalWrite(tem_PIN,LOW);
  delay(20);
  digitalWrite(tem_PIN,HIGH);
  delayMicroseconds(40);
  digitalWrite(tem_PIN,LOW);
  pinMode(tem_PIN,INPUT);

  //HIGH 레벨 응답 신호
  loopCnt=10000;
  while(digitalRead(tem_PIN) != HIGH){
    if(loopCnt-- == 0){
      //오랜 시간 동안 HIGH 레벨로 돌아가지 않으면 힌트를 출력하고 다시 시작합니다.
      Serial.println("HIGH");
      goto bgn;
    }
  }

  //Low 레벨 응답 신호
  loopCnt=30000;
  while(digitalRead(tem_PIN) != LOW){
    if(loopCnt-- == 0){
      //오랜 시간 동안 LOW 레벨로 돌아가지 않으면 힌트를 출력하고 다시 시작합니다.
      Serial.println("LOW");
      goto bgn;
    }
  }
  //비트 1부터 40까지, 총 40번 읽기 시작합니다.
  for(int i=0;i<40;i++){
    while(digitalRead(tem_PIN) == LOW){
    }
    //평상시 전기가 높을 때는 시간을 “시간”으로 기록합니다.
    time1 = micros();
    while(digitalRead(tem_PIN) == HIGH){
    }
    //낮을 때는 방금 저장한 시간을 뺀 시간을 기록합니다.
    //50초 이상의 값이 나온다면, '1' 이나 '0'으로 결론을 내리며, 배열에 저장합니다.
    if(micros() - time1 > 50){
      chr[i]=1;
    }else{
      chr[i]=0;
    }
  }
      
  //습도를 8비트에서 숫자 값으로 변환합니다.
  humi=chr[0]*128+chr[1]*64+chr[2]*32+chr[3]*16+chr[4]*8+chr[5]*4+chr[6]*2+chr[7];
      
  //온도를 8비트에서 숫자 값으로 변환합니다.
  temp=chr[16]*128+chr[17]*64+chr[18]*32+chr[19]*16+chr[20]*8+chr[21]*4+chr[22]*2+chr[23];
  
  Serial.print("temp:");
  Serial.println(temp);
  temp_print=temp;
  Serial.print("humi:");
  Serial.println(humi);
  humi_print=humi;
  
  static long previousMillis = 0;
  static int currstate = 0;
  
  //200ms
  if (millis() - previousMillis > 200){
    previousMillis = millis();
    currstate = 1 - currstate;
    digitalWrite(board_LED, currstate);

    //아날로그 A0핀에서 광센서 값 받아오기
    light_sensor=analogRead(A0);
    Serial.println(light_sensor);
  }
   
  uint8_t i;
  if (server.hasClient()){
    for (i = 0; i < MAX_SRV_CLIENTS; i++){
      Serial.println(MAX_SRV_CLIENTS);
      if (!serverClients[i] || !serverClients[i].connected()){
        if (serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        continue;
      }
    }
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  for (i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      digitalWrite(board_LED, 0);
      if (serverClients[i].available()){
        while (serverClients[i].available()) Serial.write(serverClients[i].read());
      }
    }
  }

  //통신하여 출력부 코드로 넘길 buff[] 배열
  size_t s=8;
  uint8_t buff[s];
  
  //워터 센서가 감지될 때, '1' 값 입력
  if(digitalRead(water_PIN)==HIGH){
    buff[1]='1';
  }else{
    buff[1]='0';
  }
  
  if(light_sensor>450){
    buff[2]='2';
  }else{
    buff[2]='3';
  }

        
  humi_print_10=humi_print/10;
  switch(humi_print_10){
    case 0: buff[3]='A'; break;
    case 1: buff[3]='B'; break;
    case 2: buff[3]='C'; break;
    case 3: buff[3]='D'; break;
    case 4: buff[3]='E'; break;
    case 5: buff[3]='F'; break;
    case 6: buff[3]='G'; break;
    case 7: buff[3]='H'; break;
    case 8: buff[3]='I'; break;
    case 9: buff[3]='J'; break;
  }
       
  humi_print_1=humi_print%10;
  switch(humi_print_1){
    case 0: buff[4]='a'; break;
    case 1: buff[4]='b'; break;
    case 2: buff[4]='c'; break;
    case 3: buff[4]='d'; break;
    case 4: buff[4]='e'; break;
    case 5: buff[4]='f'; break;
    case 6: buff[4]='g'; break;
    case 7: buff[4]='h'; break;
    case 8: buff[4]='i'; break;
    case 9: buff[4]='j'; break;
  }
  
  temp_print_10=temp_print/10;
  switch(temp_print_10){
    case 0: buff[5]='K'; break;
    case 1: buff[5]='L'; break;
    case 2: buff[5]='M'; break;
    case 3: buff[5]='N'; break;
    case 4: buff[5]='O'; break;
    case 5: buff[5]='P'; break;
    case 6: buff[5]='Q'; break;
    case 7: buff[5]='R'; break;
    case 8: buff[5]='S'; break;
    case 9: buff[5]='T'; break;
  }
  
  temp_print_1=temp_print%10;
  switch(temp_print_10){
    case 0: buff[6]='k'; break;
    case 1: buff[6]='l'; break;
    case 2: buff[6]='m'; break;
    case 3: buff[6]='n'; break;
    case 4: buff[6]='o'; break;
    case 5: buff[6]='p'; break;
    case 6: buff[6]='q'; break;
    case 7: buff[6]='r'; break;
    case 8: buff[6]='s'; break;
    case 9: buff[6]='t'; break;
  }
    buff[7]='Z';    


        
  //연결된 모든 텔넷 클라이언트에 UART 데이터를 푸시합니다.
  for (i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      serverClients[i].write(buff,s);
      }
  }
  
  void uv();
}
  
void uv(){
  int uvLevel = analogRead(uv_PIN);
  int refLevel=1023;
  //센서에서 매우 정확한 출력값을 얻기 위해 3.3V 전원 핀을 레퍼런스로 사용합니다.
  int outputVoltage = 3.3 / refLevel * uvLevel;
  int v;
  v=outputVoltage;
}
