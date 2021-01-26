#import "re"

println(re.split('a*', 'hello world'));
println(re.split(r'\W+', 'runoob, runoob, runoob.'));
var it = re.finditer(r"\d+","12a32bc43jf3") 
foreach(var match : it)
    println (match.group() );

var pattern = re.compile(r'\d+')   // 查找数字
var result1 = pattern.findall('runoob 123 google 456')
var result2 = pattern.findall('run88oob123google456', 0, 10)
 
println(result1)
println(result2)

var double_func(var matched){
    var value = int(matched.group('value'))
    return str(value * 2)
}

 
var s = 'A23G4HFD567'
println(re.sub(r'(?P<value>\d+)', double_func, s))

var phone = "2004-959-559"// 这是一个国外电话号码"
 
// 删除字符串中的 Python注释 
var num = re.sub(r'#.*$', "", phone)
println("电话号码是: {num}");
 
// 删除非数字(-)的字符串 
num = re.sub(r'\D', "", phone)
println("电话号码是: {num}");

var line = "Cats are smarter than dogs";
 
var matchObj = re.match( r'dogs', line)
if (matchObj)
   println("match --> matchObj.group() : {matchObj.group()}");
else
   println( "No match!!")
 
matchObj = re.search( r'dogs', line)
if (matchObj)
   println( "search --> searchObj.group() : {matchObj.group()}");
else
   println( "No match!!")
   
   
println(re.search('www', 'www.runoob.com').span())  //在起始位置匹配
println(re.search('com', 'www.runoob.com').span())  // 不在起始位置匹配


var line = "Cats are smarter than dogs"
 
var matchObj = re.match( r'(.*) are (.*?) .*', line)
 
if (matchObj)
{
    println("matchObj.group() : {matchObj.group()}");
    println("matchObj.group(1) : {matchObj.group(1)}");
    println("matchObj.group(2) : {matchObj.group(2)}");
}
else
   println("No match!!");
   
println(re.match('www', 'www.runoob.com').span());