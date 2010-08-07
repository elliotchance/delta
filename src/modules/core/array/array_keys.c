/*
 *  Delta programming language
 */

#include "delta/delta.h"


/**
 * @category modules/core/array
 *
 * @brief Return all the keys or a subset of the keys of an array.
 * @syntax array array_keys ( array input )
 *
 * array_keys() returns the keys, numeric and string, from the input array.
 *
 * @example Example #1 array_keys() example
 * array = array(0 => 100, "color" => "red");
 * print_r(array_keys(array));
 * 
 * array = array("blue", "red", "green", "blue", "blue");
 * print_r(array_keys(array, "blue"));
 * 
 * array = array("color" => array("blue", "red", "green"),
 * "size"  => array("small", "medium", "large"));
 * print_r(array_keys(array));
 * @endexample
 *
 * @param input An array containing keys to return.
 * @return Returns an array of all the keys in input.
 * @see array_values
 * @see array_key_exists
 * @see array_search
 */
DELTA_FUNCTION(array_keys)
{
	struct DeltaArray r = delta_new_array();
	
	int i, release;
	struct DeltaVariable *array = delta_cast_array(DELTA_ARG0, &release);
	struct DeltaArrayValue *element;
	
	DELTA_ENUMERATE_ARRAY(&array->value.array, element, i)
		delta_array_push_k_string(&r, delta_int_to_string(i), element->key);
	
	DELTA_RELEASE(release, array);
	DELTA_RETURN_ARRAY(r);
}
