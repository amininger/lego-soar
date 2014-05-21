/*
 * WMUtil.cpp
 *
 *  Created on: Oct 10, 2013
 *      Author: aaron
 */

#include "WMUtil.h"

// These are the three types that a WME can be, conform to Soar
static const string INTEGER_VAL = "int";
static const string FLOAT_VAL = "double";
static const string STRING_VAL = "string";

bool WMUtil::isint(const string& s){
	for(string::const_iterator i = s.begin(); i != s.end(); i++){
		if(!isdigit(*i)){
			return false;
		}
	}
	return true;
}

bool WMUtil::isfloat(const string& s){
	bool decimal = false;
	for(string::const_iterator i = s.begin(); i != s.end(); i++){
		if(isdigit(*i)){
			continue;
		} else if(*i != '.'){
			return false;
		} else if(decimal){
			return false;
		} else {
			decimal = true;
		}
	}
	return true;
}

/**
 * Returns the type of the given string, can be either INTEGER_VAL,
 * DOUBLE_VAL, or STRING_VAL
 */
 string WMUtil::getValueTypeOfString(const string& s)
{
	 if(isint(s)){
		 return INTEGER_VAL;
	 } else if(isfloat(s)){
		 return FLOAT_VAL;
	 } else {
		 return STRING_VAL;
	 }
}

/**
 * Updates the given Identifier so that it becomes at FloatWME:
 * (id ^att val)
 */
void WMUtil::updateFloatWME(Identifier* id, const string& att, double val){
	WMElement* valueWME = id->FindByAttribute(att.c_str(), 0);
	if(valueWME == 0){
		id->CreateFloatWME(att.c_str(), val);
	} else if(valueWME->GetValueType() == FLOAT_VAL){
		FloatElement* floatWME = valueWME->ConvertToFloatElement();
		if(floatWME->GetValue() != val){
			floatWME->Update(val);
		}
	} else {
		valueWME->DestroyWME();
		id->CreateFloatWME(att.c_str(), val);
	}
}

/**
 * Updates the given Identifier so that it becomes a IntWME:
 * (id ^att val)
 */
void WMUtil::updateIntWME(Identifier* id, const string& att, int val){
	WMElement* valueWME = id->FindByAttribute(att.c_str(), 0);
	if(valueWME == 0){
		id->CreateIntWME(att.c_str(), val);
	} else if(valueWME->GetValueType() == INTEGER_VAL){
		IntElement* intWME = valueWME->ConvertToIntElement();
		if(intWME->GetValue() != val){
			intWME->Update(val);
		}
	} else {
		valueWME->DestroyWME();
		id->CreateIntWME(att.c_str(), val);
	}
}

/**
* Updates the given Identifier so that it becomes at FloatWME:
* (id ^att val)
*/
void WMUtil::updateStringWME(Identifier* id, const string& att, const string& val){
	WMElement* valueWME = id->FindByAttribute(att.c_str(), 0);
	if(valueWME == 0){
		id->CreateStringWME(att.c_str(), val.c_str());
	} else if(valueWME->GetValueType() == STRING_VAL){
		StringElement* strWME = valueWME->ConvertToStringElement();
		if(strWME->GetValue() != val){
			strWME->Update(val.c_str());
		}
	} else {
		valueWME->DestroyWME();
		id->CreateStringWME(att.c_str(), val.c_str());
	}
}

#include <string>


/**
* Updates the given Identifier so that it becomes:
* (id ^att val)
*/
void WMUtil::updateWME(Identifier* id, const string& att, const string& val){
	string valueType = getValueTypeOfString(val);
	if(valueType == INTEGER_VAL){
		updateIntWME(id, att, strtol(val.c_str(), 0, 10));
	} else if(valueType == FLOAT_VAL){
		updateFloatWME(id, att, strtod(val.c_str(), 0));
	} else {
		updateStringWME(id, att, val);
	}
}

/**
 * If the given identifier has the given attribute, the associated WME will
 * be destroyed (only the first)
 */
void WMUtil::removeWME(Identifier* id, const string& att){
	WMElement* valueWME = id->FindByAttribute(att.c_str(), 0);
	if(valueWME != 0){
		valueWME->DestroyWME();
	}
}


/**
 * Given id and attribute, returns string value for WME (id ^attribute value)
 * Returns true if successful
 */
bool WMUtil::getValue(Identifier* id, const string& att, string& value){
	WMElement* valueWME = id->FindByAttribute(att.c_str(), 0);
	if(valueWME == 0 || valueWME->GetValueType() != STRING_VAL){
		return false;
	}
	value = valueWME->ConvertToStringElement()->GetValue();
	return true;
}

/**
 * Given id and attribute, returns integer value for WME (id ^attribute value)
 * Returns true if successful
 */
bool WMUtil::getValue(Identifier* id, const string& att, int& value){
	WMElement* valueWME = id->FindByAttribute(att.c_str(), 0);
	if(valueWME == 0 || valueWME->GetValueType() != INTEGER_VAL){
		return false;
	}
	value = valueWME->ConvertToIntElement()->GetValue();
	return true;
}

/**
 * Given id and attribute, returns double value for WME (id ^attribute value)
 * Returns true if successful
 */
bool WMUtil::getValue(Identifier* id, const string& att, double& value){
	WMElement* valueWME = id->FindByAttribute(att.c_str(), 0);
	if(valueWME == 0 || valueWME->GetValueType() != FLOAT_VAL){
		return false;
	}
	value = valueWME->ConvertToFloatElement()->GetValue();
	return true;
}

/**
 * Given id and attribute, returns identifier value for WME (id ^attribute value)
 * Returns true if successful
 */
bool WMUtil::getValue(Identifier* id, const string& att, Identifier*& value){
	WMElement* valueWME = id->FindByAttribute(att.c_str(), 0);
	if(valueWME == 0 || !valueWME->IsIdentifier()){
		return false;
	}
	value = valueWME->ConvertToIdentifier();
	return true;
}

/**
 * Given id and attribute, returns a set of mutli-valued attributes for each WME matching
 * (id ^attribute value)
 */
void WMUtil::getAllValuesOfAttribute(Identifier* id, const string& att, vector<string>& values){
	for(int index = 0; index < id->GetNumberChildren(); index++){
		WMElement* wme = id->GetChild(index);
		if(wme->GetAttribute() == att){
			values.push_back(wme->GetValueAsString());
		}
	}
}

/**
 * Given id and attribute, returns a set of mutli-valued attributes for each WME matching
 * (id ^attribute value)
 */
void WMUtil::getAllValuesOfAttribute(Identifier* id, const string& att, vector<int>& values){
	for(int index = 0; index < id->GetNumberChildren(); index++){
		WMElement* wme = id->GetChild(index);
		if(wme->GetAttribute() == att && wme->GetValueType() == INTEGER_VAL){
			values.push_back(wme->ConvertToIntElement()->GetValue());
		}
	}
}

/**
 * Given id and attribute, returns a set of mutli-valued attributes for each WME matching
 * (id ^attribute value)
 */
void WMUtil::getAllValuesOfAttribute(Identifier* id, const string& att, vector<double>& values){
	for(int index = 0; index < id->GetNumberChildren(); index++){
		WMElement* wme = id->GetChild(index);
		if(wme->GetAttribute() == att && wme->GetValueType() == FLOAT_VAL){
			values.push_back(wme->ConvertToFloatElement()->GetValue());
		}
	}
}

