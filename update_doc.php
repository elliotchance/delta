<?php

$keywords = array(
	'if', 'for'
);

function replaceEditableRegion($fileid, $template, $region, $tag, $content = false) {
	global $db;
	
	$len = strlen('<!-- TemplateBeginEditable name="' . $region . '" -->');
	$pos1 = strpos($template, '<!-- TemplateBeginEditable name="' . $region . '" -->') + $len;
	$pos2 = strpos($template, '<!-- TemplateEndEditable -->', $pos1);
	if($content === false) {
		$data = $db->querySingle("select value from tags where fileid=$fileid and name='$tag'");
		if($data == '')
			$data = '<i>No information.</i>';
			
		$template = substr($template, 0, $pos1) . $data . substr($template, $pos2);
	}
	else
		$template = substr($template, 0, $pos1) . $tag . substr($template, $pos2);
		
	return $template;
}


function replaceRepeatRegion($fileid, $template, $region, $region2, $repeat) {
	global $db;

	$len = strlen('<!-- TemplateBeginRepeat name="' . $region . '" -->');
	$pos1 = strpos($template, '<!-- TemplateBeginRepeat name="' . $region . '" -->') + $len;
	$pos2 = strpos($template, '<!-- TemplateEndRepeat -->', $pos1);
	
	$newblock = "";
	foreach($repeat as $r) {
		$newblock .= "\n" . '<!-- TemplateBeginEditable name="' . $region2 . '" -->' . "\n$r\n" .
		             '<!-- TemplateEndEditable -->';
	}
	
	$template = substr($template, 0, $pos1) . "$newblock\n" . substr($template, $pos2);

	return $template;
}


function formatSyntax($raw) {
	if(trim($raw) == '')
		return '<i>No information.</i>';
	
	$parts = explode(" ", $raw);
	$parts[1] = "<b>$parts[1]</b>";
	
	// params
	for($i = 4; $i < count($parts); $i += 3)
		$parts[$i] = "<span class=\"param\">$parts[$i]</span>";
	
	return implode(" ", $parts);
}


function turnOnSection($template, $name, $bool) {
	$newvalue = ($bool ? 'true' : 'false');
	
	$len = strlen('<!-- TemplateParam name="' . $name . '" type="boolean" value="');
	$pos1 = strpos($template, '<!-- TemplateParam name="' . $name . '" type="boolean" value="') + $len;
	$pos2 = strpos($template, '" -->', $pos1);
	
	return substr($template, 0, $pos1) . $newvalue . substr($template, $pos2);
}


function formatCode($code) {
	global $keywords;
	$code = trim($code);
	
	// this algorithm is fairly simple
	$r = "";
	$term = "";
	$last_term = "";
	$classes = array();
	for($i = 0; $i < strlen($code); ++$i) {
		
		// string
		if(substr($code, $i, 1) == "\"") {
			$string = "\"";
			++$i;
			for(; $i < strlen($code); ++$i) {
				$c = substr($code, $i, 1);
				$string .= $c;
				if($c == "\"" && substr($code, $i - 1, 1) != "\\") {
					++$i;
					break;
				}
			}
			$r .= "<span class=\"codestring\">$string</span>";
		}
		
		// character constant
		if(substr($code, $i, 1) == "'") {
			$string = "\"";
			++$i;
			for(; $i < strlen($code); ++$i) {
				$c = substr($code, $i, 1);
				$string .= $c;
				if($c == "'" && substr($code, $i - 1, 1) != "\\") {
					++$i;
					break;
				}
			}
			$r .= "<span class=\"codestring\">$string</span>";
		}
		
		// comments
		if(substr($code, $i, 2) == "//") {
			$comment = "";
			for(; $i < strlen($code); ++$i) {
				$c = substr($code, $i, 1);
				if($c == "\n") break;
				$comment .= $c;
			}
			$r .= "<span class=\"codecomment\">$comment</span>";
		}
		if(substr($code, $i, 2) == "/*") {
			$comment = "";
			for(; $i < strlen($code); ++$i) {
				$c = substr($code, $i, 1);
				if($c == "*" && substr($code, $i + 1, 1) == "/") {
					$comment .= "*/";
					$i += 2;
					break;
				}
				$comment .= $c;
			}
			$r .= "<span class=\"codecomment\">$comment</span>";
		}
		
		// normal code
		$c = substr($code, $i, 1);
		if(ctype_alnum($c) || $c == '.') $term .= $c;
		else {
			if($last_term == "class") {
				$r .= "<span class=\"codeobject\">$term</span>";
				array_push($classes, $term);
			} elseif(in_array($term, $keywords))
				$r .= "<span class=\"codekw\">$term</span>";
			elseif(in_array($term, $classes))
				$r .= "<span class=\"codeobject\">$term</span>";
			elseif(is_numeric($term) || is_numeric(substr($term, 0, strlen($term) - 1)))
				$r .= "<span class=\"codenum\">$term</span>";
			else $r .= $term;
			
			if($c == "\n")
				$r .= "<br/>";
			else if($c == " ")
				$r .= "&nbsp;";
			else if($c == "\t")
				$r .= "&nbsp;&nbsp;";
			else
				$r .= htmlspecialchars($c);
			
			$last_term = $term;
			$term = "";
		}
	}
	$r .= $term;
	
	return $r;
}


function removeConditionalRegion($template, $name) {
	// make sure we turn the region off
	$template = turnOnSection($template, $name, false);
	
	$pos1 = strpos($template, '<!-- TemplateBeginIf cond="' . $name . '" -->');
	$pos2 = strpos($template, '<!-- TemplateEndIf -->', $pos1);
	
	return substr($template, 0, $pos1) . substr($template, $pos2 + strlen('<!-- TemplateEndIf -->'));
}


function countRows($q) {
	$i = 0;
	while($q->fetchArray())
		++$i;
	$q->reset();
	return $i;
}


function writeModulePage($category) {
	global $db;

	// get template
	$template = file_get_contents("Templates/general.dwt");

	// title
	$category2 = str_replace('/', ' &gt; ', $category);
	$template = replaceEditableRegion($fileid, $template, 'Name', "<h1>$category2</h1>", true);

	// nav
	if($category != '') {
		$nav = '<iframe src="' . str_replace('/', '-', $category) . '.html" id="navframe"></iframe>';
		$template = replaceEditableRegion($fileid, $template, 'Nav', $nav, true);
	}
	
	$sections = '<div class="section">';
	
	// functions
	$q = $db->query("select t2.fileid, t2.value from tags as t1 join tags as t2 on t1.fileid=t2.fileid where t1.name='category' and t1.value='$category' and t2.name='name'");
	if(countRows($q)) {
		$sections .= <<<EOF
	<!-- TemplateBeginEditable name="SectionTitle" -->
		<h2><a name="functions" id="functions"></a>Functions</h2>
	<!-- TemplateEndEditable -->
	<!-- TemplateBeginEditable name="SectionBody" -->
EOF;

		while($r = $q->fetchArray()) {
			$brief = $db->querySingle("select value from tags where fileid=$r[fileid] and name='brief'");
			$url = str_replace('/', '-', $category) . "-$r[value].html";
			$sections .= "\n\t\t<a href=\"$url\">$r[value]()</a> - $brief<br/>";
		}
		$sections .= "\n\t<!-- TemplateEndEditable -->";
	}
	
	// submodules
	$q = $db->query("select distinct value from tags where name='category' and value like '$category/%'");
	if(countRows($q)) {
		$sections .= <<<EOF
	<!-- TemplateBeginEditable name="SectionTitle" -->
		<h2><a name="functions" id="functions"></a>Submodules</h2>
	<!-- TemplateEndEditable -->
	<!-- TemplateBeginEditable name="SectionBody" -->
EOF;

		while($r = $q->fetchArray()) {
			$r['value'] = substr($r['value'], strrpos($r['value'], '/') + 1);
			$url = str_replace('/', '-', $category) . "-$r[value]-index.html";
			$sections .= "\n\t\t<a href=\"$url\">$r[value]</a><br/>";
		}
		$sections .= "\n\t<!-- TemplateEndEditable -->";
	}

	// set
	$sections .= "\n</div>";
	$len = strlen('<!-- TemplateBeginRepeat name="Sections" -->');
	$pos1 = strpos($template, '<!-- TemplateBeginRepeat name="Sections" -->') + $len;
	$pos2 = strpos($template, '<!-- TemplateEndRepeat -->', $pos1);
	$template = substr($template, 0, $pos1) . "$sections\n" . substr($template, $pos2);

	// save
	$filename = 'doc/' . str_replace('/', '-', $category) . "-index.html";
	echo $filename, "\n";
	file_put_contents($filename, $template);
}


$db = new SQLite3("docdb.sqlite3");
$q = $db->query("select * from files");
while($r = $q->fetchArray()) {
	// get template
	$template = file_get_contents("Templates/function.dwt");
	
	// get file name
	$fileid = $r['id'];
	$syntax = $db->querySingle("select value from tags where fileid=$fileid and name='syntax'");
	$synparts = explode(" ", $syntax);
	
	// nav
	$category = $db->querySingle("select value from tags where fileid=$fileid and name='category'");
	$category = str_replace('/', '-', $category);
	if($category != '') {
		$nav = '<iframe src="' . $category . '.html" id="navframe"></iframe>';
		$template = replaceEditableRegion($fileid, $template, 'Nav', $nav, true);
	}
	
	// set function name
	$function = $db->querySingle("select value from tags where fileid=$fileid and name='name'");	
	$filename = "doc/$category-$function.html";
	$template = replaceEditableRegion($fileid, $template, 'FunctionName', $function, true);
	
	// set short and long description
	$template = replaceEditableRegion($fileid, $template, 'ShortDescription', 'brief');
	$template = replaceEditableRegion($fileid, $template, 'Description', 'description');
	
	// set syntax
	$syntax = '<span class="syntax">' .
		formatSyntax($syntax) .
		'</span>';
	$template = replaceEditableRegion($fileid, $template, 'Syntax', $syntax, true);
	
	// parameters
	$qparams = $db->query("select * from tags where fileid=$fileid and name='param' order by tagorder");
	$params = array();
	while($param = $qparams->fetchArray()) {
		$param_split = explode(" ", $param['value']);
		$paramdesc = trim(implode(" ", array_slice($param_split, 1)));
		
		$out = <<<EOF
	<tr>
		<td width="25" nowrap="nowrap" class="paramd">$param_split[0]</td>
		<td>$paramdesc</td>
	</tr>
EOF;
		$params[] = $out;
	}
	
	if(count($params))
		$template = replaceRepeatRegion($fileid, $template, 'Parameters', 'Parameter', $params);
	else
		$template = removeConditionalRegion($template, 'ParametersSection');
	
	// return
	$template = replaceEditableRegion($fileid, $template, 'Return', 'return');
	
	// examples
	$qexamples = $db->query("select * from tags where fileid=$fileid and name='example' order by tagorder");
	$examples = array();
	while($example = $qexamples->fetchArray()) {
		$example_split = explode("\n", $example['value']);
		$example_full = formatCode(trim(implode("\n", array_slice($example_split, 1))));
		
		$out = <<<EOF
	<table width="100%" border="0" cellspacing="0" cellpadding="3" class="examplebox">
		<tr>
			<td class="examplehead">$example_split[0]</td>
		</tr>
		<tr>
			<td class="examplebody"><pre style="margin: 0; border: none">$example_full</pre></tr>
	</table>
EOF;
		$examples[] = $out;
	}
	
	if(count($examples))
		$template = replaceRepeatRegion($fileid, $template, 'Examples', 'Example', $examples);
	else
		$template = removeConditionalRegion($template, 'ExamplesSection');
	
	// see also
	$qseealso = $db->query("select * from tags where fileid=$fileid and name='seealso' order by tagorder");
	$total_seealso = 0;
	$seealso = "\n\t<ul>\n";
	while($see = $qseealso->fetchArray()) {
		$url = str_replace('/', '-', $category) . '-' . $see['value'] . '.html';
		$briefid = $db->querySingle("select tags.fileid from tags join tags t2 on t2.fileid=tags.fileid where tags.name='category' and tags.value='modules/core/math' and t2.name='name' and t2.value='$see[value]'");
		$brief = $db->querySingle("select value from tags where name='brief' and fileid='$briefid'");
		$seealso .= "\t\t<li><a href=\"$url\">$see[value]()</a> - $brief</li>\n";
		++$total_seealso;
	}
	$seealso .= "\t</ul>\n";
	
	if($total_seealso)
		$template = replaceEditableRegion($fileid, $template, 'SeeAlso', $seealso, true);
	else
		$template = removeConditionalRegion($template, 'SeeAlsoSection');
	
	echo $filename, "\n";
	file_put_contents($filename, $template);
}


// now we do the function lists for the navigation panel
$qnav = $db->query("select distinct value from tags where name='category' order by value");
while($rnav = $qnav->fetchArray()) {
	$category = $rnav['value'];
	
	// get template
	$template = file_get_contents("Templates/function_list.dwt");

	// set sections
	$sections = <<<EOF
	  <tr>
	    <td class="nav_module"><!-- TemplateBeginEditable name="SectionTitle" --><strong><a href="index.html" target="_parent">Delta Documentation</a></strong><!-- TemplateEndEditable --></td>
	  </tr>
EOF;

	$catsplit = explode('/', $category);
	for($i = 0; $i < count($catsplit); ++$i) {
		$url = implode("-", array_slice($catsplit, 0, $i + 1)) . '-index.html';
		$before = str_repeat('&nbsp;', ($i + 1) * 2);
		$title = ucfirst($catsplit[$i]);

		$sections .= <<<EOF
		  <tr>
		    <td class="nav_module"><!-- TemplateBeginEditable name="SectionTitle" --><strong>$before<a href="$url" target="_parent">$title</a></strong><!-- TemplateEndEditable --></td>
		  </tr>
EOF;
	}

	$len = strlen('<!-- TemplateBeginRepeat name="Sections" -->');
	$pos1 = strpos($template, '<!-- TemplateBeginRepeat name="Sections" -->') + $len;
	$pos2 = strpos($template, '<!-- TemplateEndRepeat -->', $pos1);
	$template = substr($template, 0, $pos1) . "$sections\n" . substr($template, $pos2);

	// set function list
	$function_list = "\n<ul>";

	$q = $db->query("select distinct fileid from tags where name='category' and value='$category'");
	while($r = $q->fetchArray()) {
		$fname = $db->querySingle("select value from tags where fileid=$r[fileid] and name='name'");
		$fcat = $db->querySingle("select value from tags where fileid=$r[fileid] and name='category'");
		$fcat = str_replace('/', '-', $fcat);
		$function_list .= "\n\t<li><a href=\"$fcat-$fname.html\" target=\"_parent\">$fname()</a></li>";
	}

	$function_list .= "\n<ul>\n";
	$template = replaceEditableRegion($fileid, $template, 'FunctionList', $function_list, true);

	// save
	$filename = 'doc/' . str_replace('/', '-', $category) . ".html";
	echo $filename, "\n";
	file_put_contents($filename, $template);
}


// module pages
$qnav = $db->query("select distinct value from tags where name='category' order by value");
$module_pages = array();
while($rnav = $qnav->fetchArray())
	$module_pages[] = $rnav['value'];
	
// create the missing module pages
foreach($module_pages as $page) {
	$parts = explode("/", $page);
	for($i = 1; $i < count($parts); ++$i)
		$module_pages[] = implode("/", array_slice($parts, 0, $i));
}
$module_pages = array_unique($module_pages);

foreach($module_pages as $module_page)
	writeModulePage($module_page);

?>
