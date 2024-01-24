--TEST--
test1() Basic test
--EXTENSIONS--
fstopwatch
--FILE--
<?php
$ret = test1();

var_dump($ret);
?>
--EXPECT--
The extension fstopwatch is loaded and working!
NULL
