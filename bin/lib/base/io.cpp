module io{
    var getpwd(var field,var &pwd,var k = "#")
    {
        print(field);
        var chr;
        while((chr = io.getch()) != char('\r') && chr != char('\n')){
            if(chr == 8){
                if(pwd.empty()) continue;
                print("\r{field}");
                foreach(var i : xrange(1,pwd.size())){
                    print(" ");
                }
                pwd.pop_back();
                print("\r{field}");
                foreach(var i : xrange(1,pwd.size())){
                    print(k);
                }
                continue;
            }
            else if(chr == 3){
                exit(0);
            }

            pwd.push_back(chr);
            print(k);
        }
        endl();
    }
}