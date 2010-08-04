/*
 *  Delta programming language
 */

#include "modules.h"
#include "delta/vm/vm.h"
#include "delta/structs/DeltaFunction.h"
#include <dlfcn.h>
#include "delta/platform.h"


delta_module_function delta_get_module_function(void *module, char *name)
{
	const char *error;
	
	// translate to real C name
	char real_name[64];
	sprintf(real_name, "delta_function_%s", name);
	
	// get the module function
	dlerror();
	delta_module_function ret = dlsym(module, real_name);
	if((error = dlerror())) {
		fprintf(stderr, "Couldn't find %s: %s\n", real_name, error);
		return NULL;
	}
	
	return ret;
}


int delta_load_module(struct DeltaVM *vm, char *path)
{
	const char *error;
	delta_module_ptr module_functions;
	
	// load dynamically loaded library
	void *module = dlopen(path, RTLD_LAZY);
	if(!module) {
		fprintf(stderr, "Could not open %s: %s\n", path, dlerror());
		return DELTA_FAILURE;
	}
	
	// pass the active VM
	delta_module_function_vm send_vm = dlsym(module, "module_set_vm");
	if(send_vm != NULL) {
		(*send_vm)(delta_get_vm());
	}
	
	// get the module functions
	dlerror();
	module_functions = dlsym(module, "module_functions");
	if((error = dlerror())) {
		fprintf(stderr, "Couldn't find module_functions: %s\n", error);
		return DELTA_FAILURE;
	}
	
	int count;
	struct DeltaModuleFunction *functions = (*module_functions)(&count);
	
	// load the functions into the virtual machine
	int i;
	for(i = 0; i < count; ++i) {
		delta_module_function f = delta_get_module_function(module, functions[i].name);
		if(f != NULL)
			delta_vm_push_function(vm, new_DeltaFunction(functions[i].name, f,
														 functions[i].min_args,
														 functions[i].max_args));
	}
	
	
	// do not close
	// dlclose(module);
	return DELTA_SUCCESS;
}


void delta_load_modules(struct DeltaVM *vm)
{
	// TODO: issue #14: delta.ini loading
	
#ifdef DELTA_PLATFORM_MAC
	delta_load_module(vm, "libdelta_core.dylib");
	delta_load_module(vm, "libdelta_mapm.dylib");
	delta_load_module(vm, "libdelta_mysql.dylib");
	delta_load_module(vm, "libdelta_sqlite3.dylib");
	delta_load_module(vm, "libdelta_thread.dylib");
	delta_load_module(vm, "libdelta_zlib.dylib");
#endif	

#ifdef DELTA_PLATFORM_LINUX
	delta_load_module(vm, "../lib/libdelta_core.so.1");
	delta_load_module(vm, "../lib/libdelta_mapm.so.1");
	delta_load_module(vm, "../lib/libdelta_mysql.so.1");
	delta_load_module(vm, "../lib/libdelta_sqlite3.so.1");
	delta_load_module(vm, "../lib/libdelta_thread.so.1");
	delta_load_module(vm, "../lib/libdelta_zlib.so.1");
#endif
}
