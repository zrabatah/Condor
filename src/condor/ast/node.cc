// Copyright 2016 Chase Willden and The CondorLang Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "node.h"

namespace Condor {
namespace internal{

	ASTNode* ASTNode::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTNode));
		ASTNode* n = new(pt) ASTNode();
		ASTNode::SetDefaults(n, iso);
		return n;
	}

	void ASTNode::SetDefaults(ASTNode* node, Isolate* iso){
		node->row = 0;
		node->col = 0;
		node->type = UNDEFINED;
		node->name = "";
		node->isExport = false;
		node->isInParen = false;
		node->allowGC = true;
		node->print = false;
		node->importScopeId = -1;
		node->progressDepth = 0;
		node->id = iso->GetContext()->GetNextAstId();
	}

	std::string ASTNode::ToString(int tabs) {
		std::string tabStr;
		for (int i = 0; i < tabs; i++) tabStr += " ";
		std::string results;

		results += tabStr + "Node: " + Token::ToString(type) + " {\n";
		tabStr += " ";
		results += tabStr + "row: " + std::to_string(row) + "\n";
		results += tabStr + "col: " + std::to_string(col) + "\n";
		results += tabStr + "name: " + name + "\n";
		results += tabStr + "isExport: " + std::to_string(isExport) + "\n";
		results += tabStr + "isInParen: " + std::to_string(isInParen) + "\n";
		results += tabStr + "allowGc: " + std::to_string(allowGC) + "\n";
		results += tabStr + "print: " + std::to_string(print) + "\n";
		results += tabStr + "importScopeId: " + std::to_string(importScopeId) + "\n";
		results += tabStr + "progressDepth: " + std::to_string(progressDepth) + "\n";
		results += tabStr + "id: " + std::to_string(id) + "\n";

		return results;
	}

	std::string ASTFunc::ToString(int tabs) {
		std::string results = ASTNode::ToString(tabs);

		std::string tabStr;
		for (int i = 0; i < tabs + 1; i++) tabStr += " ";
		results += tabStr + "assignmentType: " + Token::ToString(assignmentType) + "\n";
		results += tabStr + "totalArgs: " + std::to_string(args.size()) + "\n";
		results += tabStr + "args: \n";
		for (unsigned long long int i = 0; i < args.size(); i++){
			results += args[i]->ToString(tabs + 3);
		}

		results += tabStr + "scope: \n";
		results += scope->ToString(tabs + 3);

		results += tabStr + "}\n";
		return results;
	}

    std::string ASTObjectInstance::ToString(int tabs) {
		std::string results = ASTNode::ToString(tabs);

		std::string tabStr;
		for (int i = 0; i < tabs + 1; i++) tabStr += " ";
		results += tabStr + "base: \n";
		if (base != NULL) results += base->ToString(tabs + 3);
		else results += tabStr + "base: NULL\n";
		if (constructor != NULL) results += constructor->ToString(tabs + 3);
		else results += tabStr + "constructor: NULL\n";

		for (std::map<std::string, ASTVar*>::iterator it = properties.begin(); it != properties.end(); it++){
			results += tabStr + "key: " + it->first + ", value: \n";
			results += it->second->ToString(tabs + 3);
		}
		results += tabStr + "}\n";
		return results;
	}

	std::string ASTObject::ToString(int tabs) {
		std::string results = ASTNode::ToString(tabs);

		std::string tabStr;
		for (int i = 0; i < tabs + 1; i++) tabStr += " ";
		results += tabStr + "extend: " + std::to_string(extend) + "\n";
		results += tabStr + "Scope: \n";
		results += scope->ToString(tabs + 3);
		results += tabStr + "}\n";
		return results;
	}

	std::string ASTVar::ToString(int tabs) {
		std::string results = ASTNode::ToString(tabs);

		std::string tabStr;
		for (int i = 0; i < tabs + 1; i++) tabStr += " ";
		results += tabStr + "baseName: " + baseName + "\n";
		results += tabStr + "baseType: " + Token::ToString(baseType) + "\n";
		results += tabStr + "assignmentType: " + Token::ToString(assignmentType) + "\n";
		results += tabStr + "op: " + Token::ToString(op) + "\n";
		results += tabStr + "scopeId: " + std::to_string(scopeId) + "\n";
		results += tabStr + "isArray: " + std::to_string(isArray) + "\n";
		results += tabStr + "isObject: " + std::to_string(isObject) + "\n";
		results += tabStr + "isArg: " + std::to_string(isArg) + "\n";
		results += tabStr + "hasDefault: " + std::to_string(hasDefault) + "\n";
		results += tabStr + "fromObject: " + std::to_string(fromObject) + "\n";
		results += tabStr + "previouslyDeclared: " + std::to_string(previouslyDeclared) + "\n";
		results += tabStr + "order: " + std::to_string(order) + "\n";
		if (member != NULL) results += tabStr + "member: \n" + member->ToString(tabs + 3);
		else results += tabStr + "member: NULL\n";
		if (value != NULL) results += tabStr + "value: \n" + value->ToString(tabs + 3);
		else results += tabStr + "value: NULL\n";
		results += tabStr + "}\n";
		return results;
	}

	std::string ASTExpr::ToString(int tabs) {
		std::string results = ASTNode::ToString(tabs);

		std::string tabStr;
		for (int i = 0; i < tabs + 1; i++) tabStr += " ";
		results += tabStr + "isBoolean: " + std::to_string(isBoolean) + "\n";
		if (cast != NULL) results += tabStr + "cast: \n" + cast->ToString(tabs + 3);
		else results += tabStr + "cast: NULL\n";
		results += tabStr + "}\n";
		return results;
	}

	std::string ASTFuncCall::ToString(int tabs) {
		std::string results = ASTExpr::ToString(tabs);

		std::string tabStr;
		for (int i = 0; i < tabs + 1; i++) tabStr += " ";
		results += tabStr + "isInternal: " + std::to_string(isInternal) + "\n";
		results += tabStr + "isInit: " + std::to_string(isInit) + "\n";
		results += tabStr + "params: \n";
		for (unsigned long long int i = 0; i < params.size(); i++) results += params[i]->ToString(tabs + 3);
		if (func != NULL) results += tabStr + "funcName: " + func->name + "\n";
		else results += tabStr + "func: NULL\n";

		results += tabStr + "}\n";
		return results;
	}

	std::string ASTBinaryExpr::ToString(int tabs) {
		std::string results = ASTExpr::ToString(tabs);

		std::string tabStr;
		for (int i = 0; i < tabs + 1; i++) tabStr += " ";
		results += tabStr + "op: " + Token::ToString(op) + "\n";
		results += tabStr + "isChain: " + std::to_string(isChain) + "\n";
		if (left != NULL) results += tabStr + "left: \n" + left->ToString(tabs + 3);
		else results += tabStr + "left: NULL\n";
		if (right != NULL) results += tabStr + "right: \n" + right->ToString(tabs + 3);
		else results += tabStr + "right: NULL\n";

		results += tabStr + "}\n";
		return results;
	}

	std::string ASTLiteral::ToString(int tabs) {
		std::string results = ASTExpr::ToString(tabs);

		std::string tabStr;
		for (int i = 0; i < tabs + 1; i++) tabStr += " ";
		results += tabStr + "value: " + value + "\n";
		results += tabStr + "litType: " + Token::ToString(litType) + "\n";
		results += tabStr + "unary: " + Token::ToString(unary) + "\n";
		results += tabStr + "calc: " + std::to_string(calc) + "\n";
		results += tabStr + "isCast: " + std::to_string(isCalc) + "\n";
		results += tabStr + "scopeId: " + std::to_string(scopeId) + "\n";
		results += tabStr + "isCalc: " + std::to_string(isCalc) + "\n";
		results += tabStr + "isPost: " + std::to_string(isPost) + "\n";
		results += tabStr + "allowAccess: " + std::to_string(allowAccess) + "\n";
		if (member != NULL) results += tabStr + "member: \n" + member->ToString(tabs + 3);
		else results += tabStr + "member: NULL\n";
		if (obj != NULL && obj->name != "this") results += tabStr + "obj: " + obj->name + "\n";
		else results += tabStr + "obj: NULL\n";
		if (var != NULL) results += tabStr + "varName: " + var->name + "\n";
		else results += tabStr + "var: NULL\n";

		results += tabStr + "}\n";
		return results;
	}

	ASTLiteral* ASTNode::GetLocal(bool pop){
		if (locals.size() > 0) {
			ASTLiteral* result = locals.back();
			if (pop) locals.pop_back();
			return result;
		}
		return NULL;
	}

	bool ASTNode::HasVisibility(TOKEN tok){
		for (unsigned int i = 0; i < visibility.size(); i++){
			if (visibility[i] == tok) return true;
		}
		return visibility.size() == 0 && tok == PUBLIC;
	}

	void ASTNode::Free(Isolate* iso){
		int t = (int) type;
		// this->~ASTNode();
		switch (t){
			case IMPORT: {
				ASTImport* import = (ASTImport*) this;
				import->~ASTImport();
				iso->FreeMemory(this, sizeof(ASTImport)); 
				break;
			}
			case INCLUDE: {
				ASTInclude* include = (ASTInclude*) this;
				include->~ASTInclude();
				iso->FreeMemory(this, sizeof(ASTInclude)); 
				break;
			}
			case FUNC: {
				ASTFunc* func = (ASTFunc*) this;
				func->~ASTFunc();
				iso->FreeMemory(this, sizeof(ASTFunc)); 
				break;
			}
			case VAR: {
				ASTVar* var = (ASTVar*) this;
				var->~ASTVar();
				iso->FreeMemory(this, sizeof(ASTVar)); 
				break;
			}
			case EXPR: {
				ASTExpr* expr = (ASTExpr*) this;
				expr->~ASTExpr();
				iso->FreeMemory(this, sizeof(ASTExpr)); 
				break;
			}
			case BINARY: {
				ASTBinaryExpr* binary = (ASTBinaryExpr*) this;
				binary->~ASTBinaryExpr();
				iso->FreeMemory(this, sizeof(ASTBinaryExpr)); 
				break;
			}
			case LITERAL: {
				ASTLiteral* lit = (ASTLiteral*) this;
				lit->~ASTLiteral();
				iso->FreeMemory(this, sizeof(ASTLiteral)); 
				break;
			}
			case FUNC_CALL: {
				ASTFuncCall* call = (ASTFuncCall*) this;
				call->~ASTFuncCall();
				iso->FreeMemory(this, sizeof(ASTFuncCall)); 
				break;
			}
			case FOR: {
				ASTForExpr* expr = (ASTForExpr*) this;
				expr->~ASTForExpr();
				iso->FreeMemory(this, sizeof(ASTForExpr)); 
				break;
			}
			case WHILE: {
				ASTWhileExpr* expr = (ASTWhileExpr*) this;
				expr->~ASTWhileExpr();
				iso->FreeMemory(this, sizeof(ASTWhileExpr)); 
				break;
			}
			case TRY_CATCH: {
				ASTTryCatchExpr* expr = (ASTTryCatchExpr*) this;
				expr->~ASTTryCatchExpr();
				iso->FreeMemory(this, sizeof(ASTTryCatchExpr)); 
				break;
			}
			case THROW: {
				ASTThrow* expr = (ASTThrow*) this;
				expr->~ASTThrow();
				iso->FreeMemory(this, sizeof(ASTThrow)); 
				break;
			}
			case IF: {
				ASTIf* expr = (ASTIf*) this;
				expr->~ASTIf();
				iso->FreeMemory(this, sizeof(ASTIf)); 
				break;
			}
			case DELETE: {
				ASTImport* import = (ASTImport*) this;
				import->~ASTImport();
				iso->FreeMemory(this, sizeof(ASTDelete)); 
				break;
			}
			case CASE: {
				ASTCase* expr = (ASTCase*) this;
				expr->~ASTCase();
				iso->FreeMemory(this, sizeof(ASTCase)); 
				break;
			}
			case SWITCH: {
				ASTSwitch* expr = (ASTSwitch*) this;
				expr->~ASTSwitch();
				iso->FreeMemory(this, sizeof(ASTSwitch)); 
				break;
			}
			case OBJECT: {
				ASTObject* obj = (ASTObject*) this;
				obj->~ASTObject();
				iso->FreeMemory(this, sizeof(ASTObject)); 
				break;
			}
			case UNDEFINED: {
				ASTUndefined* undef = (ASTUndefined*) this;
				undef->~ASTUndefined();
				iso->FreeMemory(this, sizeof(ASTUndefined)); 
				break;
			}
			case ARRAY: {
				ASTArray* ary = (ASTArray*) this;
				ary->~ASTArray();
				iso->FreeMemory(this, sizeof(ASTArray)); 
				break;
			}
			case OBJECT_INSTANCE: {
				// ASTObjectInstance* inst = (ASTObjectInstance*) this;
				// inst->~ASTObjectInstance();
				// iso->FreeMemory(this, sizeof(ASTObjectInstance)); 
				break;
			}
			case CONTINUE: {
				ASTContinue* cont = (ASTContinue*) this;
				cont->~ASTContinue();
				iso->FreeMemory(this, sizeof(ASTContinue)); 
				break;
			}
			case TOK: {
				ASTToken* tok = (ASTToken*) this;
				tok->~ASTToken();
				iso->FreeMemory(this, sizeof(ASTToken));
				break;
			}
			case 0: return;
			default: {
				printf("Node.cc: %s\n", Token::ToString(type).c_str());
			}
		}
	}

	ASTToken* ASTToken::New(Isolate* iso, TOKEN tok){
		void* pt = iso->GetMemory(sizeof(ASTToken));
		ASTToken* n = new(pt) ASTToken();
		ASTNode::SetDefaults(n, iso);
		n->type = TOK;
		n->value = Token::New(iso, tok);
		n->iso = iso;
		return n;
	}

	ASTToken::~ASTToken(){
		iso->FreeMemory(this->value, sizeof(Token));
	}

	ASTImport* ASTImport::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTImport));
		ASTImport* n = new(pt) ASTImport();
		ASTNode::SetDefaults(n, iso);
		n->type = IMPORT;
		return n;
	}

	ASTInclude* ASTInclude::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTInclude));
		ASTInclude* n = new(pt) ASTInclude();
		ASTNode::SetDefaults(n, iso);
		n->type = INCLUDE;
		return n;
	}

	ASTFunc* ASTFunc::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTFunc));
		ASTFunc* n = new(pt) ASTFunc();
		ASTNode::SetDefaults(n, iso);
		n->type = FUNC;
		n->scope = NULL;
		n->assignmentType = UNDEFINED;
		return n;
	}

	ASTVar* ASTVar::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTVar));
		ASTVar* n = new(pt) ASTVar();
		ASTNode::SetDefaults(n, iso);
		n->type = VAR;
		n->baseName = "";
		n->baseType = UNDEFINED;
		n->value = NULL;
		n->assignmentType = ILLEGAL;
		n->isArray = false;
		n->isObject = false;
		n->member = NULL;
		n->isArg = false;
		n->order = -1;
		n->previouslyDeclared = false;
		n->op = UNDEFINED;
		n->scopeId = -1;
		n->hasDefault = false;
		n->fromObject = false;
		return n;
	}

	ASTVar* ASTVar::Clone(Isolate* isolate, bool shallow){
		ASTVar* var = ASTVar::New(isolate);
		var->baseName = baseName;
		var->baseType = baseType;
		var->value = value;
		var->assignmentType = assignmentType;
		var->isArray = isArray;
		var->isObject = isObject;
		var->member = member;
		var->isArg = isArg;
		var->order = order;
		var->previouslyDeclared = previouslyDeclared;
		var->op = op;
		var->name = name;
		var->scopeId = scopeId;
		var->hasDefault = hasDefault;
		var->fromObject = fromObject;
		if (HasLocal()) var->AddLocal(GetLocal(false)->Clone(isolate, shallow));
		return var;
	}

	ASTExpr* ASTExpr::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTExpr));
		ASTExpr* n = new(pt) ASTExpr();
		ASTNode::SetDefaults(n, iso);
		n->type = EXPR;
		n->cast = NULL;
		n->isBoolean = false;
		return n;
	}

	ASTBreak* ASTBreak::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTBreak));
		ASTBreak* n = new(pt) ASTBreak();
		ASTNode::SetDefaults(n, iso);
		n->type = BREAK;
		return n;
	}

	ASTContinue* ASTContinue::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTContinue));
		ASTContinue* n = new(pt) ASTContinue();
		ASTNode::SetDefaults(n, iso);
		n->type = CONTINUE;
		return n;
	}

	ASTBinaryExpr* ASTBinaryExpr::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTBinaryExpr));
		ASTBinaryExpr* n = new(pt) ASTBinaryExpr();
		ASTNode::SetDefaults(n, iso);
		n->type = BINARY;
		n->left = NULL;
		n->right = NULL;
		n->isBoolean = false;
		n->op = ILLEGAL;
		n->isChain = false;
		return n;
	}

	ASTLiteral* ASTLiteral::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTLiteral));
		ASTLiteral* n = new(pt) ASTLiteral();
		ASTNode::SetDefaults(n, iso);
		n->value = "";
		n->type = LITERAL;
		n->litType = UNDEFINED;
		n->unary = UNDEFINED;
		n->isPost = true;
		n->var = NULL;
		n->isCast = false;
		n->calc = '\0';
		n->isCalc = false;
		n->obj = NULL;
		n->allowAccess = false;
		n->member = NULL;
		n->scopeId = -1;
		return n;
	}

	ASTLiteral* ASTLiteral::Clone(Isolate* iso, bool shallow){
		if (type == OBJECT_INSTANCE) {
			ASTObjectInstance* inst = (ASTObjectInstance*) this;
			return inst->Clone(iso, shallow);
		}
		else if (type == ARRAY){
			ASTArray* ary = (ASTArray*) this;
			return ary->Clone(iso, shallow);
		}
		ASTLiteral* n = ASTLiteral::New(iso);
		n->value = value;
		n->type = type;
		n->litType = litType;
		n->unary = unary;
		n->isPost = isPost;
		n->var = shallow ? NULL : var;
		n->isCast = isCast;
		n->calc = calc;
		n->isCalc = isCalc;
		n->obj = obj;
		n->allowAccess = allowAccess;
		n->scopeId = scopeId;
		n->member = member;
		return n;
	}

	ASTFuncCall* ASTFuncCall::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTFuncCall));
		ASTFuncCall* n = new(pt) ASTFuncCall();
		ASTNode::SetDefaults(n, iso);
		n->type = FUNC_CALL;
		n->isInternal = false;
		n->func = NULL;
		n->isInit = false;
		n->callback = nullptr;
		return n;
	}

	ASTForExpr* ASTForExpr::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTForExpr));
		ASTForExpr* n = new(pt) ASTForExpr();
		ASTNode::SetDefaults(n, iso);
		n->type = FOR;
		n->tick = NULL;
		n->condition = NULL;
		n->var = NULL;
		n->scope = NULL;
		return n;
	}

	ASTWhileExpr* ASTWhileExpr::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTWhileExpr));
		ASTWhileExpr* n = new(pt) ASTWhileExpr();
		ASTNode::SetDefaults(n, iso);
		n->type = WHILE;
		n->condition = NULL;
		return n;
	}

	ASTTryCatchExpr* ASTTryCatchExpr::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTTryCatchExpr));
		ASTTryCatchExpr* n = new(pt) ASTTryCatchExpr();
		ASTNode::SetDefaults(n, iso);
		n->type = TRY_CATCH;
		n->tryScope = NULL;
		n->catchScope = NULL;
		return n;
	}

	ASTThrow* ASTThrow::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTThrow));
		ASTThrow* n = new(pt) ASTThrow();
		ASTNode::SetDefaults(n, iso);
		n->type = THROW;
		n->expr = NULL;
		return n;
	}

	ASTIf* ASTIf::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTIf));
		ASTIf* n = new(pt) ASTIf();
		ASTNode::SetDefaults(n, iso);
		n->type = IF;
		n->condition = NULL;
		n->scope = NULL;
		n->isElse = false;
		return n;
	}

	ASTDelete* ASTDelete::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTDelete));
		ASTDelete* n = new(pt) ASTDelete();
		ASTNode::SetDefaults(n, iso);
		n->type = DELETE;
		n->node = NULL;
		return n;
	}

	ASTCase* ASTCase::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTCase));
		ASTCase* n = new(pt) ASTCase();
		ASTNode::SetDefaults(n, iso);
		n->type = CASE;
		n->condition = NULL;
		n->scope = NULL;
		n->isDefault = false;
		return n;
	}

	ASTSwitch* ASTSwitch::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTSwitch));
		ASTSwitch* n = new(pt) ASTSwitch();
		ASTNode::SetDefaults(n, iso);
		n->type = SWITCH;
		n->value = NULL;
		return n;
	}

	ASTObject* ASTObject::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTObject));
		ASTObject* n = new(pt) ASTObject();
		ASTNode::SetDefaults(n, iso);
		n->type = OBJECT;
		n->scope = NULL;
		n->extend = false;
		return n;
	}

	ASTUndefined* ASTUndefined::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTUndefined));
		ASTUndefined* n = new(pt) ASTUndefined();
		ASTNode::SetDefaults(n, iso);
		n->type = UNDEFINED;
		n->value = "undefined";
		return n;
	}

	ASTArray* ASTArray::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTArray));
		ASTArray* n = new(pt) ASTArray();
		ASTNode::SetDefaults(n, iso);
		n->type = ARRAY;
		n->Get = false;
		return n;
	}

	ASTArray* ASTArray::Clone(Isolate* iso, bool shallow){
		ASTArray* n = ASTArray::New(iso);
		n->value = value;
		n->type = type;
		n->litType = litType;
		n->Get = Get;
		n->unary = unary;
		n->isPost = isPost;
		n->var = shallow ? NULL : var;
		n->isCast = isCast;
		n->calc = calc;
		n->isCalc = isCalc;
		n->obj = obj;
		n->allowAccess = allowAccess;
		n->scopeId = scopeId;
		n->member = member;
		for (unsigned int i = 0; i < this->members.size(); i++){
			n->members.push_back(this->members[i]);
		}
		return n;
	}	

	ASTObjectInstance* ASTObjectInstance::New(Isolate* iso){
		void* pt = iso->GetMemory(sizeof(ASTObjectInstance));
		ASTObjectInstance* n = new(pt) ASTObjectInstance();
		ASTNode::SetDefaults(n, iso);
		n->type = OBJECT_INSTANCE;
		n->litType = OBJECT;
		n->constructor = NULL;
		n->base = NULL;
		n->obj = NULL;
		n->constructorCalled = false;
		return n;
	}

	ASTVar* ASTObjectInstance::CreateProp(Isolate* isolate, std::string name){
		if (HasProp(name)){
			properties[name]->Free(isolate);
			properties.erase(properties.find(name));
		}
		if (base == NULL) base = obj;
		ASTVar* baseVar = NULL;
		CHECK(base != NULL);
		std::vector<ASTNode*> vars = base->scope->Lookup(name, false);
		for (unsigned int i = 0; i < vars.size(); i++){
			if (vars[i]->type == VAR){
				baseVar = (ASTVar*) vars[i];
				break;
			}
		}
		if (baseVar == NULL) return NULL;
		ASTVar* clone = baseVar->Clone(isolate);
		if (properties.size() == 0) properties.clear();
		properties[name] = clone;
		return clone;
	}

	ASTVar* ASTObjectInstance::GetProp(Isolate* isolate, std::string name){
		if (HasProp(name)) return properties[name];
		return CreateProp(isolate, name);
	}

	ASTObjectInstance* ASTObjectInstance::Clone(Isolate* iso, bool shallow){
		ASTObjectInstance* n = ASTObjectInstance::New(iso);
		n->value = value;
		n->type = type;
		n->litType = litType;
		n->unary = unary;
		n->isPost = isPost;
		n->var = shallow ? NULL : var;
		n->isCast = isCast;
		n->calc = calc;
		n->isCalc = isCalc;
		n->obj = obj;
		n->allowAccess = allowAccess;
		n->scopeId = scopeId;
		n->constructor = constructor;
		n->base = base;
		n->obj = obj;
		n->constructorCalled = constructorCalled;
		//n->properties = properties;
		for (std::map<std::string, ASTVar*>::iterator i = properties.begin(); i != properties.end(); ++i){
			n->properties[i->first] = i->second->Clone(iso, shallow);	
		}
		return n;
	}

	void ASTObjectInstance::SetProp(std::string name, ASTLiteral* value){
		propValues[name] = value; // No scoping necessary
	}

} // namespace internal
}