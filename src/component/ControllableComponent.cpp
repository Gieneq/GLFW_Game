#include "ControllableComponent.h"


void WSADControllableComponent::press_w() {
    std::cout << "Pressed W" << std::endl;
    if(transform_cmpnt) {
        transform_cmpnt->direction_north();
        transform_cmpnt->go();
    }
}

void WSADControllableComponent::press_a() {
    std::cout << "Pressed A" << std::endl;
    if(transform_cmpnt) {
        transform_cmpnt->direction_west();
        transform_cmpnt->go();
    }
}

void WSADControllableComponent::press_s() {
    std::cout << "Pressed S" << std::endl;
    if(transform_cmpnt) {
        transform_cmpnt->direction_south();
        transform_cmpnt->go();
    }
}

void WSADControllableComponent::press_d() {
    std::cout << "Pressed D" << std::endl;
    if(transform_cmpnt) {
        transform_cmpnt->direction_east();
        transform_cmpnt->go();
    }
}


void WSADControllableComponent::release_w() {
    if(transform_cmpnt) {
        transform_cmpnt->stop();
    }
}
void WSADControllableComponent::release_a() {
    if(transform_cmpnt) {
        transform_cmpnt->stop();
    }
}
void WSADControllableComponent::release_s() {
    if(transform_cmpnt) {
        transform_cmpnt->stop();
    }
}
void WSADControllableComponent::release_d() {
    if(transform_cmpnt) {
        transform_cmpnt->stop();
    }
}