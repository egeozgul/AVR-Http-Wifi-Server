 const int maxNumberOfClients = 10;

int SIZE = 3899;
char *htmlCode = "<!DOCTYPE html><html lang=\"en\" dir=\"ltr\"><head><meta charset=\"utf-8\"><title>Clock Settings</title><meta name=\"author\" content=\"Maker Club\"><meta name=\"description\" content=\"Welcome to the settings page of the Maker Club clock.\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\" /> <script type=\"text/javascript\">function startTime(){var today=new Date();var h=today.getHours();var m=today.getMinutes();var s=today.getSeconds();var c=\":\";if((s%2)==0){c=\":\";}else{c=\" \";} m=checkTime(m);document.getElementById('time').innerHTML=h+c+m;var t=setTimeout(startTime,500);} function checkTime(i){if(i<10){i=\"0\"+i};return i;} function openTab(evt,tabName){var i,tabcontent,tablinks;tabcontent=document.getElementsByClassName(\"tabcontent\");for(i=0;i<tabcontent.length;i++){tabcontent[i].style.display=\"none\";} tablinks=document.getElementsByClassName(\"tablinks\");for(i=0;i<tablinks.length;i++){tablinks[i].className=tablinks[i].className.replace(\" active\",\"\");} document.getElementById(tabName).style.display=\"block\";evt.currentTarget.className+=\" active\";}</script> <style>input[type=time],select{width:100%;padding:12px 20px;margin:8px 0;display:inline-block;border:1px solid #ccc;border-radius:4px;box-sizing:border-box}.button{background-color:#f44336;border:none;color:white;padding:15px 32px;text-align:center;text-decoration:none;display:inline-block;font-size:16px}.tab{overflow:hidden;border:1px solid #ccc;background-color:#f1f1f1}.tab button{background-color:inherit;float:left;border:none;outline:none;cursor:pointer;padding:14px 16px;transition:0.3s;font-size:17px}.tab button:hover{background-color:#ddd}.tab button.active{background-color:#ccc}.tabcontent{display:none;padding:6px 12px;border:1px solid #ccc;border-top:none}.slider{-webkit-appearance:none;width:80%;margin:auto;height:15px;background:#d3d3d3;outline:none;opacity:0.6;-webkit-transition: .4s;transition:opacity .4s}.slider:hover{opacity:1}.slider:active{opacity:1}.slider::-webkit-slider-thumb{-webkit-appearance:none;appearance:none;width:15px;height:15px;cursor:pointer}.red::-webkit-slider-thumb{background:#f00}.green::-webkit-slider-thumb{background:#0f0}.blue::-webkit-slider-thumb{background:#00f}.bright::-webkit-slider-thumb{background:#000}.slider::-moz-range-thumb{width:25px;height:25px;background:#434951;cursor:pointer}</style></head><body><body onload=\"startTime()\"><div id=\"time\"></div><div class=\"tab\"> <button class=\"tablinks\" onclick=\"openTab(event, 'Alarm')\">Alarm</button> <button class=\"tablinks\" onclick=\"openTab(event, 'LED')\">LED</button> <button class=\"tablinks\" onclick=\"openTab(event, 'RGB')\">RGB</button> <button class=\"tablinks\" onclick=\"openTab(event, 'Brightness')\">Brightness</button></div><div id=\"Alarm\" class=\"tabcontent\"><form class=\"buton1\" method=\"get\" id=\"alarmkur\"> <input type=\"time\" name=\"alarm\" placeholder=\"06:30\" min=\"00:00\" max=\"24:00\" required pattern=\"[0-9]{2}:[0-9]{2}\"></form> <button class=\"button\" type=\"submit\" form=\"alarmkur\" value=\"Submit\">AlarmÄ± Kur</button></div><div id=\"LED\" class=\"tabcontent\"><form class=\"lamp\" method=\"get\"> <input class=\"button\" type=\"submit\" name=\"Submit\" value=\"Lamp ON\"></form></div><div id=\"Brightness\" class=\"tabcontent\"><form class=\"brightness\" method=\"get\" id=\"brightForm\"> <input type=\"range\" name=\"red\" min=\"0\" max=\"100\" value=\"100\" class=\"slider bright\"></form> <button class=\"button\" type=\"submit\" form=\"brightForm\" value=\"Submit\">Set Brightness</button></div><div id=\"RGB\" class=\"tabcontent\"><form class=\"RGB\" method=\"get\" id=\"rgbform\"> <input type=\"range\" name=\"red\" min=\"0\" max=\"255\" value=\"255\" class=\"slider red\"> <input type=\"range\" name=\"green\" min=\"0\" max=\"255\" value=\"255\" class=\"slider green\"> <input type=\"range\" name=\"blue\" min=\"0\" max=\"255\" value=\"255\" class=\"slider blue\"></form> <button class=\"button\" type=\"submit\" form=\"rgbform\" value=\"Submit\">Set Color</button></div></body></html> ";

void getReply3(int wait)
{
  long int time = millis();
  while ( (time + wait) > millis())
  {
    while (Serial3.available())
      Serial.write(Serial3.read());
  }
}

void loadPacket(char *packet, int len , int clientId)
{
  String packetHeader = "AT+CIPSEND=" + String(clientId, DEC) + ',' + String(len, DEC) + "\r\n";

  delay(300);
  Serial3.print(packetHeader);

  String d = "";
  do
  {
    if (Serial3.available())
    {
      d = Serial3.readString();
    }
  }
  while (d.indexOf(">") < 0);
  delay(100);

  Serial.println("SENT:" + packetHeader);

  //Serial3.print("\r\n");
  Serial3.print( packet );
  //Serial3.print("\r\n");

  d = "";
  do
  {
    if (Serial3.available())
    {
      d = Serial3.readString();
    }
  }
  while (d.indexOf("SEND") < 0);
  delay(100);


  Serial.print("SENT:");
  Serial.print(packet);

}

void loadHTML(int clientId)
{

  int packageSize = 800;

  int nPackage = ((int)(SIZE / packageSize) + 1);

  String headerText = "HTTP/1.1 200 OK\nContent-Length: " + String(SIZE , DEC) + "\nContent-Type: text/html\nConnection: Closed";
  loadPacket(headerText.c_str(), headerText.length(), clientId);

  String part = "";
  int from = 0;
  int to = from + packageSize;

  for (int i = 0; i < nPackage ; i++)
  {
    Serial.print("sending body packet ");
    Serial.print(i);
    Serial.print(" ...");

    getReply3(3000);

    from = i * packageSize;
    to = from + packageSize;

    if (to > SIZE)
      to = SIZE;

    part = "";
    for (int k = from; k < to; k++)
      part += htmlCode[k];

    loadPacket(part.c_str(), part.length(), clientId);
    Serial.print("DONE SNEDING DATA");
    Serial.println(i);
  }

  Serial.println("body is loaded");
}



void breakConnection(int clientId)
{
  String command = "AT+CIPCLOSE=" + String(clientId, DEC) + "\r\n";
  Serial3.print(command);
  Serial.print("SENT:" + command);
}

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  Serial3.begin(115200);
  Serial.begin(115200);
  while (!Serial);
  while (!Serial3);

  Serial.println("starting...");

  Serial3.print("AT+RST\r\n");
  getReply3(2000);


  Serial3.print("AT+CWMODE=3\r\n");
  getReply3(2000);

  Serial3.print("AT+CIPMUX=1\r\n");
  getReply3(2000);
  
  Serial3.print("AT+CIPSERVER=1,80\r\n");
  getReply3(2000);
  
  Serial.println("*****READY*****");

  digitalWrite(13, HIGH);
}


class dynamicStack
{
  public:
    int clientId[maxNumberOfClients];
    int topPointer = -1;

    int linearSearch(int data)
    {
      for (int i = 0; i <= topPointer ; i++)
      {
        if (clientId[i] == data)
          return i;
      }
      return -1;
    }

    bool isFull()
    {
      return (topPointer == maxNumberOfClients - 1);
    }

    bool isEmpty()
    {
      (topPointer == -1);
    }

    bool push(int data)
    {
      topPointer++;
      if (topPointer >= maxNumberOfClients || topPointer < 0)
        return false;

      clientId[topPointer] = data;
      return true;
    }

    int pop()
    {
      topPointer--;
      if (topPointer < -1)
        topPointer = -1;
      return clientId[topPointer + 1];
    }
};

dynamicStack clientIdStack;
String data = "";
void loop()
{
  


  //********FETCHING MESSAGE***********
START:

    Serial3.print("AT+GMR\r\n");
  getReply3(2000);

  goto START;
  while (!Serial3.available());

  data = Serial3.readString();

  if (data.length() <= 0)
    goto START;

  Serial.println("RECEIVED:");
  Serial.println( "length: " + String(data.length(), DEC));
  Serial.print("##");
  Serial.print(data);
  Serial.println("##");

  //********FETCHING MESSAGE END***********

  int index =  data.indexOf("CONNECT");
  if (index >= 0)
  {
    Serial.println( "found CONNECT keyword at index ");
    Serial.print(index);

    int newClientId = (data[index - 2]) - '0';
    clientIdStack.push(newClientId);

    Serial.print("NEW CLIENT IS CONNECTED with client ID ");
    Serial.println(newClientId);
  }

  index = data.indexOf("+IPD");
  if (index >= 0)
  {
    Serial.print( "found +IPD keyword at index ");
    Serial.println(index);

    int clientId = data[index + 5] - '0';

    Serial.print("GET request is made by client ");
    Serial.println(clientId);

    Serial.println("*****LOADING HTML PAGE******");
    loadHTML(clientId);
    getReply3(2000);
    Serial.println("*****HTML PAGE IS LOADED******");

    breakConnection(clientId);
    clientIdStack.pop();
  }

}

