#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <re2/stringpiece.h>
#include <re2/filtered_re2.h>
#include <re2/re2.h>
#include <re2/set.h>

#include <cpps/cpps.h>

using namespace cpps;
using namespace std;
using namespace re2;

static inline re2::StringPiece FromBytes(object& val) {
	if (val.isstring()) {
		std::string* str = cpps_get_string(val.getval());
		return re2::StringPiece(str->data(), str->size());
	}
	return re2::StringPiece("");
}


class Set {
public:
// 	Set(RE2::Anchor anchor, const RE2::Options& options)
// 		: set_(options, anchor) {}
	Set(){
		set_ = NULL;
	}
	~Set()
	{
		if (set_) {
			CPPSDELETE(set_);
			set_ = NULL;
		}
	}

	void constructor(object anchor, RE2::Options* options) {
		set_ = CPPSNEW( RE2::Set)(*options,(RE2::Anchor) anchor.toint());
	}

	// Not copyable or movable.
	Set(const Set&) = delete;
	Set& operator=(const Set&) = delete;

	int Add(object buffer) {
		auto pattern = FromBytes(buffer);
		int index = set_->Add(pattern, /*error=*/NULL);  // -1 on error
		return index;
	}

	bool Compile() {
		// Compiling can fail.
		return set_->Compile();
	}

	object Match(C* c, object buffer) const {
		object::vector ret = object::vector::create(c);
		auto text = FromBytes(buffer);
		std::vector<int> matches;
		set_->Match(text, &matches);
		for (auto& it : matches) {
			ret.push_back(object::create(c, it));
		}
		return ret.toobject();
	}

private:
	RE2::Set *set_;
};

class Filter {
public:
	Filter() = default;
	~Filter() = default;

	// Not copyable or movable.
	Filter(const Filter&) = delete;
	Filter& operator=(const Filter&) = delete;

	int Add(object buffer, RE2::Options* options) {
		auto pattern = FromBytes(buffer);
		int index = -1;  // not clobbered on error
		filter_.Add(pattern, *options, &index);
		return index;
	}

	bool Compile() {
		std::vector<std::string> atoms;
		filter_.Compile(&atoms);
		RE2::Options options;
		options.set_literal(true);
		options.set_case_sensitive(false);
		set_ = std::unique_ptr<RE2::Set>(new RE2::Set(options, RE2::UNANCHORED));
		for (int i = 0; i < static_cast<int>(atoms.size()); ++i) {
			if (set_->Add(atoms[i], /*error=*/NULL) != i) {
				// Should never happen: the atom is a literal!
				cpps::fail("set_->Add() failed");
			}
		}
		// Compiling can fail.
		return set_->Compile();
	}

	object Match(C*c,object buffer, bool potential) const {
		object::vector ret = object::vector::create(c);
		auto text = FromBytes(buffer);
		std::vector<int> atoms;
		set_->Match(text, &atoms);
		std::vector<int> matches;
		if (potential) {
			filter_.AllPotentials(atoms, &matches);
		}
		else {
			filter_.AllMatches(text, atoms, &matches);
		}
		for (auto& it : matches) {
			ret.push_back(object::create(c, it));
		}
		return ret.toobject();
	}

private:
	re2::FilteredRE2 filter_;
	std::unique_ptr<RE2::Set> set_;
};
static inline int OneCharLen(const char* ptr) {
	return "\1\1\1\1\1\1\1\1\1\1\1\1\2\2\3\4"[(*ptr & 0xFF) >> 4];
}
ssize_t CharLenToBytes(object buffer, ssize_t pos, ssize_t len) {
	auto text = FromBytes(buffer);
	auto ptr = text.data() + pos;
	auto end = text.data() + text.size();
	while (ptr < end && len > 0) {
		ptr += OneCharLen(ptr);
		--len;
	}
	return ptr - (text.data() + pos);
}

// Helper function for when Python decodes bytes to Text and then needs to
// convert bytes offsets to Text offsets. Assumes that text is valid UTF-8.
ssize_t BytesToCharLen(object buffer, ssize_t pos, ssize_t endpos) {
	auto text = FromBytes(buffer);
	auto ptr = text.data() + pos;
	auto end = text.data() + endpos;
	ssize_t len = 0;
	while (ptr < end) {
		ptr += OneCharLen(ptr);
		++len;
	}
	return len;
}

class _RE2 
{
public:
	_RE2() 
	{
		_re2 = NULL;
	}
	~_RE2() {
		if (_re2) {
			CPPSDELETE(_re2);
		}
	}
	void constructor(object buffer, RE2::Options* options) {
		auto pattern = FromBytes(buffer);
		_re2 = CPPSNEW(RE2)(pattern, *options);
	}

	bool ok() {
		return _re2->ok();
	}
	std::string error() {
		return _re2->error();
	}

	RE2::Options* RE2_options() {
		return (RE2::Options*) & _re2->options();
	}
	//std::vector<std::pair<py::bytes, int>> RE2NamedCapturingGroupsShim(
	object RE2NamedCapturingGroupsShim( C* c) {
		object::map groups = object::map::create(c);
		for (const auto& it : _re2->NamedCapturingGroups()) {
			groups.insert(cpps::object::create(c, it.first), cpps::object::create(c, it.second));
		}
		return groups.toobject();
	}

	object RE2ProgramFanoutShim( C* c) {
		object::vector ret = object::vector::create(c);
		std::vector<int> histogram;
		_re2->ProgramFanout(&histogram);
		for (auto& it : histogram) {
			ret.push_back(object::create(c, it));
		}
		return ret.toobject();
	}
	object RE2ReverseProgramFanoutShim( C* c) {
		object::vector ret = object::vector::create(c);
		std::vector<int> histogram;
		_re2->ReverseProgramFanout(&histogram);
		for (auto& it : histogram) {
			ret.push_back(object::create(c, it));
		}
		return ret.toobject();
	}
	object RE2MatchShim( C* c,
		object anchor,
		object buffer,
		ssize_t pos,
		ssize_t endpos) {
		auto text = FromBytes(buffer);
		const int num_groups = _re2->NumberOfCapturingGroups() + 1;  // need $0
		std::vector<re2::StringPiece> groups;
		groups.resize(num_groups);
		if (pos == endpos)
		{
			for (auto& it : groups) {
				it = re2::StringPiece();
			}
		}
		else {
			if (!_re2->Match(text, pos, endpos, (RE2::Anchor) anchor.toint(), groups.data(), (int)groups.size())) {
				// Ensure that groups are null before converting to spans!
				for (auto& it : groups) {
					it = re2::StringPiece();
				}
			}
		}

		object::vector spans = object::vector::create(c);
		for (const auto& it : groups) {
			if (it.data() == NULL) {
				object::pair _pair = object::pair::create(c, object::create(c, -1), object::create(c, -1));
				spans.push_back(_pair.toobject());
			}
			else {
				if (it.size() == 0) {
					object::pair _pair = object::pair::create(c, object::create(c, text.size()),
						object::create(c, text.size()));
					spans.push_back(_pair.toobject());
				}
				else {
					object::pair _pair = object::pair::create(c, object::create(c, it.data() - text.data()),
						object::create(c, it.data() - text.data() + it.size()));
					spans.push_back(_pair.toobject());
				}
			}
		}
		return spans.toobject();
	}
	cpps_integer NumberOfCapturingGroups() {
		return (cpps_integer)_re2->NumberOfCapturingGroups();
	}
	cpps_integer ProgramSize() {
		return (cpps_integer)_re2->ProgramSize();
	}
	cpps_integer ReverseProgramSize() {
		return (cpps_integer)_re2->ReverseProgramSize();
	}
	RE2* _re2;
};

cpps_integer RE2_OPTIONS_ENCODING(RE2::Options* self) {
	return (cpps_integer)self->encoding();
}
void RE2_OPTIONS_SET_ENCODING(RE2::Options* self, object v) {
	return self->set_encoding((RE2::Options::Encoding)v.toint());
}
std::string RE2QuoteMeta(object buffer)
{
	auto text = FromBytes(buffer);
	return RE2::QuoteMeta(text);
}

cpps_export_void cpps_attach(cpps::C* c)
{
	
	cpps::cpps_init_cpps_class(c);

	cpps::_module(c, "re")[
		def("CharLenToBytes", CharLenToBytes),
		def("BytesToCharLen", BytesToCharLen),
		def("QuoteMeta", RE2QuoteMeta),
		_enum(c,"Anchor")
			.value("UNANCHORED", RE2::Anchor::UNANCHORED)
			.value("ANCHOR_START", RE2::Anchor::ANCHOR_START)
			.value("ANCHOR_BOTH", RE2::Anchor::ANCHOR_BOTH),
		_enum(c,"Encoding")
			.value("UTF8", RE2::Options::Encoding::EncodingUTF8)
			.value("LATIN1", RE2::Options::Encoding::EncodingLatin1),
		_class<_RE2>("RE2")
			.def("constructor", &_RE2::constructor)
			.def("ok", &_RE2::ok)
			.def("error", &_RE2::error)
			.def("options", &_RE2::RE2_options)
			.def("NumberOfCapturingGroups", &_RE2::NumberOfCapturingGroups)
			.def_inside("NamedCapturingGroups", &_RE2::RE2NamedCapturingGroupsShim)
			.def("ProgramSize", &_RE2::ProgramSize)
			.def("ReverseProgramSize", &_RE2::ReverseProgramSize)
			.def_inside("ProgramFanout", &_RE2::RE2ProgramFanoutShim)
			.def_inside("ReverseProgramFanout", &_RE2::RE2ReverseProgramFanoutShim)
			.def_inside("Match", &_RE2::RE2MatchShim),
		_class<RE2::Options>("Options")
			.def("encoding", RE2_OPTIONS_ENCODING)
			.def("set_encoding", RE2_OPTIONS_SET_ENCODING)
			.def("posix_syntax",&RE2::Options::posix_syntax)
			.def("set_posix_syntax",&RE2::Options::set_posix_syntax)
			.def("longest_match",&RE2::Options::longest_match)
			.def("set_longest_match",&RE2::Options::set_longest_match)
			.def("log_errors",&RE2::Options::log_errors)
			.def("set_log_errors",&RE2::Options::set_log_errors)
			.def("max_mem",&RE2::Options::max_mem)
			.def("set_max_mem",&RE2::Options::set_max_mem)
			.def("literal",&RE2::Options::literal)
			.def("set_literal",&RE2::Options::set_literal)
			.def("never_nl",&RE2::Options::never_nl)
			.def("set_never_nl",&RE2::Options::set_never_nl)
			.def("dot_nl",&RE2::Options::dot_nl)
			.def("set_dot_nl",&RE2::Options::set_dot_nl)
			.def("never_capture",&RE2::Options::never_capture)
			.def("set_never_capture",&RE2::Options::set_never_capture)
			.def("case_sensitive",&RE2::Options::case_sensitive)
			.def("set_case_sensitive",&RE2::Options::set_case_sensitive)
			.def("perl_classes",&RE2::Options::perl_classes)
			.def("set_perl_classes",&RE2::Options::set_perl_classes)
			.def("word_boundary",&RE2::Options::word_boundary)
			.def("set_word_boundary",&RE2::Options::set_word_boundary)
			.def("one_line",&RE2::Options::one_line)
			.def("set_one_line",&RE2::Options::set_one_line),
		_class<Set>("RE2Set")
			.def("constructor", &Set::constructor)
			.def("Add", &Set::Add)
			.def("Compile", &Set::Compile)
			.def_inside("Match", &Set::Match),
		_class<Filter>("RE2Filter")
			.def("Add", &Filter::Add)
			.def("Compile", &Filter::Compile)
			.def_inside("Match", &Filter::Match)
	];



   
}
cpps_export_void  cpps_detach(cpps::C * c)
{
	cpps::_unmodule(c, "re");
}

cpps_export_finish