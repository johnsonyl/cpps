#import "socket"

var ss = socket.render2str("html2str.html",{
	"testname" : "haha"
});
println(ss);