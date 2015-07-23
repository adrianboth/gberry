#include <testutils/qtgtest.h>

#include "result.h"
using namespace GBerryLib;

ERRORCLASS(CommunicationError)

/*
class CommunicationError : public GBerryLib::Error {
public:
    CommunicationError(uint code) : GBerryLib::Error(code) {}
    CommunicationError(uint code, const QString& name) : GBerryLib::Error(code, name) {}
    CommunicationError(uint code, const QString& name, const QString& description) : GBerryLib::Error(code, name, description) {}
    CommunicationError(uint code, const QString& name, const QString& description, const QString& l10nContext) : GBerryLib::Error(code, name, description, l10nContext) {}
    virtual ~CommunicationError() {}
};
*/

static const CommunicationError CONNECTION_ERROR(
        1000, "CONNECTION_FAILED", QT_TRANSLATE_NOOP("Errors", "TXT_Connection failed to #{address}"));

//static const CommunicationError CONNECTION_TIMEOUT_ERROR = CommunicationError("CONNECTION_FAILED") << Error::Reason("TIMEOUT") << ErrorDescription("Connection failed because of connection timeout.");
static const CommunicationError CONNECTION_TIMEOUT_ERROR =
        CommunicationError(
            1001,
            "CONNECTION_TIMEOUT",
            QT_TRANSLATE_NOOP("Errors", "TXT_Connection failed because of connection timeout."));


TEST(Errors, ErrorGetters)
{
    EXPECT_TRUE(CONNECTION_ERROR.code() == 1000);
    EXPECT_TRUE(CONNECTION_ERROR.name() == "CONNECTION_FAILED");
    EXPECT_TRUE(CONNECTION_ERROR.description() == "Connection failed to #{address}") << CONNECTION_ERROR.description();
    EXPECT_TRUE(CONNECTION_ERROR.localizable());
    EXPECT_TRUE(CONNECTION_ERROR.errorL10nContext() == "Errors");
    EXPECT_TRUE(CONNECTION_ERROR.errorL10nKey() == "TXT_Connection failed to #{address}");

// -- unlocalized
    Error testErr(1, "TEST", "Test error");
    EXPECT_TRUE(testErr.code() == 1);
    EXPECT_TRUE(testErr.name() == "TEST");
    EXPECT_TRUE(testErr.description() == "Test error");
    EXPECT_FALSE(testErr.localizable());
    EXPECT_TRUE(testErr.errorL10nContext() == "Errors");
    EXPECT_TRUE(testErr.errorL10nKey() == "");

// localized with different context
    Error testErr2(1, "TEST", QT_TRANSLATE_NOOP("TestCtx", "TXT_Test error"), "TestCtx");
    EXPECT_TRUE(testErr2.description() == "Test error");
    EXPECT_TRUE(testErr2.localizable());
    EXPECT_TRUE(testErr2.errorL10nContext() == "TestCtx");
    EXPECT_TRUE(testErr2.errorL10nKey() == "TXT_Test error");
}

TEST(Errors, CreateResult)
{

// -- no errors
    Result okResult;
    ASSERT_FALSE(okResult.hasErrors());
    ASSERT_FALSE(okResult.hasSubresults());
    ASSERT_EQ(0, okResult.errors().length());
    ASSERT_EQ(0, okResult.subresults().length());

    ASSERT_TRUE(okResult.metas().size() == 0);
    ASSERT_FALSE(okResult.hasMeta("address"));
    ASSERT_EQ(0, okResult.reasons().length());

// --
    Result result(CONNECTION_TIMEOUT_ERROR);
    EXPECT_TRUE(result.hasErrors());
    EXPECT_EQ(1, result.errors().length());
    EXPECT_EQ(0, result.subresults().length());
    EXPECT_TRUE(result.errors().first().description() == "Connection failed because of connection timeout.") << result.errors().first().description();

    Error err = result.errors().first();
    EXPECT_TRUE(err.name() == "CONNECTION_TIMEOUT") << err.name();
    EXPECT_TRUE(err.description() == "Connection failed because of connection timeout.");
    EXPECT_TRUE(err.errorL10nKey() == "TXT_Connection failed because of connection timeout.");
    EXPECT_TRUE(err.errorL10nContext() == "Errors");
    EXPECT_TRUE(err.localizable());

// -- result with meta data
    result = Result(CONNECTION_ERROR) << Result::Meta("address", "127.0.0.1");
    EXPECT_TRUE(result.metas().size() == 1);
    EXPECT_TRUE(result.hasMeta("address"));
    EXPECT_TRUE(result.metaValue("address") == "127.0.0.1");
    EXPECT_FALSE(result.meta("address").localizable());

// -- result with reason added afterwards
    QString address = "127.0.0.1";

    result = Result(CONNECTION_ERROR)
            << Result::reasonFromCode("INTERNAL_ERROR", "Internal error")
            << Result::Meta("address", address);

    EXPECT_TRUE(result.hasReasons());
    EXPECT_TRUE(result.reasons().length() == 1);

    Result::Reason reason = result.reasons().first();
    EXPECT_TRUE(reason.description() == "Internal error");
    EXPECT_FALSE(reason.localizable());
    EXPECT_TRUE(reason.l10nKey() == "");

// -- subresult
    result = Result(CONNECTION_ERROR)
            << Result::Meta("address", address)
            << Result(CONNECTION_TIMEOUT_ERROR);

    EXPECT_TRUE(result.hasSubresults());
    EXPECT_EQ(1, result.subresults().length());

    Result subresult = result.subresults().first();
    EXPECT_TRUE(subresult.errors().first().name() == "CONNECTION_TIMEOUT");
}
