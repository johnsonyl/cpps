module itertools
{
	var islice(var iter,var split)
	{
		var i = 0;
		var maxsplit = len(iter);
		if(isint(split)) maxsplit = max(0,min(split,len(iter));
		var ret;
		if(ismap(iter))
		{
			ret = {};
			foreach(var it : iter){
				i++;
				ret.insert(it.first(),it.second());
				if(i >= maxsplit) break;
			}
		}
		else if(isvector(iter))
		{
			ret = [];
			foreach(var ii: xrange(i,maxsplit))
			{
				ret.push_back(iter[ii]);
			}
		}
		else if(isstring(iter))
		{
			return iter.substr(i,maxsplit);
		}
		return ret;
	}
}