class CBlogmanager
{
    var bloglist = [];
    var bloglist_id = {};

    var loadbloglist()
    {
        var bloglist_json = io.readfile("data/bloglist.json");
        var bloglist_json_object = json.decode(bloglist_json);
        foreach(var blogjson : bloglist_json_object){
            var modelblog = serializer.decode(models::blog,blogjson);
            bloglist.push_back(modelblog);
            bloglist_id.insert(modelblog.id,modelblog);
        }
    }
    var getmaxpage()
    {
        var maxpage = (bloglist.size()-1) / globalconfig["page-count"] + 1;
        return maxpage;
    }

    var getbloglist(var page)
    {
        var maxpage = getmaxpage();
        if(page > maxpage) return 0,0,[];

        var ret = [];
        
        var start = (page-1) * globalconfig["page-count"];
        var end = start + globalconfig["page-count"];
        if(end >= bloglist.size()) end = bloglist.size();
        for(; start < end; start++){
            ret.push_back(bloglist[start]);   
        }

        var prepageid = page-1;
        var nextpageid = page+1;
        if(prepageid <= 0) prepageid = 0;
        if(nextpageid > maxpage) nextpageid = 0;
        return prepageid,nextpageid,ret;
    }
    var getblog(var id)
    {
        return bloglist_id[id];
    }
}

var blogmanager = NULL;
var initblogmanager()
{
    blogmanager = new CBlogmanager();
    blogmanager.loadbloglist();
}