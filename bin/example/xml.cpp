#import "xml"

var xml_doc = xml.load("<root><a/><b/></root>");
var nodes = xml_doc.children();
foreach(var node : nodes)
{
    println(node.name());
}
var nodes = xml_doc.children()[0].children();
foreach(var node : nodes)
{
    println(node.name());
}

