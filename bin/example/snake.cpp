var hX = 0,hY = 0, lens = 4, maps = new vector[900],c = char('d'), cl = char('d') , i = 0;
console.cursor(false);
foreach(var cc : xrange(0,maps.size())) maps[cc] = 0;
system("mode con:cols=60 lines=30");
for(maps[math.rand()%900] = -1; true; Sleep(100)){
    if(io.kbhit() && (cl = io.getch()) && cl < 97 ? cl += 32 : 1)
        if(cl == char('a') && c != char('d') || cl == char('d') && c != char('a') ||
           cl == char('w') && c != char('s') || cl == char('s') && c != char('w')) c = cl;
    if(c == char('a') &&--hX < 0 || c == char('d') && ++hX == 30 ||
       c == char('w') &&--hY < 0 || c == char('s') && ++hY == 30) break;
    if(maps[hY * 30 + hX] && (maps[hY * 30 + hX] > 0 ? break : ++lens))
        for(i = math.rand() % 900; maps[i] || !(maps[i] = -1); i = math.rand() % 900);
    else for(i = 0; i < 900; i++) maps[i] > 0 ? maps[i] -= 1 : 0;
    console.clear();maps[hY * 30 + hX] = lens;i = 0;
    for(;i < 900; i++) print(maps[i] > 0 ? "()" : maps[i] ? "00" : "  ");
}