/*
 *  Delta programming language
 */

#include "modules.h"
#include "delta/vm/vm.h"
#include "delta/structs/DeltaFunction.h"
#include "delta/platform.h"
#include <assert.h>
#include <dlfcn.h>


struct DeltaINI *delta_ini = NULL;


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


int delta_load_module_defines(struct DeltaCompiler *c, char *path)
{
	const char *error = NULL;
	int count;
	
	// load dynamically loaded library
	void *module = dlopen(path, RTLD_LAZY);
	if(!module) {
		fprintf(stderr, "Could not open %s: %s\n", path, dlerror());
		return DELTA_FAILURE;
	}
	
	// get the module defines
	dlerror();
	delta_module_defines_ptr module_defines = dlsym(module, "module_defines");
	if((error != dlerror())) {
		struct DeltaDefine *defines = (*module_defines)(&count);
		
		// load the defines into the virtual machine
		int i;
		for(i = 0; i < count; ++i)
			delta_vm_push_define(c, defines[i].name, defines[i].value);
	}
	
	return DELTA_SUCCESS;
}


int delta_load_module(struct DeltaVM *vm, char *path)
{
	const char *error;
	int function_count, count;
	int i, j;
	
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
	delta_module_ptr module_functions = dlsym(module, "module_functions");
	if((error = dlerror())) {
		fprintf(stderr, "Couldn't find module_functions: %s\n", error);
		return DELTA_FAILURE;
	}
	
	struct DeltaModuleFunction *functions = (*module_functions)(&function_count);
	
	// load the functions into the virtual machine
	for(i = 0; i < function_count; ++i) {
		delta_module_function f = delta_get_module_function(module, functions[i].name);
		if(f != NULL)
			delta_vm_push_function(vm, new_DeltaFunction(functions[i].name, f,
														 functions[i].min_args,
														 functions[i].max_args));
	}
	
	// get the module alias functions
	dlerror();
	delta_module_aliases_ptr module_alias_functions = dlsym(module, "module_aliases");
	if(module_alias_functions != NULL) {
		struct DeltaFunctionAlias *alias_functions = (*module_alias_functions)(&count);
		
		for(i = 0; i < count; ++i) {
			delta_module_function f = delta_get_module_function(module,
																alias_functions[i].original);
			
			if(f != NULL)
				delta_vm_push_function(vm, new_DeltaFunction(alias_functions[i].alias, f,
															 alias_functions[i].min_args,
															 alias_functions[i].max_args));
		}
	}
	
	// do not close
	// dlclose(module);
	return DELTA_SUCCESS;
}


void delta_load_ini()
{
	delta_ini = (struct DeltaINI*) malloc(sizeof(struct DeltaINI));
	
	delta_ini->module_count = 6;
	delta_ini->module_paths = (char**) calloc(delta_ini->module_count, sizeof(char*));
	
	// TODO: issue #14: delta.ini loading
	
#ifdef DELTA_PLATFORM_MAC
	delta_ini->module_paths[0] = "libdelta_core.dylib";
	delta_ini->module_paths[1] = "libdelta_mapm.dylib";
	delta_ini->module_paths[2] = "libdelta_mysql.dylib";
	delta_ini->module_paths[3] = "libdelta_sqlite3.dylib";
	delta_ini->module_paths[4] = "libdelta_thread.dylib";
	delta_ini->module_paths[5] = "libdelta_zlib.dylib";
#endif	
	
#ifdef DELTA_PLATFORM_LINUX
	delta_ini->module_paths[0] = "../lib/libdelta_core.so.1";
	delta_ini->module_paths[1] = "../lib/libdelta_mapm.so.1";
	delta_ini->module_paths[2] = "../lib/libdelta_mysql.so.1";
	delta_ini->module_paths[3] = "../lib/libdelta_sqlite3.so.1";
	delta_ini->module_paths[4] = "../lib/libdelta_thread.so.1";
	delta_ini->module_paths[5] = "../lib/libdelta_zlib.so.1";
#endif
}


void delta_load_modules(struct DeltaVM *vm)
{
	assert(delta_ini != NULL);
	
	int i;
	for(i = 0; i < delta_ini->module_count; ++i)
		delta_load_module(vm, delta_ini->module_paths[i]);
}


void delta_load_defines(struct DeltaCompiler *c)
{
	assert(delta_ini != NULL);
	
	int i;
	for(i = 0; i < delta_ini->module_count; ++i)
		delta_load_module_defines(c, delta_ini->module_paths[i]);
}
