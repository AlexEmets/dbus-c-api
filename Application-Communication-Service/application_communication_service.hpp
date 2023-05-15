#include <dbus/dbus.h>

class ApplicationCommunicationService {
public:
    ApplicationCommunicationService();
    ~ApplicationCommunicationService();
    
    void start();
    void stop();
    
    void connectToFlightController();
    void disconnectFromFlightController();
    
    void takeOff();
    void land();
    
    void setAltitude(double altitude);
    void setHeading(double heading);
    void setSpeed(double speed);
    
    void moveTo(double latitude, double longitude, double altitude);
    
    // Додайте інші методи, які потрібні для вашої функціональності
    
private:
    DBusConnection* m_dbus_connection;
};