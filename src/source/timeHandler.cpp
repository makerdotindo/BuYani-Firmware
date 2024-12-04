#include "header/timeHandler.h"

const long utcOffsetInSeconds = 25200;
RTC_DS3231 rtc;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "id.pool.ntp.org", utcOffsetInSeconds);

TimeHandler::TimeHandler()
{
}

TimeHandler::~TimeHandler()
{
}

void TimeHandler::updateTime()
{
    if (timeClient.update())
    {
        time_t epochTime = timeClient.getEpochTime();
        struct tm *ptm = NULL;
        ptm = gmtime((time_t *)&epochTime);
        int monthDay = ptm->tm_mday;
        Serial.print("Month day: ");
        Serial.println(monthDay);

        int currentMonth = ptm->tm_mon + 1;
        Serial.print("Month: ");
        Serial.println(currentMonth);

        int currentYear = ptm->tm_year + 1900;
        Serial.print("Year: ");
        Serial.println(currentYear);

        // Print complete date:
        String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
        Serial.print("Current date: ");
        Serial.println(currentDate);

        // Kalibrasi Waktu RTC dengan NTP
        // rtc.begin();
        // DateTime now = rtc.now();

        // int64_t rtc_current_time = now.unixtime();
        // int64_t ntp_current_time = epochTime;

        // Serial.print("RTC time: " + (String)now.unixtime());
        // Serial.print(" NTP time: " + (String)epochTime);

        if (currentYear != 2036)
        {

            if (rtc.lostPower())
            {
                rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
                Serial.println("RTC lost power, let's set the time!");
            }

            rtc.adjust(DateTime(currentYear, currentMonth, monthDay, timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds()));
            Serial.println("Time synced");
        }
        else
        {
            Serial.println("Time Not Synced, NTP Error");
        }
    }
    else
    {
        Serial.println("Time Not Synced, NTP Not Available");
    }
}

String TimeHandler::getDate()
{
    // DateTime now = rtc.now();
    String day = clock_now.day() < 10 ? "0" + String(clock_now.day()) : String(clock_now.day());
    String month = clock_now.month() < 10 ? "0" + String(clock_now.month()) : String(clock_now.month());
    String year = String(clock_now.year());
    String date = day + "/" + month + "/" + year;
    return date;
}

String TimeHandler::getRestOfWeekFrom(String date)
{
    // 01/01/2021
    // DateTime now = rtc.now();
    DateTime target = DateTime(date.substring(6, 10).toInt(), date.substring(3, 5).toInt(), date.substring(0, 2).toInt(), 0, 0, 0);
    TimeSpan rest;
    if (clock_now > target)
    {
        rest = TimeSpan(0, 0, 0, 0);
    }
    else
    {
        rest = target - clock_now;
    }

    String restString = String(rest.days() / 7);
    return restString;
}

bool TimeHandler::isBefore(String date)
{
    // 01/01/2021
    // DateTime now = rtc.now();
    DateTime target = DateTime(date.substring(6, 10).toInt(), date.substring(3, 5).toInt(), date.substring(0, 2).toInt(), 0, 0, 0);
    if (clock_now <= target)
    {
        return true;
    }
    else
    {
        return false;
    }
}

String TimeHandler::getDateWithCalc(int week)
{
    // DateTime now = rtc.now();
    DateTime now = clock_now;
    now = now + TimeSpan(week * 7, 0, 0, 0);
    String day = now.day() < 10 ? "0" + String(now.day()) : String(now.day());
    String month = now.month() < 10 ? "0" + String(now.month()) : String(now.month());
    String year = String(now.year());
    String date = day + "/" + month + "/" + year;
    return date;
}

void TimeHandler::initRTC()
{
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
    }
    else
    {
        clock_now = rtc.now();
    }
}

void TimeHandler::getSourceTime()
{
    clock_now = rtc.now();
}

void TimeHandler::syncTime()
{

    if (rtc.lostPower())
    {
        Serial.println("RTC lost power, let's set the time!");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }

    timeClient.begin();
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = NULL;

    if (timeClient.update())
    {
        epochTime = timeClient.getEpochTime();
        // Get a time structure
        ptm = gmtime((time_t *)&epochTime);

        // DateTime now = timeClient.getEpochTime();
        // Serial.println(
        //     String(now.year()) + "-" +
        //     String(now.month()) + "-" +
        //     String(now.day()) + " " +
        //     String(now.hour()) + ":" +
        //     String(now.minute()) + ":" +
        //     String(now.second()));
        // rtc.adjust(now);

        // time_t epochTime = timeClient.getEpochTime();
        // Get a time structure
        // ptm = gmtime((time_t *)&epochTime);

        int monthDay = ptm->tm_mday;
        Serial.print("Month day: ");
        Serial.println(monthDay);

        int currentMonth = ptm->tm_mon + 1;
        Serial.print("Month: ");
        Serial.println(currentMonth);

        int currentYear = ptm->tm_year + 1900;
        Serial.print("Year: ");
        Serial.println(currentYear);

        if (currentYear != 2036)
        {
            // Print complete date:
            String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
            Serial.print("Current date: ");
            Serial.println(currentDate);

            // Kalibrasi Waktu RTC dengan NTP
            // rtc.begin();
            // DateTime now = rtc.now();

            // int64_t rtc_current_time = now.unixtime();
            // int64_t ntp_current_time = epochTime;

            // Serial.print("RTC time: " + (String)now.unixtime());
            // Serial.print(" NTP time: " + (String)epochTime);

            if (rtc.lostPower())
            {
                rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
                Serial.println("RTC lost power, let's set the time!");
            }

            rtc.adjust(DateTime(currentYear, currentMonth, monthDay, timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds()));
            clock_now = rtc.now();
            Serial.println("Time synced");
        }
    }
    else
    {
        clock_now = rtc.now();
        Serial.println("Time Not Synced, NTP Not Available");
    }

    // else
    // {
    //     if (!(ntp_current_time - rtc_current_time > 1000) && !(ntp_current_time - rtc_current_time < -1000))
    //     {

    //         Serial.println("Time synced");
    //         /* code */
    //     }
    //     else
    //     {
    //         // Serial.println("Time not synced, NTP not available");
    //         /* code */
    //     }
    // }
}

String TimeHandler::getTime()
{
    // DateTime now = rtc.now();
    String hour = clock_now.hour() < 10 ? "0" + String(clock_now.hour()) : String(clock_now.hour());
    String minute = clock_now.minute() < 10 ? "0" + String(clock_now.minute()) : String(clock_now.minute());
    String second = clock_now.second() < 10 ? "0" + String(clock_now.second()) : String(clock_now.second());
    String time = hour + ":" + minute + ":" + second;
    return time;
}

String TimeHandler::getTimeHourMinute()
{
    // DateTime now = rtc.now();
    String hour = clock_now.hour() < 10 ? "0" + String(clock_now.hour()) : String(clock_now.hour());
    String minute = clock_now.minute() < 10 ? "0" + String(clock_now.minute()) : String(clock_now.minute());
    String time = hour + ":" + minute;
    return time;
}

String TimeHandler::getDay()
{
    // DateTime now = rtc.now();
    String day = String(clock_now.day());

    return day;
}

String TimeHandler::getDayofThenWeek()
{
    // DateTime now = rtc.now();
    String day = String(clock_now.dayOfTheWeek());
    if (day == "1")
    {
        day = "senin";
    }
    else if (day == "2")
    {
        day = "selasa";
    }
    else if (day == "3")
    {
        day = "rabu";
    }
    else if (day == "4")
    {
        day = "kamis";
    }
    else if (day == "5")
    {
        day = "jum'at";
    }
    else if (day == "6")
    {
        day = "sabtu";
    }
    else if (day == "0")
    {
        day = "minggu";
    }

    return day;
}

String TimeHandler::getMonth()
{
    // DateTime now = rtc.now();
    String month = String(clock_now.month());
    return month;
}

String TimeHandler::getYear()
{
    // DateTime now = rtc.now();
    String year = String(clock_now.year());
    return year;
}

String TimeHandler::getHour()
{
    // DateTime now = rtc.now();
    String hour = String(clock_now.hour());
    return hour;
}

String TimeHandler::getMinute()
{
    // DateTime now = rtc.now();
    String minute = String(clock_now.minute());
    return minute;
}

String TimeHandler::getSecond()
{
    // DateTime now = rtc.now();
    String second = String(clock_now.second());
    return second;
}
