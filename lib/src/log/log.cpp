#include "log.h"
#include "logcontrol.h"

#include <QDateTime>

#include <log/stdoutlogmsghandler.h>

class PrivateImpl
{
public:
    PrivateImpl() : logControl(nullptr), highestLoggingLevel(Log::ALL) {}
    ~PrivateImpl()
    {
        // logControl is released elsewhere
        logControl = nullptr;
    }

    void addSpace()
    {
        if (!skipSpaceOnFollowing)
            buffer << " ";

        skipSpaceOnFollowing = false;
    }

    LogControl* logControl;
    int highestLoggingLevel;

    // information about currently processed message
    QStringList buffer;
    LogMsg msg;
    bool skipSpaceOnFollowing;

};


const EndOfLine Log::eol;
const NoSpace Log::nospace;

Log::Log() : _impl(nullptr)
{
    _impl = new PrivateImpl();
}

Log::~Log()
{
    delete _impl;
}

void Log::use(LogControl *logControl)
{
    _impl->logControl = logControl;
    _impl->highestLoggingLevel = logControl->highestLogLevel();

    QObject::connect(logControl, &LogControl::highestLogLevelChanged,
                     [&] () { _impl->highestLoggingLevel = logControl->highestLogLevel(); });
}

void Log::reset()
{
    // TODO: how these default are released -> maybe they are static defaults
    LogControl* logControl = new LogControl();
    StdoutLogMsgHandler* handler = new StdoutLogMsgHandler(Log::DEBUG);

    logControl->registerMsgHandler(handler);
    //_impl->logControl = nullptr;
    _impl->logControl = logControl;
}

Log& Log::begin(const int& level, const char* area, const char* file, const int& lineno)
{
    _impl->msg.level = level;
    _impl->msg.time = QDateTime::QDateTime::currentMSecsSinceEpoch();
    _impl->msg.area = area;
    _impl->msg.sourceFile = file;
    _impl->msg.sourceLine = lineno;
    _impl->skipSpaceOnFollowing = true; // first item doesn't get leading space

    return *this;
}

int Log::highestLogLevel() const
{
    return _impl->highestLoggingLevel;
}

Log& Log::operator<<(QString const& str)
{
    _impl->addSpace();
    _impl->buffer << str;
    return *this;
}

Log& Log::operator<<(QChar t)
{
    _impl->addSpace();
    _impl->buffer << t;
    return *this;
}

Log& Log::operator<<(bool t)
{
    _impl->addSpace();
    if (t)
    {
        _impl->buffer << "true";
    }
    else
    {
        _impl->buffer << "false";
    }
    return *this;
}

Log& Log::operator<<(char t)
{
    _impl->addSpace();
    _impl->buffer << QString(t);
    return *this;
}

Log& Log::operator<<(signed short i)
{
    _impl->addSpace();
    _impl->buffer << QString::number(i);
    return *this;
}

Log& Log::operator<<(unsigned short i)
{
    _impl->addSpace();
    _impl->buffer << QString::number(i);
    return *this;
}

Log& Log::operator <<(signed int i)
{
    _impl->addSpace();
    _impl->buffer << QString::number(i);
    return *this;
}

Log& Log::operator <<(unsigned int i)
{
    _impl->addSpace();
    _impl->buffer << QString::number(i);
    return *this;
}

Log& Log::operator<<(signed long l)
{
    _impl->addSpace();
    _impl->buffer << QString::number(l);
    return *this;
}

Log& Log::operator<<(unsigned long l)
{
    _impl->addSpace();
    _impl->buffer << QString::number(l);
    return *this;
}

Log& Log::operator<<(qint64 i)
{
    _impl->addSpace();
    _impl->buffer << QString::number(i);
    return *this;
}

Log& Log::operator<<(quint64 i)
{
    _impl->addSpace();
    _impl->buffer << QString::number(i);
    return *this;
}

Log& Log::operator<<(float f)
{
    _impl->addSpace();
    _impl->buffer << QString::number(f);
    return *this;
}

Log& Log::operator<<(double f)
{
    _impl->addSpace();
    _impl->buffer << QString::number(f);
    return *this;
}

Log& Log::operator<<(const char * s)
{
    _impl->addSpace();
    _impl->buffer << QString(s);
    return *this;
}

Log& Log::operator<<(const QStringRef & s)
{
    _impl->addSpace();
    _impl->buffer << s.toString();
    return *this;
}

Log& Log::operator<<(const QLatin1String & s)
{
    _impl->addSpace();
    _impl->buffer << qPrintable(s);
    return *this;
}

Log& Log::operator<<(const QByteArray & b)
{
    _impl->addSpace();
    _impl->buffer << b;
    return *this;
}

Log& Log::operator<<(const QStringList & s)
{
    _impl->addSpace();
    _impl->buffer << "[" << s.join(", ") << "]";
    return *this;
}

Log& Log::operator<<(EndOfLine const& eol)
{
    Q_UNUSED(eol);

    // TODO: format message
    _impl->msg.text = _impl->buffer.join("");
    if (_impl->logControl)
    {
        for (auto handler: _impl->logControl->handlers(_impl->msg.area))
        {
            // TODO: should check log area, level, etc ...
            // TODO: one default formatter, but also possible custom formatters
            handler->process(_impl->msg);
        }
    }

    _impl->buffer.clear();
    return *this;
}

Log& Log::operator <<(NoSpace const& nospace)
{
    Q_UNUSED(nospace);
    _impl->skipSpaceOnFollowing = true;
    return *this;
}

Log& Log::singleton()
{
    static Log singletonImpl;
    return singletonImpl;
}

Log::Log(const Log& other)
{
    // no impl, make compiler happy
    Q_UNUSED(other);
}

Log& Log::operator= (const Log& other)
{
    // no impl, make compiler happy
    Q_UNUSED(other);
    return *this;
}


LogMsg::LogMsg() : level(0), text(""), area(""), sourceFile(""), sourceLine(0)
{
}

LogMsg::LogMsg(const LogMsg &other) :
    level(other.level),
    text(other.text),
    time(other.time),
    area(other.area),
    sourceFile(other.sourceFile),
    sourceLine(other.sourceLine)
{
}

LogMsg& LogMsg::operator =(const LogMsg &other)
{
    this->level = other.level;
    this->area = other.area;
    this->time = other.time;
    this->sourceFile = other.sourceFile;
    this->sourceLine = other.sourceLine;
    this->text = other.text;

    return *this;
}
