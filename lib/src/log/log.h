#ifndef LOG_H
#define LOG_H

// TODO: stream operator requires this; not directly but something
#include <QDebug>

#define LOG_TRACE(__log, __msg) if (__log.highestLogLevel() >= Log::TRACE) { __log.begin(Log::TRACE, LOG_AREA, __FILE__, __LINE__) << __msg << Log::eol; }
#define LOG_DEBUG(__log, __msg) if (__log.highestLogLevel() >= Log::DEBUG) { __log.begin(Log::DEBUG, LOG_AREA, __FILE__, __LINE__) << __msg << Log::eol; }
#define LOG_INFO(__log, __msg) if (__log.highestLogLevel() >= Log::INFO)  { __log.begin(Log::INFO,  LOG_AREA, __FILE__, __LINE__) << __msg << Log::eol; }
#define LOG_WARN(__log, __msg) if (__log.highestLogLevel() >= Log::WARN) { __log.begin(Log::WARN, LOG_AREA, __FILE__, __LINE__) << __msg << Log::eol; }
#define LOG_ERROR(__log, __msg) if (__log.highestLogLevel() >= Log::ERROR) { __log.begin(Log::ERROR, LOG_AREA, __FILE__, __LINE__) << __msg << Log::eol; }


#define TRACE(__msg) LOG_TRACE(Log::singleton(), __msg);
#define DEBUG(__msg) LOG_DEBUG(Log::singleton(), __msg);
#define INFO(__msg) LOG_INFO(Log::singleton(), __msg);
#define WARN(__msg) LOG_WARN(Log::singleton(), __msg);
#define ERROR(__msg) LOG_ERROR(Log::singleton(), __msg);

#define TRACE_FUNC() LOG_TRACE(Log::singleton(), __func__ << Log::nospace << "()"); // requires C+11
#define TRACE_FUNC2(__msg) LOG_TRACE(Log::singleton(), __func__ << Log::nospace << "():" << __msg);

// TODO: impl no space marker
#define TRACE_FUNC_IN() LOG_TRACE(Log::singleton(), "IN:" << __func__ << Log::nospace << "()");
#define TRACE_FUNC_OUT() LOG_TRACE(Log::singleton(), "OUT:" << __func__ << Log::nospace << "()");

// user of logging can define their own log area
#ifndef LOG_AREA
  #define LOG_AREA "Default"
#endif
/**
 * @brief Developer logging interface
 *
 * Log class provides
 * TODO:
 *
 * log for mobile application
 *  - possible to direct to file (on desktop)
 *  - possible to send over network (on background), together with activity log
 *  - by default console logging (happens right away)
 *  - module information in prefixes (+ line number and file) -> macro
 *  - lightweight when disabled (macro)
 *  - logging level
 *  - can be enabled/disabled overnetwork (control center)
 *  -
 **/

//namespace geektools
//{

class EndOfLine {};
class NoSpace {};
class PrivateImpl;

// forward declaration
class LogControl;

/**
 * @brief The Log class
 */
class Log
{
public:
    Log();
    ~Log();

    // higher log level -> more messages
    enum { NONE = 0, ERROR = 20, WARN = 40, INFO = 60, DEBUG = 80, TRACE = 100, ALL = 150 };
    static const EndOfLine eol;
    static const NoSpace nospace;

    int highestLogLevel() const;
    void use(LogControl* logControl);
    void reset();

    Log& begin(const int& level, const char* area, const char* file, const int& lineno);

    Log& operator<<( QString const& str );
    Log& operator<<( QChar t );
    Log& operator<<( bool t );
    Log& operator<<( char t );
    Log& operator<<( signed short i );
    Log& operator<<( unsigned short i );
    Log& operator<<( signed int i );
    Log& operator<<( unsigned int i );
    Log& operator<<( signed long l );
    Log& operator<<( unsigned long l );
    Log& operator<<( qint64 i );
    Log& operator<<( quint64 i );
    Log& operator<<( float f );
    Log& operator<<( double f );
    Log& operator<<( const char * s );
    Log& operator<<( const QStringRef & s );
    Log& operator<<( const QLatin1String & s );
    Log& operator<<( const QByteArray & b );

    Log& operator<<(EndOfLine const& eol);
    Log& operator<<(NoSpace const& nospace);

    static Log& singleton();

private:
    // prevent use
    Log(const Log& other);
    Log& operator= (const Log& other);

    PrivateImpl* _impl;
};

class LogMsg
{

public:
    LogMsg();
    LogMsg(const LogMsg& other);
    LogMsg& operator= (const LogMsg& other);

    int level;
    QString text;
    qint64 time;
    QString area;
    QString sourceFile;
    int sourceLine;
};

//} // of namespace

#endif // LOG_H
