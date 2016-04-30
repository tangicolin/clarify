# Clarify

Clarify is a BDD-style unit test framework for C. It allows you to write Given-When-Then style unit tests by including a single header file.

## Features

- BDD test names in plain text (no more gigantic test function names!).
- It's just C.
- Small and lightweight.
- No test-running boiler plate required.
- No long jump required.

## How to use it

Just copy the [clarify.h](https://raw.githubusercontent.com/ElectronVector/clarify/master/clarify.h) header file somewhere into your project, and then create test files like this:

```c
#include "clarify.h"

RUN_TESTS()
{
    GIVEN("Some initial state")
    {
        WHEN("something happens")
        {
            THEN("the expected result occurs")
            {
                REQUIRE(expected == actual);
            }
        }
        WHEN("something else happens")
        {
            THEN("the another expected result occurs")
            {
                REQUIRE(expected == actual);
            }
        }
    }

    GIVEN("Some other initial state")
    {
        WHEN("something happens")
        {
            THEN("the expected result occurs")
            {
                REQUIRE(expected == actual);
            }
        }
    }
}
```

The `RUN_TESTS()` macro provides a `main()` for your application, so you can run your test file by compiling it with **GCC**. No test runners or boiler plate required!

```
gcc -W -Wall my_test_file.c file_under_test.c -o my_test
```

Then execute your tests by running the binary:

```
./my_test
```

## Nesting tests (and changing state)

For each WHEN or THEN clause, the parent GIVEN and/or WHEN clauses are re-run, so that the initial state is reset before each test is run. This means that no "setup" or "teardown" functions are necessary -- which are common in other types of test frameworks.


## Test Anything Procotol output
Ouput are TAP compliant. See TAP website for more information ([https://testanything.org/])

Example:
```
#---------------------------------------------------
# test_examples.c: Running Tests...
#---------------------------------------------------
ok 1 - GIVEN some integers
	WHEN I test the ones that are equal
	THEN the test passes
not ok 2 - GIVEN some integers
	WHEN I test the ones that are not equal
	THEN the test fails
 ---
  FAIL:test_examples.c:17
  Expected:0xffffff9d
  Actual:0x63
 ---
ok 3 - GIVEN some unsigned integers
	WHEN I test the ones that are equal
	THEN the test passes
not ok 4 - GIVEN some unsigned integers
	WHEN I test the ones that are not equal
	THEN the test fails
 ---
  FAIL:test_examples.c:33
  Expected:0xffffff9d
  Actual:0x64
 ---
ok 5 - GIVEN some bytes
	WHEN I test the ones that are equal
	THEN the test passes
not ok 6 - GIVEN some bytes
	WHEN I test the ones that are not equal
	THEN the test fails
 ---
  FAIL:test_examples.c:49
  Expected:0x99
  Actual:0xa0
 ---
ok 7 - GIVEN some strings
	WHEN I test the ones that are equal
	THEN the test passes
not ok 8 - GIVEN some strings
	WHEN I test the ones that are not equal
	THEN the test fails
 ---
  FAIL:test_examples.c:65
  Expected:the quick brown fox
  Actual:jumps over the
 ---
ok 9 - GIVEN some arrays
	WHEN I test the ones that are equal
	THEN the test passes
not ok 10 - GIVEN some arrays
	WHEN I test the ones that are not equal
	THEN the test fails
 ---
  FAIL:test_examples.c:81
 ---
#Failed: 5
1..10
```

## Other REQUIRE tests

The plain-old REQUIRE test doesn't print the expected or actual values if there is an error. If you'd like to print these values, you can use one of these type specific tests:

- `REQUIRE_EQUAL_INT(expected, actual)`
- `REQUIRE_EQUAL_UINT(expected, actual)`
- `REQUIRE_EQUAL_BYTE(expected, actual)`

If you'd like something more specific, you can use the raw format function:

- `REQUIRE_EQUAL_FORMAT(expected, actual, format)`

The `format` is a printf-style format, e.g. to compare characters, you would use `%c`.

There's also test for strings:

- `REQUIRE_EQUAL_STRING(expected, actual)`

And one for memory:

- `REQUIRE_EQUAL_MEMORY(expected, actual, size)`

## Example

There is an example in [examples/power_button](https://github.com/ElectronVector/clarify/tree/master/examples/power_button). You can use `rake` to run the tests (requires installation of Ruby and Rake).

## Limitations

- The Clarify macros use nested functions, and so require GCC for compilation.
- You can only have a single `RUN_TESTS()` macro per test file.
- To define a test, you must have at least one `GIVEN()`, `WHEN()`, and `THEN()` clause.
- Each `THEN()` stops after the first REQUIRE fails. Other tests continue to run.

Warning: Watch out for stray semicolons at then end of your statements. For example:
```c
    WHEN("something happens"); // <-- This semicolon will break your tests.
```
This will break your tests in non-obvious ways. I recommend compiling with -W -Wall flags to warn on these errors.

Don't put any code in the places indicated below. It won't be run.

```c
#include "clarify.h"

RUN_TESTS()
{
    GIVEN("Some initial state")
    {
        WHEN("something happens")
        {
            THEN("the expected result occurs")
            {
                REQUIRE(expected == actual);
            }
            // ** Don't put code here.
        }
        // ** Don't put code here.
    }
}
```
