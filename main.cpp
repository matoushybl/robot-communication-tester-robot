#include <iostream>
#include <thread>
#include "UDP.h"
#include "Variable.h"

using std::cout;
using std::endl;

void repeatAsynchronously(unsigned int sleepMs, const std::function<void (void)> &fun) {
    std::thread([sleepMs, fun]() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
        while (true) {
            fun();

            std::this_thread::sleep_for(
                    std::chrono::milliseconds(sleepMs));
        }
#pragma clang diagnostic pop
    }).detach();
}

struct __attribute__((packed, aligned(1))) MeasurementResult {
    uint32_t seconds;
    uint32_t packets;
};

inline long long millis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

int main(int argc, char* argv[]) {
    std::cout << "Welcome to BaseStation communication tester." << std::endl;

    if (argc < 4) {
        cout << "Incorrect number of arguments - usage is: tester MY_PORT ROBOT_IP ROBOT_PORT" << endl;
    }

    const int myPort = atoi(argv[1]);
    const char* robotIP = argv[2];
    const int robotPort = atoi(argv[3]);

    cout << "Will be listening on port: " << myPort << endl;
    cout << "Status data will be sent to robot: " << robotIP << ":" << robotPort << endl;

    UDP* udp = new UDP(myPort);

    auto receivedPackets = new Variable<uint32_t>(0);
    auto lastReceivedPackets = new Variable<uint32_t>(0);
    auto seconds = new Variable<uint32_t>(0);

    repeatAsynchronously(1000, [receivedPackets, lastReceivedPackets, seconds]() {
        lastReceivedPackets->set(receivedPackets->get());
        receivedPackets->set(0);

        //cout << "seconds: " << seconds->get() << " packets: " << lastReceivedPackets->get() << endl;
        seconds->set(seconds->get() + 1);
    });

    repeatAsynchronously(50, [lastReceivedPackets, udp, seconds, robotIP, robotPort]() {
        MeasurementResult result = { seconds->get(), lastReceivedPackets->get() };

        uint8_t buffer[sizeof(MeasurementResult)];
        memcpy(buffer, &result, sizeof(MeasurementResult));

        udp->send(robotIP, robotPort, buffer, sizeof(MeasurementResult));
    });

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    auto buffer = new unsigned char[1500];
    while (true) {
        if (udp->available()) {
            udp->receive(&buffer);
            receivedPackets->set(receivedPackets->get() + 1);
        }
    }
#pragma clang diagnostic pop
    return 0;
}