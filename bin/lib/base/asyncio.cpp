module asyncio{
	async var call(var func,... params){
		
		var fu = std::_async(func);
		var status = fu.wait_for(1);
		
		while(status == future_status::timeout && !asyncio.is_terminate()){
			yield;
			status = fu.wait_for(1);
		}
		return fu.get();
	}
}
