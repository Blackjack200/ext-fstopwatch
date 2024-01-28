<?php
namespace wow{
class A {
function a():void{
    static $i=0;
    if(++$i >= 2) {
    return;
    }
        var_dump(null);
    usleep(300000);
    $this->a();

}
}
(new A())->a();
}