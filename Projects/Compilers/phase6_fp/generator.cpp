/*
 * File:	generator.cpp
 *
 * Description:	This file contains the public and member function
 *		definitions for the code generator for Simple C.
 *
 *		Extra functionality:
 *		- putting all the global declarations at the end
 */

# include <sstream>
# include <iostream>
# include <cassert>
# include "generator.h"
# include "machine.h"
# include "Tree.h"
# include "Label.h"

using namespace std;


/* This needs to be set to zero if temporaries are placed on the stack. */

# define SIMPLE_PROLOGUE 0


/* This should be set if we want to use the callee-saved registers. */

# define CALLEE_SAVED 0


/* The registers and their related functions */

typedef vector<Register *>Registers;

static Register *eax = new Register("%eax", "%al");
static Register *ecx = new Register("%ecx", "%cl");
static Register *edx = new Register("%edx", "%dl");
int offset = 0;
static Register *ebx = new Register("%ebx", "%bl");
static Register *esi = new Register("%esi");
static Register *edi = new Register("%edi");

static Register *xmm0 = new Register("xmm0", "");
static Register *xmm1 = new Register("xmm1", "");
static Register *xmm2 = new Register("xmm2", "");
static Register *xmm3 = new Register("xmm3", "");
static Register *xmm4 = new Register("xmm4", "");
static Register *xmm5 = new Register("xmm5", "");
static Register *xmm6 = new Register("xmm6", "");
static Register *xmm7 = new Register("xmm7", "");

static Registers fp_registers = { xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7 };

static Registers registers = { eax, ecx, edx };

# define FP(expr)	((expr)->type().isReal())
# define BYTE(expr)	((expr)->type().size() == 1)

string function_exit;

# if CALLEE_SAVED
static Registers callee_saved = {ebx, esi, edi};
# else
static Registers callee_saved = {};
# endif

/*
 * Assign, from lecture slides.
 * Takes an expression and a register and links the two. Also destroy any other
 * existing links with other expressions or registers.
 */

void assign(Expression *expr, Register *reg){
	if(expr != nullptr) {
		if(expr->_register != nullptr) {
			expr->_register->_node = nullptr;
		}
		
		expr->_register = reg;
	}
	if(reg != nullptr) {
		if(reg->_node != nullptr){
			reg->_node->_register = nullptr;
		}
		
		reg->_node = expr;
	}
}

void release(){

	for (unsigned i = 0; i < registers.size(); i ++){
		assign(nullptr, registers[i]);
	}
}

/*
 * Function:	operator << (private)
 *
 * Description:	Write an expression to the specified stream.  This function
 *		first checks to see if the expression is in a register, and
 *		if not then uses its operand.
 */

static ostream &operator <<(ostream &ostr, Expression *expr){
    if (expr->_register != nullptr)
	return ostr << expr->_register;

    return ostr << expr->_operand;
}

void assigntemp(Expression *expr){
	stringstream ss;
	
	offset = offset - expr->type().size();
	ss << offset << "(%ebp)";
	expr->_operand = ss.str();
}

/*
 * suffix, from class.
 * Returns the instruction suffix of an expression.
 */
 
string suffix(Expression *expr){
	 return FP(expr) ? "sd\t" : (BYTE(expr) ? "b\t" : "l\t");
 }

/*
 * load, from lecture slides.
 * Takes an expression and a register and loads the expression into the register.
 * This function outputs a single mov op.
 */
void load(Expression *expr, Register *reg){

	if(reg->_node != expr){

		if(reg->_node != nullptr) {
			unsigned size = reg->_node->type().size();

			assigntemp(reg->_node);
			cout << "\tmov" << suffix(reg->_node);
			cout << reg->name(size) << ", ";
			cout << reg->_node->_operand << endl;
		}

		if(expr != nullptr) {
			unsigned size = expr->type().size();
			cout << "\tmov" << suffix(expr) << expr;
			cout << ", " << reg->name(size) << endl;
		}
		
		assign(expr, reg);
	}
 }
 
 /*
 * getReg, from lecture slides.
 * Finds and returns the next available register.
 */
Register *getReg(){
	for(unsigned i = 0; i < registers.size(); i++){
			assert(registers[i] != nullptr);
			if(registers[i]->_node == nullptr){
				return registers[i];
			}
	}

	cout << "\t\t\t\t#SPILL" << endl;
	load(nullptr, registers[0]);
	return registers[0];
}

Register *fp_getReg() {

	for (unsigned i= 0; i< fp_registers.size(); i++){
		if (fp_registers[i]->_node == nullptr)
			return fp_registers[i];
	}

	load(nullptr, fp_registers[0]);
	return fp_registers[0];
}
 
/*
 * Function:	align (private)
 *
 * Description:	Return the number of bytes necessary to align the given
 *		offset on the stack.
 */

static int align(int offset){
    if (offset % STACK_ALIGNMENT == 0)
	return 0;

    return STACK_ALIGNMENT - (abs(offset) % STACK_ALIGNMENT);
}

/*
 * Function:	Identifier::generate
 *
 * Description:	Generate code for an identifier.  Since there is really no
 *		code to generate, we simply update our operand.
 */

void Identifier::generate(){
    stringstream ss;


    if (_symbol->_offset != 0)
		ss << _symbol->_offset << "(%ebp)";
    else
		ss << global_prefix << _symbol->name();

    _operand = ss.str();
}


/*
 * Function:	Integer::generate
 *
 * Description:	Generate code for an integer.  Since there is really no
 *		code to generate, we simply update our operand.
 */

void Integer::generate(){
    stringstream ss;


    ss << "$" << _value;
    _operand = ss.str();
}


/*
 * Function:	Call::generate
 *
 * Description:	Generate code for a function call expression.
 *		TODO
 *		NOT FINISHED: Only guaranteed to work if the argument is
 *		either an integer literal or an integer scalar variable.
 */

void Call::generate(){ //TODO (needs testing to figure out problems)
    unsigned bytesPushed = 0;


    /* Compute how many bytes will be pushed on the stack. */

    for (int i = _args.size() - 1; i >= 0; i --) {
	bytesPushed += _args[i]->type().size();

	if (STACK_ALIGNMENT != 4 && _args[i]->_hasCall)
	    _args[i]->generate();
    }


    /* Adjust the stack keep it aligned.  We do this by simply subtracting
       the necessary number of bytes from the stack pointer.  Effectively,
       we are pushing nonexistent arguments on the stack so that the total
       number of bytes pushed is a multiple of the alignment. */

    if (align(bytesPushed) > 0) {
	cout << "\tsubl\t$" << align(bytesPushed) << ", %esp" << "\t\t#Call: stack space allocation" << endl;
	bytesPushed += align(bytesPushed);
    }


    /* Push each argument on the stack. */

    for (int i = _args.size() - 1; i >= 0; i --) {
		if (STACK_ALIGNMENT == 4 || !_args[i]->_hasCall){
	    	_args[i]->generate();
			cout << "\tpushl\t" << _args[i] << "\t\t#Call: Push arguments[" << i << "]  " << _args[i]->type() << endl;
		}
	}
    
	// Spill caller-saved registers
	for(unsigned i = 0; i < registers.size(); i++){
		load(nullptr, registers[i]);
	} 

    /* Call the function and then adjust the stack pointer back. */

    cout << "\tcall\t" << global_prefix << _id->name() << "\t\t#Call: function call" << endl;

    if (bytesPushed > 0)
		cout << "\taddl\t$" << bytesPushed << ", %esp" << "\t\t#Call: stack pointer readjustment" << endl;

	assign(this, eax);

}

/*
 * Function:	Assignment::generate
 *
 * Description:	Generate code for an assignment statement.
 *		TODO
 *		NOT FINISHED: Only guaranteed to work if the right-hand
 *		side is an integer literal and the left-hand side is an
 *		integer scalar.
 */

void Assignment::generate(){ //TODO (update for derefs)

	if(_left->isDeref() == nullptr){

   		_left->generate();
		_right->generate();

		if(_right->_register == nullptr){
			load(_right, getReg());
		}

    	cout << "\tmov" << suffix(_left) << "\t" << _right << ", " << _left << "\t\t#Assignment" << endl;
	
	} else {

		_left->isDeref()->generate();
		_right->generate();

		if(_right->_register == nullptr){
			load(_right, eax);
		}

		if(_left->isDeref()->_register == nullptr){
			load(_left->isDeref(), ecx);
		} 

		cout << "\tmov" << suffix(_left->isDeref()) << "\t" << _right << ", (" << _left->isDeref() << ")\t\t#Deref Assignment" << endl;

	}
}

/*
 * Function:	Block::generate
 *
 * Description:	Generate code for this block, which simply means we
 *		generate code for each statement within the block.
 */

void Block::generate(){
    for (unsigned i = 0; i < _stmts.size(); i ++){
		_stmts[i]->generate();
	}
	release();
}

/*
 * Function:	Function::generate
 *
 * Description:	Generate code for this function, which entails allocating
 *		space for local variables, then emitting our prologue, the
 *		body of the function, and the epilogue.
 *
 *		The stack must be aligned at the point at which a function
 *		begins execution.  Since the call instruction pushes the
 *		return address on the stack and each function is expected
 *		to push its base pointer, we adjust our offset by that
 *		amount and then perform the alignment.
 *
 *		On a 32-bit Intel platform, 8 bytes are pushed (4 for the
 *		return address and 4 for the base pointer).  Since Linux
 *		requires a 4-byte alignment, all we need to do is ensure
 *		the stack size is a multiple of 4, which will usually
 *		already be the case.  However, since OS X requires a
 *		16-byte alignment (thanks, Apple, for inventing your own
 *		standards), we will often see an extra amount of stack
 *		space allocated.
 *
 *		On a 64-bit Intel platform, 16 bytes are pushed (8 for the
 *		return address and 8 for the base pointer).  Both Linux and
 *		OS X require 16-byte alignment.
 */

void Function::generate(){
    int param_offset;
	stringstream ss;

    /* Generate our prologue. */

    param_offset = PARAM_OFFSET + SIZEOF_REG * callee_saved.size();
    offset = param_offset;
    allocate(offset);

    cout << global_prefix << _id->name() << ":" << "\t\t#Function" << endl;
    cout << "\tpushl\t%ebp" << endl;

    for (unsigned i = 0; i < callee_saved.size(); i ++)
	cout << "\tpushl\t" << callee_saved[i] << endl;

    cout << "\tmovl\t%esp, %ebp" << endl;

    if (SIMPLE_PROLOGUE) {
	offset -= align(offset - param_offset);
	cout << "\tsubl\t$" << -offset << ", %esp" << endl;
    } else
	cout << "\tsubl\t$" << _id->name() << ".size, %esp" << endl;

	ss << _id->name() << ".exit";
	function_exit = ss.str();

    /* Generate the body of this function. */

    _body->generate();

    /* Generate our epilogue. */

	cout << function_exit << ":" << endl;

    cout << "\tmovl\t%ebp, %esp" << endl;

    for (int i = callee_saved.size() - 1; i >= 0; i --)
	cout << "\tpopl\t" << callee_saved[i] << endl;

    cout << "\tpopl\t%ebp" << endl;
    cout << "\tret" << endl << endl;

    if (!SIMPLE_PROLOGUE) {
	offset -= align(offset - param_offset);
	cout << "\t.set\t" << _id->name() << ".size, " << -offset << endl;
    }

    cout << "\t.globl\t" << global_prefix << _id->name() << endl << endl;
}

void String::generate(){
	
	Label label;
	stringstream ss;

	cout << "\t.data" << endl;
	cout << label << ":\t.asciz\t" << _value << endl;
	cout << "\t.text" << endl;

	ss << label;
	_operand = ss.str();
}

void Real::generate(){ //TODO (and also everything involving reals... yikes)

	Label label;
	stringstream ss;

	cout << "\t.data" << endl;
	cout << label << ":\t.double\t" << _value << endl;
	cout << "\t.text" << endl;

	ss << label;
	_operand = ss.str();
}

void Add::generate(){
	
	_left->generate();
	_right->generate();

	assert(_left != nullptr);

	if(_left->_register == nullptr){
		load(_left, FP(_left) ? fp_getReg() :getReg());
	}

	cout << "\tadd" << suffix(_left) << "\t";
	cout << _right << ", " << _left << "\t\t#ADD" << endl;
	
	assign(_right, nullptr);
	assign(this, _left->_register);
}

void Subtract::generate(){
	
	_left->generate();
	_right->generate();
	
	if(_left->_register == nullptr){
		load(_left, FP(_left) ? fp_getReg() :getReg());
	}
	
	cout << "\tsub" << suffix(_left);
	cout << _right << ", " << _left << "\t\t#SUB" << endl;
	
	assign(_right, nullptr);
	assign(this, _left->_register);
}

void Multiply::generate(){
	
	_left->generate();
	_right->generate();
	if(_left->_register == nullptr){
		load(_left, FP(_left) ? fp_getReg() :getReg());
	}

	if(FP(_left) || FP(_right)){
		cout << "\tmul" << suffix(_left);
		cout << _right << ", " << _left << "\t\t#MUL" << endl;
	} else {
		cout << "\timul" << suffix(_left);
		cout << _right << ", " << _left << "\t\t#MUL" << endl;
	}
	assign(_right, nullptr);
	assign(this, _left->_register);
}

void Divide::generate(){
	
	_left->generate();
	_right->generate();
	
	if(FP(_left) || FP(_right)){

		if(_left->_register == nullptr){
			load(_left, FP(_left) ? fp_getReg() :getReg());
		}
		cout << "\tdivsd\t" << _right << ", " << left << endl;
		assign(this, _right->_register);
		assign(_right, nullptr);
	} else {
		load(_left, eax);
		load(_left, edx);
		load(_right, ecx);
	
		cout << "\tsarl\t$31, %edx" << "\t\t#DIV" << endl;
		//sign extension
	
		cout << "\tidiv" << suffix(_left);
		cout << _right << endl;
	
		assign(_right, nullptr);
		assign(nullptr, edx);
		assign(this, eax);
	}
}

void Remainder::generate(){
		
	_left->generate();
	_right->generate();
	
	load(_left, eax);
	load(_left, edx);
	load(_right, ecx);
	
	cout << "\tsarl\t$31, %edx" << "\t\t#REM" << endl;
	//sign extension
	
	cout << "\tidiv" << suffix(_left);
	cout << _right << endl;
	
	assign(_right, nullptr);
	assign(nullptr, eax);
	assign(this, edx);
}

void Expression::test(const Label &label, bool ifTrue){
	
	generate();

	if(_register== nullptr){
		load(this, FP(_register) ? fp_getReg() : getReg());
	}

	if(FP(this)){
		load(nullptr, xmm7);
		cout << "\tpxor\t" << xmm7 << ", " << xmm7 << endl;
		cout << "\tucomisd\t" << xmm7 << ", " << _register << endl;
		cout << "\tsete"
		assign(this, nullptr);
		
	} else {
		
		cout << "\tcmpl\t$0, " << this << endl;
		cout << (ifTrue ? "\tjne\t" : "\tje\t") << label << endl;

		assign(this, nullptr);
	}
}

void Negate::generate(){

	_expr->generate();
	
	if(_expr->_register == nullptr){
		load(_expr, getReg());
	}

	cout << "\tneg" << suffix(_expr) << "\t" << _expr << "\t\t#NEG" << endl;

	assign(this, _expr->_register);
	assign(_expr, nullptr);

}

void Not::generate(){

	_expr->generate();

	if(_expr->_register == nullptr){
		load(_expr, getReg());
	}

	cout << "\tcmp" << suffix(_expr) << "\t$0, " << _expr << "\t\t#NOT" << endl;
	cout << "\tsete\t" << _expr->_register->byte() << endl;
	cout << "\tmovzbl\t" << _expr->_register->byte() << ", " << _expr << endl;

	assign(this, _expr->_register);
	assign(_expr, nullptr);

}

void Address::generate(){

	cout << "\t\t\t\t#ADDR" << endl;
	
	_expr->generate();

	if(_expr->isDeref() == nullptr){
		assign(this, getReg());
		assign(_expr, nullptr);
		cout << "\tleal\t" << _expr << ", " << _register << endl;
	} else {
		_operand = _expr->_operand;
		assign(this, _expr->_register);
		assign(_expr, nullptr);
	}
}

void Dereference::generate(){ //TODO

	_expr->generate();

	cout << "\t\t\t\t\t#DEREF" << endl;
	
	if(_expr->lvalue()) {
		cout << "\t\t\t\t\t#LVALUE DEREF" << endl;
		_operand = _expr->_operand;
		assign(this, _expr->_register);
		assign(_expr, nullptr);
	} else {
		cout << "\t\t\t\t\t#NON-LVALUE DEREF" << endl;
		if(_expr->_register == nullptr){
			load(_expr, getReg());
		}
		cout << "\tmov" << suffix(_expr) << "\t(" << _expr->_register <<"), " << _expr->_register << endl;
		assign(this, _expr->_register);
		assign(_expr, nullptr);
	}
}

void While::generate()
{
	Label loop, exit;
	
	cout << loop << ":" << endl;
	
	_expr->test(exit, false);
	_stmt->generate();
	release();
	
	cout << "\tjmp\t" << loop << endl;
	cout << exit << ":" << endl;
}

void If::generate(){
	
	Label skip, exit;
	
	_expr->test(skip, false);
	_thenStmt->generate();

	if(_elseStmt == nullptr){
		cout << skip << ":" << endl;
	} else {
		cout << "\tjmp\t" << exit << endl;
		cout << skip << ":" << endl;
		_elseStmt->generate();
		cout << exit << ":" << endl;
	}

	release();
}

void LogicalOr::generate(){

	Label first, second;

	_left->test(first, true);
	_right->test(first, true);


	assign(_left, nullptr);
	assign(_right, nullptr);
	assign(this, getReg());
	
	cout << "\t\t\t\t#OR" << endl;

	cout << "\tmovl\t$0, " << _register << endl;
	cout << "\tjmp\t" << second << endl;
	cout << first << ":" << endl;
	cout << "\tmovl\t$1, " << _register << endl;
	cout << second << ":" << endl;
}

void LogicalAnd::generate(){

	Label first, second;

	_left->test(first, false);
	_right->test(first, false);

	assign(_left, nullptr);
	assign(_right, nullptr);
	assign(this, getReg());

	cout << "\t\t\t\t#AND" << endl;

	cout << "\tmovl\t$1, " << _register << endl;
	cout << "\tjmp\t" << second << endl;
	cout << first << ":" << endl;
	cout << "\tmovl\t$0, " << _register << endl;
	cout << second << ":" << endl;
}

void comparison(Expression *_left, Expression *_right, string comparisonop, Expression *self){

	_left->generate();
	_right->generate();

	if(_left->_register == nullptr){
		load(_left, getReg());
	}	

	cout << "\t\t\t\t#cmp" << comparisonop << endl;

	cout << "\tcmp" << suffix(_left) << "\t";
	cout << _right << ", " << _left << endl;
	cout << "\tset" << comparisonop << "\t" << _left->_register->byte() << endl;
	cout << "\tmovzbl\t" << _left->_register->byte() << ", " << _left << endl;

	assign(self, _left->_register);
	assign(_left, nullptr);
	assign(_right, nullptr);
}

void LessThan::generate(){ 
	comparison(_left, _right, "l", this);
}

void GreaterThan::generate(){ 
	comparison(_left, _right, "g", this);
}

void LessOrEqual::generate(){ 
	comparison(_left, _right, "le", this);
}

void GreaterOrEqual::generate(){ 
	comparison(_left, _right, "le", this);
}

void Equal::generate(){ 
	comparison(_left, _right, "e", this);
}

void NotEqual::generate(){ 
	comparison(_left, _right, "ne", this);
}

void Return::generate(){

	_expr->generate();
	cout << "\t\t\t\t#RETURN" << endl;
	load(_expr, eax);
	cout << "\tjmp \t" << function_exit << endl;

}

void Cast::generate(){

	_expr->generate();

	if(_expr->_register == nullptr){
		load(_expr, getReg());
	}

	cout << "\t\t\t\t#CAST" << endl;
	//Cast to CHAR
	if(_type.size() == 1){
		//From CHAR
		if(_expr->type().size() == 1){
			assign(this, _expr->_register);
			return;
		}
		//From INT
		else if (_expr->type().size() == 4){
			cout << "\tmovsbl\t" << _expr->_register->byte() << ", " << _expr->_register->name() << endl;
			assign(this, _expr->_register);
			return;
		}
		//From REAL
		else if(_expr->type().size() == 8){

		}
	}
	//Cast to INT
	else if(_type.size() == 4){
		//From CHAR
		if(_expr->type().size() == 1){
			//_expr->_operand = _expr->_register->byte();
			assign(this, _expr->_register);
			return;
		}
		//From INT
		else if (_expr->type().size() == 4){
			assign(this, _expr->_register);
			return;
		}
		//From REAL
		else if(_expr->type().size() == 8){
			
		}
	}
	//Cast to REAL
	else if(_type.size() == 8){
		//From CHAR
		if(_expr->type().size() == 1){
			return;
		}
		//From INT
		else if (_expr->type().size() == 4){
			cout << "\tmovsbl\t" << _expr->_register->byte() << ", " << _expr->_register->name() << endl;
			return;
		}
		//From REAL
		else if(_expr->type().size() == 8){
			
		}
	}
} 

/*
 * Function:	generateGlobals
 *
 * Description:	Generate code for any global variable declarations.
 */

void generateGlobals(Scope *scope)
{
    const Symbols &symbols = scope->symbols();

    for (unsigned i = 0; i < symbols.size(); i ++)
	if (!symbols[i]->type().isFunction()) {
	    cout << "\t.comm\t" << global_prefix << symbols[i]->name() << ", ";
	    cout << symbols[i]->type().size() << endl;
	}
}
