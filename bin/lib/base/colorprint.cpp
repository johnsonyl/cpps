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
const var COLOR_RED = 1;
const var COLOR_GREEN = 2;
const var COLOR_YELLOW = 3;
const var COLOR_BLUE = 4;
const var COLOR_PURPLE = 5;
const var COLOR_CYAN = 6;