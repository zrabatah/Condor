#include "scope.h"

namespace Cobra{
	Scope::Scope(){

	}

	Scope::~Scope(){
		delete outer;
	}

	Scope* Scope::NewScope(){
		Scope* scope = new Scope;
		scope->outer = this;
		return scope;
	}

	ASTNode* Scope::Lookup(std::string name){
		auto obj = objects.find(name);
		if (obj == objects.end()){
			return NULL;
		}
		else{
			return obj->second;
		}
	}

	void Scope::Insert(ASTNode* node){
		if (node != NULL){
			objects[node->name] = node;
		}
	}

	ASTNode* Scope::NewObject(std::string name){
		ASTNode* obj = new ASTNode;
		obj->name = name;
		Insert(obj);
		return obj;
	}

	void Scope::String(){
		for (std::map<std::string, ASTNode*>::iterator it = objects.begin(); it != objects.end(); ++it){
			printf("Found: %s\n", it->first.c_str());
		}
	}
} // namespace Cobra	