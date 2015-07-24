#ifndef MOCK_IQSIGNAL_H
#define MOCK_IQSIGNAL_H

#include <gmock/gmock.h>

#include <iqsignal.h>

class MockIQSignal : public IQSignal
{
 public:
     MOCK_METHOD1(slot1, void(QString data));

     // no need to mock signals are they don't have impl and they are not called
     // TODO: we could have a macro generate following kinds of functions
     void emitSignal2(QString data)
     {
         emit signal2(data);
     }
};

#endif // MOCK_IQSIGNAL_H

