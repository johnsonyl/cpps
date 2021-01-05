var globalconfig ;
var initconfig(){
	var file = io.fopen("config/app.json","rb");
	var buffer = new Buffer();
	if(file == NULL){
		logger.error("config init faild...");
		exit(0);
	}
	io.fread(file,buffer,io.fsize(file));
	globalconfig = json.decode(buffer.tostring());
	io.fclose(file);
}
