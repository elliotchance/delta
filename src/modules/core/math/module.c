/*
 *  Delta programming language
 */


// prototypes here so we dont have to include more files
int delta_vm_push_function(struct DeltaFunction* f);
struct DeltaFunction* new_DeltaFunction(char *name,
										void (*function_ptr)(struct DeltaInstruction *d),
										int min_args, int max_args);


void delta_load_module_math()
{
	// TODO: IMPORTANT incomplete
	delta_vm_push_define("M_PI",       "3.14159265358979323846");
	delta_vm_push_define("M_E",        "2.7182818284590452354");
	delta_vm_push_define("M_LOG2E",    "1.4426950408889634074");
	delta_vm_push_define("M_LOG10E",   "0.43429448190325182765");
	delta_vm_push_define("M_LN2",      "0.69314718055994530942");
	delta_vm_push_define("M_LN10",     "2.30258509299404568402");
	delta_vm_push_define("M_PI_2",     "1.57079632679489661923");
	delta_vm_push_define("M_PI_4",     "0.78539816339744830962");
	delta_vm_push_define("M_1_PI",     "0.31830988618379067154");
	delta_vm_push_define("M_2_PI",     "0.63661977236758134308");
	delta_vm_push_define("M_SQRTPI",   "1.77245385090551602729");
	delta_vm_push_define("M_2_SQRTPI", "1.12837916709551257390");
	delta_vm_push_define("M_SQRT2",    "1.41421356237309504880");
	delta_vm_push_define("M_SQRT3",    "1.73205080756887729352");
	delta_vm_push_define("M_SQRT1_2",  "0.70710678118654752440");
	delta_vm_push_define("M_LNPI",     "1.14472988584940017414");
	delta_vm_push_define("M_EULER",    "0.57721566490153286061");
	delta_vm_push_define("INF",        "1e2000");
}
