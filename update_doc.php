<?php


function replaceEditableRegion($fileid, $template, $region, $tag, $content = false) {
	global $db;
	
	$len = strlen('<!-- TemplateBeginEditable name="' . $region . '" -->');
	$pos1 = strpos($template, '<!-- TemplateBeginEditable name="' . $region . '" -->') + $len;
	$pos2 = strpos($template, '<!-- TemplateEndEditable -->', $pos1);
	if($content === false) {
		$template = substr($template, 0, $pos1) .
		            $db->querySingle("select value from tags where fileid=$fileid and name='$tag'") .
		            substr($template, $pos2);
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
	return $code;
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
	$template = turnOnSection($template, "ParametersSection", count($synparts) > 3);
	
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
	
	$template = replaceRepeatRegion($fileid, $template, 'Parameters', 'Parameter', $params);
	
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
	
	$template = replaceRepeatRegion($fileid, $template, 'Examples', 'Example', $examples);
	
	// see also
	$qseealso = $db->query("select * from tags where fileid=$fileid and name='seealso' order by tagorder");
	$total_seealso = 0;
	$seealso = "\n\t<ul>\n";
	while($see = $qseealso->fetchArray()) {
		$seealso .= "\t\t<li><a href=\"#\">$see[value]()</a> - $see[value]</li>\n";
		++$total_seealso;
	}
	$seealso .= "\t</ul>\n";
	$template = replaceEditableRegion($fileid, $template, 'SeeAlso', $seealso, true);
	
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
	    <td style="border-bottom: solid 1px #666; background-color: #DDDDDD"><!-- TemplateBeginEditable name="SectionTitle" --><strong><a href="index.html" target="_parent">Delta Documentation</a></strong><!-- TemplateEndEditable --></td>
	  </tr>
EOF;

	$catsplit = explode('/', $category);
	for($i = 0; $i < count($catsplit); ++$i) {
		$url = implode("-", array_slice($catsplit, 0, $i + 1)) . '.html';
		$title = ucfirst($catsplit[$i]);

		$sections .= <<<EOF
		  <tr>
		    <td style="border-bottom: solid 1px #666; background-color: #DDDDDD"><!-- TemplateBeginEditable name="SectionTitle" --><strong><a href="$url" target="_parent">$title</a></strong><!-- TemplateEndEditable --></td>
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

?>
