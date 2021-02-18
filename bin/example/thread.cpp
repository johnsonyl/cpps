// thread

var tt = new thread([](var data){
	println(data);
},"hello world");


tt.join();





//class 方式的basethread
var iii = 10;
class testthread : basethread
{
	var o;
	var run(){
		this_thread.sleep_for(chrono.seconds(1));
		println(this_thread.get_id());
		println(iii+o);
		return true;
	}
}

var t = new testthread(){
	o = 1
};
t.start();

var t2 = new testthread(){
	o = 2
};
t2.start();

sleep(5000);
t.stop();
t2.stop();