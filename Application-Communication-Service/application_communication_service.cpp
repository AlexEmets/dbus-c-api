#include "application_communication_service.hpp"
#include <iostream>


static void check_and_abort(DBusError *error);


ApplicationCommunicationService::ApplicationCommunicationService()
{
    DBusError error;
    dbus_error_init(&error);
    m_dbus_connection = dbus_bus_get(DBUS_BUS_SESSION, &error);

    if (NULL == m_dbus_connection) { 
        std::cout << "m_dbus_connection is NULL";
        exit(1); 
    }
}


ApplicationCommunicationService::~ApplicationCommunicationService()
{
    // dbus_connection_close(m_dbus_connection);
}


void ApplicationCommunicationService::moveTo(double latitude, double longitude, double altitude) {
    DBusMessage *msgQuery = NULL;
    DBusMessage *msgReply = NULL;
    DBusMessageIter args;
    DBusError error;
    const char *busName = NULL;
    const char* moveDroneAnswer;

    dbus_error_init(&error);

    busName = "company.drone.server";
 
    msgQuery = dbus_message_new_method_call(
        busName,
        "/Company/DBus/Server/Object",
        "org.company.dbus.DroneController",
        "moveDroneTo");

    dbus_message_append_args(msgQuery, 
        DBUS_TYPE_DOUBLE, &latitude, 
        DBUS_TYPE_DOUBLE, &longitude, 
        DBUS_TYPE_DOUBLE, &altitude,
        DBUS_TYPE_INVALID);

    msgReply = dbus_connection_send_with_reply_and_block(m_dbus_connection, msgQuery, 1000, &error);
    check_and_abort(&error);
    dbus_message_unref(msgQuery);

    dbus_message_get_args(msgReply, &error, DBUS_TYPE_STRING, &moveDroneAnswer, DBUS_TYPE_INVALID);

    std::cout << "Answer from FlightControllerService: " << moveDroneAnswer << std::endl;
     
    dbus_message_unref(msgReply);
}

static void check_and_abort(DBusError *error) {
    if (!dbus_error_is_set(error)) return;
    puts(error->message);
    abort();
}