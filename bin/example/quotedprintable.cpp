#import "quotedprintable"

var a = '<div class="testclass">test</div>';

var en = quotedprintable.encode(a);
println(en);
var de = quotedprintable.decode(en);
println(de);