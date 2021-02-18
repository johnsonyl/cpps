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
		while(_thread.state == threadstate::running){
			var b = _thread.run();
			if(b == false) break;
		}
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