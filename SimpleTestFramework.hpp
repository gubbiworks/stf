/*
 * SimpleTestFramework.hpp
 *
 *  Created on: 14-Apr-2016
 *      Author: gubbiworks
 */

#ifndef SIMPLETESTFRAMEWORK_HPP_
#define SIMPLETESTFRAMEWORK_HPP_

// ----- Abbreviations -----
// STF - Simple Test Framework


// ----- Headers -----
#include <stdio.h>
#include <stdlib.h>  /* For exit() function */
#include <string>
#include <time.h>

using namespace std;

// ----- Interfaces -----
	/*
	 * ANSI Escape console color codes
	 * Define (#define STF_COLOR_TEXT) STF_COLOR_TEXT to enable colored output
	 */
	//#define STF_COLOR_TEXT	//Configurable parameter?
	#ifdef STF_COLOR_TEXT
		#define ANSI_COLOR_RED     "\x1b[31m"
		#define ANSI_COLOR_GREEN   "\x1b[32m"
		//#define ANSI_COLOR_YELLOW  "\x1b[33m"
		//#define ANSI_COLOR_BLUE    "\x1b[34m"
		//#define ANSI_COLOR_MAGENTA "\x1b[35m"
		//#define ANSI_COLOR_CYAN    "\x1b[36m"
		#define ANSI_COLOR_RESET   "\x1b[0m"
	#else
		#define ANSI_COLOR_RED     ""
		#define ANSI_COLOR_GREEN   ""
		//#define ANSI_COLOR_YELLOW  ""
		//#define ANSI_COLOR_BLUE    ""
		//#define ANSI_COLOR_MAGENTA ""
		//#define ANSI_COLOR_CYAN    ""
		#define ANSI_COLOR_RESET   ""
	#endif


	static string _name = "";	//Module name that is being tested
	static string _current_testcase_name = "";	//Current test case name

	static long _testcase_count = 0;	//Total number of Test cases
	static long _testcase_pass_count = 0;	//Total test cases passed
	static long _testcase_fail_count = 0;	//Total test cases failed


	static clock_t _start_clock;	//cpu clock at test case start
	static clock_t _end_clock;	//cpu clock at test case end
	//time taken for current test case execution
	static double _current_test_time = 0.0;
	//total execution time for all test cases
	static double _total_test_time = 0.0;
	//total execution time for all passed test cases
	static double _total_pass_test_time = 0.0;
	//total execution time for all failed test cases
	static double _total_fail_test_time = 0.0;


	//Current local time from epoch
	static time_t current_time = (time_t)-1;
	//Current local time as string
	static const char* c_time_string = NULL;

	//Text report, same data as on console
	static const char * stf_reportname = "stfreport.txt";
	static FILE * fptr = NULL;	//report file handle
	//Configurable parameter?, Set this to true generate report as text file
	static bool generate_report = false;

	void _stfopenfile(bool header = false) {

		if(!generate_report)
			return;

		fptr = fopen(stf_reportname,"a+");
		if(fptr == NULL){
		  printf("Error! opening file");
		  exit(1);
		}
		if(header)
			fprintf(fptr, "\n\t-----");
	}

	void _stfclosefile(bool header = false) {
		if(!generate_report)
			return;

		if(fptr != NULL)
		{
			if(header)
				fprintf(fptr, "\n\t-----");
			fclose(fptr);
		}
		fptr = NULL;
	}


	/*
	 * Print header before execution of any test case.
	 * Used by RunTest macro
	 */
	void _print_header(const char *testname)
	{
		current_time = time(NULL);
		if (current_time != ((time_t)-1))
		{
			/* Convert to local time format. */
			c_time_string = ctime(&current_time);
			if (c_time_string == NULL)
			{
				c_time_string = "Time: NA";
			}
		}
		else
		{
			c_time_string = "Time: NA";
		}

		printf("\n");
		printf("Test name: %s, %s\n", testname, c_time_string);

		_stfopenfile(true);
		if(fptr != NULL)
		{
			fprintf(fptr, "\n");
			fprintf(fptr, "Test name: %s, %s\n", testname, c_time_string);
		}
		_stfclosefile();
	}

	/*
	 * Assign test name and print the header
	 * rest of user code is retained as is and executed
	 */
	#define RunTest(TEST_NAME)\
		_name.assign(#TEST_NAME);\
		_print_header(#TEST_NAME);

	//Shorter macro for RunTest
	#define RT(TEST_NAME)\
			RunTests(TEST_NAME);

	/*
	 * Function called by user to define the test name
	 * and test condition to be tested.
	 */
	#define stftest(TESTCASE_DESCRIPTION, TEST_CONDITION)\
			_current_testcase_name.assign(#TESTCASE_DESCRIPTION);\
			_startclock();\
			if( (TEST_CONDITION) ){\
				_endclock();\
				_passed();\
			} else {\
				_endclock();\
				_failed();\
			}\
			_stftestcase_summary();

	/*
	 * Start the clock and store at start of test
	 */
	void _startclock()
	{
		_start_clock = clock();
	}

	/*
	 * Stop clock and store value, calculate time taken in seconds
	 */
	void _endclock()
	{
		_end_clock = clock();
		_current_test_time = (double)(_end_clock - _start_clock) / CLOCKS_PER_SEC;
	}

	/*
	 * Function called when test condition is successfully met
	 */
	void _passed()
	{
		_total_pass_test_time += _current_test_time;
		_testcase_pass_count++;
		printf("%s\tPass, %s, Time: %.4f sec%s\n",\
				ANSI_COLOR_GREEN, _current_testcase_name.c_str(),\
				_total_pass_test_time, ANSI_COLOR_RESET);

		_stfopenfile();
		if(fptr != NULL)
			fprintf(fptr, "\tPass, %s, Time: %.4f sec\n",\
					_current_testcase_name.c_str(),\
					_total_pass_test_time);
		_stfclosefile();
	}

	/*
	 * Function called when test condition fails
	 */
	void _failed()
	{
		_total_fail_test_time += _current_test_time;
		_testcase_fail_count++;
		printf("%s\tFail, %s, Time: %.4f sec%s\n",\
				ANSI_COLOR_RED, _current_testcase_name.c_str(),\
				_total_fail_test_time, ANSI_COLOR_RESET);

		_stfopenfile();
		if(fptr != NULL)
			fprintf(fptr, "\tFail, %s, Time: %.4f sec\n",\
					_current_testcase_name.c_str(),\
					_total_fail_test_time);
		_stfclosefile();
	}

	/*
	 * Function called on completion of each test case
	 */
	void _stftestcase_summary()
	{
		_total_test_time += _current_test_time;
		_testcase_count++;
	}

	/*
	 * Called by user to print total count of test cases and time taken
	 * at the end of testing
	 */
	void stftest_summary()
	{
		printf("\n");
		printf("Total test cases: %ld, Time: %.2f sec\n",\
				_testcase_count, _total_test_time);
		printf("Total PASSED cases: %ld, Time: %.4f sec\n",\
				_testcase_pass_count, _total_pass_test_time);
		printf("Total FAILED cases: %ld, Time: %.4f sec\n",\
				_testcase_fail_count, _total_fail_test_time);

		_stfopenfile();
		if(fptr != NULL)
		{
			fprintf(fptr, "\n");
			fprintf(fptr, "Total test cases: %ld, Time: %.2f sec\n",\
					_testcase_count, _total_test_time);
			fprintf(fptr, "Total PASSED cases: %ld, Time: %.4f sec\n",\
					_testcase_pass_count, _total_pass_test_time);
			fprintf(fptr, "Total FAILED cases: %ld, Time: %.4f sec\n",\
					_testcase_fail_count, _total_fail_test_time);
		}
		_stfclosefile(true);

	}

#endif /* SIMPLETESTFRAMEWORK_HPP_ */
