//*******************************libraries********************************
// RFID-----------------------------
#include <SPI.h>
#include <MFRC522.h>
// NodeMCU--------------------------
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

//************************************************************************
#define SS_PIN D8  // D2
#define RST_PIN D3 // D1
//************************************************************************
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
//************************************************************************
/* Set these to your desired credentials. */
const char *ssid = "petrusos24";
const char *password = "00423229845";
const char *device_token = "816a6d3de4b88ed2";
//************************************************************************
String URL = "http://192.168.0.10/getdata.php"; // computer IP or the server domain
String getData, Link;
String OldCardID = "";
unsigned long previousMillis = 0;
//************************************************************************
void setup()
{
    pinMode(D0, OUTPUT);
    delay(1000);
    lcd.init();
    Printing("SAGA II", "Version 0.8", 4000);
    Serial.begin(115200);
    SPI.begin();        // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card
    //---------------------------------------------
    connectToWiFi();
}
//************************************************************************
void loop()
{
    // check if there's a connection to Wi-Fi or not

    if (!WiFi.isConnected())
    {
        connectToWiFi(); // Retry to connect to Wi-Fi
    }

    //---------------------------------------------
    if (millis() - previousMillis >= 15000)
    {
        previousMillis = millis();
        OldCardID = "";
    }
    delay(50);
    //---------------------------------------------
    // look for new card
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return; // got to start of loop if there is no card present
    }
    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return; // if read card serial(0) returns 1, the uid struct contians the ID of the read card.
    }
    String CardID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        CardID += mfrc522.uid.uidByte[i];
    }
    //---------------------------------------------
    if (CardID == OldCardID)
    {
        return;
    }
    else
    {
        OldCardID = CardID;
    }
    //---------------------------------------------
    //  Serial.println(CardID);
    SendCardID(CardID);
    delay(1000);
}
//************send the Card UID to the website*************
void SendCardID(String Card_uid)
{
    Serial.println("Sending the Card ID");

    if (WiFi.isConnected())
    {
        HTTPClient http; // Declare object of class HTTPClient
        // GET Data
        getData = "?card_uid=" + String(Card_uid) + "&device_token=" + String(device_token); // Add the Card ID to the GET array in order to send it
        // GET methode
        Link = URL + getData;
        http.begin(Link); // initiate HTTP request   //Specify content-type header

        int httpCode = http.GET();         // Send the request
        String payload = http.getString(); // Get the response payload

        //    Serial.println(Link);   //Print HTTP return code
        Serial.println(httpCode); // Print HTTP return code
        Serial.println(Card_uid); // Print Card ID
        Serial.println(payload);  // Print request response payload
        payload.trim();
        Serial.println(":" + payload + ":");
        Serial.println(payload.substring(0, 5));
        if (httpCode == 200)
        {
            if (payload.substring(0, 5) == "login")
            {
                String user_name = payload.substring(5);
                Serial.println(user_name);

                Serial.println("Bienvenido");
                Serial.println(user_name);
                Printing("Bienvenido", user_name, 4000);
            }
            else if (payload.substring(0, 6) == "logout")
            {
                String user_name = payload.substring(6);
                Serial.println(user_name);
                Serial.println("Adios");
                Serial.println(user_name);
                Printing("Adios", user_name, 4000);
            }
            else if (payload == "succesful")
            {
            }
            else if (payload == "available")
            {
            }
            else if (payload == "Not found!")
            {
                Printing("No habilitada", "Acceso denegado", 4000);
            }

            else if (payload == "Not registerd!")
            {
                Printing("No registrado", "Acceso denegado", 4000);
            }

            else if (payload.substring(0, 8) == "accessin")
            {
                Printing("Acceso permitido", "", 2000);
                digitalWrite(D0, HIGH);
                delay(4000);
                digitalWrite(D0, LOW);
            }

            else if (payload.substring(0, 9) == "accessout")
            {
                Printing("Acceso permitido", "", 000);
                digitalWrite(D0, HIGH);
                delay(4000);
                digitalWrite(D0, LOW);
            }

            delay(100);
            http.end(); // Close connection
        }
    }
}
//********************connect to the WiFi******************
void connectToWiFi()
{
    Printing("Conectando WiFi", "...", 4000);
    WiFi.mode(WIFI_OFF); // Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connected");
    Printing("Listo", "", 4000);

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP()); // IP address assigned to your ESP

    delay(3000);
}

void Printing(String primera, String segunda, int delaytime)
{
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print(primera);
    lcd.setCursor(0, 1);
    lcd.print(segunda);
    delay(delaytime);
    lcd.noBacklight();
}