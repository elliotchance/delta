<?php

$root = "src";
$bin = "sandbox/bin";
$lib = "sandbox/lib";
$files = array();
$projects = array();

function buildCheckPattern($path, $rules) {
	$m = array();
	
	// check exclude patterns first
	foreach($rules as $r) {
		$parts = explode(" ", $r);
		if(substr($parts[1], 0, 1) != "!")
			continue;
			
		if(substr($parts[1], 0, 1) == "!") {
			if(fnmatch(substr($parts[1], 1), $path) ||
			   substr($parts[1], 1) == basename($path))
					return $m;
		}
	}
	
	// now check include patterns
	foreach($rules as $r) {
		$parts = explode(" ", $r);
		if(substr($parts[1], 0, 1) == "!")
			continue;
			
		if(substr($parts[1], 0, 1) != "!") {
			if(fnmatch($parts[1], $path) || $parts[1] == basename($path))
				$m[] = $parts[0];
		}
	}
	
	return $m;
}

function buildRecurseDirectory($d) {
	global $files, $projects;
	
	// get the delta_project.txt if it exists
	if(file_exists("$d/delta_project.txt")) {
		$delta_project = file("$d/delta_project.txt");
		
		$proj = "";
		foreach($delta_project as $p) {
			$line = trim($p);
			
			if(substr($line, 0, 5) == "begin") {
				$parts = explode(" ", $line);
				$proj = $parts[1];
				continue;
			}
			elseif($line == "end")
				continue;
			
			$key = substr($line, 0, strpos($line, "="));
			$projects[$proj][$key] = trim(substr($line, strlen($key) + 1));
		}
	}
	
	// get the delta_build.txt for the current directory
	$delta_build = array();
	if(file_exists("$d/delta_build.txt")) {
		$delta_build = file("$d/delta_build.txt");
		for($i = 0; $i < count($delta_build); ++$i)
			$delta_build[$i] = trim($delta_build[$i]);
	}
	
	$fs = scandir($d);
	foreach($fs as $f) {
		// ignore hidden files
		if(substr($f, 0, 1) == '.')
			continue;
		
		// recurse directories
		if(is_dir("$d/$f"))
			buildRecurseDirectory("$d/$f");
		else {
			// we only include files that match the pattern in delta_build.txt
			$matches = buildCheckPattern("$d/$f", $delta_build);
			if(count($matches) > 0) {
				foreach($matches as $m)
					$files[$m][] = "$d/$f";
			}
		}
	}
}

echo "Building file list...\n";
buildRecurseDirectory($root);
echo "Done (", count($files), " build projects found)\n\n";

//print_r($files);
//die(0);

echo "Preparing projects... ";

// make sure that we have the build details for each project
foreach($files as $k => $v) {
	if(!in_array($k, array_keys($projects)))
		die("\nCan not find project information for '$k', exiting.\n\n");
}
echo "Done\n\n";

foreach($projects as $k => $v) {
	$clean = array();

	echo "Compiling $k...\n";
	foreach($files[$k] as $f) {
		echo "  $f... ";
		system("gcc -c -w \"$f\" -o \"$f.o\" -Isrc");
		$clean[] = "$f.o";
		echo "Done\n";
	}
	echo "Done\n";
	
	echo "Building $v[name]... ";
	$sys = "gcc";
	if($v['type'] == "module")
		$sys .= " -dynamiclib";
		
	// might need to link against delta_core
	if($v['type'] == "module" && $v['name'] != "libdelta_core.dylib")
		$sys .= " \"$lib/libdelta_core.dylib\"";
		
	$sys .= " -o ";
	if($v['type'] == "executable")
		$sys .= "\"$bin/$v[name]\"";
	elseif($v['type'] == "module")
		$sys .= "\"$lib/$v[name]\"";
		
	foreach($clean as $c)
		$sys .= " \"$c\"";
	system($sys);
	echo "Done\n";
	
	echo "Cleaning up... ";
	foreach($clean as $c)
		unlink($c);
	echo "Done\n\n";
}

?>
