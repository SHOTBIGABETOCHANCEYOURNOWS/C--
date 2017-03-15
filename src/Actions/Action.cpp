#include "../../h/Action.h"

#include "../../h/ErrorHandler.h"

#include "../../h/msclStringFuncs.h"

ActionData::ActionData(Type returnTypeIn, Type inLeftTypeIn, Type inRightTypeIn)
{
	returnType=returnTypeIn;
	inLeftType=inLeftTypeIn;
	inRightType=inRightTypeIn;
	
	if (!returnType || !inLeftType || !inRightType)
	{
		throw PineconeError("ActionData created with null type", INTERNAL_ERROR);
	}
}

string ActionData::toString()
{
	return description;
	
	//return returnType->getName() + " <- " + inLeftType->getName() + " " + text + " " + inRightType->getName();
}

string ActionData::getTypesString()
{
	return returnType->getString()+" <- "+inLeftType->getString()+"."+inRightType->getString();
}

//LambdaAction::LambdaAction(Type returnTypeIn, function<void*(void*,void*)> lambdaIn, Type inLeftTypeIn, Type inRightTypeIn, string textIn)
	
//void* LambdaAction::execute(void* inLeft, void* inRight)

class VoidAction: public ActionData
{
public:
	VoidAction(): ActionData(Void, Void, Void)
	{
		setDescription("Void Action");
	}
	
	void* execute(void* inLeft, void* inRight)
	{
		return nullptr;
	}
	
	void addCppCodeToProg(Action inLeft, Action inRight, CppProgram* prog)
	{
		//prog
	}
};

class LambdaAction: public ActionData
{
public:
	LambdaAction(Type returnTypeIn, function<void*(void*,void*)> lambdaIn, Type inLeftTypeIn, Type inRightTypeIn, string cppCodeIn, string textIn): ActionData(returnTypeIn, inLeftTypeIn, inRightTypeIn)
	{
		lambda=lambdaIn;
		if (cppCode.empty())
		{
			cppCode="/* lambda action '" + textIn + "' has not yet been implemented for C++ */";
		}
		else
		{
			cppCode=cppCodeIn;
		}
		setDescription(textIn);
	}
	
	void* execute(void* inLeft, void* inRight)
	{
		return lambda(inLeft, inRight);
	}
	
	void addCppCodeToProg(Action inLeft, Action inRight, CppProgram* prog)
	{
		int start=0;
		int i;
		
		do
		{
			i=searchInString(cppCode, "$", start);
			
			prog->addCode(cppCode.substr(start, (i<0?cppCode.size():i)-start));
			
			if (i>=0)
			{
				if (substringMatches(cppCode, i, "$."))
				{
					inLeft->addCppCodeToProg(prog);
					start=i+string("$.").size();
				}
				else if (substringMatches(cppCode, i, "$:"))
				{
					inRight->addCppCodeToProg(prog);
					start=i+string("$:").size();
				}
				else if (substringMatches(cppCode, i, "$$"))
				{
					prog->addCode("$");
					start=i+string("$$").size();
				}
				else
				{
					throw PineconeError("invalid '$' escape in C++ code: "+cppCode, INTERNAL_ERROR);
				}
			}
			
		} while (i>=0);
		
		//prog->addComment("lambda action (not yet implemented)");
	}
	
private:
	function<void*(void*,void*)> lambda;
	string cppCode;
};

Action lambdaAction(Type inLeftTypeIn, Type inRightTypeIn, Type returnTypeIn, function<void*(void*,void*)> lambdaIn, string cppCodeIn, string textIn)
{
	return Action(new LambdaAction(returnTypeIn, lambdaIn, inLeftTypeIn, inRightTypeIn, cppCodeIn, textIn));
}

Action createNewVoidAction()
{
	return Action(new VoidAction());
}
