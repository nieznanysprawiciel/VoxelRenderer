#include <rapidjson/error/error.h>

/**@brief Stringi reprezentuj¹ce kody b³êdów generowane przez rapid jsona.*/
const char* errorCodes[] =
{
	"No error.",

	"The document is empty.",
	"The document root must not follow by other values.",

	"Invalid value.",

	"Missing a name for object member.",
	"Missing a colon after a name of object member.",
	"Missing a comma or '}' after an object member.",

	"Missing a comma or ']' after an array element.",

	"Incorrect hex digit after \\u escape in string.",
	"The surrogate pair in string is invalid.",
	"Invalid escape character in string.",
	"Missing a closing quotation mark in string.",
	"Invalid encoding in string",

	"Number too big to be stored in double.",
	"Miss fraction part in number.",
	"Miss exponent in number.",

	"Parsing was terminated.",
	"Unspecific syntax error."
};


/**@brief Zwraca ci¹g znaków reprezentuj¹cy kod b³êdu podany w parametrze.*/
const char* GetStringFromCode( rapidjson::ParseErrorCode code )
{
	return errorCodes[ static_cast<int>( code ) ];
}

