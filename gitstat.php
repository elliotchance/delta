<?php

$f = popen("git log --shortstat | cat", "r");
$lines = explode("\n", stream_get_contents($f));
pclose($f);

$files = 0;
$insertions = 0;
$deletions = 0;
$commits = 0;

foreach($lines as $line) {
	if(strpos($line, "files changed") !== false) {
		$parts = explode(" ", $line);
		$files += $parts[1];
		$insertions += $parts[4];
		$deletions += $parts[6];
		++$commits;
	}
}

// print result
echo "$files files changed, $insertions insertions(+), $deletions deletions(-)\n";
echo "$commits commits, ", ($insertions - $deletions), " net insertions(+)\n";

?>
