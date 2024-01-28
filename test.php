<?php
namespace foefkeo{
class A {
function a():void{
    static $i=0;
    if($i++ > 7) {
    return;}
        var_dump(null);
    sleep(1);
    $this->a();

}
}
(new A())->a();
}