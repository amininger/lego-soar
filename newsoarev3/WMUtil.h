/*
 * WMUtil.h
 *
 *  Created on: Oct 10, 2013
 *      Author: aaron
 */

#ifndef WMUTIL_H_
#define WMUTIL_H_


#include <string>
#include <vector>

#include "sml_Client.h"

using namespace std;
using namespace sml;

class WMUtil{
public:
	/*****
	 * Returns true if the given string is an integer
	 */
	static bool isint(const string& s);

	/*****
	 * Returns true if the given string is a float
	 */
	static bool isfloat(const string& s);

	/**
	 * Returns the type of the given string, can be either INTEGER_VAL,
	 * DOUBLE_VAL, or STRING_VAL
	 */
	static  string getValueTypeOfString(const string& s);

	/**
	 * Updates the given Identifier so that it becomes at FloatWME:
	 * (id ^att val)
	 */
	static void updateFloatWME(Identifier* id, const string& att, double val);
	/**
	 * Updates the given Identifier so that it becomes a IntWME:
	 * (id ^att val)
	 */
	static void updateIntWME(Identifier* id, const string& att, int val);

	/**
	* Updates the given Identifier so that it becomes at FloatWME:
	* (id ^att val)
	*/
	static void updateStringWME(Identifier* id, const string& att, const string& val);

	/**
	* Updates the given Identifier so that it becomes:
	* (id ^att val)
	*/
	static void updateWME(Identifier* id, const string& att, const string& val);

	/**
	 * If the given identifier has the given attribute, the associated WME will
	 * be destroyed (only the first)
	 */
	static void removeWME(Identifier* id, const string& att);

	/**
	 * Given id and attribute, returns string value for WME (id ^attribute value)
	 * Returns true if successful
	 */
	static bool getValue(Identifier* id, const string& att, string& value);

	/**
	 * Given id and attribute, returns integer value for WME (id ^attribute value)
	 * Returns true if successful
	 */
	static bool getValue(Identifier* id, const string& att, int& value);

	/**
	 * Given id and attribute, returns double value for WME (id ^attribute value)
	 * Returns true if successful
	 */
	static bool getValue(Identifier* id, const string& att, double& value);

	/**
	 * Given id and attribute, returns identifier value for WME (id ^attribute value)
	 * Returns true if successful
	 */
	static bool getValue(Identifier* id, const string& att, Identifier*& value);


	/**
	 * Given id and attribute, returns a set of mutli-valued attributes for each WME matching
	 * (id ^attribute value)
	 */
	static void getAllValuesOfAttribute(Identifier* id, const string& att, vector<string>& values);

	/**
	 * Given id and attribute, returns a set of mutli-valued attributes for each WME matching
	 * (id ^attribute value)
	 */
	static void getAllValuesOfAttribute(Identifier* id, const string& att, vector<double>& values);

	/**
	 * Given id and attribute, returns a set of mutli-valued attributes for each WME matching
	 * (id ^attribute value)
	 */
	static void getAllValuesOfAttribute(Identifier* id, const string& att, vector<int>& values);

};

#endif /* WMUTIL_H_ */
