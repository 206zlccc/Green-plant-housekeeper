int a=600;   //补光光强
int b=100;  //报警光强
int c=900;   //补充光强

void setup( ) {
  Serial.begin(115200);           //串口初始化
  analogReference(INTERNAL);  //调用板载1.1V基准源
  pinMode(11, OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  digitalWrite(11, LOW);
}

void loop()
{
    int n = analogRead(A3);   //读取模拟口A3，获取光强
    Serial.println(n);               // 用于IDE串口观察窗
    if (n>=a&&n<=c)                         //对光强进行判断
       { digitalWrite(10,HIGH);
       digitalWrite(9,LOW);}
    else if(n>c)
      { digitalWrite(10,HIGH);
       digitalWrite(9,HIGH);}
    
    else
       { digitalWrite(10,LOW);
       digitalWrite(9,LOW);}
    
    if (n<=b) {   
         
         digitalWrite(11, HIGH);
        
       }
       else
       digitalWrite(11,LOW);
        
    
}
