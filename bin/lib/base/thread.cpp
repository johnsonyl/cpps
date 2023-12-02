enum threadstate {
	pending,
	running,
	stoping,
	stoped
};

class basethread
{
	var __thread;
	var state;
	basethread(){
		state = threadstate::pending;
		__thread = null;
	}
	//virsual
	var 	run(){ return false;}
	//static
	var 	_basethread_thread_func(var _thread){
		if(isvalid(_thread.init)) _thread.init();
		while(_thread.state == threadstate::running){
			var b = _thread.run();
			if(b == false) break;
		}
		_thread.state = threadstate::stoped;
	}
	var 	start(){
		state = threadstate::running;
		__thread = new thread(_basethread_thread_func,this);
	}
	var 	stop(){
		if(state == threadstate::running){
			state = threadstate::stoping;
			__thread.join();
			__thread = null;
			state = threadstate::stoped;
		}
	}
	var isrunning(){
		return state == threadstate::running;
	}
}

enum future_status{
	ready = 0,
	timeout = 1,
	deferred = 2	//延迟执行的暂未实现
};

class future{
	var _thread = null;
	var lock = new mutex();
	future(var func,... params){
		_thread = new thread(func,params);
	}
	var wait_for(var tm){
		lock.lock();
		var curtm = time.gettickcount();
		while(time.gettickcount() - curtm < tm){
			if(_thread.isdone()) {
				lock.unlock();
				return future_status::ready;
			}
			sleep(1);
		}
		lock.unlock();
		
		return future_status::timeout;
	}
	var wait(){
		lock.lock();
		_thread.join();
		_thread = null;
		lock.unlock();
	}
	var get(){
		return _thread.get();
	}
}

namespace std{
	//std::async(
	var _async(var func,... params){
		return new future(func,params);
	}
}