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
		//int n = _id->type().parameters()->size() * 4;
		cout << "\t.globl\t" << _id->name() << endl;
		cout << "\t.type\t" << _id->name() << ", @function" << endl;
		cout << _id->name() << ":" << endl;	
		//prefix

		//generate body
		_body->generate();

		//postfix
		//cout << "\tmovl\t\%ebp, \%esp" << endl;
		//cout << "\tpopl\t\%ebp" << endl;
		cout << "\tleave" << endl;
		cout << "\tret" << endl;
}

void Block::generate()
{
	int offset = 0;

	for(unsigned i = 0; i < declarations()->symbols().size(); i++){
		offset = offset - declarations()->symbols().at(i)->type().size();
		declarations()->symbols().at(i)->offset = offset;
	}

	cout << "\tpushl\t\%ebp" << endl;
	cout << "\tmovl\t\%esp, \%ebp" << endl;
	cout << "\tsubl\t$" << -offset << ",  \%esp" << endl;
	cout << endl;

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
		if(dynamic_cast<Integer *>(_args.at(i))){
			cout << "\tmovl\t$" << ((Integer *)_args.at(i))->value() << ", \%eax" << endl;
		//local
		} else if(((Identifier*)_args.at(i))->symbol()->offset < 0){
			cout << "\tmovl\t" << ((Identifier*)_args.at(i))->symbol()->offset << "(\%ebp), \%eax" << endl;
		//params (yes, this is the same, but I'm not sure it's supposed to be.		   ^-- was %esp
		} else if(((Identifier*)_args.at(i))->symbol()->offset > 0) {
			cout << "\tmovl\t" << ((Identifier*)_args.at(i))->symbol()->offset << "(\%ebp), \%eax" << endl;

		//globals
		} else {
			cout << "\tmovl\t" << ((Identifier*)_args.at(i))->symbol()->name() << ", \%eax" << endl;
			
		}
			cout << "\tmovl\t\%eax, " << /*8 + */4 * i << "(\%esp)" << endl;
	}
	cout << "\tcall\t" << _id->name() << endl << endl;

	//cout << "\taddl\t" <<
}

void Identifier::generate()
{

}

void Integer::generate()
{

}

