#include "DragonLightServer.h"

using namespace DarkDragons;

void DragonLightServer::onRoot()
{
    StaticJsonDocument<RESPONSE_BUFFER_SIZE> doc;
    doc["hostname"] = hostname;
    doc["isLightOn"] = isLightOn;
    doc["lightLevel"] = lightLevel;
    doc["isConnected"] = isConnected;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["dewPoint"] = dewPoint;
    sendDocument(doc);
}

void DragonLightServer::handleAlpacaManagementConfiguredDevices()
{
    uint32_t clientTransactionID = strtoul(server->arg("ClientTransactionID").c_str(), NULL, 10);

    StaticJsonDocument<RESPONSE_BUFFER_SIZE> doc;

    JsonArray devices = doc.createNestedArray("Value");

    JsonObject device = devices.createNestedObject();

    device["DeviceName"] = "Dragon Light";
    device["DeviceType"] = "CoverCalibrator";
    device["DeviceNumber"] = 0;
    device["UniqueID"] = "test"; // TODO: Fix this

    doc["ClientTransactionID"] = clientTransactionID;
    doc["ServerTransactionID"] = transactionId++;

    sendDocument(doc);
}

void DragonLightServer::handleAlpacaGetDescription()
{
    uint32_t clientTransactionID = strtoul(server->arg("ClientTransactionID").c_str(), NULL, 10);

    StaticJsonDocument<RESPONSE_BUFFER_SIZE> doc;

    doc["ClientTransactionID"] = clientTransactionID;
    doc["ServerTransactionID"] = transactionId++;
    doc["Value"] = "Dragon Light Flat Panel Controller";

    sendDocument(doc);
}

void DragonLightServer::handleAlpacaGetName()
{
    uint32_t clientTransactionID = strtoul(server->arg("ClientTransactionID").c_str(), NULL, 10);

    StaticJsonDocument<RESPONSE_BUFFER_SIZE> doc;

    doc["ClientTransactionID"] = clientTransactionID;
    doc["ServerTransactionID"] = transactionId++;
    doc["Value"] = "Dragon Light";

    sendDocument(doc);
}

