#ifndef REQUIRE_HXX
#define REQUIRE_HXX

#include <rapidjson/document.h>
#include <string>

using std::string;
using namespace rapidjson;

// JSON parsing utility functions for required fields with validation

/**
 * Get a required object field from a JSON document
 * @param doc The JSON document/value
 * @param name The field name to extract
 * @return Reference to the object value
 * @throws runtime_error if field is missing or not an object
 */
Value& requireObject(Value& doc, const char* name);

/**
 * Get a required array field from a JSON document
 * @param doc The JSON document/value
 * @param name The field name to extract
 * @return Reference to the array value
 * @throws runtime_error if field is missing or not an array
 */
Value& requireArray(Value& doc, const char* name);

/**
 * Get a required array field with specific capacity from a JSON document
 * @param doc The JSON document/value
 * @param name The field name to extract
 * @param capacity Expected number of elements in the array
 * @return Reference to the array value
 * @throws runtime_error if field is missing, not an array, or wrong capacity
 */
Value& requireArray(Value& doc, const char* name, int capacity);

/**
 * Get a required integer field from a JSON document
 * @param doc The JSON document/value
 * @param name The field name to extract
 * @return The integer value
 * @throws runtime_error if field is missing or not an integer
 */
int requireInt(Value& doc, const char* name);

/**
 * Get a required float field from a JSON document
 * @param doc The JSON document/value
 * @param name The field name to extract
 * @return The float value
 * @throws runtime_error if field is missing or not a float
 */
float requireFloat(Value& doc, const char* name);

/**
 * Get a required boolean field from a JSON document
 * @param doc The JSON document/value
 * @param name The field name to extract
 * @return The boolean value
 * @throws runtime_error if field is missing or not a boolean
 */
bool requireBool(Value& doc, const char* name);

/**
 * Get a required string field from a JSON document
 * @param doc The JSON document/value
 * @param name The field name to extract
 * @return The string value
 * @throws runtime_error if field is missing or not a string
 */
string requireString(Value& doc, const char* name);

#endif // REQUIRE_HXX