system("chcp 65001"); //use utf8 string
const var CHECKBROADSIZE = 15;

var g_checkboard = new vector[15];
foreach(var i : xrange(0,g_checkboard.size()))
    g_checkboard[i] = new vector[15]; //2维数组

var g_currentgamer = 1; //黑子
var g_corsorX = int(CHECKBROADSIZE / 2),g_corsorY = int(CHECKBROADSIZE/2);
var g_count = 0;


var init()
{
    foreach(var i : xrange(0,CHECKBROADSIZE))
        foreach(var j : xrange(0,CHECKBROADSIZE))
            g_checkboard[i][j] = 0;

}

var drawcheckboard()
{
    for(var i = 0; i < CHECKBROADSIZE; i++){
        for(var j = 0; j < CHECKBROADSIZE; j++){
            if(i == g_corsorX && j == g_corsorY)
                print(" ╬ ")
            else if(i == 0 && j == 0)
                print("╔ ");
            else if(i== 0 && j == CHECKBROADSIZE -1)
                print(" ╗");
            else if(i == CHECKBROADSIZE - 1  && j == 0)
                print("╚ ");
            else if(i == CHECKBROADSIZE - 1 && j == CHECKBROADSIZE - 1)
                print(" ╝");
            else if(i == 0)
                print(" ┳ ");
            else if(i == CHECKBROADSIZE - 1)
                print(" ┻ ");
            else if(j == 0)
                print("┣ ");
            else if(j == CHECKBROADSIZE - 1)
                print(" ┫");
            else if(g_checkboard[i][j] == 1)
                print(" ○")
            else if(g_checkboard[i][j] == 2)
                print(" ●")
            else
                print(" ┼ ");
        }
        endl();
    }
}
var put()
{
    if(g_corsorX == 0 || g_corsorX >= CHECKBROADSIZE) return 0;
    if(g_corsorY == 0 || g_corsorY >= CHECKBROADSIZE) return 0;

    if(g_checkboard[g_corsorX][g_corsorY] == 0){
        g_checkboard[g_corsorX][g_corsorY] = g_currentgamer;
       
        return 1;
    }
    return 0;
}
var check()
{
    var nHori = 1;
    var nVert = 1;
    var nPositive = 1;
    var nSkew = 1;

    for(var i = 1; i < 5; i++){
        if(g_corsorY + i < CHECKBROADSIZE && g_checkboard[g_corsorX][g_corsorY+i] == g_currentgamer)
            nHori++;
        else
            break;
    }
    for(var i = 1; i < 5; i++){
        if(g_corsorY - i >= 0 && g_checkboard[g_corsorX][g_corsorY-i] == g_currentgamer)
            nHori++;
        else
            break;
    }
    if(nHori >= 5)
        return g_currentgamer;

    for(var i = 1; i < 5; i++){
        if(g_corsorX + i < CHECKBROADSIZE && g_checkboard[g_corsorX+i][g_corsorY] == g_currentgamer)
            nVert++;
        else
            break;
    }
    for(var i = 1; i < 5; i++){
        if(g_corsorX - i >= 0 && g_checkboard[g_corsorX-i][g_corsorY] == g_currentgamer)
            nVert++;
        else
            break;
    }
    if(nVert >= 5)
        return g_currentgamer;


    for(var i = 1; i < 5; i++){
        if(g_corsorX - i >= 0 && g_corsorY + i < CHECKBROADSIZE && g_checkboard[g_corsorX-i][g_corsorY+i] == g_currentgamer)
            nPositive++;
        else
            break;
    }
    for(var i = 1; i < 5; i++){
        if(g_corsorX + i < CHECKBROADSIZE && g_corsorY - i >= 0 && g_checkboard[g_corsorX+i][g_corsorY-i] == g_currentgamer)
            nPositive++;
        else
            break;
    }
    if(nPositive >= 5)
        return g_currentgamer;

    for(var i = 1; i < 5; i++){
        if(g_corsorX - i >= 0 && g_corsorX - i >= 0  && g_checkboard[g_corsorX-i][g_corsorY-i] == g_currentgamer)
            nSkew++;
        else
            break;
    }
    for(var i = 1; i < 5; i++){
        if(g_corsorX + i < CHECKBROADSIZE && g_corsorY + i < CHECKBROADSIZE && g_checkboard[g_corsorX+i][g_corsorY+i] == g_currentgamer)
            nSkew++;
        else
            break;
    }
    if(nSkew >= 5)
        return g_currentgamer;
    
    return 0;
}
var rungame()
{
        console.clear();
        drawcheckboard();

        var nInput = io.getch();
        if(nInput == char('a')){
            g_corsorY--;
        }
        else if(nInput == char('d')){
            g_corsorY++;
        }
        else if(nInput == char('w')){
            g_corsorX--;
        }
        else if(nInput == char('s')){
            g_corsorX++;
        }
        else if(nInput == 32){
            if(put()){
                
                var Winner = check();
                
                if(Winner == 1){
                    println("黑子赢.");
                    exit(0);
                }
                else if(Winner == 2){
                    println("白子赢.");
                    exit(0);
                }

                if(g_currentgamer == 1)
                    g_currentgamer = 2;
                else
                    g_currentgamer = 1;

            }
        }
        else if (nInput == 3){
            exit(0);
        }
        if(g_corsorY <= 0)
            g_corsorY = CHECKBROADSIZE - 2;
        if(g_corsorY >= CHECKBROADSIZE-1)
            g_corsorY = 1;
        if(g_corsorX <= 0)
            g_corsorX = CHECKBROADSIZE - 2;
        if(g_corsorX >= CHECKBROADSIZE-1)
            g_corsorX = 1;
    return 0;
}
var main()
{
    SetConsoleTitle("五子棋");
    system("mode con:cols=45 lines=16");
     init();
    console.cursor(false);
    while(true)
    {
       

        rungame();
    }
}

main();