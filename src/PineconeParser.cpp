#include "../h/PineconeParser.h"
#include "../h/LiteralElement.h"
#include "../h/IdentifierElement.h"
#include "../h/OperatorElement.h"
#include "../h/ErrorHandler.h"

PineconeParser::PineconeParser()
{
	populateCharVectors();
	populateStandardLibrary();
}

void PineconeParser::cleanUp()
{
	program.clear();
}

void PineconeParser::resolveProgram(bool printOutput)
{
	initialProgramPopulation();
	
	if (printOutput)
		cout << endl << program.printToBoxedString("unstructured element list") << endl;
	
	program.structureByOperators();
	
	if (printOutput)
		cout << endl << program.printToBoxedString("structured by operators") << endl;
	
	program.resolveIdentifiers();
	
	if (printOutput)
		cout << endl << program.printToBoxedString("identifiers resolved") << endl;
}

void PineconeParser::populateStandardLibrary()
{
	//Identifier * ptr;
	
	program.addAction
	(
		new LambdaAction
		(
			Type(Type::VOID),
			
			[](void* left, void* right)->void*
			{
				cout << *(double*)right << endl;
				return nullptr;
			},
			
			Type(Type::VOID), Type(Type::DUB),
			
			"print"
		)
	);
	
	program.addAction
	(
		new LambdaAction
		(
			Type(Type::VOID),
			
			[](void* left, void* right)->void*
			{
				cout << *(int*)right << endl;
				return nullptr;
			},
			
			Type(Type::VOID), Type(Type::INT),
			
			"print"
		)
	);
}

void PineconeParser::populateCharVectors()
{
	///whitespace
	
	whitespaceChars.push_back(' ');
	whitespaceChars.push_back('\t');
	whitespaceChars.push_back('\n');
	whitespaceChars.push_back(';');
	
	
	///letters
	
	for (char c='a'; c<='z'; ++c)
		letterChars.push_back(c);
	
	for (char c='A'; c<='Z'; ++c)
		letterChars.push_back(c);
	
	letterChars.push_back('_');
	
	
	///digits
	
	for (char c='0'; c<='9'; ++c)
		digitChars.push_back(c);
	
	
	///operators
	
	operatorChars.push_back('+');
	operatorChars.push_back('-');
	//operatorChars.push_back('*');
	//operatorChars.push_back('/');
	//operatorChars.push_back('=');
	operatorChars.push_back(':');
	operatorChars.push_back('.');
	operatorChars.push_back('(');
	operatorChars.push_back(')');
	
	
	///comments
	
	singleLineComment='#';
}

void PineconeParser::initialProgramPopulation()
{
	string elemTxt;
	int line=1;
	
	ElementData::Type type=ElementData::UNKNOWN;
	
	for (auto i=inSource.begin(); i<inSource.end(); ++i)
	{
		ElementData::Type newType=getElementType(*i, type);
		
		if (newType!=type || type==ElementData::OPERATOR)
		{
			if (!elemTxt.empty() && type!=ElementData::COMMENT)
			{
				ElementData data=ElementData(elemTxt, inFileName, line, type);
				Element * elem=makeElement(data);
				
				if (elem)
					program.appendElement(elem);
				else
				{
					error.log("could not resolve '" + elemTxt + "' into an element", data, INTERNAL_ERROR);
				}
			}
			elemTxt="";
			type=newType;
		}
		
		if (newType!=ElementData::WHITESPACE)
		{
			elemTxt+=*i;
		}
		
		if ((*i)=='\n')
			line++;
	}
}

ElementData::Type PineconeParser::getElementType(char c, ElementData::Type previousType)
{
	//if (previousType==COMMENT && c!='\n')
	
	if (previousType==ElementData::COMMENT)
	{
		if (c=='\n')
			return ElementData::WHITESPACE;
		else
			return ElementData::COMMENT;
	}
	
	if (c==singleLineComment)
	{
		return ElementData::COMMENT;
	}
	
	for (auto i=whitespaceChars.begin(); i<whitespaceChars.end(); ++i)
	{
		if (*i==c)
		{
			return ElementData::WHITESPACE;
		}
	}
	
	for (auto i=operatorChars.begin(); i<operatorChars.end(); ++i)
	{
		if (*i==c)
		{
			return ElementData::OPERATOR;
		}
	}
	
	for (auto i=letterChars.begin(); i<letterChars.end(); ++i)
	{
		if (*i==c)
		{
			if (previousType==ElementData::LITERAL)
				return ElementData::LITERAL;
			else
				return ElementData::IDENTIFIER;
		}
	}
	
	for (auto i=digitChars.begin(); i<digitChars.end(); ++i)
	{
		if (*i==c)
		{
			if (previousType==ElementData::IDENTIFIER)
				return ElementData::IDENTIFIER;
			else
				return ElementData::LITERAL;
		}
	}
	
	return ElementData::UNKNOWN;
}

Element * PineconeParser::makeElement(ElementData data)
{
	switch (data.type)
	{
		case ElementData::IDENTIFIER: return IdentifierElement::makeNew(data);
			
		case ElementData::LITERAL: return LiteralElement::makeNew(data);
			
		case ElementData::OPERATOR: return OperatorElement::makeNew(data);
			
		default: return nullptr;
	}
	
	return nullptr;
}

/*string PineconeParser::unresolvedToString()
{
	string out;
	
	//out+="|";
	
	for (auto i=unresolved.begin(); i!=unresolved.end(); ++i)
	{
		out+="\n";
		out+=(*i).toString();
		//out+="|";
	}
	
	out+="\n";
	
	return out;
}*/


