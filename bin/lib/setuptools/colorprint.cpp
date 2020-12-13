var println_color(var s,var color)
{
	console.color(color);
	println(s);
	console.clearcolor();
}
var print_color(var s,var color)
{
	console.color(color);
	print(s);
	console.clearcolor();
}
var sleepexit(var ms)
{
	sleep(ms);
	exit(0);
}