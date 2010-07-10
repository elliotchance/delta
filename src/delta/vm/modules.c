/*
 *  Delta programming language
 */

#include "modules.h"

// module includes
#include "modules/array/module.h"
#include "modules/ctype/module.h"
#include "modules/date/module.h"
#include "modules/errors/module.h"
#include "modules/file/module.h"
#include "modules/io/module.h"
#include "modules/math/module.h"
#include "modules/mapm/module.h"
#include "modules/misc/module.h"
#include "modules/string/module.h"
#include "modules/variables/module.h"
#include "modules/zlib/module.h"


void delta_load_modules()
{
	delta_load_module_array();
	delta_load_module_ctype();
	delta_load_module_date();
	delta_load_module_errors();
	delta_load_module_file();
	delta_load_module_io();
	delta_load_module_math();
	delta_load_module_mapm();
	delta_load_module_misc();
	delta_load_module_string();
	delta_load_module_variables();
	delta_load_module_zlib();
}
