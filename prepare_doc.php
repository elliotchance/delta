<?php

$db = NULL;

function parse_doc($fileid, $doc, $full) {
	global $db;
	
	// name
	$namepos = strpos($full, "DELTA_FUNCTION(") + 15;
	$name = substr($full, $namepos, strpos($full, ")", $namepos) - $namepos);

	$lines = explode("\n", $doc);
	for($i = 0; $i < count($lines); ++$i)
		$lines[$i] = trim(substr($lines[$i], strpos($lines[$i], "*") + 1));
	
	// extract tags
	$tags = array();
	$tags['name'] = $name;
	for($i = 0; $i < count($lines); ++$i) {
		$l = $lines[$i];
		
		if(strpos($l, "@brief") !== false)
			$tags['brief'] = trim(substr($l, strpos($l, "@brief") + 6));
		elseif(strpos($l, "@page") !== false)
			$tags['page'] = trim(substr($l, strpos($l, "@page") + 5));
		elseif(strpos($l, "@category") !== false)
			$tags['category'] = trim(substr($l, strpos($l, "@category") + 9));
		elseif(strpos($l, "@param") !== false) {
			$param = trim(substr($l, strpos($l, "@param") + 6));
			$key = substr($param, 0, strpos($param, " "));
			$param = substr($param, strlen($key));
			$tags['param'][] = trim("$key $param");
		}
		elseif(strpos($l, "@return") !== false)
			$tags['return'] = trim(substr($l, strpos($l, "@return") + 7));
		elseif(strpos($l, "@see") !== false)
			$tags['seealso'][] = trim(substr($l, strpos($l, "@see") + 4));
		elseif(strpos($l, "@syntax") !== false)
			$tags['syntax'][] = trim(substr($l, strpos($l, "@syntax") + 7));
		elseif(strpos($l, "@example") !== false) {
			$example = "";
			for(; $i < count($lines); ++$i) {
				$l = $lines[$i];
				if(strpos($l, "@endexample") !== false) {
					++$i;
					break;
				}
				$example .= "$l\n";
			}
			$tags['example'][] = substr($example, 8);
		}
		else
			$tags['description'] .= " $l";
	}
	
	foreach($tags as $k => $v) {
		$order = 0;
		if(is_array($v)) {
			foreach($v as $k2 => $v2) {
				$v2 = trim($v2);
				$sql = "insert into tags (fileid, tagorder, name, value) values " .
					   "('$fileid', $order, '$k', '" . $db->escapeString($v2) . "')";
				if(!$db->exec($sql))
					echo "SQL: $sql\n";
				++$order;
			}
		} else {
			$v = trim($v);
			$sql = "insert into tags (fileid, tagorder, name, value) values " .
				   "('$fileid', $order, '$k', '" . $db->escapeString($v) . "')";	
			if(!$db->exec($sql))
				echo "SQL: $sql\n";
		}
	}
}

function read_file($file) {
	global $db;
	
	// read whole file
	$contents = file_get_contents($file);
		
	$db->exec("insert into files (filepath, mtime) values " .
			  "(\"$file\", \"" . filemtime($file) . "\")");
	$fileid = $db->querySingle('SELECT max(id) FROM files');
	
	// extract the right doc tag
	$pos = strpos($contents, '@category');
	$pos = strrpos(substr($contents, 0, $pos), "/**") + 3;
	$doc = substr($contents, $pos, strpos($contents, "*/", $pos) - $pos);

	// if the doctag does not contain @category we ignore
	if(strpos($doc, "@category") === false)
		return false;
		
	echo "+ $file\n";
	parse_doc($fileid, $doc, $contents);
}

function read_directory($dir) {
	$files = scandir($dir);
	foreach($files as $f) {
		if(substr($f, 0, 1) == '.')
			continue;
			
		if(is_dir("$dir/$f"))
			read_directory("$dir/$f");
		else
			read_file("$dir/$f");
	}
}

function init() {
	global $db;
	
	if(file_exists("docdb.sqlite3")) {
		$db = new SQLite3("docdb.sqlite3");
		return true;
	}
		
	$db = new SQLite3("docdb.sqlite3");

	$db->exec("CREATE TABLE files (id INTEGER PRIMARY KEY, filepath TEXT, mtime TEXT)");
	$db->exec('CREATE TABLE tags (fileid INTEGER, tagorder INTEGER, name TEXT, value TEXT)');
}

unlink("docdb.sqlite3");
$dir = "src";
init();
read_directory($dir);

?>
