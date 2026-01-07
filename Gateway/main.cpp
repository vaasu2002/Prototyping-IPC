#include "Gateway.h"

int main() {
    try {
        Exchange::Gateway::Gateway gateway("Gateway");
        gateway.start();
    }
    catch (Engine::EngException& ex) {
        LOG_ERROR("Exception caught:");
        return 1;
    }
    return 0;
}