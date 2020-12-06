var outer() {
      var result = [];
      result.resize(10);

      for (var i = 0; i<10;i++){
        result[i] = [](var num){
             return [](){
                   println(num);   // 此时访问的num，是上层函数执行环境的num，数组有10个函数对象，每个对象的执行环境下的number都不一样
             };
        }(i);
     }
     return result;
}
var l = outer();
foreach(var n : l)
{
	n();
}