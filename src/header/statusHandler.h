#include "dev_config.h"
#include <ArduinoJson.h>

class statusHandler
{
private:
    String ID = "NF00";
    String FirmwareVersion = "0.0.0";
    bool Activation = false;

    int totalPekan = 0;
    int totalOutput = 0;
    int totalInput = 0;

    String tglAkhirPenjadwalan = "9/9/1999";
    int sisaPekan = 0;
    int currentPekan = 0;

    // output
    bool mainPump = false;
    bool waterPump = false;
    bool nutrientPump = false;
    bool lahan1 = false;
    bool lahan2 = false;
    bool lahan3 = false;
    bool lahan4 = false;

    // input
    float dhtLahan1Temp = 0;
    float dhtLahan1Hum = 0;
    float dhtLahan2Temp = 0;
    float dhtLahan2Hum = 0;
    float flowMeterLahan1_flow = 0;
    float flowMeterLahan1_volume = 0;
    float flowMeterLahan2_flow = 0;
    float flowMeterLahan2_volume = 0;

    // dateTime
    String date = "9/9/1999";
    String time = "00:00";

    String tipe_terdekat_hari_ini = "-----";
    String waktu_pendajwalan_terdekat_hari_ini = "--:--";

    bool isPenjadwalanOn = false;


    bool isChanged = false;

    StaticJsonDocument<JSON_SIZE> doc;

    int currentIndexSetOnTimes;

public:
    statusHandler(/* args */);
    void updateFromLoadConfig(StaticJsonDocument<JSON_SIZE> doc);
    void updateJSON();
    StaticJsonDocument<JSON_SIZE> getStatusJSON();
    void setFirmwareVersion(String FirmwareVersion);
    void setActivation(bool Activation);
    void setStatusJSON(StaticJsonDocument<JSON_SIZE> doc);
    void setID(String ID);
    void setTotalPekan(int totalPekan);
    void setTotalOutput(int totalOutput);
    void setTotalInput(int totalInput);
    void setTglAkhirPenjadwalan(String tglAkhirPenjadwalan);
    void setSisaPekan(int sisaPekan);
    void setMainPump(bool mainPump);
    void setWaterPump(bool waterPump);
    void setNutrientPump(bool nutrientPump);
    void setLahan1(bool lahan1);
    void setLahan2(bool lahan2);
    void setLahan3(bool lahan3);
    void setLahan4(bool lahan4);
    void setDHTLahan1Temp(float dhtLahan1Temp);
    void setDHTLahan1Hum(float dhtLahan1Hum);
    void setDHTLahan2Temp(float dhtLahan2Temp);
    void setDHTLahan2Hum(float dhtLahan2Hum);
    void setFlowMeterLahan1Flow(float flowMeterLahan1_flow);
    void setFlowMeterLahan1Volume(float flowMeterLahan1_volume);
    void setFlowMeterLahan2Flow(float flowMeterLahan2_flow);
    void setFlowMeterLahan2Volume(float flowMeterLahan2_volume);
    void setDate(String date);
    void setTime(String time);
    void setCurrentPekan(int currentPekan);
    void setOnTimesStatus(bool condition, int currentLand, String type);
    void setCurrentIndexSetOnTimes(int currentIndexSetOnTimes);
    bool isStatusChanged();
    void setIsStatusChanged(bool isChanged);
    void setTipeTerdekatHariIni(String tipe_terdekat_hari_ini);
    void setWaktuPendajwalanTerdekatHariIni(String waktu_pendajwalan_terdekat_hari_ini);
    void setPenjadwalanOn(bool isPenjadwalanOn);
    bool getPenjadwalanOn();
    


    String getTipeTerdekatHariIni();
    String getWaktuPendajwalanTerdekatHariIni();

    int getCurrentIndexSetOnTimes();

    int getCurrentPekan();
    String getFirmwareVersion();
    bool getActivation();
    String getID();
    int getTotalPekan();
    int getTotalOutput();
    int getTotalInput();
    String getTglAkhirPenjadwalan();
    int getSisaPekan();
    bool getMainPump();
    bool getWaterPump();
    bool getNutrientPump();
    bool getLahan1();
    bool getLahan2();
    bool getLahan3();
    bool getLahan4();
    float getDHTLahan1Temp();
    float getDHTLahan1Hum();
    float getDHTLahan2Temp();
    float getDHTLahan2Hum();
    float getFlowMeterLahan1Flow();
    float getFlowMeterLahan1Volume();
    float getFlowMeterLahan2Flow();
    float getFlowMeterLahan2Volume();
    String getDate();
    String getTime();
    ~statusHandler();
};
