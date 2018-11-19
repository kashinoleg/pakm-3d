#include "stdafx.h"
#include "units.h"


	/*  Define a few useful constants  */
const char *unitNameCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *separators = " \t*-";
const char *validchars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789. \t*^/+-";

/*  head of lists  */
static Unit *uroot = NULL;

 /*  Make a new copy of a string  */
 /*  string to copy  */
char * UnitConvert::stringcopy(const char *string)
{
	int length = strlen(string) + 1;
	char *copy = (char *)malloc(length);
	strcpy_s(copy, length, string);
	return copy;
}

/*  Append a string, allocating more memory  */
/*  char *pointer to be reallocated  */
/*  source string  */
char * UnitConvert::stringappend(char *target, const char *source) {
	size_t newsize = (size_t)strlen(target) + strlen(source) + 1;
	target = (char *)realloc(target, newsize);
	strcat_s(target, newsize, source);
	return(target);
}

/*  new  */
/*  name of new unit  */
/*  base units (must be reducible  */
void UnitConvert::add(string name, string baseUnits) {
	/*  check for initialization  */
	if (uroot == NULL && unitInitialize() == false) {
		return;
	}

	/*  check for duplicates  */
	for (Unit *u = uroot; u != NULL; u = u->next) {
		if (u->name == name) {
			throw_exception("unit " + name + "is already defined");
			return;
		}
	}

	/*  Check for valid characters  */
	if (strspn(name.c_str(), unitNameCharacters) != strlen(name.c_str())) {
		throw_exception("non-alphabetic characters in unit name " + name);
		return;
	}

	/*  Compute reduced units  */
	Unit *newunit = reduceUnit(baseUnits.c_str());
	if (newunit == NULL) {
		throw_exception(baseUnits + " cannot be reduced to primitive units");
		return;
	}

	/*  Add unit to list  */
	newunit->name = name;
	if (uroot == NULL) {
		uroot = newunit;
	}
	else {
		/* find end of list */
		Unit *u;
		for (u = uroot; u->next != NULL; u = u->next) {
			/* do nothing */
		}
		u->next = newunit;
	}
}


/*  convert  */
/*  value string to convert  */
/*  target units for conversion  */
double UnitConvert::convert(string value, string targetUnits) {
	/*  check for initialization  */
	if (uroot == NULL && unitInitialize() == false) {
		/* exception code has already been set, so just return */
		return 0.0;
	}

	/*  Reduce each of value and target to primitive units */
	/*  reduced value unit   */
	Unit *rValue = reduceUnit(value.c_str());
	/*  reduced target unit  */
	Unit *rTarget = reduceUnit(targetUnits.c_str());
	if (rValue == NULL || rTarget == NULL) {
		delete rValue;
		delete rTarget;
		/* exception code has already been set, so just return */
		return 0.0;
	}

	/*
	 *  If the value has units, it must be compatible with
	 *  the target.  (If it is unitless, then compatibility
	 *  is not required.
	 */
	if ((rValue->numerator != NULL) || (rValue->denominator != NULL)) {
		Unit *u1;
		Unit *u2;
		for (u1 = rValue->numerator, u2 = rTarget->numerator; (u1 != NULL) && (u2 != NULL); u1 = u1->next, u2 = u2->next) {
			if (u1->name != u2->name) {
				break;
			}
		}
		if ((u1 != NULL) || (u2 != NULL)) {
			throw_exception(value + " and " + targetUnits + " have incompatible units");
			delete rValue;
			delete rTarget;
			return 0.0;
		}
		for (u1 = rValue->denominator, u2 = rTarget->denominator; (u1 != NULL) && (u2 != NULL); u1 = u1->next, u2 = u2->next) {
			if (u1->name != u2->name) {
				break;
			}
		}
		if ((u1 != NULL) || (u2 != NULL)) {
			throw_exception(value + " and " + targetUnits + " have incompatible units");
			delete rValue;
			delete rTarget;
			return 0.0;
		}
	}


	/*  Compute and return scaled value  */
	double result = rValue->factor / rTarget->factor;
	delete rValue;
	delete rTarget;
	return result;
}


/*  reduce  */
/*  unit string to reduce  */
string UnitConvert::units_reduce(string unitString) {
	string returnValue = "                         ";

	Unit *u;    /* reduced unitString  */
	Unit *uptr; /* pointer for traversing u's numer & denomin  */

	/*  check for initialization  */
	if (uroot == NULL) {
		if (unitInitialize() == false) {
			return NULL;
		}
	}

	u = reduceUnit(unitString.c_str());
	if (u == NULL) {
		/* exception code has already been set, so just return */
		return NULL;
	}

	/*  Assemble reduced unit string from Unit structure  */
	returnValue = to_string(u->factor);
	if (strcspn(returnValue.c_str(), ".eE") == strlen(returnValue.c_str())) {
		returnValue = returnValue +".0";
	}
	for (uptr = u->numerator; uptr != NULL; uptr = uptr->next) {
		returnValue = returnValue + " ";
		returnValue = returnValue + uptr->name;
	}
	if (u->denominator != NULL) {
		returnValue = returnValue + " /";
		for (uptr = u->denominator; uptr != NULL; uptr = uptr->next) {
			returnValue = returnValue + " ";
			returnValue = returnValue + uptr->name;
		}
	}

	delete u;
	return returnValue;
}



/*  local functions  */
/*  unit string to reduce  */
Unit * UnitConvert::reduceUnit(const char *unitString) {
	char *tokenString;       /*  tokenized copy of unitString  */
	int numeratorlen;        /*  for splitting unitString  */
	char *subunit;           /*  one token from tokenString  */
	char *subunitcopy;       /*  copy of subunit, for plural chopping  */
	double factor;           /*  value of constant factor subunits  */
	int numerflag = 1;       /*  boolean switch between numer/denom  */
	int exponent;            /*  subunit exponent value  */
	int e;                   /*  loop counter for exponents  */
	int len, pos;            /*  for separating prefix and unit name  */
	Unit *up1, *up2;         /*  for searching the unit list  */
	UnitPrefix *p;               /*  for walking prefix list  */
	Unit *usub;              /*  new subunits (added to u)  */


	/*  check for initialization  */
	if (uroot == NULL && unitInitialize() == false) {
		return NULL;
	}

	/* create the Unit to return  */
	Unit *u = new Unit();

	/* check for primitive unit */
	if (unitString == "-primitive") {
		u->name = unitString;
		u->primitive = true;
		return u;
	}

	/*  verify syntax of unit string   */

	/*  it may contain at most one "/"  */
	/*  for traversing strings  */
	char *cp = strpbrk(const_cast<char*>(unitString), "/");
	if (cp != NULL && strpbrk(cp + 1, "/") != NULL) {
		string error("invalid unit string ");
		error += unitString;
		error += ":  only one / allowed";
		throw_exception(error);
		delete u;
		return NULL;
	}

	/*  it may contain only letters, digits, the power character ('^'),
		decimal points, digits, +- signs, and separators
	*/
	if (strspn(unitString, validchars) < strlen(unitString)) {
		string error("invalid characters in unit string ");
		throw_exception(error + unitString);
		delete u;
		return NULL;
	}

	/*  Disassemble string into scale factor and numerator/denominator  */

	/*  get factor
		This is a little tricky because strtod() could skip white space,
		then not find a valid float, and return 0.0.  So we do a little
		manual whitespace-skipping to ensure that we are really getting
		a float value, or not.
	*/
	while (isspace(*unitString)) {
		unitString++;
	}
	u->factor = strtod(unitString, &cp);
	if (cp == unitString) {
		/* no leading factor (we got a 0.0 value - fix it)  */
		u->factor = 1.0;
	}
	else {
		/* we've capture the factor - skip over characters */
		unitString = cp;
	}


	/*  Make a copy of unitString, and replace any "/" with " / "  */
	tokenString = stringcopy(unitString);
	numeratorlen = strcspn(tokenString, "/");
	if (numeratorlen < strlen(tokenString)) {
		tokenString[numeratorlen] = '\0';
		tokenString = stringappend(tokenString, " / ");
		tokenString = stringappend(tokenString, unitString + numeratorlen + 1);
	}


	/*  process each subunit in turn, starting in the numerator

		Note that we're going to use a boolean flag to switch
		between numerator and denominator if we encounter a "/".
		This same style is used for processing recursively
		reduced subunits
	*/
	char *next_token = NULL;
	for (subunit = strtok_s(tokenString, separators, &next_token); subunit != NULL; subunit = strtok_s(NULL, separators, &next_token))
	{
		/* check for null subunit  */
		if (strlen(subunit) == 0)
		{
			continue;
		}

		/*  check for "/"  */
		if (strcmp(subunit, "/") == 0) {
			numerflag = numerflag ? 0 : 1;
			continue;
		}

		/*  constant factor  */

		factor = strtod(subunit, &cp);
		if (cp > subunit) {
			if (factor == 0.0) {
				string error("illegal zero factor in ");
				throw_exception(error + unitString);
				delete u ;
				free(tokenString);
				return NULL;
			}
			if (numerflag) {
				u->factor *= factor;
			}
			else {
				u->factor /= factor;
				continue;
			}
		}

		/*  Check for power character (e.g., "s^2")  */
		cp = strchr(subunit, '^');
		if (cp != NULL) {
			*cp = '\0';  /* terminate subunit name  */
			exponent = strtol(++cp, &cp, 10);
			if ((*cp != '\0') || (exponent <= 0)) {
				string error("invalid integer exponent in ");
				throw_exception(error + unitString);
				delete u;
				free(tokenString);
				return NULL;
			}
		}
		else {
			exponent = 1;
		}

		/*  Check subunit name syntax  */
		if (strspn(subunit, unitNameCharacters) != strlen(subunit)) {
			string error("invalid non-alphabetic unit name in ");
			throw_exception(error + unitString);
			delete u;
			free(tokenString);
			return NULL;
		}


		/*  Search the unit list in order, because we
		wouldn't want to accidentally match the "m"
		at the end of "gram" and conclude that we
		have "meter".   */
		len = strlen(subunit);
		p = NULL;

		for (up1 = uroot; up1 != NULL; up1 = up1->next) {

			/*  Try to match the string starting at the
			at the end, just in case there is a prefix.
			We only have a match if both the prefix and
			unit name are exact matches.  */
			pos = len - strlen(up1->name.c_str());
			if ((subunit + pos) == up1->name) {
				/* zero length prefix is OK */
				if (pos == 0) {
					break;
				}
				/* otherwise, search prefix list */
				int i = 0;
				for (; i < prefixs.size(); i++) {
					p = &prefixs[i];
					if (p->name.length() == pos && strncmp(subunit, p->name.c_str(), pos) == 0) {
						break;
					}
				}
				if (i != prefixs.size()) {
					break;
				}
			}
		}

		/*  If we didn't find a match, then try again
		if there is a trailing "s".
		*/
		if (up1 == NULL && subunit[len - 1] == 's') {
			subunitcopy = stringcopy(subunit);
			len--;
			subunitcopy[len] = '\0';
			for (up1 = uroot; up1 != NULL; up1 = up1->next) {
				pos = len - strlen(up1->name.c_str());
				if ((subunitcopy + pos) == up1->name) {
					/* zero length prefix is OK */
					if (pos == 0) {
						break;
					}
					/* otherwise, search prefix list */
					int i = 0;
					for (; i < prefixs.size(); i++) {
						p = &prefixs[i];
						if (strncmp(subunitcopy, p->name.c_str(), pos) == 0) {
							break;
						}
					}
					if (i != prefixs.size()) {
						break;
					}
				}
			}

			/*  If we still didn't find a match, then try again
			if there is a trailing "e" (which was a
			trailing "es".)
			*/
			if (up1 == NULL && subunit[len - 1] == 'e') {
				len--;
				subunitcopy[len] = '\0';
				for (up1 = uroot; up1 != NULL; up1 = up1->next) {
					pos = len - strlen(up1->name.c_str());
					if ((subunitcopy + pos) == up1->name) {
						/* zero length prefix is OK */
						if (pos == 0) {
							break;
						}
						/* otherwise, search prefix list */
						int i = 0;
						for (; i < prefixs.size(); i++) {
							p = &prefixs[i];
							if (strncmp(subunitcopy, p->name.c_str(), pos) == 0) {
								break;
							}
						}
						if (i != prefixs.size()) {
							break;
						}
					}
				}
			} /* end if ends in "es" */
			free(subunitcopy);
		} /* end if ends in "s" */


		/*  If we fell through to here, without a match,
		then we really have a bad unit name
		*/
		if (up1 == NULL) {
			string error("invalid unit name ");
			error += subunit;
			error += " in ";
			error += unitString;
			throw_exception(error);
			delete u;
			free(tokenString);
			return NULL;
		}


		/*  Multiply the factor by prefix factor ('exponent' times)  */
		if (p != NULL) {
			for (e = 0; e < exponent; e++) {
				if (numerflag) {
					u->factor *= p->factor;
				}
				else {
					u->factor /= p->factor;
				}
			}
		}


		/*  Is this a primitive subunint?  */
		if (up1->primitive) {
			/*  just append the matching subunit to
			the result.
			Actually, we can get away with inserting
			at the head of the numerator/denominator
			lists because we will sort them later.
			*/
			for (e = 0; e < exponent; e++) {
				usub = new Unit();
				usub->name = up1->name;
				if (numerflag) {
					usub->next = u->numerator;
					u->numerator = usub;
				}
				else {
					usub->next = u->denominator;
					u->denominator = usub;
				}
			}
		}
		else {
			/*  Recursively reduce.  */
			/*  except that this is already reduced  */
			/* up1 = reduceUnit( subunit ); */

			/*  Include multiple factor from reduced unit  */
			for (e = 0; e < exponent; e++) {
				if (numerflag) {
					u->factor *= up1->factor;
				}
				else {
					u->factor /= up1->factor;
				}
			}

			/*  Add primitive subunits to numerator/denominator

			Note that we're use a nested boolean flag to switch
			between numerator and denominator.  Subunits in
			the numerator of the unitString are processed
			normally, but subunits in the denominator of
			unitString must be inverted.

			Editors note:  This inverting numerator/denominator
			flag business looks a little odd translating from
			the reference (Tcl) implementation to here.  Please
			excuse the very un-C-like verbosity.
			*/
			for (e = 0; e < exponent; e++) {
				for (up2 = up1->numerator; up2 != NULL; up2 = up2->next) {
					usub = new Unit();
					usub->name = up2->name;
					if (numerflag) {
						usub->factor *= up2->factor;
						usub->next = u->numerator;
						u->numerator = usub;
					}
					else {
						usub->factor /= up2->factor;
						usub->next = u->denominator;
						u->denominator = usub;
					}
				}
				for (up2 = up1->denominator; up2 != NULL; up2 = up2->next) {
					usub = new Unit();
					usub->name = up2->name;
					if (numerflag) {
						usub->factor /= up2->factor;
						usub->next = u->denominator;
						u->denominator = usub;
					}
					else {
						usub->factor *= up2->factor;
						usub->next = u->numerator;
						u->numerator = usub;
					}
				}
			}

		} /* end if non-primitive unit */

	} /* end for each subunit */

	/*  Sort both numerator and denominator  */
	u->numerator = sortUnitList(u->numerator);
	u->denominator = sortUnitList(u->denominator);

	/*  Cancel any duplicate units.  */
	cancelUnitList(&u->numerator, &u->denominator);

	/*  delete string and return */
	free(tokenString);
	return u;


}

/*  linked list of units to sort  */
Unit * UnitConvert::sortUnitList(Unit *u) {
	/*
	 *  The author can barely comprehend the necessity
	 *  for writing list sorting code in this day
	 *  and age.  *sigh*
	 *
	 *  Even though the lists are *very* short,
	 *  copy the pointers into an array, then sort
	 *  the pointers.
	 */
	Unit *u1;
	Unit **a;
	int i;
	int j;

	/* count list length */
	int len = 0;
	for (u1 = u; u1 != NULL; u1 = u1->next) {
		len++;
	}
	/*  do we really have to do this?  */
	if (len < 2) {
		return u;
	}
	/* copy list to array */
	a = (Unit**)malloc((size_t)len * sizeof(Unit*));
	len = 0;
	for (u1 = u; u1 != NULL; u1 = u1->next) {
		*(a + len) = u1;
		len++;
	}
	/*
	 *  Sort the array of pointers using an insertion sort.
	 *  This isn't really any more complicated than writing
	 *  the comparison function and calling qsort().
	 */
	for (i = 1; i < len; i++) {
		int inserted = 0;
		j = i;
		while ((j >= 1) && (inserted == 0)) {
			if (strcmp(a[j]->name.c_str(), a[j - 1]->name.c_str()) < 0) {
				Unit *tmp;
				tmp = a[j];
				a[j] = a[j - 1];
				a[j - 1] = tmp;
			}
			else {
				inserted = 1;
			}
			j--;
		}
	}
	/* reassemble the linked list */
	for (i = 0; i < len - 1; i++) {
		a[i]->next = a[i + 1];
	}
	a[len - 1]->next = NULL;
	u1 = a[0];
	free(a);
	return u1;
}


/*  numerator list of units  */
/*  denominator list of units  */
void UnitConvert::cancelUnitList(Unit **numerator, Unit **denominator) {
	/*
	 *  March through the list with dual pointers so we
	 *  can keep the forward-only links intact.
	 */
	Unit *n1 = NULL;
	Unit *n2 = *numerator;
	while (n2 != NULL) {

		/* search denominator for matching unit */
		Unit *d1 = NULL;
		Unit *d2 = *denominator;
		while (d2 != NULL) {
			if (n2->name == d2->name) {
				/*  delete unit from both lists  */
				if (n1 == NULL) {
					*numerator = n2->next;
					delete n2;
					n2 = *numerator;
				}
				else {
					n1->next = n2->next;
					delete n2;
					n2 = n1->next;
				}
				if (d1 == NULL) {
					*denominator = d2->next;
					delete d2;
					d2 = *denominator;
				}
				else {
					d1->next = d2->next;
					delete d2;
					d2 = d1->next;
				}
				break;
			}
			else {
				/* look to next denominator unit */
				d1 = d2;
				d2 = d2->next;
			}
		} /* end while searching denominator */

		/*
		 *  If we ran off end of denominator, move on to
		 *  next numerator element.  If not, then we fell
		 *  out of the loops after deleting a subunit, so
		 *  we can't increment the numerator pointers.
		 */
		if (d2 == NULL) {
			/*  have all numerators been deleted?  */
			if (n2 != NULL) {
				n1 = n2;
				n2 = n2->next;
			}
		}
	}
}

/*------------------------------------------------------------
 *
 *  UNIT table
 *
 *  Make sure that you change the unitcount variable if
 *  you change the unit initialization.
 *
 *  Users can add units to this table with the function
 *  int newUnitConversion( const char* unitName, const char* baseUnits)
 *
 *  Note that unit strings are parsed from the end using a first-
 *  match comparison.  The table should list the long unit names
 *  first, then abbreviations, such that a short match won't
 *  override a long match.  For example, the "s" abbreviation for
 *  "seconds" should come after the "siemens" unit.  It is OK
 *  to define units in terms of other units' abbreviations, even
 *  though they appear later in the table.
 *
 *  The SI units included here are described in NIST Special
 *  Publication 330, 1991 Edition, "The Interational System of Units
 *  (SI)" and NIST Special Publication 811, 1995 Edition, "Guide for
 *  the Use of the International System of Units (SI)".  Note that the
 *  units table defines gram instead of kilogram because the prefix
 *  rules define 1e-6kg = 1mg (and NOT 1ukg).  Also note that an
 *  abbreviation for "ohm" is not included because you can't easily
 *  type an omega.
 *
 *------------------------------------------------------------*/




/*  SI Derived Units with Special Names  */
const char *derivedUnits[] = {
  "radian",	"meter/meter",
  "steradian",	"meter^2/meter^2",
  "hertz",	"/second",
  "newton",	"meter-kilogram/second^2",
  "pascal",	"kilogram/meter-second^2",
  "joule",	"meter^2-kilogram/second^2",
  "watt",	"meter^2-kilogram/second^3",
  "coulomb",	"second-ampere",
  "volt",	"meter^2-kilogram/second^3-ampere",
  "farad",	"second^4-ampere^2/meter^2-kilogram",
  "ohm",	"meter^2-kilogram/second^3-ampere^2",
  "siemens",	"second^3-ampere^2/meter^2-kilogram",
  "weber",	"meter^2-kilogram/second^2-ampere",
  "tesla",	"kilogram/second^2-ampere",
  "henry",	"meter^2-kilogram/second^2-ampere^2",
  "lumen",	"candela-steradian",
  "lux",	"candela-steradian/meter^2",
  "becquerel",	"/second",
  "gray",	"meter^2/second^2",
  "sievert",	"meter^2/second^2",
  NULL,         NULL };

/*  Non-SI units  */
const char *nonSIUnits[] = {
  "angstrom",	"1.0E-10meter",
  "astronomicalUnit",	"1.495979E11meter",
  "atmosphere",	"1.01325E5pascal",
  "bar",	"1.0E5pascal",
  "calorie",	"4.1868joule",
  "curie",	"3.7E10becquerel",
  "day",	"8.64E4second",
  "degree",	"1.745329E-2radian",
  "erg",	"1.0E-7joule",
  "faraday",	"9.648531coulomb",
  "fermi",	"1.0E-15meter",
  "foot",	"3.048E-1meter",
  "gauss",	"1.0E-4tesla",
  "gilbert",	"7.957747E-1ampere",
  "grain",	"6.479891E-5kilogram",
  "hectare",	"1.0E4meter^2",
  "hour",	"3.6E3second",
  "inch",	"2.54E-2meter",
  "lightYear",	"9.46073E15meter",
  "liter",	"1.0E-3meter^3",
  "maxwell",	"1.0E-8weber",
  "mho",	"1.0siemens",
  "micron",	"1.0E-6meter",
  "mil",	"2.54E-5meter",
  "mile",	"1.609344E3meter",
  "minute",	"6.0E1second",
  "parsec",	"3.085E16meter",
  "pica",	"4.233333E-3meter",
  "pound",	"4.535924E-1kilogram",
  "revolution",	"6.283185radian",
  "revolutionPerMinute",	"1.047198E-1radian/second",
  "yard",	"9.144E-1meter",
  "year",	"3.1536E7second",
  NULL,         NULL };




/*  Abbreviations  */
const char *abbreviations[] = {
  "m",	"meter",
  "g",	"gram",
  "s",	"second",
  "A",	"ampere",
  "K",	"kelvin",
  "mol","mole",
  "cd",	"candela",
  "rad","radian",
  "sr",	"steradian",
  "Hz",	"hertz",
  "N",	"newton",
  "Pa",	"pascal",
  "J",	"joule",
  "W",	"watt",
  "C",	"coulomb",
  "V",	"volt",
  "F",	"farad",
  "S",	"siemens",
  "Wb",	"weber",
  "T",	"tesla",
  "H",	"henry",
  "lm",	"lumen",
  "lx",	"lux",
  "Bq",	"becquerel",
  "Gy",	"gray",
  "Sv",	"sievert",
  NULL, NULL };

const char *nonSIabbreviations[] = {
  "AU",	"astronomicalUnit",
  "ft",	"foot",
  "gr",	"grain",
  "ha",	"hectare",
  "h",	"hour",
  "in",	"inch",
  "L",	"liter",
  "Mx",	"maxwell",
  "mi",	"mile",
  "min","minute",
  "pc",	"parsec",
  "lb",	"pound",
  "r",	"revolution",
  "rpm","revolutionPerMinute",
  "yd",	"yard",
  NULL,   NULL };


/*------------------------------------------------------------
 *
 *  PREFIX table
 *
 *  Make sure that you change the prefixcount variable if
 *  you change the prefix initialization.
 *
 *  Note that prefix strings are parsed from the beginning using a
 *  first-match comparison.  The table should list the long prefixes
 *  first, then abbreviations, such that a short match won't
 *  override a long match.  For example, the "m" abbreviation for
 *  "milli" should come after both "milli" and "micro".
 *
 *  The SI prefixes included here are described in NIST Special
 *  Publication 330, 1991 Edition, "The Interational System of
 *  Units (SI)".  Note that we support the non-USA spelling of
 *  deka- (deca-) and that we take the liberty of allowing
 *  the "micro-" prefix to be typed with a "u".
 *
 *------------------------------------------------------------*/

bool UnitConvert::unitInitialize() {
	Unit *u;
	Unit *up = nullptr;
	const char **name;
	const char **value;

	/*  Load Units List with Base (primitive) units  */
	/* SI Base Units */
	/* SI Supplementary Units */
	vector<string> baseUnits = {
		"meter",
		"gram",
		"second",
		"ampere",
		"kelvin",
		"mole",
		"candela"};
	for (string base : baseUnits)
	{
		Unit *unit = new Unit();
		unit->name = base;
		unit->primitive = true;
		if (uroot == NULL)
		{
			uroot = unit;        /* start new uroot */
			up = unit;
		}
		else if (up != nullptr)
		{
			
			up->next = unit;     /* append to end of list */
			up = unit;
		}
	}

	/*  Load Units List with Derived Units  */
	for (name = derivedUnits, value = name + 1; *name != NULL; name += 2, value += 2) {
		u = reduceUnit(*value);
		if (u == NULL) {
			string error("units failure initializing derived unit ");
			error += *name;
			error += " (";
			error += *value;
			error += ")";

			throw_exception(error);
			return false;
		}
		u->name = *name;
		up->next = u;
		up = u;
	}

	/*  Load Units List with Non-SI Units  */
	for (name = nonSIUnits, value = name + 1; *name != NULL; name += 2, value += 2) {
		u = reduceUnit(*value);
		if (u == NULL) {
			string error("units failure initializing non-SI unit ");
			error += *name;
			error += " (";
			error += *value;
			error += ")";

			throw_exception(error);
			return false;
		}
		u->name = *name;
		up->next = u;
		up = u;
	}

	/*  Load Units List with Abbreviations  */
	for (name = abbreviations, value = name + 1; *name != NULL; name += 2, value += 2) {
		u = reduceUnit(*value);
		if (u == NULL) {
			string error("units failure initializing abbreviation ");
			error += *name;
			error += " (";
			error += *value;
			error += ")";

			throw_exception(error);
			return false;
		}
		u->name = *name;
		up->next = u;
		up = u;
	}

	/*  Load Units List with non-SI Abbreviations  */
	for (name = nonSIabbreviations, value = name + 1; *name != NULL; name += 2, value += 2) {
		u = reduceUnit(*value);
		if (u == NULL) {
			string error("units failure initializing non-SI abbreviation ");
			error += *name;
			error += " (";
			error += *value;
			error += ")";

			throw_exception(error);
			return false;
		}
		u->name = *name;
		up->next = u;
		up = u;
	}

	return true;
}
