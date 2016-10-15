#pragma once

#include "Element.h"

using std::to_string;

class LiteralElement: public Element
{
public:
	LiteralElement(ElementData dataIn): Element(dataIn) {}
	
	static LiteralElement * makeNew(ElementData dataIn);
	
	ElementData::Type getElemType() {return ElementData::LITERAL;}
};

class IntLiteral: public LiteralElement
{
public:
	string getReadableName() {return to_string(val) + " (Int literal)";}
	
	IntLiteral(ElementData dataIn, int in): LiteralElement(dataIn) {val=in;}
	
	Type getReturnType() {return Type::INT;}
	
	void* execute() {return new int(val);}
	
private:
	int val=0;
};

class DubLiteral: public LiteralElement
{
public:
	string getReadableName() {return to_string(val) + " (Dub literal)";}
	
	DubLiteral(ElementData dataIn, double in): LiteralElement(dataIn) {val=in;}
	
	Type getReturnType() {return Type::DUB;}
	
	void* execute() {return new double(val);}
	
private:
	double val=0;
};

class BoolLiteral: public LiteralElement
{
public:
	string getReadableName() {return to_string(val) + " (Bool literal)";}
	
	BoolLiteral(ElementData dataIn, bool in): LiteralElement(dataIn) {val=in;}
	
	Type getReturnType() {return Type::BOOL;}
	
	void* execute() {return new bool(val);}
	
private:
	bool val=0;
};


