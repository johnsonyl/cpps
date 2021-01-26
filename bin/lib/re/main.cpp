#import "encoding"
#import "itertools"



module re
{

    var compile(var pattern,var options) {
        if ( options == NULL )
            options = new re::Options();
        return new re::_Regexp(pattern, options);
    }
    
   var search(var pattern, var text, var options)
   {
        return compile(pattern, options ).search(text);
   }
   var match(var pattern, var text, var options)
   {
		return compile(pattern, options).match(text);
   }
   var fullmatch(var pattern, var text, var options)
   {
		return compile(pattern, options).fullmatch(text);
   }
   var finditer(var pattern, var text, var options)
   {
		return compile(pattern, options).finditer(text);
   }
   var findall(var pattern, var text, var options)
   {
		return compile(pattern, options).findall(text);
   }
   var split(var pattern, var text,var maxsplit = 0, var options)
   {
		return compile(pattern, options).split(text,maxsplit);
   }
   var subn(var pattern,var repl, var text,var count = 0, var options)
   {
		return compile(pattern, options).subn(repl,text,count);
   }
   var sub(var pattern,var repl, var text,var count = 0, var options)
   {
		return compile(pattern, options).sub(repl,text,count);
   }
   var escape(var pattern)
   {
       return re.QuoteMeta(pattern);
   }

   class _Regexp
   {
       var _pattern;
       var _regexp;
       _Regexp(var pattern, var options)
       {
           if ( options == NULL )
                options = new re::Options();

           _pattern = pattern;
           _regexp = new re::RE2(_pattern, options);

           //
           if (!_regexp.ok()) {
               throw _regexp.error();
           }

       }
       var _match(var anchor, var text, var pos, var endpos)
       {
           
           if (isint(pos)) 
               pos = max(0, min(pos, len(text)));
           else
               pos = 0;
           
           if (isint(endpos)) 
               endpos = max(0, min(endpos,len(text)));
            else
                endpos = len(text);
                
           if (pos > endpos) return;
           var ret = [];
           while (true) {

			   var spans = _regexp.Match(anchor, text, pos, endpos);
               if (spans[0] == _NULL_SPAN)  break;
            
               
               ret.push_back(new _Match(this, text, pos, endpos, spans));
               if (pos == endpos) break;
               else if (pos == spans[0][1])
                   pos += 1;
               else
                   pos = spans[0][1];
           }
           return ret;
       }
       var search(var text, var pos, var endpos) {
           var matchd = _match(re.Anchor::UNANCHORED, text, pos, endpos);
           return matchd.size() > 0 ? matchd[0] : NULL;

       }
	   var match(var text,var pos, var endpos) {
		   var matchd = _match(re.Anchor::ANCHOR_START, text, pos, endpos);
           return matchd.size() > 0 ? matchd[0] : NULL;
	   }
	   var fullmatch(var text,var pos, var endpos) {
		   var matchd = _match(re.Anchor::ANCHOR_BOTH, text, pos, endpos);
           return matchd.size() > 0 ? matchd[0] : NULL;
	   }
	   var finditer(var text,var pos, var endpos) {
           return _match(re.Anchor::UNANCHORED, text, pos, endpos);
	   }
       var findall(var text, var pos, var endpos) {
           var empty = ""; 

           var items = [];
           foreach(var match : finditer(text, pos, endpos)) {
               var item;
               if (groups() == 0)
                   item = match.group();
               else if (groups() == 1)
                   item = match.groups(empty)[0];
               else
                   item = match.groups(empty);

               items.append(item);
           }
           return items;
       }
       var _split(var cb, var text, var maxsplit = 0)
       {
           var matchiter;
            if (maxsplit < 0) {
                var ret = {};
                ret[text] = 0;
                return ret;
            }
            else if (maxsplit > 0)
                matchiter = itertools.islice(finditer(text), maxsplit)
            else
                matchiter = finditer(text);
           
            var pieces = [];
            var end = 0;
            var numsplit = 0;
            foreach(var match : matchiter) {
                pieces.push_back(text[end:match.start()]);
                pieces.extend(cb(match));
                end = match.end();
                numsplit += 1;
            }
            if(end != len(text)) pieces.push_back(text[end:]);
            return pieces, numsplit;
       }
       var split(var text, var maxsplit = 0) {
           var self = this;
           var cb = [](var match) {
               var ret = [];
               foreach(var group : xrange(1, self.groups() + 1))
                   ret.push_back(match[group]);
               return ret;
           };
           var [pieces, _] = _split(cb, text, maxsplit);
           return pieces;
       }
       var subn(var repl, var text, var count = 0) {
           var cb = [](var match) {
               var ret = [];
               
               if (isfunction(repl))
                   ret.push_back(repl(match));
               else
                   match.expand(repl);
                return ret;
           };
		   var empty = "";
		   var [pieces, numsplit] = _split(cb, text, count);
           var joined_pieces = empty.join(pieces);
           return joined_pieces,numsplit;
       }
       var sub(var repl, var text, var count = 0)
       {
           var [joined_pieces, _] = subn(repl, text, count);
           return joined_pieces;
       }
       var pattern() {
           return _pattern;
       }
       var options()
       {
           return _regexp.options();
       }
       var groups()
       {
           return _regexp.NumberOfCapturingGroups();
       }
       var groupindex()
       {
           return _regexp.NamedCapturingGroups();
       }
       var programsize() {
           return _regexp.ProgramSize();
	   }
	   var reverseprogramsize() {
		   return _regexp.ReverseProgramSize();
	   }
	   var programfanout() {
		   return _regexp.ProgramFanout();
	   }
	   var reverseprogramfanout() {
		   return _regexp.ReverseProgramFanout();
	   }

   };



   class _Match
   {
       var _regexp;
       var _text;
       var _pos;
       var _endpos;
       var _spans;


       _Match(var regexp, var text, var pos, var endpos, var spans) {
           _regexp = regexp;
           _text = text;
           _pos = pos;
           _endpos = endpos;
           _spans = spans;
       }

       var _split(var template)
       {
           var empty = "";
           var backslash = string.chr(0x5C);
           var pieces = [empty];
           var index = template.find(backslash);

           while (index != string.npos) {
               var piece = template[:index];
               template = template[index:];
               pieces[-1] ..= piece;

               var octal_match = _OCTAL_RE.match(template);
               var group_match = _GROUP_RE.match(template);

               if (!octal_match && group_match) {
                    index = group_match.end();
                    piece = template[:index];
                    template = template[index:];
                    pieces.extend([piece, empty]);
               }
               else {
                    index = 2;
                    piece = template[:index];
                    template = template[index:];
                    pieces[-1] ..= piece;
               }
               index = template.find(backslash);
           }
           pieces[-1] ..= template;
           return pieces;
       }
       var expand(var template) {
           var empty = "";
           var pieces = _split(template);
           var index = 0;
           foreach(var piece : pieces) {
               if (index % 2 == 0)
                   pieces[index] = encoding.unescape(piece);
               else {
                   var group = 0;
                   if (len(piece) <= 3)
                       group = int(piece[1:]);
                   else {
                       group = pieces[3:-1];
                       group = int(group);
                   }
                   pieces[index] = this[group] ? this[group] : empty;
               }
               index++;
           }
           var joined_pieces = empty.join(pieces);
           return joined_pieces;
       }
       var operator [](var group) {
		   if (!isint(group)) {
               group = _regexp.groupindex()[group];
               if (!group)
                   throw "bad group name";
		   }
           if (group < 0 || group > _regexp.groups())
               throw "bad group index";

           var span = _spans[group];
           if (span == _NULL_SPAN) 
               return NULL;

           return _text[span[0]:span[1]];
       }

       var group(var groups) {
           if (groups == NULL) {
               groups = [0];
           }
           if(!isvector(groups)){
                return this[groups];
           }


           var self = this;
           
           var items = groups.select([](var v) { return self[v]; });
           return len(groups) == 1 ? items[0] : items;
       }
       var groups(var default)
       {
           var items = [];
           foreach(var group : xrange(1, _regexp.groups() + 1)) {
               var item = this[group];
               items.push_back(item == NULL ? default: item);
           }
           return items;
       }
       var groupdict(var default)
       {
           var item = {};
           foreach(var it : _regexp.groupindex()) {
               var item = this[it.second()];
               items.push_back(item == NULL ? default:item);
           }
       }
       var start(var group = 0)
       {
           if (group < 0 || group > _regexp.groups()) 
               return;
           return _spans[group][0];
       }
       var end(var group = 0) {

		   if (group < 0 || group > _regexp.groups())
			   return;

		   return _spans[group][1];
       }
       var span(var group = 0) {
           
		   if (group < 0 || group > _regexp.groups())
			   return;
		   return _spans[group];
       }
       var re()
       {
           return _regexp;
       }
       var text()
       {
           return _text;
       }
       var pos()
       {
           return _pos;
       }
       var endpos()
       {
           return _endpos;
       }
       var lastindex()
       {
           var max_end = -1;
           var max_group = NULL;
           foreach(var group : xrange(1, _regexp.groups()+1)) {
               var end = _spans[group][1];
               if (max_end < end) {
                   max_end = end;
                   max_group = group;
               }
           }
           return max_group;
       }
       var lastgroup()
       {
           var max_group = lastindex();
           if (max_group == NULL) return NULL;;
           foreach(var it : _regexp.groupindex()) {
               if (max_group == it.second())
                   return it.first();
           }
           return NULL;
       }
           
   };
   var SearchSet(var options) {
       return new re.Set(re.Anchor::UNANCHORED, options);
   }
   var MatchSet(var options) {
       return new re.Set(re.Anchor::ANCHOR_START, options);
   }
   var FullMatchSet(var options) {
	   return new re.Set(re.Anchor::ANCHOR_BOTH, options);
   }
   class Set
   {
       var _set;
       
       Set(var anchor,var options)
       {
           if (options == NULL)
               options = new re::Options();
           _set = new re::RE2Set(anchor, options);
       }

       var Add(var pattern) {
           var index = _set.Add(pattern);

           if (index == -1)
               throw 'failed to add {pattern} to Set';
           return index;
       }
       var Compile()
       {
           if (!_set.Compile())
               throw 'failed to compile Set';
       }
       var Match(var text)
       {
           return _set.Match(text);
       }
   };

   class Filter
   {
       var _filter;

       Filter()
       {
           _filter = new re::RE2Filter();
       }
       var Add(var pattern, var options) {
           if (options == NULL)
               options = new re::Options();
           var index = _filter.Add(pattern, options);
           if (index == -1)
               throw 'failed to add {pattern} to Filter';

           return index;
       }
       var Compile() {
		   if (!_filter.Compile())
			   throw 'failed to compile Filter';
       }
       var Match(var text, var potential = false)
       {
           return self._filter.Match(text, potential);
       }

   };

}
var _OCTAL_RE = new re::_Regexp('\\\\[0-7][0-7][0-7]');
var _GROUP_RE = new re::_Regexp('\\\\[1-9][0-9]?|\\\\g<\\w+>');
var _NULL_SPAN = new pair(-1, -1);