#include "dev_config.h"
#include "RTClib.h"
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

class TimeHandler
{
private:
    DateTime clock_now;
public:
    TimeHandler();
    void syncTime();
    void initRTC();
    void updateTime();
    String getTime();
    String getTimeHourMinute();
    String getDay();
    String getDayofThenWeek();
    String getDate();
    String getDateWithCalc(int week);
    bool isBefore(String date);
    String getRestOfWeekFrom(String date);
    void getSourceTime();
    
    String getMonth();
    String getYear();
    String getHour();
    String getMinute();
    String getSecond();
    ~TimeHandler();
};


