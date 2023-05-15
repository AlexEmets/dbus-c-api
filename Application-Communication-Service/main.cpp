#include <iostream>
#include "application_communication_service.hpp"

int main()

{
   ApplicationCommunicationService service;
   service.moveTo(1,2,3);
   while(true){}
}