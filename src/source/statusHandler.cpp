#include "header/statusHandler.h"

statusHandler::statusHandler(/* args */)
{
}

// void statusHandler::setPenjadwalanOn(bool )

void statusHandler::setTipeTerdekatHariIni(String tipe_terdekat_hari_ini)
{
    this->tipe_terdekat_hari_ini = tipe_terdekat_hari_ini;
}

void statusHandler::setWaktuPendajwalanTerdekatHariIni(String waktu_pendajwalan_terdekat_hari_ini)
{
    this->waktu_pendajwalan_terdekat_hari_ini = waktu_pendajwalan_terdekat_hari_ini;
}

String statusHandler::getTipeTerdekatHariIni()
{
    return this->tipe_terdekat_hari_ini;
}

String statusHandler::getWaktuPendajwalanTerdekatHariIni()
{
    return this->waktu_pendajwalan_terdekat_hari_ini;
}

void statusHandler::setIsStatusChanged(bool isChanged)
{
    this->isChanged = isChanged;
}

bool statusHandler::isStatusChanged()
{
    if (this->isChanged == true)
    {
        this->isChanged = false;
        return true;
    }

    return this->isChanged;
}

void statusHandler::setCurrentIndexSetOnTimes(int index)
{
    this->currentIndexSetOnTimes = index;
}

int statusHandler::getCurrentIndexSetOnTimes()
{
    return this->currentIndexSetOnTimes;
}

int statusHandler::getCurrentPekan()
{
    return this->currentPekan;
}

void statusHandler::setCurrentPekan(int pekan)
{
    this->currentPekan = pekan;
}

void statusHandler::setPenjadwalanOn(bool isPenjadwalanOn)
{
    this->isPenjadwalanOn = isPenjadwalanOn;
}

bool statusHandler::getPenjadwalanOn()
{
    return this->isPenjadwalanOn;
}

void statusHandler::setOnTimesStatus(bool condition, int currentLand, String type)
{
    this->doc["statusJadwal"][type]["setontimes"][this->currentIndexSetOnTimes]["status"]["kondisi"] = condition == true ? "on" : "off";
    if (currentLand == 0)
    {
        this->doc["statusJadwal"][type]["setontimes"][this->currentIndexSetOnTimes]["status"]["currentLand"] = nullptr;
    }
    else
    {
        this->doc["statusJadwal"][type]["setontimes"][this->currentIndexSetOnTimes]["status"]["currentLand"] = currentLand;
    }
}

void statusHandler::updateFromLoadConfig(StaticJsonDocument<JSON_SIZE> doc)
{
    this->doc.clear();
    this->tglAkhirPenjadwalan = doc["date"].as<String>();
    this->doc["ID"] = this->ID;
    this->doc["firmware"] = this->FirmwareVersion;
    this->doc["activation"] = this->Activation == true ? "Activated" : "Not Activated";
    this->doc["specification"]["totalOutput"] = this->totalOutput;
    this->doc["specification"]["totalInput"] = this->totalInput;
    this->doc["statusJadwal"] = doc["jadwal"];
    this->doc.remove("jadwal");
    this->totalPekan = this->doc["statusJadwal"]["minggu"];
    this->doc["statusJadwal"].remove("minggu");
    this->doc["statusJadwal"]["tglAkhirPenjadwalan"] = this->tglAkhirPenjadwalan;
    this->doc["statusJadwal"]["totalPekan"] = this->totalPekan;
    this->doc["statusJadwal"]["sisaPekan"] = this->sisaPekan;
    this->doc["statusJadwal"]["currentPekan"] = this->currentPekan;
    for (int i = 0; i < this->doc["statusJadwal"]["air"]["setontimes"].size(); i++)
    {
        this->doc["statusJadwal"]["air"]["setontimes"][i]["status"]["kondisi"] = "off";
        this->doc["statusJadwal"]["air"]["setontimes"][i]["status"]["currentLand"] = nullptr;
    }

    for (int i = 0; i < this->doc["statusJadwal"]["pemupukan"]["setontimes"].size(); i++)
    {
        this->doc["statusJadwal"]["pemupukan"]["setontimes"][i]["status"]["kondisi"] = "off";
        this->doc["statusJadwal"]["pemupukan"]["setontimes"][i]["status"]["currentLand"] = nullptr;
    }

    this->doc["statusPerangkat"]["output"]["pompaUtama"] = this->mainPump == true ? "on" : "off";
    this->doc["statusPerangkat"]["output"]["valveAir"] = this->waterPump == true ? "on" : "off";
    this->doc["statusPerangkat"]["output"]["valvePemupukan"] = this->nutrientPump == true ? "on" : "off";
    this->doc["statusPerangkat"]["output"]["valveLahan1"] = this->lahan1 == true ? "on" : "off";
    this->doc["statusPerangkat"]["output"]["valveLahan2"] = this->lahan2 == true ? "on" : "off";
    this->doc["statusPerangkat"]["output"]["valveLahan3"] = this->lahan3 == true ? "on" : "off";
    this->doc["statusPerangkat"]["output"]["valveLahan4"] = this->lahan4 == true ? "on" : "off";

    this->doc["statusPerangkat"]["input"]["DHTLahan1"]["temperature"] = this->dhtLahan1Temp;
    this->doc["statusPerangkat"]["input"]["DHTLahan1"]["humidity"] = this->dhtLahan1Hum;
    this->doc["statusPerangkat"]["input"]["DHTLahan2"]["temperature"] = this->dhtLahan2Temp;
    this->doc["statusPerangkat"]["input"]["DHTLahan2"]["humidity"] = this->dhtLahan2Hum;
    this->doc["statusPerangkat"]["input"]["flowMeterLahan1"]["flow"] = this->flowMeterLahan1_flow;
    this->doc["statusPerangkat"]["input"]["flowMeterLahan1"]["volume"] = this->flowMeterLahan1_volume;
    this->doc["statusPerangkat"]["input"]["flowMeterLahan2"]["flow"] = this->flowMeterLahan2_flow;
    this->doc["statusPerangkat"]["input"]["flowMeterLahan2"]["volume"] = this->flowMeterLahan2_volume;

    this->doc["statusPerangkat"]["dateTime"]["date"] = this->date;
    this->doc["statusPerangkat"]["dateTime"]["time"] = this->time;

    // String temp;
    // serializeJson(this->doc, temp);
    // Serial.println("Loaded Data" + temp);
}

void statusHandler::updateJSON()
{
    // this->doc["statusJadwal"].remove("tglAkhirPenjadwalan");
    this->doc["statusJadwal"]["tglAkhirPenjadwalan"] = this->tglAkhirPenjadwalan;
    // this->doc["statusJadwal"].remove("totalPekan");
    this->doc["statusJadwal"]["totalPekan"] = this->totalPekan;
    // this->doc["statusJadwal"].remove("sisaPekan");
    this->doc["statusJadwal"]["sisaPekan"] = this->sisaPekan;
    // this->doc["statusJadwal"].remove("currentPekan");
    this->doc["statusJadwal"]["currentPekan"] = this->currentPekan;

    // this->doc["statusPerangkat"].remove("output");
    this->doc["statusPerangkat"]["output"]["pompaUtama"] = this->mainPump == true ? "on" : "off";
    this->doc["statusPerangkat"]["output"]["valveAir"] = this->waterPump == true ? "on" : "off";
    this->doc["statusPerangkat"]["output"]["valvePemupukan"] = this->nutrientPump == true ? "on" : "off";
    this->doc["statusPerangkat"]["output"]["valveLahan1"] = this->lahan1 == true ? "on" : "off";
    this->doc["statusPerangkat"]["output"]["valveLahan2"] = this->lahan2 == true ? "on" : "off";
    this->doc["statusPerangkat"]["output"]["valveLahan3"] = this->lahan3 == true ? "on" : "off";
    this->doc["statusPerangkat"]["output"]["valveLahan4"] = this->lahan4 == true ? "on" : "off";

    // this->doc["statusPerangkat"].remove("input");
    this->doc["statusPerangkat"]["input"]["DHTLahan1"]["temperature"] = this->dhtLahan1Temp;
    this->doc["statusPerangkat"]["input"]["DHTLahan1"]["humidity"] = this->dhtLahan1Hum;
    this->doc["statusPerangkat"]["input"]["DHTLahan2"]["temperature"] = this->dhtLahan2Temp;
    this->doc["statusPerangkat"]["input"]["DHTLahan2"]["humidity"] = this->dhtLahan2Hum;
    this->doc["statusPerangkat"]["input"]["flowMeterLahan1"]["flow"] = this->flowMeterLahan1_flow;
    this->doc["statusPerangkat"]["input"]["flowMeterLahan1"]["volume"] = this->flowMeterLahan1_volume;
    this->doc["statusPerangkat"]["input"]["flowMeterLahan2"]["flow"] = this->flowMeterLahan2_flow;
    this->doc["statusPerangkat"]["input"]["flowMeterLahan2"]["volume"] = this->flowMeterLahan2_volume;

    // this->doc["statusPerangkat"].remove("dateTime");
    this->doc["statusPerangkat"]["dateTime"]["date"] = this->date;
    this->doc["statusPerangkat"]["dateTime"]["time"] = this->time;
}

StaticJsonDocument<JSON_SIZE> statusHandler::getStatusJSON()
{
    updateJSON();
    return this->doc;
}

void statusHandler::setFirmwareVersion(String FirmwareVersion)
{
    this->FirmwareVersion = FirmwareVersion;
}

void statusHandler::setActivation(bool Activation)
{
    this->Activation = Activation;
}

void statusHandler::setStatusJSON(StaticJsonDocument<JSON_SIZE> doc)
{
    this->doc = doc;
}

void statusHandler::setID(String ID)
{
    this->ID = ID;
}

void statusHandler::setTotalPekan(int totalPekan)
{
    this->totalPekan = totalPekan;
}

void statusHandler::setTotalOutput(int totalOutput)
{
    this->totalOutput = totalOutput;
}

void statusHandler::setTotalInput(int totalInput)
{
    this->totalInput = totalInput;
}

void statusHandler::setTglAkhirPenjadwalan(String tglAkhirPenjadwalan)
{
    this->tglAkhirPenjadwalan = tglAkhirPenjadwalan;
}

void statusHandler::setSisaPekan(int sisaPekan)
{
    this->sisaPekan = sisaPekan;
}

void statusHandler::setMainPump(bool mainPump)
{
    this->isChanged = true;
    this->mainPump = mainPump;
}

void statusHandler::setWaterPump(bool waterPump)
{
    this->isChanged = true;
    this->waterPump = waterPump;
}

void statusHandler::setNutrientPump(bool nutrientPump)
{
    this->isChanged = true;
    this->nutrientPump = nutrientPump;
}

void statusHandler::setLahan1(bool lahan1)
{
    this->isChanged = true;
    this->lahan1 = lahan1;
}

void statusHandler::setLahan2(bool lahan2)
{
    this->isChanged = true;
    this->lahan2 = lahan2;
}

void statusHandler::setLahan3(bool lahan3)
{
    this->isChanged = true;
    this->lahan3 = lahan3;
}

void statusHandler::setLahan4(bool lahan4)
{
    this->isChanged = true;
    this->lahan4 = lahan4;
}

void statusHandler::setDHTLahan1Temp(float dhtLahan1Temp)
{
    this->dhtLahan1Temp = dhtLahan1Temp;
}

void statusHandler::setDHTLahan1Hum(float dhtLahan1Hum)
{
    this->dhtLahan1Hum = dhtLahan1Hum;
}

void statusHandler::setDHTLahan2Temp(float dhtLahan2Temp)
{
    this->dhtLahan2Temp = dhtLahan2Temp;
}

void statusHandler::setDHTLahan2Hum(float dhtLahan2Hum)
{
    this->dhtLahan2Hum = dhtLahan2Hum;
}

void statusHandler::setFlowMeterLahan1Flow(float flowMeterLahan1Flow)
{
    this->flowMeterLahan1_flow = flowMeterLahan1_flow;
}

void statusHandler::setFlowMeterLahan1Volume(float flowMeterLahan1Volume)
{
    this->flowMeterLahan1_volume = flowMeterLahan1Volume;
}

void statusHandler::setFlowMeterLahan2Flow(float flowMeterLahan2Flow)
{
    this->flowMeterLahan2_flow = flowMeterLahan2Flow;
}

void statusHandler::setFlowMeterLahan2Volume(float flowMeterLahan2Volume)
{
    this->flowMeterLahan2_volume = flowMeterLahan2Volume;
}

void statusHandler::setDate(String date)
{
    this->date = date;
}

void statusHandler::setTime(String time)
{
    this->time = time;
}

String statusHandler::getFirmwareVersion()
{
    return this->FirmwareVersion;
}

bool statusHandler::getActivation()
{
    return this->Activation;
}

String statusHandler::getID()
{
    return this->ID;
}

int statusHandler::getTotalPekan()
{
    return this->totalPekan;
}

int statusHandler::getTotalOutput()
{
    return this->totalOutput;
}

int statusHandler::getTotalInput()
{
    return this->totalInput;
}

String statusHandler::getTglAkhirPenjadwalan()
{
    return this->tglAkhirPenjadwalan;
}

int statusHandler::getSisaPekan()
{
    return this->sisaPekan;
}

bool statusHandler::getMainPump()
{
    return this->mainPump;
}

bool statusHandler::getWaterPump()
{
    return this->waterPump;
}

bool statusHandler::getNutrientPump()
{
    return this->nutrientPump;
}

bool statusHandler::getLahan1()
{
    return this->lahan1;
}

bool statusHandler::getLahan2()
{
    return this->lahan2;
}

bool statusHandler::getLahan3()
{
    return this->lahan3;
}

bool statusHandler::getLahan4()
{
    return this->lahan4;
}

float statusHandler::getDHTLahan1Temp()
{
    return this->dhtLahan1Temp;
}

float statusHandler::getDHTLahan1Hum()
{
    return this->dhtLahan1Hum;
}

float statusHandler::getDHTLahan2Temp()
{
    return this->dhtLahan2Temp;
}

float statusHandler::getDHTLahan2Hum()
{
    return this->dhtLahan2Hum;
}

float statusHandler::getFlowMeterLahan1Flow()
{
    return this->flowMeterLahan1_flow;
}

float statusHandler::getFlowMeterLahan1Volume()
{
    return this->flowMeterLahan1_volume;
}

float statusHandler::getFlowMeterLahan2Flow()
{
    return this->flowMeterLahan2_flow;
}

float statusHandler::getFlowMeterLahan2Volume()
{
    return this->flowMeterLahan2_volume;
}

String statusHandler::getDate()
{
    return this->date;
}

String statusHandler::getTime()
{
    return this->time;
}

statusHandler::~statusHandler()
{
}
