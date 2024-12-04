#include "header/displayLCDHandler.h"
#include <ConfiguratorMKR.h>

extern ConfiguratorMKR configurator;

byte temp_icon[8] = // icon for termometer
    {
        B00100,
        B01010,
        B01010,
        B01110,
        B01110,
        B11111,
        B11111,
        B01110};

byte hum_icon[8] = // icon for water droplet
    {
        B00100,
        B00100,
        B01010,
        B01010,
        B10001,
        B10001,
        B10001,
        B01110,
};

byte connection_icon[] = {
    B00001,
    B00001,
    B00001,
    B00101,
    B00101,
    B00101,
    B10101,
    B10101};

displayLCDHandler::displayLCDHandler() : lcd(0x27, 20, 4)
{
}

displayLCDHandler::~displayLCDHandler()
{
}

void displayLCDHandler::displayWifiConnecting()
{
    this->lcd.clear();
    this->lcd.setCursor(3, 1);
    this->lcd.print("Connecting");
    this->lcd.setCursor(3, 2);
    this->lcd.print("Please wait...");
    delay(2000);
}


void displayLCDHandler::displayWifiConnected()
{
    this->lcd.clear();
    this->lcd.setCursor(3, 1);
    this->lcd.print("WiFi Connected");
    delay(2000);
}

void displayLCDHandler::displayWifiManagerStarted(String AP_Name, int timeout)
{
    this->lcd.clear();
    this->lcd.setCursor(2, 0);
    this->lcd.print("Please connect to");
    this->lcd.setCursor(5, 1);
    this->lcd.print(AP_Name);
    this->lcd.setCursor(2, 2);
    this->lcd.print("to configure WiFi");
    this->lcd.setCursor(3, 3);
    this->lcd.print("Timeout in " + String(timeout) + "s");
}

void displayLCDHandler::reinitLCD()
{
    clock = ",,,,,";
    date = ",,,,,,,,,,";
    suhu = ",,,,,,,";
    kelembaban = ",,,,,,,";
    tipe = ",,,,,";
    statusPendjadwalan = ",";
    waktuPendjadwalanTerdekat = ",,,,,";
    lahan1 = ",";
    lahan2 = ",";
    lahan3 = ",";
    lahan4 = ",";
    connectionStatus = ",";

    this->lcd.init();
    this->lcd.backlight();
    this->lcd.clear();
    this->lcd.createChar(1, temp_icon);
    this->lcd.createChar(2, hum_icon);
    this->lcd.createChar(3, connection_icon);

    // this->lcd.clear();
    this->lcd.setCursor(17, 0);
    this->lcd.write(3);
    this->lcd.print(":");

    this->lcd.setCursor(0, 1);
    this->lcd.write(1);
    this->lcd.print(":");

    this->lcd.setCursor(9, 1);
    this->lcd.write(2);
    this->lcd.print(":");

    this->lcd.setCursor(0, 2);
    this->lcd.print("T:");
    this->lcd.setCursor(8, 2);
    this->lcd.print("P:");
    this->lcd.setCursor(12, 2);
    this->lcd.print("W:");
    this->lcd.setCursor(0, 3);
    this->lcd.print("L1:");
    this->lcd.setCursor(5, 3);
    this->lcd.print("L2:");
    this->lcd.setCursor(10, 3);
    this->lcd.print("L3:");
    this->lcd.setCursor(15, 3);
    this->lcd.print("L4:");
}

void displayLCDHandler::displayWifiNotConnected()
{
    this->lcd.clear();
    this->lcd.setCursor(1, 1);
    this->lcd.print("WiFi Not Connected");
    delay(2000);
}

void displayLCDHandler::displayContinueWithoutWiFi()
{
    this->lcd.clear();
    this->lcd.setCursor(2, 1);
    this->lcd.print("Continue without");
    this->lcd.setCursor(7, 2);
    this->lcd.print("WiFi...");
    delay(2000);
}

void displayLCDHandler::initDisplayInformation()
{
    this->lcd.clear();
    this->lcd.setCursor(0, 0);
    delay(DELAY_LCD_UPDATE);
    this->lcd.print("00/00/0000 00:00");
    this->lcd.setCursor(17, 0);
    delay(DELAY_LCD_UPDATE);
    this->lcd.write(3);
    this->lcd.print(":");

    this->lcd.setCursor(0, 1);
    delay(DELAY_LCD_UPDATE);
    this->lcd.write(1);
    this->lcd.print(":");
    this->lcd.setCursor(9, 1);
    delay(DELAY_LCD_UPDATE);
    this->lcd.write(2);
    this->lcd.print(":");

    this->lcd.setCursor(0, 2);
    delay(DELAY_LCD_UPDATE);
    this->lcd.print("T:");
    this->lcd.setCursor(8, 2);
    delay(DELAY_LCD_UPDATE);
    this->lcd.print("P:");
    this->lcd.setCursor(12, 2);
    delay(DELAY_LCD_UPDATE);
    this->lcd.print("W:");
    this->lcd.setCursor(0, 3);
    delay(DELAY_LCD_UPDATE);
    this->lcd.print("L1:");
    this->lcd.setCursor(5, 3);
    delay(DELAY_LCD_UPDATE);
    this->lcd.print("L2:");
    this->lcd.setCursor(10, 3);
    delay(DELAY_LCD_UPDATE);
    this->lcd.print("L3:");
    this->lcd.setCursor(15, 3);
    delay(DELAY_LCD_UPDATE);
    this->lcd.print("L4:");
}

void displayLCDHandler::initLCD()
{

    this->lcd.init();
    this->lcd.backlight();
    this->lcd.clear();
    this->lcd.createChar(1, temp_icon);
    this->lcd.createChar(2, hum_icon);
    this->lcd.createChar(3, connection_icon);

    this->lcd.setCursor(2, 1);
    // this->lcd.print("BITANIC PRO BT12");
    this->lcd.print((String)"NUTRIFERADS " + (String)configurator.getVariable("id"));

    delay(3000);
}

void displayLCDHandler::displayUpdating()
{
    this->lcd.clear();
    this->lcd.setCursor(3, 0);
    this->lcd.print("Working on");
    this->lcd.setCursor(3, 1);
    this->lcd.print("Updates...");
}

void displayLCDHandler::showUpdateStatus(bool state)
{
    this->lcd.clear();
    this->lcd.setCursor(1, 0);
    if (state)
    {
        this->lcd.print("Update Success");
        this->lcd.setCursor(1, 1);
        this->lcd.print("Restarting...");
    }
    else
    {
        this->lcd.print("Update Failed");
    }
    delay(2000);
}


void displayLCDHandler::displayConnectionStatus(String status)
{
    int start_col = 19;
    int start_row = 0;
    for (int i = 0; i < status.length(); i++)
    {
        if (status[i] != this->connectionStatus[i])
        {
            if (isInit)
            {
                delay(DELAY_LCD_UPDATE);
            }
            this->lcd.setCursor(start_col + i, start_row);
            this->connectionStatus[i] = status[i];
            this->lcd.print(this->connectionStatus[i]);
        }
    }
}

void displayLCDHandler::updateSecondsTick()
{
    if (this->stateSecondsTick)
    {
        this->lcd.setCursor(this->date.length() + 3, 0);
        this->lcd.print(" ");
        this->stateSecondsTick = false;
    }
    else
    {
        this->lcd.setCursor(this->date.length() + 3, 0);
        this->lcd.print(":");
        this->stateSecondsTick = true;
    }
}

void displayLCDHandler::displayWaktuPendjadwalanTerdekat(String waktu)
{
    int start_col = 14;
    int start_row = 2;
    for (int i = 0; i < waktu.length(); i++)
    {
        if (waktu[i] != this->waktuPendjadwalanTerdekat[i])
        {
            if (isInit)
            {
                delay(DELAY_LCD_UPDATE);
            }
            this->lcd.setCursor(start_col + i, start_row);
            this->waktuPendjadwalanTerdekat[i] = waktu[i];
            this->lcd.print(this->waktuPendjadwalanTerdekat[i]);
        }
    }
}

void displayLCDHandler::displayStatusPendjadwalan(String status)
{
    int start_col = 10;
    int start_row = 2;
    for (int i = 0; i < status.length(); i++)
    {
        if (status[i] != this->statusPendjadwalan[i])
        {
            if (isInit)
            {
                delay(DELAY_LCD_UPDATE);
            }
            this->lcd.setCursor(start_col + i, start_row);
            this->statusPendjadwalan[i] = status[i];
            this->lcd.print(this->statusPendjadwalan[i]);
        }
    }
}

void displayLCDHandler::displayLahan4(String status)
{
    int start_col = 18;
    int start_row = 3;
    for (int i = 0; i < status.length(); i++)
    {
        if (status[i] != this->lahan4[i])
        {
            if (isInit)
            {
                delay(DELAY_LCD_UPDATE);
            }
            this->lcd.setCursor(start_col + i, start_row);
            this->lahan4[i] = status[i];
            this->lcd.print(this->lahan4[i]);
        }
    }
}

void displayLCDHandler::displayLahan3(String status)
{
    int start_col = 13;
    int start_row = 3;
    for (int i = 0; i < status.length(); i++)
    {
        if (status[i] != this->lahan3[i])
        {
            if (isInit)
            {
                delay(DELAY_LCD_UPDATE);
            }
            this->lcd.setCursor(start_col + i, start_row);
            this->lahan3[i] = status[i];
            this->lcd.print(this->lahan3[i]);
        }
    }
}

void displayLCDHandler::displayLahan2(String status)
{
    int start_col = 8;
    int start_row = 3;
    for (int i = 0; i < status.length(); i++)
    {
        if (status[i] != this->lahan2[i])
        {
            if (isInit)
            {
                delay(DELAY_LCD_UPDATE);
            }
            this->lcd.setCursor(start_col + i, start_row);
            this->lahan2[i] = status[i];
            this->lcd.print(this->lahan2[i]);
        }
    }
}

void displayLCDHandler::displayLahan1(String status)
{
    int start_col = 3;
    int start_row = 3;
    for (int i = 0; i < status.length(); i++)
    {
        if (status[i] != this->lahan1[i])
        {
            if (isInit)
            {
                delay(DELAY_LCD_UPDATE);
            }
            this->lcd.setCursor(start_col + i, start_row);
            this->lahan1[i] = status[i];
            this->lcd.print(this->lahan1[i]);
        }
    }
}

void displayLCDHandler::displayTipe(String tipe)
{

    int start_col = 2;
    int start_row = 2;
    for (int i = 0; i < tipe.length(); i++)
    {
        if (tipe[i] != this->tipe[i])
        {
            if (isInit)
            {
                delay(DELAY_LCD_UPDATE);
            }
            this->lcd.setCursor(start_col + i, start_row);
            this->tipe[i] = tipe[i];
            this->lcd.print(this->tipe[i]);
        }
    }
}

void displayLCDHandler::displayKelembaban(String kelembaban)
{
    if (kelembaban.toFloat() < 10)
    {
        kelembaban = kelembaban + "% ";
    }
    else
    {
        kelembaban = kelembaban + "%";
    }
    int start_col = 11;
    int start_row = 1;
    for (int i = 0; i < kelembaban.length(); i++)
    {
        if (kelembaban[i] != this->kelembaban[i])
        {
            if (isInit)
            {
                delay(DELAY_LCD_UPDATE);
            }
            this->lcd.setCursor(start_col + i, start_row);
            this->kelembaban[i] = kelembaban[i];
            this->lcd.print(this->kelembaban[i]);
        }
    }
}

void displayLCDHandler::setInit(bool isInit)
{
    this->isInit = isInit;
}

void displayLCDHandler::displaySuhu(String suhu)
{
    suhu = suhu + "\xDF";
    if (suhu.toFloat() < 10)
    {
        suhu = suhu + "C ";
    }
    else
    {
        suhu = suhu + "C";
    }

    int start_col = 2;
    int start_row = 1;
    for (int i = 0; i < suhu.length(); i++)
    {
        if (suhu[i] != this->suhu[i])
        {
            if (isInit)
            {
                delay(DELAY_LCD_UPDATE);
            }
            delay(DELAY_LCD_UPDATE);
            this->lcd.setCursor(start_col + i, start_row);
            this->suhu[i] = suhu[i];
            this->lcd.print(this->suhu[i]);
        }
    }
}

void displayLCDHandler::displayClock(String date, String time)
{
    // Serial.println("Test Point Display Clock 1");
    // Serial.println("Test Point Display Clock 1");
    int start_col = 0;
    int start_row = 0;
    for (int i = 0; i < date.length(); i++)
    {
        if (isInit)
        {
            delay(DELAY_LCD_UPDATE);
        }

        if (date[i] != this->date[i])
        {
            if (isInit)
            {
                delay(DELAY_LCD_UPDATE);
            }
            this->lcd.setCursor(start_col + i, start_row);
            this->date[i] = date[i];
            this->lcd.print(this->date[i]);
        }
    }

    for (int i = 0; i < time.length(); i++)
    {
        if (time[i] != this->clock[i])
        {
            if (isInit)
            {
                delay(DELAY_LCD_UPDATE);
            }
            this->lcd.setCursor(start_col + date.length() + i + 1, start_row);
            this->clock[i] = time[i];
            this->lcd.print(this->clock[i]);
        }
    }
}
