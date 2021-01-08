class CBlogmanager
{
    var bloglist = [];
    var bloglist_id = {};

    var loadbloglist()
    {
        bloglist = json.decode_file("data/bloglist.json",models::blog);
        foreach(var blog : bloglist){
            bloglist_id.insert(blog.id,blog);
        }
    }
    var getmaxpage()
    {
        var maxpage = (bloglist.size()-1) / globalconfig.page_count + 1;
        return maxpage;
    }

    var getbloglist(var page)
    {
        var maxpage = getmaxpage();
        if(page > maxpage) return 0,0,[];

        var ret = [];
        
        var start = (page-1) * globalconfig.page_count;
        var end = start + globalconfig.page_count;
        var size = bloglist.size()-1;
        if(end >= bloglist.size()) end = bloglist.size();
        // for(; start < end; start++){
        //     ret.push_back(bloglist[size-start]);   //反序
        // }
        foreach(var i : xrange(start , end-1))
             ret.push_back(bloglist[size-i]);   //反序
        
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
    var save()
    {
        json.encode_file("data/bloglist.json",bloglist);
    }
    var remove(var id)
    {
        bloglist.remove([](var item){ return item.id == id;});
        bloglist_id.erase(id);
    }
}

var blogmanager = NULL;
var initblogmanager()
{
    blogmanager = new CBlogmanager();
    blogmanager.loadbloglist();
}