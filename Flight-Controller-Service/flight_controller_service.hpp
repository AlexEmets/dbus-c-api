#include <dbus/dbus.h>
#include <iostream>


class FlightControllerService {
public:
    FlightControllerService();
    ~FlightControllerService();
    
    void start();
    void stop();
    
    void takeOff();
    void land();
    
    void setAltitude(double altitude);
    void setHeading(double heading);
    void setSpeed(double speed);
    
    void ExposeMethodToBus();
    static void moveDroneTo(double latitude, double longitude, double altitude);
    DBusConnection* connection_;
    
    static DBusObjectPathVTable vtable_;

    static void handleMethodCall(DBusConnection* connection, DBusMessage* message, void* user_data);
private:
    // Додайте приватні поля, методи та додаткову логіку, яка необхідна для класу
    DBusConnection* m_dbus_connection;
};

