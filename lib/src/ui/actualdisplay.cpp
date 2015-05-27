#include "actualdisplay.h"

ActualDisplay::ActualDisplay(QScreen* usedScreen, QObject* parent) :
    Display(parent),
    _screen(usedScreen)
{

}

ActualDisplay::~ActualDisplay()
{

}
