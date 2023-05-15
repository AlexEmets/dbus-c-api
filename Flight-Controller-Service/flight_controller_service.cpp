#include "flight_controller_service.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <string>

static DBusHandlerResult process_dbus_messages(DBusConnection *connection, DBusMessage *message, void *user_data);
static void respond_to_introspect(DBusConnection *connection, DBusMessage *request);
static void respond_to_moveDroneTo(DBusConnection *connection, DBusMessage *request);

DBusHandlerResult handle_method_call(DBusConnection *connection, DBusMessage *message, void *user_data)
{
    // Обробка виклику методу DBus

    return DBUS_HANDLER_RESULT_HANDLED;
}

void unregister_object_path(DBusConnection *connection, void *user_data)
{
    // Обробка скасування реєстрації об'єктного шляху
}

FlightControllerService::FlightControllerService() {
    // FlightControllerService - server in context of D-Bus architecture. This classprovides methods to call in another classes.
    DBusError err;
    int ret;
    // initialise the errors
    dbus_error_init(&err);

    // connect to the bus
    m_dbus_connection = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) { 
        fprintf(stderr, "Connection Error (%s)\n", err.message); 
        dbus_error_free(&err); 
    }
    
    if (NULL == m_dbus_connection) { 
        exit(1); 
    }

    // request a name on the bus
    ret = dbus_bus_request_name(m_dbus_connection, "company.drone.server", 
            DBUS_NAME_FLAG_REPLACE_EXISTING 
            , &err);
    if (dbus_error_is_set(&err)) { 
        fprintf(stderr, "Name Error (%s)\n", err.message); 
        dbus_error_free(&err); 
    }
    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) { 
        exit(1);
    }

    DBusObjectPathVTable vtable;
    vtable.message_function = process_dbus_messages;
    vtable.unregister_function = NULL;

    // Реєстрація об'єктного шляху
    const char *object_path = "/Company/DBus/Server/Object";
    if (!dbus_connection_register_object_path(m_dbus_connection, object_path, &vtable, NULL)) {
        // Обробка помилки
        std::cout << "Error in dbus_connection_register_object_path\n";
    }
    
}

void FlightControllerService::ExposeMethodToBus()
{
    while(1) {
        dbus_connection_read_write_dispatch(m_dbus_connection, 1000);
    }
}

FlightControllerService::~FlightControllerService() {

}

void FlightControllerService::moveDroneTo(double latitude, double longitude, double altitude) {
    std::cout << "Moving drone to: Latitude=" << latitude << ", Longitude=" << longitude << ", Altitude=" << altitude << std::endl;
    // Реалізація логіки руху
}

static DBusHandlerResult process_dbus_messages(DBusConnection *connection, DBusMessage *message, void *user_data) {
    const char *interface_name = dbus_message_get_interface(message);
    const char *member_name = dbus_message_get_member(message);
     
    if (0==strcmp("org.freedesktop.DBus.Introspectable", interface_name) &&
        0==strcmp("Introspect", member_name)) {
 
        respond_to_introspect(connection, message);
        return DBUS_HANDLER_RESULT_HANDLED;
    } else if (0==strcmp("org.company.dbus.DroneController", interface_name) &&
           0==strcmp("moveDroneTo", member_name)) {
         
        respond_to_moveDroneTo(connection, message);
        return DBUS_HANDLER_RESULT_HANDLED;
    } else {
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }
}

static void respond_to_moveDroneTo(DBusConnection *connection, DBusMessage *request) {
    DBusMessage *reply;
    DBusError error;

    double latitude;
    double longitude;
    double altitude;

    dbus_error_init(&error);
 
    dbus_message_get_args(request, &error,
                  DBUS_TYPE_DOUBLE, &latitude,
                  DBUS_TYPE_DOUBLE, &longitude,
                  DBUS_TYPE_DOUBLE, &altitude,
                  DBUS_TYPE_INVALID);
    
    if (dbus_error_is_set(&error)) {
        reply = dbus_message_new_error(request, "wrong_arguments", "Illegal arguments to Sum");
        dbus_connection_send(connection, reply, NULL);
        dbus_message_unref(reply);
        return;
    }
 
    FlightControllerService::moveDroneTo(latitude, longitude, altitude);
    std::string answer = "Drone was moved to coordinates: " 
    + std::to_string(latitude) + " " + std::to_string(longitude) + " " + std::to_string(altitude) + "\n";
     
    reply = dbus_message_new_method_return(request);
    dbus_message_append_args(reply,
                 DBUS_TYPE_STRING, &answer,
                 DBUS_TYPE_INVALID);
    
    std::cout << answer << "\n";
    dbus_connection_send(connection, reply, NULL);
    dbus_message_unref(reply);
    
}

static void respond_to_introspect(DBusConnection *connection, DBusMessage *request) {
    DBusMessage *reply;
 
    const char *introspection_data =
        " <!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\" "
        "\"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">"
        " <!-- dbus-sharp 0.8.1 -->"
        " <node>"
        "   <interface name=\"org.company.dbus.Introspectable\">"
        "     <method name=\"Introspect\">"
        "       <arg name=\"data\" direction=\"out\" type=\"s\" />"
        "     </method>"
        "   </interface>"
        "   <interface name=\"org.company.dbus.DroneController\">"
        "     <method name=\"moveDroneTo\">"
        "       <arg name=\"a\" direction=\"in\" type=\"i\" />"
        "       <arg name=\"b\" direction=\"in\" type=\"i\" />"
        "       <arg name=\"ret\" direction=\"out\" type=\"i\" />"
        "     </method>"
        "   </interface>"
        " </node>";
     
    reply = dbus_message_new_method_return(request);
    dbus_message_append_args(reply,
                 DBUS_TYPE_STRING, &introspection_data,
                 DBUS_TYPE_INVALID);
    dbus_connection_send(connection, reply, NULL);
    dbus_message_unref(reply);
}