#include <testutils/qtgtest.h>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

#include "result.h"
#include "resultmessageformatter.h"
using namespace GBerryLib;

#include "utils/jsonutils.h"

#include "testutils/stringutils.h"

ERRORCLASS(CommunicationError)

static const CommunicationError CONNECTION_ERROR(
        1000, "CONNECTION_FAILED", QT_TRANSLATE_NOOP("Errors", "TXT_Connection failed to #{address}."));

static const CommunicationError CONNECTION_TIMEOUT_ERROR =
        CommunicationError(
            1001,
            "CONNECTION_TIMEOUT",
            QT_TRANSLATE_NOOP("Errors", "TXT_Connection failed because of connection timeout."));

static const CommunicationError INTERNAL_ERROR(
        1002, "INTERNAL_ERROR", "Connection failed to #{address}.");



TEST(ResultMessageFormatter, NoErrorsResult)
{
    Result okResult;
    ResultMessageFormatter formatter(okResult);
    EXPECT_TRUE(formatter.createDeveloperMessage() == "No errors.");
    EXPECT_TRUE(formatter.createEndUserMessage() == "TXT_No errors."); // localization not yet working

    // TODO: should this be in own class
    QJsonObject json = formatter.toJson(); // TODO: this is more like result details
    ASSERT_EQ(2, json.keys().length());
    JsonUtils::debugPrint("RESULT_DETAILS", json);
    ASSERT_JSON_STRING_VALUE(json["error_string"], "No errors.");
    ASSERT_JSON_STRING_VALUE(json["error_message"], "TXT_No errors."); // TODO: localization not yet working
}

TEST(ResultMessageFormatter, SimpleError) {

// -- plain message
    Result result(CONNECTION_TIMEOUT_ERROR);
    ResultMessageFormatter formatter(result);

    ASSERT_TRUE(formatter.createDeveloperMessage() == "Connection failed because of connection timeout.") << formatter.createDeveloperMessage();
    // TODO: at this point we don't really have localization
    ASSERT_TRUE(formatter.createEndUserMessage() == "TXT_Connection failed because of connection timeout.") << formatter.createEndUserMessage();

    QJsonObject json = formatter.toJson();
    ASSERT_TRUE(json.contains("errors"));
    QJsonArray arrayJson = json["errors"].toArray();
    ASSERT_EQ(1, arrayJson.size());
    JsonUtils::debugPrint("ERROR_ARRAY", arrayJson);

    QJsonObject err = arrayJson.first().toObject();
    JsonUtils::debugPrint("FIRST ERROR", err);
    ASSERT_EQ(1001, err["code"].toInt());
    ASSERT_TRUE(err["name"].toString() == "CONNECTION_TIMEOUT");

    ASSERT_TRUE(json["error_string"].toString() == "Connection failed because of connection timeout.") << json["error_string"].toString();
}

TEST(ResultMessageFormatter, ErrorDescriptionWithParams) {

    Result result = Result(CONNECTION_ERROR);
    result << Result::Meta("address", "127.0.0.1");

    ResultMessageFormatter formatter(result);

    ASSERT_TRUE(formatter.createDeveloperMessage() == "Connection failed to 127.0.0.1.") << formatter.createDeveloperMessage();
    // TODO: no real localization yet
    ASSERT_TRUE(formatter.createEndUserMessage() == "TXT_Connection failed to 127.0.0.1.");

    QJsonObject json = formatter.toJson();
    QJsonObject err = json["errors"].toArray().first().toObject();
    ASSERT_TRUE(json["error_string"].toString() == "Connection failed to 127.0.0.1.");

    QJsonArray metas = json["metas"].toArray();
    ASSERT_EQ(1, metas.size());
    QJsonObject meta = metas.first().toObject();
    ASSERT_TRUE(meta["key"] == "address");
    ASSERT_TRUE(meta["value"] == "127.0.0.1");
}

TEST(ResultMessageFormatter, ErrorDescriptionWithTwoParams) {
// test parameter expanding
    Result result = Result(Error(1, "TEST", "TXT_test #{param2} is #{param1} fun."));
    result << Result::Meta("param1", "foo") << Result::Meta("param2", "bar");

    ResultMessageFormatter formatter(result);

    ASSERT_TRUE(formatter.createDeveloperMessage() == "test bar is foo fun.");
    ASSERT_TRUE(formatter.createEndUserMessage() == "TXT_test bar is foo fun.");

    QJsonObject json = formatter.toJson();
    ASSERT_TRUE(json["error_string"].toString() == "test bar is foo fun.");

    QJsonArray metas = json["metas"].toArray();
    ASSERT_EQ(2, metas.size());
}


TEST(ResultMessageFormatter, ErrorWithUnlocalizedReason) {

    Result result = Result(Error(1, "TEST", "Test error"));
    result << Result::reasonFromDesc("Test reason"); // unlocalized

    ResultMessageFormatter formatter(result);

    ASSERT_TRUE(formatter.createDeveloperMessage() == "Test error: Test reason.") << formatter.createDeveloperMessage();
    ASSERT_TRUE(formatter.createEndUserMessage() == "");

    QJsonObject json = formatter.toJson();
    ASSERT_TRUE(json["error_string"].toString() == "Test error: Test reason.");

    QJsonArray reasons = json["reasons"].toArray();
    ASSERT_EQ(1, reasons.size());
    QJsonObject reasonJson = reasons.first().toObject();
    ASSERT_FALSE(reasonJson.contains("code"));
    ASSERT_TRUE(reasonJson["description"] == "Test reason");
}

TEST(ResultMessageFormatter, ErrorWithLocalizedReason) {

    Result result = Result(Error(1, "TEST", "TXT_Test error"));
    result << Result::reasonFromDesc("TXT_Test reason");

    ResultMessageFormatter formatter(result);

    ASSERT_TRUE(formatter.createDeveloperMessage() == "Test error: Test reason.");
    ASSERT_TRUE(formatter.createEndUserMessage() == "TXT_Test error: TXT_Test reason.") << formatter.createEndUserMessage();

    QJsonObject json = formatter.toJson();
    ASSERT_TRUE(json["error_string"].toString() == "Test error: Test reason.");

    QJsonArray reasons = json["reasons"].toArray();
    ASSERT_EQ(1, reasons.size());
    QJsonObject reasonJson = reasons.first().toObject();
    ASSERT_FALSE(reasonJson.contains("code"));
    ASSERT_TRUE(reasonJson["description"] == "Test reason");
}

TEST(ResultMessageFormatter, ErrorWithLocalizedReasonWithParameters) {

    Result result = Result(Error(1, "TEST", "TXT_Test error '#{param1}'"));
    Result::Reason reason = Result::reasonFromDesc("TXT_Test reason '#{param1}', '#{param2}', '#{param3}'");
    reason << Result::Meta("param2", "TXT_param2 value")
           << Result::Meta("param3", "param3 value");

    result << Result::Meta("param1", "TXT_param1 value")
           << reason;

    ResultMessageFormatter formatter(result);
    ASSERT_QSTRING_EQ(formatter.createDeveloperMessage(), "Test error 'param1 value': Test reason 'param1 value', 'param2 value', 'param3 value'.");
    ASSERT_QSTRING_EQ(formatter.createEndUserMessage(), "TXT_Test error 'TXT_param1 value': TXT_Test reason 'TXT_param1 value', 'TXT_param2 value', 'param3 value'.");

    //ASSERT_TRUE(formatter.createDeveloperMessage() == "Test error 'param1 value': Test reason 'param1 value', 'param2 value', 'param3 value'.") << formatter.createDeveloperMessage();
    //ASSERT_TRUE(formatter.createEndUserMessage() == "TXT_Test error 'TXT_param1 value': TXT_Test reason 'TXT_param1 value', 'param2 value', 'TXT_param3 value'.") << formatter.createEndUserMessage();
}

TEST(ResultMessageFormatter, ErrorWithReasonWithCode) {

    Result result = Result(Error(1, "TEST", "TXT_Test error"));
    result << Result::reasonFromCode("TEST_REASON", "Test reason"); // unlocalized

    ResultMessageFormatter formatter(result);

    ASSERT_QSTRING_EQ(formatter.createDeveloperMessage(), "Test error: Test reason.");
    ASSERT_QSTRING_EQ(formatter.createEndUserMessage(), "TXT_Test error.");

    QJsonObject json = formatter.toJson();
    ASSERT_TRUE(json["error_string"].toString() == "Test error: Test reason.");

    QJsonArray reasons = json["reasons"].toArray();
    ASSERT_EQ(1, reasons.size());
    QJsonObject reasonJson = reasons.first().toObject();
    JsonUtils::debugPrint("REASON", reasonJson);
    ASSERT_JSON_STRING_VALUE(reasonJson["code"], "TEST_REASON");
    ASSERT_JSON_STRING_VALUE(reasonJson["description"], "Test reason");
}

TEST(ResultMessageFormatter, ErrorWithTwoReasons) {

    Result result = Result(Error(1, "TEST", "TXT_Test error"));
    result << Result::reasonFromCode("TEST_REASON", "Test reason")
           << Result::reasonFromCode("TEST_REASON2", "Test reason2");

    ResultMessageFormatter formatter(result);

    ASSERT_QSTRING_EQ(formatter.createDeveloperMessage(), "Test error: Test reason, Test reason2.");
    ASSERT_QSTRING_EQ(formatter.createEndUserMessage(), "TXT_Test error.");

    QJsonObject json = formatter.toJson();
    ASSERT_JSON_STRING_VALUE(json["error_string"], "Test error: Test reason, Test reason2.");

    QJsonArray reasons = json["reasons"].toArray();
    ASSERT_EQ(2, reasons.size());
}

TEST(ResultMessageFormatter, ComplexError) {

    Result result = Result(Error(1, "TEST", "TXT_Test error"));
    result << Result::reasonFromCode("TEST_REASON", "Test reason")
           << Result::reasonFromCode("TEST_REASON2", "TXT_Test reason2")
           << Error(2, "TEST2", "TXT_Test error2");

    Result subresult = Result(Error(10, "SUBTEST", "TXT_Subtest error."));
    subresult << Result::reasonFromCode("SUBTEST_REASON", "Subtest reason")
              << Result::reasonFromCode("SUBTEST_REASON2", "TXT_Subtest reason2");

    result << subresult;

    ResultMessageFormatter formatter(result);

    // TODO: problem in design, reasons are for result not for error -> should be changed
    //ASSERT_QSTRING_EQ(formatter.createDeveloperMessage(), "Test error: Test reason, Test reason2. Subtest reason: Subtest reason, Subtest reason2.");
    ASSERT_QSTRING_EQ(formatter.createDeveloperMessage(), "Test error. Test error2: Test reason, Test reason2. Subtest error: Subtest reason, Subtest reason2.");

    ASSERT_QSTRING_EQ(formatter.createEndUserMessage(), "TXT_Test error. TXT_Test error2: TXT_Test reason2. TXT_Subtest error: TXT_Subtest reason2.");
    //ASSERT_QSTRING_EQ(formatter.createEndUserMessage(), "TXT_Test error: TXT_Test reason2. TXT_Subtest reason: TXT_Subtest reason2.");
}


// TODO: multiple errors
// TODO: reason with parameters
// TODO: reason with parameters from parent
// TODO: two reasons
// TODO: subresult, two subresults
// TODO: reason + subresult
// TODO: several: reason + subresult
