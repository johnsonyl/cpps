#import "htmlparser"
var html = io.readfile("test.html");

var parser = new htmlparser::parser();
var element = parser.parse(html);
if(element)
{
    println(element.text());
}