<?php

$x = 50000000;
$i = 2;
$factors = 0;
while($i < $x) {
	if($x % $i == 0) {
		$factors = $factors + 1;
	}
	$i = $i + 1;
}
echo "Factors = ", $factors, "\n";

?>
