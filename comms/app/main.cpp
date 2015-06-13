#include "comms.h"

int main(int argc, char *argv[])
{
    GBerryComms::Comms comms;
    return comms.run(argc, argv);
}
