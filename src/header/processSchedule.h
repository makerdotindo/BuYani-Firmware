#include "dev_config.h"

#include <ArduinoJson.h>
#include <header/outputHandler.h>
#include <ArduinoQueue.h>
#include <header/timeHandler.h>
#include <header/statusHandler.h>
#include <header/inputHandler.h>

// Queue creation:
typedef struct
{
    int id;
    int duration = 0;
    int tipe;
    int seconds = 0;
    bool isPemupukan = false;

} LandsConfig;

class processSchedule
{
public:
    processSchedule();

    void processAutoMode(StaticJsonDocument<JSON_SIZE> doc);
    void processManualMode(StaticJsonDocument<JSON_SIZE> doc);
    void processData(String type, StaticJsonDocument<JSON_SIZE> doc);
    void pumpHandler(String type);
    void buzzerBeep(unsigned long ms);
    bool getIsSecondsBeat();
    bool getIsSeconds(int seconds);
    bool getIsSeconds2(int seconds);
    void resetCounter();
    void resetQueue();

private:
    int counterDetik = 0;
    int counterDetik2 = 0;
    LandsConfig current_land;
    bool isOperate;
    bool isDelay;
    bool state_water_pump;
    bool state_nutrient_pump;
    bool isBuzzer;
    String delay_over_lands;
    String *lands;
    String time;
    String today;
    unsigned long timerBuzzer;
    unsigned long lastTime;
    unsigned long timerDelay;
    unsigned long lastSecond;
    unsigned long second;
    DateTime nearest;
    bool flag = false;
    bool flag_near = false;

    bool isDetik;
    // day_t days;
};
