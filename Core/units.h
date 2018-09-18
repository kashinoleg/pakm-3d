#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>

#include <string>
#include <vector>
#include <iostream>
using namespace std;


class Unit {
public:
	string name;
	double factor;
	bool primitive;
	Unit *numerator;
	Unit *denominator;
	Unit *next;

	Unit() {
		name = "";
		factor = 1.0;
		primitive = false;
		numerator = NULL;
		denominator = NULL;
		next = NULL;
	}

	~Unit() {
		Unit *u1;
		Unit *u2;
		//free(u->name);
		u1 = numerator;
		while (u1 != NULL) {
			u2 = u1->next;
			delete u1;
			u1 = u2;
		}
		u1 = denominator;
		while (u1 != NULL) {
			u2 = u1->next;
			delete u1;
			u1 = u2;
		}
	}
};

class UnitPrefix {
public:
	/// <summary>
	/// Имя префикса.
	/// </summary>
	string name;
	/// <summary>
	/// Множитель префикса.
	/// </summary>
	double factor;

	/// <summary>
	/// Конструктор.
	/// </summary>
	/// <param name="setName">имя префикса</param>
	/// <param name="setFactor">множитель префикса</param>
	UnitPrefix(string setName, double setFactor) {
		name = setName;
		factor = setFactor;
	}
};


class UnitConvert {
private:
	string errorMsg;
	bool error = false;

	/*  Load the Prefix List  */
	vector<UnitPrefix> prefixs = {
		UnitPrefix("yotta", 1e24),
		UnitPrefix("zetta", 1e21),
		UnitPrefix("exa",	1e18),
		UnitPrefix("peta",	1e15),
		UnitPrefix("tera",	1e12),
		UnitPrefix("giga",	1e9),
		UnitPrefix("mega",	1e6),
		UnitPrefix("kilo",	1e3),
		UnitPrefix("hecto", 1e2),
		UnitPrefix("deka",	1e1),
		UnitPrefix("deca",	1e1),
		UnitPrefix("deci",	1e-1),
		UnitPrefix("centi",	1e-2),
		UnitPrefix("milli",	1e-3),
		UnitPrefix("micro",	1e-6),
		UnitPrefix("nano",	1e-9),
		UnitPrefix("pico",	1e-12),
		UnitPrefix("femto", 1e-15),
		UnitPrefix("atto",	1e-18),
		UnitPrefix("zepto",	1e-21),
		UnitPrefix("yocto",	1e-24),
		UnitPrefix("Y",		1e24),
		UnitPrefix("Z",		1e21),
		UnitPrefix("E",		1e18),
		UnitPrefix("P",		1e15),
		UnitPrefix("T",		1e12),
		UnitPrefix("G",		1e9),
		UnitPrefix("M",		1e6),
		UnitPrefix("k",		1e3),
		UnitPrefix("h",		1e2),
		UnitPrefix("da",	1e1),
		UnitPrefix("d",		1e-1),
		UnitPrefix("c",		1e-2),
		UnitPrefix("m",		1e-3),
		UnitPrefix("u",		1e-6),
		UnitPrefix("n",		1e-9),
		UnitPrefix("p",		1e-12),
		UnitPrefix("f",		1e-15),
		UnitPrefix("a",		1e-18),
		UnitPrefix("z",		1e-21),
		UnitPrefix("y",		1e-24)
	};
public:
	/*
	 *-------------------------------------------------------------------
	 *
	 * units_new --
	 *
	 *  Create a new unit conversion
	 *
	 * Results:
	 *      Creates a new unit conversion with the specified name. The new
	 *      unit name must be only alphabetic (upper or lower case)
	 *      letters.  The baseUnits string can consist of any valid units
	 *      conversion string, including constant factors, numerator and
	 *      denominator parts, units with prefixes, and exponents. Base
	 *      units could also be the string "-primitive" to represent a new
	 *      kind of quantity which cannot be derived from other units, but
	 *      you probably don't want to do that.
	 *
	 * Side Effects:
	 *      Future reductions and conversions could use this unit.
	 *
	 *-------------------------------------------------------------------
	 */
	void add(string name, string baseUnits);

	/*
	 *-------------------------------------------------------------------
	 *
	 * units_convert --
	 *
	 *      Convert a quantity string to a floating point number
	 *
	 * Results:
	 *      Converts the value string into a floating point number, scaled
	 *      to the specified targetUnits. The value string may contain a
	 *      number and units. If units are specified, then they must be
	 *      compatible with the targetUnits. If units are not specified
	 *      for the value, then it will be scaled to the target units. For
	 *      example, scaling "1.0" to "millimeter" would result in
	 *      "1000.0".
	 *
	 * Side Effects:
	 *      None.
	 *
	 *-------------------------------------------------------------------
	 */
	double convert(string value, string targetUnits);

	/*
	 *-------------------------------------------------------------------
	 *
	 * units_reduce --
	 *
	 *      Reduce a unit string to a scale factor and primitive units
	 *
	 * Results:
	 *      Returns a unit string consisting of a scale factor followed by
	 *      a space separated list of sorted and reduced primitive
	 *      units. The reduced unit string may include a forward-slash
	 *      (separated from the surrounding primitive subunits by spaces)
	 *      indicating that the remaining subunits are in the
	 *      denominator. Generates an error if the unitString is invalid.
	 *
	 * Side Effects:
	 *      None.
	 *
	 *-------------------------------------------------------------------
	 */
	string units_reduce(string unitString);

	/*
	 *-------------------------------------------------------------------
	 *
	 * units_clear_exception --
	 * units_check_exception --
	 *
	 *      Unit function error checking
	 *
	 * Results:

	 *      These two error checking functions must be used to detect
	 *      errors in the unit reduction and conversion functions.
	 *      Implementation is designed primarily to work with SWIG's
	 *      exception mechanism.  (http://www.swig.org)

	 *      Each call to a units conversion function should be preceeded
	 *      by a call to unit_clear_exception.  After calling the units
	 *      function, unit_check_exception will return NULL if there was
	 *      no error, or a pointer to a static character buffer containing
	 *      an error message if there was an error.
	 *
	 * Side Effects:
	 *      None.
	 *
	 *-------------------------------------------------------------------
	 */
	




	/// String manipulation functions
	char *stringcopy(const char *string);
	char *stringappend(char *target, const char *source);

	/// Unit untility functions
	Unit *reduceUnit(const char *unitString);
	Unit *sortUnitList(Unit *u);
	void cancelUnitList(Unit **numerator, Unit **denominator);
	bool unitInitialize();

	void setError(bool setError) {
		error = setError;
	}

	bool isError() {
		return error;
	}

	string getErrorMsg() {
		return errorMsg;
	}

	/// message string
	/// argument to format into message
	void throw_exception(string setErrorMsg) {
		errorMsg = setErrorMsg;
		error = true;
	}
};
