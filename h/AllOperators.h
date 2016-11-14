#pragma once

#include <vector>
using std::vector;

#include <unordered_map>
using std::unordered_map;

#include "Operator.h"

class AllOperators
{
public:
	
	// sets ops to a new instance
	static void init();
	
	// sets out to the operators (0, 1 or more) for the given text
	void get(string text, vector<Operator>& out);
	
	#define DECLARE_OP(name, text, left, right, overload)\
		const Operator name{new OperatorData(text, left, right, overload)};
	
	#define ALL_OPS \
	DECLARE_OP(	pipe,		"|",	5,		5,		false	);	\
	DECLARE_OP(	loop,		"@",	10,		11,		false	);	\
	DECLARE_OP(	ifOp,		"?",	10,		11,		false	);	\
	DECLARE_OP(	plus,		"+",	20,		21,		true	);	\
	DECLARE_OP(	minus,		"-",	20,		21,		true	);	\
	DECLARE_OP(	multiply,	"*",	30,		31,		true	);	\
	DECLARE_OP(	divide,		"/",	30,		31,		true	);	\
	DECLARE_OP(	equal,		"=",	40,		41,		true	);	\
	DECLARE_OP(	greater,	">",	40,		41,		true	);	\
	DECLARE_OP(	less,		"<",	40,		41,		true	);	\
	DECLARE_OP(	comma,		",",	60,		61,		false	);	\
	DECLARE_OP(	dot,		".",	70,		71,		false	);	\
	DECLARE_OP(	colon,		":",	95,		15,		false	);	\
	DECLARE_OP(	openPeren,	"(",	0,		101,	false	);	\
	DECLARE_OP(	closePeren,	")",	100,	0,		false	);	\
	
	ALL_OPS;
	
	//for read only, dont screw with this
	unordered_map<string, Operator>& getOpsMap() {return opsMap;}

private:
	
	// is only called in init
	AllOperators();
	
	// used internally by the constructor
	void putOpInMap(Operator op);
	
	// a hash map of all the operators
	unordered_map<string, Operator> opsMap;
};

// the single instance of this class, starts out as nullptr but chages to a real instance in AllOperators::init
extern shared_ptr<AllOperators> ops;
