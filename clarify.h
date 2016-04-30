/*
 Clarify is a BDD-style unit testing framework for C.
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define PRINT_OK(nb,given,when,then) printf("ok %d - GIVEN %s\n\tWHEN %s\n\tTHEN %s\n",nb,given,when,then)

#define PRINT_KO(nb,given,when,then) printf("not ok %d - GIVEN %s\n\tWHEN %s\n\tTHEN %s\n ---\n  FAIL:%s:%d\n ---\n",nb,given,when,then,__FILE__, __LINE__)

#define PRINT_KO_FORMAT_STRING(nb,given,when,then,expected,actual) printf("not ok %d - GIVEN %s\n\tWHEN %s\n\tTHEN %s\n ---\n  FAIL:%s:%d\n  Expected:%s\n  Actual:%s\n ---\n",nb,given,when,then,__FILE__, __LINE__,expected,actual)

#define PRINT_KO_FORMAT_HEX(nb,given,when,then,expected,actual) printf("not ok %d - GIVEN %s\n\tWHEN %s\n\tTHEN %s\n ---\n  FAIL:%s:%d\n  Expected:0x%x\n  Actual:0x%x\n ---\n",nb,given,when,then,__FILE__, __LINE__,expected,actual)

/*
    Require that the expression evaluates to true.
*/
#define REQUIRE(boolean_test) \
    do { \
        if (!(boolean_test)) { \
            test_count_failed++; \
            testNumber++; \
            PRINT_KO(testNumber,this_test.given,this_test.when,this_test.then); \
            return; \
        } \
    } while (0)

#define REQUIRE_EQUAL_FORMAT(expected, actual, format_string)       \
    do {                                                            \
        if (!((expected) == (actual))) {                            \
            test_count_failed++;                                    \
            testNumber++; \
            PRINT_KO_FORMAT_HEX(testNumber,this_test.given,this_test.when,this_test.then,expected,actual); \
            return;                                                 \
        }                                                           \
    } while (0)


#define REQUIRE_EQUAL_INT(expected, actual) REQUIRE_EQUAL_FORMAT(expected, actual, %d)
#define REQUIRE_EQUAL_UINT(expected, actual) REQUIRE_EQUAL_FORMAT(expected, actual, %u)
#define REQUIRE_EQUAL_BYTE(expected, actual) REQUIRE_EQUAL_FORMAT(expected, actual, 0x%02x)

#define REQUIRE_EQUAL_STRING(expected, actual)                      \
    do {                                                            \
        if (strcmp(expected, actual) != 0) {                        \
            test_count_failed++;                                    \
            testNumber++;\
            PRINT_KO_FORMAT_STRING(testNumber,this_test.given,this_test.when,this_test.then,expected,actual); \
            return;                                                 \
        }                                                           \
    } while (0)

#define REQUIRE_EQUAL_MEMORY(expected_ptr, actual_ptr, size)    \
do {                                                            \
    if (memcmp(expected_ptr, actual_ptr, size) != 0) {          \
        test_count_failed++;                                    \
        testNumber++; \
        PRINT_KO(testNumber,this_test.given,this_test.when,this_test.then); \
        return;                                                 \
    }                                                           \
} while (0)
/*
    State maintined by clarify during the execution of tests.
*/
typedef struct {
    char * given;                   // The strings describing each
    char * when;
    char * then;
    int starting_line_number;       // The starting line number of this
                                    // test (where the GIVEN statement is).
    bool test_executed_this_pass;   // True when a full Given-When-Then test has
                                    // been run on this pass through the function.
    bool skip_this_clause;          // Temp flag used to skip a clause.

    int current_when_line;          // The starting line of the WHEN clause
                                    // currently being executed.
    int last_then_line_executed;    // The starting line of the most recent THEN
                                    // clause to be executed.
} TEST_CASE;

static int testNumber = 0;


// Create a unique test function name for each test function, based on the line
// number. We need two levels of indirection to get the line number to be
// evaluated during the string concatentation.
// http://stackoverflow.com/questions/1597007/creating-c-macro-with-and-line-token-concatenation-with-positioning-macr/1597129
#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define UNIQUE_TEST_FUNCTION_NAME TOKENPASTE2(test_, __LINE__)

#define PRINT_HEADER() \
    printf("#---------------------------------------------------\n"); \
    printf("# %s: Running Tests...\n", __FILE__); \
    printf("#---------------------------------------------------\n"); \

#define PRINT_RESULTS() \
    printf("#Failed: %d\n", test_count_failed); \
    printf("1..%d\n",test_count_total); \

#define RUN_TESTS() \
    TEST_CASE this_test; \
    int test_count_total = 0; \
    int test_count_failed = 0; \
    int previous_test_count_failed = 0;\
    void all_tests(void); \
    int main(void) {\
        PRINT_HEADER(); \
        all_tests(); \
        PRINT_RESULTS(); \
        return test_count_failed; \
    }\
    void all_tests(void)

#define GIVEN(precondition) \
    auto void UNIQUE_TEST_FUNCTION_NAME (void); \
    this_test.starting_line_number = __LINE__; \
    this_test.given = precondition; \
    this_test.last_then_line_executed = 0; \
    this_test.current_when_line = 0; \
    do { \
        this_test.test_executed_this_pass = false; \
        UNIQUE_TEST_FUNCTION_NAME(); \
    } while (this_test.test_executed_this_pass); \
            if(test_count_failed > previous_test_count_failed)\
            { \
                previous_test_count_failed = test_count_failed; \
            } \
            else \
            { \
                testNumber++; \
                PRINT_OK(testNumber,this_test.given,this_test.when,this_test.then); \
            } \
    void UNIQUE_TEST_FUNCTION_NAME (void)

#define WHEN(condition) \
    this_test.skip_this_clause = false; \
    if (this_test.test_executed_this_pass) \
    { \
        /* We just completed the last WHEN, switch to this when.*/ \
        if(test_count_failed > previous_test_count_failed)\
        { \
                previous_test_count_failed = test_count_failed; \
        } \
        else \
        { \
            testNumber++; \
            PRINT_OK(testNumber,this_test.given,this_test.when,this_test.then); \
        } \
        this_test.current_when_line = __LINE__; \
        return; \
    } \
    if ((this_test.current_when_line == 0) || (this_test.current_when_line == __LINE__)) \
    { \
        /* This is current when clause, so we're going to run it. */ \
        this_test.when = condition; \
    } \
    else \
    { \
        /* This is not the current WHEN clause... so don't run it. */ \
        this_test.skip_this_clause = true; \
    } \
    if (!this_test.skip_this_clause)

#define THEN(result) \
    this_test.skip_this_clause = false; \
    if (this_test.test_executed_this_pass) \
    { \
        return; \
    } \
    else if (this_test.last_then_line_executed >= __LINE__) \
    { \
        /*We've already run this test, skip it. */ \
        this_test.skip_this_clause = true; \
    } \
    else \
    { \
        /* We are executing this THEN clause this pass. */ \
        this_test.last_then_line_executed = __LINE__; \
        this_test.test_executed_this_pass = true; \
    } \
    if (!this_test.skip_this_clause) \
    { \
        this_test.then = result; \
        test_count_total++; \
    } \
    if (!this_test.skip_this_clause)
