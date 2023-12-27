//simple like iostream..
class EndLineStream{

}

class COutStream
{
	
	var operator << (var v){
		if(iskindof(EndLineStream,v)){
			print("\r\n");
		}else{
			print(v);
		}
		return this;
	}
	
}
class CInStream{
	var operator >> (var &v){
		v = io.getc();
	}
}

var endl = new EndLineStream();
var cout = new COutStream();
var cin = new CInStream();

cout << "test" << endl; 
cout << "test2" << endl; 

var s = "";

cin >> s;

cout << "s = " << s << endl;
