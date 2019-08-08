# include "generator.h"
# include "Tree.h"
# include <iostream>

using namespace std;

void generateGlobals(const Scope &scope)
{
	for(Symbol *symbol : scope.symbols()){
		if(!symbol->type().isFunction()){
			cout << "	.comm	" << symbol->name() << "," << symbol->type().size() << endl;
		}
	}
}

void Function::generate()
{
		//function "declaration"
		cout << "\t.globl\t" << _id->name() << endl;
		cout << "\t.type\t" << _id->name() << ", @function" << endl;
		cout << _id->name() << ":" << endl;

		//setting offsets
		//parameters
		for(unsigned i = 0; i < _id->type().parameters()->size(); i++){
			_body->declarations()->symbols().at(i)->offset = 8 + 4 * i;
		}

		//locals
		int offset = 0;
		for(unsigned i = _id->type().parameters()->size(); i < _body->declarations()->symbols().size(); i++){
			offset = offset - _body->declarations()->symbols().at(i)->type().size();
			_body->declarations()->symbols().at(i)->offset = offset;
		}		

		//prefix
		cout << "\tpushl\t%ebp" << endl;
		cout << "\tmovl\t%esp, %ebp" << endl;
		cout << "\tsubl\t$" << -offset << ", %esp" << endl << endl;

		//generate body
		_body->generate();

		//postfix
		//cout << "\tmovl\t\%ebp, \%esp" << endl;
		//cout << "\tpopl\t\%ebp" << endl;
		cout << "\tleave\t" << endl;
		cout << "\tret" << endl;
}

void Block::generate()
{
	for(unsigned i = 0; i < _stmts.size(); i++){
		_stmts.at(i)->generate();
	}
}

void Assignment::generate()
{
	_left->generate();
	_right->generate();
	if(((Identifier*)_left)->symbol()->offset){
		cout << "\tmovl\t" << "$" << ((Integer*)_right)->value() << ", " << ((Identifier*)_left)->symbol()->offset << "(\%ebp)" << endl;
	} else {
		cout << "\tmovl\t" << "$" << ((Integer*)_right)->value() << ", " << "\%eax" << endl;
		cout << "\tmovl\t" << "\%eax" << ", " << ((Identifier*)_left)->symbol()->name() << endl;
	}
	cout << endl;
}

void Call::generate()
{
	for(int i = _args.size() - 1; i >=0; i--){
		_args.at(i)->generate();
		if(dynamic_cast<Integer*>(_args.at(i))){
			cout << "\tpushl\t$" << ((Integer *)_args.at(i))->value() << endl;
		} else if(((Identifier*)_args.at(i))->symbol()->offset != 0){
			cout << "\tpushl\t" << ((Identifier*)_args.at(i))->symbol()->offset << "(%ebp)"<< endl;
		} else {
			cout << "\tpushl\t" << ((Identifier*)_args.at(i))->symbol()->name() << endl;
		}
	}
	cout << "\tcall\t" << _id->name() << endl;

	cout << "\taddl\t$" << _args.size() * 4 << ", %esp" << endl << endl;
}

void Identifier::generate()
{

}

void Integer::generate()
{

}

