module sys
{
    var a = 10;

    var test()
    {
        println("sys.test");
    }
    class TEST
    {
        TEST()
        {

        }
        var test(){
            println("sys::TEST::test()");
        }
    }
}

println(sys.a);
println(sys.version);
sys.test();

var t = new sys::TEST();
t.test();