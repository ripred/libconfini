/**

	@file		confini.h
	@brief		libconfini header
	@author		Stefano Gioffr&eacute;
	@copyright 	GNU Public License v3
	@date		2016

**/

#ifndef _LIBCONFINI_HEADER_
#define _LIBCONFINI_HEADER_



/* PRIVATE (HEADER-SCOPED) MACROS */


#define _LIBCONFINI_SETTINGS_AS_DECLARATION_(SIZE, PROPERTY, VALUE) unsigned char PROPERTY:SIZE;
#define _LIBCONFINI_SETTINGS_AS_ASSIGNEMENT_(SIZE, PROPERTY, VALUE) VALUE,
#define _LIBCONFINI_FORMAT_STRUCT_ struct IniFormat { _LIBCONFINI_EXPAND_SETTINGS_MACRO_AS_(_LIBCONFINI_SETTINGS_AS_DECLARATION_) }
#define _LIBCONFINI_DEFAULT_SETTINGS_ { _LIBCONFINI_EXPAND_SETTINGS_MACRO_AS_(_LIBCONFINI_SETTINGS_AS_ASSIGNEMENT_) }



/* PUBLIC MACROS */


#define _LIBCONFINI_EXPAND_SETTINGS_MACRO_AS_(______)                          /*-*\

    ______(     BITS      NAME                        DEFAULT VALUE            )/--/
                                                                               /-*/\
    ______(     8,        delimiter_symbol,           '='                      )/*-/
                                                                               /-*/\
    ______(     2,        semicolon,                  INI_PARSE_COMMENT        )   \
    ______(     2,        hash,                       INI_PARSE_COMMENT        )   \
    ______(     2,        multiline_entries,          INI_EVERYTHING_MULTILINE )   \
    ______(     1,        no_single_quotes,           0                        )   \
    ______(     1,        no_double_quotes,           0                        )/*-/
                                                                               /-*/\
    ______(     1,        case_sensitive,             0                        )   \
    ______(     1,        preserve_spaces_in_values,  0                        )   \
    ______(     1,        implicit_is_special,        0                        )   \
    ______(     1,        disabled_can_be_implicit,   0                        )   \
    ______(     1,        no_disabled_after_spaces,   0                        )   \
    ______(     3,        _LIBCONFINI_RESERVED_,      0                        )/*-/
                                                                               /-*/



/* PUBLIC TYPEDEFS */


/** @brief	24-bit bitfield representing a unique format of an INI file (INI dialect) **/
typedef _LIBCONFINI_FORMAT_STRUCT_ IniFormat;

/** @brief	Global statistics about an INI file **/
typedef struct IniStatistics {
	const IniFormat format;
	unsigned long int bytes;
	unsigned long int members;
} IniStatistics;

/** @brief	Dispatch of a single INI member **/
typedef struct IniDispatch {
	char *append_to;
	const IniFormat format;
	unsigned short int type;
	char *data;
	char *value;
	unsigned long int d_length;
	unsigned long int v_length;
	unsigned long int dispatch_id;
} IniDispatch;

/** @brief	24-bit bitmask representing the format of an INI file **/
typedef unsigned long long int IniFormatMask;



/* PUBLIC FUNCTIONS */


extern unsigned int load_ini_file (
	const char *path,
	const IniFormat format,
	int (*f_init)(
		IniStatistics *statistics,
		void *init_other
	),
	int (*f_foreach)(
		IniDispatch *dispatch,
		void *foreach_other
	),
	void *user_data
);

extern void ini_set_implicit_value (char *implicit_value, unsigned long int implicit_v_length);

extern IniFormatMask ini_format_calculate_mask (const IniFormat format);

extern void ini_format_set_from_mask (IniFormat *dest_format, IniFormatMask mask);

extern unsigned long int ini_unquote (char *ini_string, const IniFormat format);

extern unsigned long int ini_array_get_length (const char *ini_string, const char delimiter, const IniFormat format);

extern unsigned int ini_array_foreach (
	const char *ini_string,
	const char delimiter,
	const IniFormat format,
	int (*f_foreach)(
		const char *member,
		unsigned int offset,
		unsigned int length,
		void *foreach_other
	),
	void *user_data
);

extern unsigned int ini_split_array (
	char *ini_string,
	const char delimiter,
	const IniFormat format,
	int (*f_foreach)(
		char *element,
		unsigned int length,
		void *foreach_other
	),
	void *user_data
);

extern signed int ini_get_bool (const char *ini_string, const signed int return_value);

extern signed int ini_get_lazy_bool (const char *ini_string, const signed int return_value);



/* PUBLIC WRAPPERS */


extern double (*ini_get_float) (const char *ini_string);

extern long long int (*ini_get_llint) (const char *ini_string);

extern long int (*ini_get_lint) (const char *ini_string);

extern int (*ini_get_int) (const char *ini_string);



/* PUBLIC CONSTANTS AND VARIABLES */


/** @brief	Error codes **/
enum ConfiniErrorNo {
	CONFINI_EFEINTR = 1,	/** Interrupted by the user during f_foreach **/
	CONFINI_EFEOOR = 2,	/** The loop is longer than expected (out of range) **/
	CONFINI_EIINTR = 3,	/** Interrupted by the user during f_init **/
	CONFINI_ENOMEM = 4,	/** Error allocating memory **/
	CONFINI_EIO = 5,	/** Error reading the file **/
	CONFINI_ENOENT = 6	/** File inaccessible **/
};

/** @brief	INI nodes types **/
enum IniNodeType {
	INI_UNKNOWN = 0,
	INI_DOCUMENT = 1,
	INI_SECTION = 2,
	INI_KEY = 3,
	INI_COMMENT = 4,
	INI_INLINE_COMMENT = 5,
	INI_DISABLED_SECTION = 6,
	INI_DISABLED_KEY = 7
};

/** @brief	Behaviors of '#' and ';' **/
enum IniComments {
	INI_PARSE_COMMENT = 0,
	INI_SHOW_COMMENT = 1,
	INI_ERASE_COMMENT = 2,
	INI_NORMAL_CHARACTER = 3
};

/** @brief	Multiline entries **/
enum IniMultiline {
	INI_EVERYTHING_MULTILINE = 0,
	INI_ACTIVE_AND_DISABLED_MULTILINE = 1,
	INI_ACTIVE_MULTILINE = 2,
	INI_NO_MULTILINE = 3
};

/** @brief	A model format **/
static const IniFormat INI_DEFAULT_FORMAT = _LIBCONFINI_DEFAULT_SETTINGS_;

/** @brief	Default value of implicit keys **/
extern char *INI_IMPLICIT_VALUE;

/** @brief	Default length of implicit keys' value -- it can be set to any unsigned number, independently of #INI_IMPLICIT_VALUE **/
extern unsigned long int INI_IMPLICIT_V_LENGTH;



/* CLEAN THE PRIVATE ENVIRONMENT */


#undef _LIBCONFINI_SETTINGS_AS_DECLARATION_
#undef _LIBCONFINI_SETTINGS_AS_ASSIGNEMENT_
#undef _LIBCONFINI_FORMAT_STRUCT_
#undef _LIBCONFINI_DEFAULT_SETTINGS_



/* END OF _LIBCONFINI_HEADER_ */


#endif



/* EOF */

