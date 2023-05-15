#include <iostream>
#include "flight_controller_service.hpp"

int main()

{
   FlightControllerService service;
   std::cout << "FlightControllerService is started\n";
   service.ExposeMethodToBus();
}