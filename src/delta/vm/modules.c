/*
 *  Delta programming language
 */

#include "modules.h"
#include "delta/vm/vm.h"
#include "delta/structs/DeltaFunction.h"
#include "delta/platform.h"
#include "delta/compiler/strings.h"
#include <assert.h>
#include <dlfcn.h>
#include <sys/stat.h>


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
	char *apath = path; //delta_combine_paths(delta_ini->module_base, path);
	void *module = dlopen(apath, RTLD_NOW);
	if(!module) {
		fprintf(stderr, "Could not open %s: %s\n", apath, dlerror());
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
	int function_count, count, i, j;
	
	// load dynamically loaded library
	char *apath = path; //delta_combine_paths(delta_ini->module_base, path);
	void *module = dlopen(apath, RTLD_NOW);
	if(!module) {
		fprintf(stderr, "Could not open %s: %s\n", apath, dlerror());
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
														 functions[i].max_args,
														 functions[i].is_static,
														 functions[i].permission));
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
															 alias_functions[i].max_args,
															 alias_functions[i].is_static,
															 alias_functions[i].permission));
		}
	}
	
	// do not close
	// dlclose(module);
	return DELTA_SUCCESS;
}

int delta_load_compiler_module(struct DeltaCompiler *c, char *path)
{
	const char *error;
	int function_count, count, i, j;
	
	// load dynamically loaded library
	char *apath = path; //delta_combine_paths(delta_ini->module_base, path);
	void *module = dlopen(apath, RTLD_NOW);
	if(!module) {
		fprintf(stderr, "Could not open %s: %s\n", apath, dlerror());
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
	for(i = 0; i < function_count; ++i)
		c->delta_functions[c->total_delta_functions++] = functions[i];
	
	// get the module alias functions
	/*dlerror();
	delta_module_aliases_ptr module_alias_functions = dlsym(module, "module_aliases");
	if(module_alias_functions != NULL) {
		struct DeltaFunctionAlias *alias_functions = (*module_alias_functions)(&count);
		
		for(i = 0; i < count; ++i) {
			delta_module_function f = delta_get_module_function(module,
																alias_functions[i].original);


			if(f != NULL)
				delta_vm_push_function(vm, new_DeltaFunction(alias_functions[i].alias, f,
															 alias_functions[i].min_args,
															 alias_functions[i].max_args,
															 alias_functions[i].is_static,
															 alias_functions[i].permission));
		}
	}*/
	
	// do not close
	// dlclose(module);
	return DELTA_SUCCESS;
}


int delta_file_exists(char *path)
{
	struct stat stFileInfo;
	int intStat = stat(path, &stFileInfo);
	return (intStat == 0);
}


struct DeltaINI* new_DeltaINI()
{
	struct DeltaINI *r = (struct DeltaINI*) malloc(sizeof(struct DeltaINI));
	
	r->module_base = NULL;
	r->module_count = 0;
	r->module_paths = NULL;
	
	return r;
}


void delta_load_ini()
{
	// init
	delta_ini = new_DeltaINI();
	
	// find INI
	FILE *ini = NULL;
	char *ini_path = delta_combine_paths(delta_cwd(), "delta.ini");
	if(delta_file_exists(ini_path)) {
		ini = fopen(ini_path, "r");
	}
	else {
		printf("Could not find %s\n", ini_path);
		exit(0);
	}

	// read lines
	char line[1024];
	while(fgets(line, sizeof(line), ini) != NULL) {
		char *l = delta_trim(line);
		
		// ignore comments
		if(l[0] == '#')
			continue;
		
		// map to INI struct
		int parts_len;
		char **parts = delta_split(l, "=", &parts_len);
		char *k = delta_trim(parts[0]), *v = delta_trim(parts[1]);
		
		if(!strcmp(k, "module_base"))
			delta_ini->module_base = delta_combine_paths(delta_cwd(), v);
		if(!strcmp(k, "module_add"))
			delta_ini->module_paths = delta_push(delta_ini->module_paths,
												 &delta_ini->module_count, v);
		
	}
	//printf("delta_ini->module_base = '%s'\n", delta_ini->module_base);
	
	fclose(ini);
	
	// Mac OS X has stupid policies for dlopen() and ignores absolute paths, we have to manually set
	// the DYLD_LIBRARY_PATH
#ifdef DELTA_PLATFORM_MAC
	setenv("DYLD_LIBRARY_PATH", delta_ini->module_base, 1);
	printf("DYLD_LIBRARY_PATH = '%s'\n", getenv("DYLD_LIBRARY_PATH"));
#endif
	
	delta_ini->module_paths[0] = "/Users/elliot/Xcode/delta/build/Debug/libdelta_core.dylib";
	delta_ini->module_paths[1] = "/Users/elliot/Xcode/delta/build/Debug/libdelta_mapm.dylib";
	delta_ini->module_paths[2] = "/Users/elliot/Xcode/delta/build/Debug/libdelta_mysql.dylib";
	delta_ini->module_paths[3] = "/Users/elliot/Xcode/delta/build/Debug/libdelta_sqlite3.dylib";
	delta_ini->module_paths[4] = "/Users/elliot/Xcode/delta/build/Debug/libdelta_thread.dylib";
	delta_ini->module_paths[5] = "/Users/elliot/Xcode/delta/build/Debug/libdelta_zlib.dylib";
	
	// TODO: issue #14: delta.ini loading
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


void delta_load_compiler_modules(struct DeltaCompiler *c)
{
	assert(delta_ini != NULL);
	c->alloc_delta_functions = 200;
	c->total_delta_functions = 0;
	c->delta_functions = (struct DeltaModuleFunction*)
		calloc(c->alloc_delta_functions, sizeof(struct DeltaModuleFunction));
	
	int i;
	for(i = 0; i < delta_ini->module_count; ++i)
		delta_load_compiler_module(c, delta_ini->module_paths[i]);
}
