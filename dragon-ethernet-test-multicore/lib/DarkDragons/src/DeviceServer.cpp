#include "DeviceServer.h"

#define VERSION "0.0.1"

using namespace DarkDragons;

void DeviceServer::registerHandlers(EthernetWebServer &server)
{
    this->server = &server;

    server.on("/management/apiversions", HTTP_GET, [&]() {
        handleAlpacaManagementApiVersions();
    });

    server.on("/management/v1/description", HTTP_GET, [&]() {
        handleAlpacaManagementDescription();
    });

    server.on("/management/v1/configureddevices", HTTP_GET, [&]() {
        handleAlpacaManagementConfiguredDevices();
    });
}

void DeviceServer::sendDocument(StaticJsonDocument<RESPONSE_BUFFER_SIZE> &doc)
{
    size_t length = measureJsonPretty(doc);

    server->setContentLength(length);

    server->send(200, "application/json");

    EthernetClient client = server->client();

    serializeJsonPretty(doc, client);
}

void DeviceServer::sendError(uint32_t clientTransactionId, int32_t errorNumber, String errorMessage)
{
    StaticJsonDocument<RESPONSE_BUFFER_SIZE> doc;

    doc["ClientTransactionID"] = clientTransactionId;
    doc["ServerTransactionID"] = transactionId++;
    doc["ErrorNumber"] = errorNumber;
    doc["ErrorMessage"] = errorMessage;

    sendDocument(doc);
}

void DeviceServer::handleNotImplemented()
{
    uint32_t clientTransactionID = strtoul(server->arg("ClientTransactionID").c_str(), NULL, 10);
    sendError(clientTransactionID, 0x400, "Not implemented");
}

void DeviceServer::handleAlpacaManagementApiVersions()
{
    uint32_t clientTransactionID = strtoul(server->arg("ClientTransactionID").c_str(), NULL, 10);

    StaticJsonDocument<RESPONSE_BUFFER_SIZE> doc;

    JsonArray apiVersions = doc.createNestedArray("Value");
    apiVersions.add(1);
    doc["ClientTransactionID"] = clientTransactionID;
    doc["ServerTransactionID"] = transactionId++;

    sendDocument(doc);
}

void DeviceServer::handleAlpacaManagementDescription()
{
    uint32_t clientTransactionID = strtoul(server->arg("ClientTransactionID").c_str(), NULL, 10);

    StaticJsonDocument<RESPONSE_BUFFER_SIZE> doc;

    doc["Value"]["Manufacturer"] = "Dark Dragons Astronomy, LLC";
    doc["Value"]["ManufacturerVersion"] = VERSION;
    doc["Value"]["Location"] = "";
    doc["Value"]["ServerName"] = hostname;
    doc["ClientTransactionID"] = clientTransactionID;
    doc["ServerTransactionID"] = transactionId++;

    sendDocument(doc);
}

void DeviceServer::handleAlpacaGetVersion()
{
    uint32_t clientTransactionID = strtoul(server->arg("ClientTransactionID").c_str(), NULL, 10);

    StaticJsonDocument<RESPONSE_BUFFER_SIZE> doc;

    doc["ClientTransactionID"] = clientTransactionID;
    doc["ServerTransactionID"] = transactionId++;
    doc["Value"] = VERSION;

    sendDocument(doc);
}

void DeviceServer::handleAlpacaGetConnected()
{
    uint32_t clientTransactionID = strtoul(server->arg("ClientTransactionID").c_str(), NULL, 10);

    StaticJsonDocument<RESPONSE_BUFFER_SIZE> doc;

    doc["Value"] = isConnected;
    doc["ClientTransactionID"] = clientTransactionID;
    doc["ServerTransactionID"] = transactionId++;

    sendDocument(doc);
}

void DeviceServer::handleAlpacaPutConnected()
{
    uint32_t clientTransactionID = strtoul(server->arg("ClientTransactionID").c_str(), NULL, 10);

    isConnected = server->arg("Connected").equalsIgnoreCase("true");

    StaticJsonDocument<RESPONSE_BUFFER_SIZE> doc;

    doc["ClientTransactionID"] = clientTransactionID;
    doc["ServerTransactionID"] = transactionId++;

    sendDocument(doc);
}

void DeviceServer::handleAlpacaGetInterfaceVersion()
{
    uint32_t clientTransactionID = strtoul(server->arg("ClientTransactionID").c_str(), NULL, 10);

    StaticJsonDocument<RESPONSE_BUFFER_SIZE> doc;

    doc["ClientTransactionID"] = clientTransactionID;
    doc["ServerTransactionID"] = transactionId++;
    doc["Value"] = 2;

    sendDocument(doc);
}

void DeviceServer::handleAlpacaGetSupportedActions()
{
    uint32_t clientTransactionID = strtoul(server->arg("ClientTransactionID").c_str(), NULL, 10);

    StaticJsonDocument<RESPONSE_BUFFER_SIZE> doc;

    doc["ClientTransactionID"] = clientTransactionID;
    doc["ServerTransactionID"] = transactionId++;
    doc.createNestedArray("Value");

    sendDocument(doc);
}
