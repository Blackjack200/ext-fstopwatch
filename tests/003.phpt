--TEST--
test2() Basic test
--EXTENSIONS--
fstopwatch
--FILE--
<?php
var_dump(null);
var_dump(null);
var_dump(null);
var_dump(null);
?>
--EXPECT--
string(11) "Hello World"
string(9) "Hello PHP"
