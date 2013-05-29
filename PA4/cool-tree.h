#ifndef COOL_TREE_H
#define COOL_TREE_H
//////////////////////////////////////////////////////////
//
// file: cool-tree.h
//
// This file defines classes for each phylum and constructor
//
//////////////////////////////////////////////////////////


#include "tree.h"
#include "cool-tree.handcode.h"
#include "semant.h"


// define the class for phylum
// define simple phylum - Program
typedef class Program_class *Program;

class Program_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Program(); }
   virtual Program copy_Program() = 0;

#ifdef Program_EXTRAS
   Program_EXTRAS
#endif
};


// define simple phylum - Class_
typedef class Class__class *Class_;

class Class__class : public tree_node {
public:
   tree_node *copy()		 { return copy_Class_(); }
   virtual Class_ copy_Class_() = 0;

   virtual void collect_Methods( method_table_type *method_table) = 0;
   virtual bool check_Class_Types() = 0;

#ifdef Class__EXTRAS
   Class__EXTRAS
#endif
};


// define simple phylum - Feature
typedef class Feature_class *Feature;

class Feature_class : public tree_node {
   Type feature_type;
public:
   tree_node *copy()		 { return copy_Feature(); }
   virtual Feature copy_Feature() = 0;

   virtual void collect_Feature_Types( method_table_type *) = 0;
   virtual bool check_Feature_Types() = 0;
   virtual void install_Feature_Types() = 0;

#ifdef Feature_EXTRAS
   Feature_EXTRAS
#endif
};


// define simple phylum - Formal
typedef class Formal_class *Formal;

class Formal_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Formal(); }
   virtual Formal copy_Formal() = 0;

   virtual Type collect_Formal_Type() = 0;
   virtual bool check_Formal_Type() = 0;
   virtual void install_Formal_Type() = 0;

#ifdef Formal_EXTRAS
   Formal_EXTRAS
#endif
};


// define simple phylum - Expression
typedef class Expression_class *Expression;

class Expression_class : public tree_node {
   Type   type;
   bool   checked;
public:
   tree_node *copy()		 { return copy_Expression(); }
   virtual Expression copy_Expression() = 0;

   virtual Type do_Check_Expr_Type() = 0;
   Type get_Expr_Type()
   {
	   if ( !checked)
	   {
		   type = do_Check_Expr_Type();
		   checked = true;
	   }
	   return type;
   }

   virtual bool is_no_expr() const
   {
	   return false;
   }

#ifdef Expression_EXTRAS
   Expression_EXTRAS
#endif
};


// define simple phylum - Case
typedef class Case_class *Case;

class Case_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Case(); }
   virtual Case copy_Case() = 0;

#ifdef Case_EXTRAS
   Case_EXTRAS
#endif
};


// define the class for phylum - LIST
// define list phlyum - Classes
typedef list_node<Class_> Classes_class;
typedef Classes_class *Classes;


// define list phlyum - Features
typedef list_node<Feature> Features_class;
typedef Features_class *Features;


// define list phlyum - Formals
typedef list_node<Formal> Formals_class;
typedef Formals_class *Formals;


// define list phlyum - Expressions
typedef list_node<Expression> Expressions_class;
typedef Expressions_class *Expressions;


// define list phlyum - Cases
typedef list_node<Case> Cases_class;
typedef Cases_class *Cases;


// define the class for constructors
// define constructor - program
class program_class : public Program_class {
protected:
   Classes classes;
public:
   program_class(Classes a1) {
      classes = a1;
   }
   Program copy_Program();
   void dump(ostream& stream, int n);

#ifdef Program_SHARED_EXTRAS
   Program_SHARED_EXTRAS
#endif
#ifdef program_EXTRAS
   program_EXTRAS
#endif
};


// define constructor - class_
class class__class : public Class__class {
protected:
   Symbol name;
   Symbol parent;
   Features features;
   Symbol filename;
public:
   class__class(Symbol a1, Symbol a2, Features a3, Symbol a4) {
      name = a1;
      parent = a2;
      features = a3;
      filename = a4;
   }
   Class_ copy_Class_();
   void dump(ostream& stream, int n);

   void collect_Methods( method_table_type *method_table)
   {
	   for ( int i = features->first(); features->more( i); i = features->next( i))
	   {
		   features->nth( i)->collect_Feature_Types( method_table);
	   }
   }

   bool check_Class_Types()
   {
	   for ( int i = features->first(); features->more( i); i = features->next( i))
	   {
		   Feature ft = features->nth( i);
		   ft->install_Feature_Types();
	   }

	   for ( int i = features->first(); features->more( i); i = features->next( i))
	   {
		   Feature ft = features->nth( i);
		   if ( !ft->check_Feature_Types())
		   {
			   return false;
		   }
	   }

	   return true;
   }

#ifdef Class__SHARED_EXTRAS
   Class__SHARED_EXTRAS
#endif
#ifdef class__EXTRAS
   class__EXTRAS
#endif
};


// define constructor - method
class method_class : public Feature_class {
protected:
   Symbol name;
   Formals formals;
   Symbol return_type;
   Expression expr;
public:
   method_class(Symbol a1, Formals a2, Symbol a3, Expression a4) {
      name = a1;
      formals = a2;
      return_type = a3;
      expr = a4;
   }
   Feature copy_Feature();
   void dump(ostream& stream, int n);

   void collect_Feature_Types( method_table_type method_table)
   {
	   feature_type = class_table->lookup( return_type);
	   List<class_tree_node_type> *syms = new List<class_tree_node_type>( feature_type, NULL);
	   for ( int i = formals.first(); formals->more( i); i = formals->next( i))
	   {
		   Type type = formals->nth( i)->collect_Formal_Type();
		   syms = new List<class_tree_node_type>( type, syms);
	   }

	   method_table->addid( name, syms);
   }

   void install_Feature_Types()
   {
   }

   bool check_Feature_Types()
   {
	   var_table->enter_scope();
	   for ( int i = formals.first(); formals->more( i); i = formals->next( i))
	   {
		   Formal fm = formals->nth( i);
		   if ( !fm->check_Formal_Type())
		   {
			   return false;
		   }

		   fm->install_Formal_Type();
	   }

	   Type type = feature_type;
	   Type body_type = expr->get_Expr_Type();

	   var_table->exit_scope();

	   return type && type->is_defined() && body_type && body_type->is_subtype_of( type);
   }

#ifdef Feature_SHARED_EXTRAS
   Feature_SHARED_EXTRAS
#endif
#ifdef method_EXTRAS
   method_EXTRAS
#endif
};


// define constructor - attr
class attr_class : public Feature_class {
protected:
   Symbol name;
   Symbol type_decl;
   Expression init;
public:
   attr_class(Symbol a1, Symbol a2, Expression a3) {
      name = a1;
      type_decl = a2;
      init = a3;
   }
   Feature copy_Feature();
   void dump(ostream& stream, int n);

   void collect_Feature_Types( method_table_type *method_table)
   {
   }

   bool check_Feature_Types()
   {
	   Type type = feature_type;
	   Type t2 = init->is_no_expr() ? init->get_Expr_Type() : type;

	   return type && type->is_defined() && t2 && t2->is_subtype_of( type);
   }

   void install_Feature_Types()
   {
	   feature_type = lookup_install_type( type_decl);
	   var_table->addid( name, feature_type);
   }

#ifdef Feature_SHARED_EXTRAS
   Feature_SHARED_EXTRAS
#endif
#ifdef attr_EXTRAS
   attr_EXTRAS
#endif
};


// define constructor - formal
class formal_class : public Formal_class {
   Type type;
protected:
   Symbol name;
   Symbol type_decl;
public:
   formal_class(Symbol a1, Symbol a2) {
      name = a1;
      type_decl = a2;
   }
   Formal copy_Formal();
   void dump(ostream& stream, int n);

   Type collect_Formal_Type()
   {
	   type = val_table->probe( name);
	   if ( type == NULL)
	   {
		   type = lookup_install_type( type_decl);
	   }
	   else
	   {
		   type = No_type;
	   }

	   return type;
   }

   bool check_Formal_Type()
   {
	   return type && type->is_defined();
   }

   void install_Formal_Type()
   {
	   var_table->addid( name, type);
   }

#ifdef Formal_SHARED_EXTRAS
   Formal_SHARED_EXTRAS
#endif
#ifdef formal_EXTRAS
   formal_EXTRAS
#endif
};


// define constructor - branch
class branch_class : public Case_class {
protected:
   Symbol name;
   Symbol type_decl;
   Expression expr;
public:
   branch_class(Symbol a1, Symbol a2, Expression a3) {
      name = a1;
      type_decl = a2;
      expr = a3;
   }
   Case copy_Case();
   void dump(ostream& stream, int n);

   Type check_Case_Type( Type path_type)
   {
	   Type id_type = class_table->lookup( type_decl);
	   Type ret = No_type;

	   if ( id_type && id_type->is_defined() &&
			   ( id_type->is_subtype_of( path_type)
			     || path_type->is_subtype_of( id_type)
			     )
	      )
	   {
		   var_table->enter_scope();
		   var_table->addid( name, id_type);

		   ret = expr->get_Expr_Type();

		   var_table->exit_scope();
	   }

	   return ret;
   }

#ifdef Case_SHARED_EXTRAS
   Case_SHARED_EXTRAS
#endif
#ifdef branch_EXTRAS
   branch_EXTRAS
#endif
};


// define constructor - assign
class assign_class : public Expression_class {
protected:
   Symbol name;
   Expression expr;
public:
   assign_class(Symbol a1, Expression a2) {
      name = a1;
      expr = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   Type n1 = var_table->lookup( name);
	   Type n2 = expr->get_Expr_Type();

	   return n1 && n1->is_defined() && n2 && n2->is_subtype_of( n1) ? n2 : No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef assign_EXTRAS
   assign_EXTRAS
#endif
};


// define constructor - static_dispatch
class static_dispatch_class : public Expression_class {
protected:
   Expression expr;
   Symbol type_name;
   Symbol name;
   Expressions actual;
public:
   static_dispatch_class(Expression a1, Symbol a2, Symbol a3, Expressions a4) {
      expr = a1;
      type_name = a2;
      name = a3;
      actual = a4;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   Type caller = expr->get_Expr_Type();
	   Type real_caller = class_table->lookup( type_name);
	   if ( !real_caller || !real_caller->is_defined() ||
			   !caller || !caller->is_subtype_of( real_caller))
	   {
		   // What's the fuck with caller.
		   return No_type;
	   }

	   // Copied from dispatch class.
	   class_method *types = real_caller->find_method( name);
	   Type ret_type = types->hd();
	   ret_type = ret_type == Self_type ? caller : ret_type;

	   types = types->tl();

	   int i = act_type->first();
	   while ( actual->more( i) && types && flag)
	   {
		   Expression expr = actual->nth( i);
		   Type act_type = expr->get_Expr_Type();
		   Type para_type = types->hd();

		   if ( act_type && para_type && para_type->is_defined() &&
				   act_type->is_subtype_of( para_type))
		   {
			   types = types->tl(), i = actual->next( i);
		   }
		   else
		   {
			   break;
		   }
	   }
	   if ( actual->more( i) || types)
	   {
		   return No_type;
	   }

	   return ret_type == Self_type ? Current_type : ret_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef static_dispatch_EXTRAS
   static_dispatch_EXTRAS
#endif
};


// define constructor - dispatch
class dispatch_class : public Expression_class {
protected:
   Expression expr;
   Symbol name;
   Expressions actual;
public:
   dispatch_class(Expression a1, Symbol a2, Expressions a3) {
      expr = a1;
      name = a2;
      actual = a3;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   Type caller = expr->get_Expr_Type();
	   if ( !caller)
	   {
		   // What's the fuck with caller.
		   return No_type;
	   }

	   Type real_caller = caller == Self_type ? Current_type : caller;

	   class_method *types = real_caller->find_method( name);
	   Type ret_type = types->hd();
	   ret_type = ret_type == Self_type ? caller : ret_type;

	   types = types->tl();

	   int i = act_type->first();
	   while ( actual->more( i) && types && flag)
	   {
		   Expression expr = actual->nth( i);
		   Type act_type = expr->get_Expr_Type();
		   Type para_type = types->hd();

		   if ( act_type && para_type && para_type->is_defined() &&
				   act_type->is_subtype_of( para_type))
		   {
			   types = types->tl(), i = actual->next( i);
		   }
		   else
		   {
			   break;
		   }
	   }
	   if ( actual->more( i) || types)
	   {
		   return No_type;
	   }

	   return ret_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef dispatch_EXTRAS
   dispatch_EXTRAS
#endif
};


// define constructor - cond
class cond_class : public Expression_class {
protected:
   Expression pred;
   Expression then_exp;
   Expression else_exp;
public:
   cond_class(Expression a1, Expression a2, Expression a3) {
      pred = a1;
      then_exp = a2;
      else_exp = a3;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   Type then_type = then_exp->get_Expr_Type();
	   Type else_type = else_exp->get_Expr_Type();
	   return pred->get_Expr_Type() == Bool_type &&
		   then_type && else_type
		   ? find_lca( then_type, else_type) : No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef cond_EXTRAS
   cond_EXTRAS
#endif
};


// define constructor - loop
class loop_class : public Expression_class {
protected:
   Expression pred;
   Expression body;
public:
   loop_class(Expression a1, Expression a2) {
      pred = a1;
      body = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   return pred->get_Expr_Type() == Bool_type && body->get_Expr_Type()
		   ? Object_type : No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef loop_EXTRAS
   loop_EXTRAS
#endif
};


// define constructor - typcase
class typcase_class : public Expression_class {
protected:
   Expression expr;
   Cases cases;
public:
   typcase_class(Expression a1, Cases a2) {
      expr = a1;
      cases = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   Type path_type = expr->get_Expr_Type();
	   Type value_type = No_type;
	   if ( path_type)
	   {
		   for ( int i = cases->first(); cases->more( i); i = cases->next( i))
		   {
			   Case br = cases->nth( i);
			   Type br_type = br->check_Case_Type( path_type);
			   if ( !br_type)
			   {
				   value_type = No_type;
			   }
			   else
			   {
				   if ( value_type)
				   {
					   value_type = find_lca( value_type, br_type);
				   }
				   else
				   {
					   value_type = br_type;
				   }
			   }

			   if ( !value_type)
			   {
				   break;
			   }
		   }
	   }

	   return value_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef typcase_EXTRAS
   typcase_EXTRAS
#endif
};


// define constructor - block
class block_class : public Expression_class {
protected:
   Expressions body;
public:
   block_class(Expressions a1) {
      body = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   Type ret = Object_type;
	   for ( int i = body->first(); body->more( i) && ret; i = body->next( i))
	   {
		   ret = body->nth(i)->get_Expr_Type();
	   }
	   return ret;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef block_EXTRAS
   block_EXTRAS
#endif
};


// define constructor - let
class let_class : public Expression_class {
protected:
   Symbol identifier;
   Symbol type_decl;
   Expression init;
   Expression body;
public:
   let_class(Symbol a1, Symbol a2, Expression a3, Expression a4) {
      identifier = a1;
      type_decl = a2;
      init = a3;
      body = a4;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   Type id_type = class_table->lookup( type_decl);
	   id_type = id_type == Self_type ? Current_type : id_type;
	   Type expr_type = init->is_no_expr() ? init->get_Expr_Type() : id_type;

	   Type ret = No_type;
	   if ( id_type && id_type->is_defined() &&
			   expr_type && expr_type->is_subtype_of( id_type))
	   {
		   var_table->enter_scope();
		   var_table->addid( identifier, id_type);

		   Type body_type = body->get_Expr_Type();

		   if ( body_type && body_type->is_defined())
		   {
			   ret = body_type;
		   }
		   var_table->exit_scope();
	   }

	   return ret;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef let_EXTRAS
   let_EXTRAS
#endif
};


// define constructor - plus
class plus_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   plus_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   return this->type =
		   e1->get_Expr_Type() == Int_type && e2->get_Expr_Type() == Int_type
		   ? Int_type : No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef plus_EXTRAS
   plus_EXTRAS
#endif
};


// define constructor - sub
class sub_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   sub_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   return this->type =
		   e1->get_Expr_Type() == Int_type && e2->get_Expr_Type() == Int_type
		   ? Int_type : No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef sub_EXTRAS
   sub_EXTRAS
#endif
};


// define constructor - mul
class mul_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   mul_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   return this->type =
		   e1->get_Expr_Type() == Int_type && e2->get_Expr_Type() == Int_type
		   ? Int_type : No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef mul_EXTRAS
   mul_EXTRAS
#endif
};


// define constructor - divide
class divide_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   divide_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   return this->type =
		   e1->get_Expr_Type() == Int_type && e2->get_Expr_Type() == Int_type
		   ? Int_type : No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef divide_EXTRAS
   divide_EXTRAS
#endif
};


// define constructor - neg
class neg_class : public Expression_class {
protected:
   Expression e1;
public:
   neg_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   return this->typpe =
		   e1->get_Expr_Type() == Int_type
		   ? Int_type : No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef neg_EXTRAS
   neg_EXTRAS
#endif
};


// define constructor - lt
class lt_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   lt_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   return this->type =
		   e1->get_Expr_Type() == Int_type &&
		   e2->get_Expr_Type() == Int_type
		   ? Bool_type : No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef lt_EXTRAS
   lt_EXTRAS
#endif
};


// define constructor - eq
class eq_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   eq_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   Type type1 = e1->get_Expr_Type();
	   Type type2 = e2->get_Expr_Type();
	   this->type = type1 && type2 ? Bool_type : No_type;

	   if ( ( type1 != type2) &&
			   ( type1 == Int_type || type2 == Int_type ||
			     type1 == Str_type || type2 == Str_type ||
			     type1 == Bool_type || type2 == Bool_type))
	   {
		   this->type = No_type;
	   }

	   return this->type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef eq_EXTRAS
   eq_EXTRAS
#endif
};


// define constructor - leq
class leq_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   leq_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   return this->type =
		   e1->get_Expr_Type() == Int_type &&
		   e1->get_Expr_Type() == Int_type
		   ? Bool_type : No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef leq_EXTRAS
   leq_EXTRAS
#endif
};


// define constructor - comp
class comp_class : public Expression_class {
protected:
   Expression e1;
public:
   comp_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   return this->type = e1->get_Expr_Type() == Bool_type ? Bool_type : No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef comp_EXTRAS
   comp_EXTRAS
#endif
};


// define constructor - int_const
class int_const_class : public Expression_class {
protected:
   Symbol token;
public:
   int_const_class(Symbol a1) {
      token = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   return this->type = Int_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef int_const_EXTRAS
   int_const_EXTRAS
#endif
};


// define constructor - bool_const
class bool_const_class : public Expression_class {
protected:
   Boolean val;
public:
   bool_const_class(Boolean a1) {
      val = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   return this->type = Bool_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef bool_const_EXTRAS
   bool_const_EXTRAS
#endif
};


// define constructor - string_const
class string_const_class : public Expression_class {
protected:
   Symbol token;
public:
   string_const_class(Symbol a1) {
      token = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   return this->type = Str_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef string_const_EXTRAS
   string_const_EXTRAS
#endif
};


// define constructor - new_
class new__class : public Expression_class {
protected:
   Symbol type_name;
public:
   new__class(Symbol a1) {
      type_name = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   Type type = class_table->lookup( type_name);
	   type = type == Self_type ? Current_type : Self_type;

	   return type && type->is_defined() ? type : No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef new__EXTRAS
   new__EXTRAS
#endif
};


// define constructor - isvoid
class isvoid_class : public Expression_class {
protected:
   Expression e1;
public:
   isvoid_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   return this->type = e1->get_Expr_Type() ? Bool_type : No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef isvoid_EXTRAS
   isvoid_EXTRAS
#endif
};


// define constructor - no_expr
class no_expr_class : public Expression_class {
protected:
public:
   no_expr_class() {
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   bool is_no_expr() const
   {
	   return true;
   }

   Type do_Check_Expr_Type()
   {
	   return this->type = No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef no_expr_EXTRAS
   no_expr_EXTRAS
#endif
};


// define constructor - object
class object_class : public Expression_class {
protected:
   Symbol name;
public:
   object_class(Symbol a1) {
      name = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

   Type do_Check_Expr_Type()
   {
	   Type ret = var_table->lookup( name);
	   return ret && ret->is_defined() ? ret : No_type;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef object_EXTRAS
   object_EXTRAS
#endif
};


// define the prototypes of the interface
Classes nil_Classes();
Classes single_Classes(Class_);
Classes append_Classes(Classes, Classes);
Features nil_Features();
Features single_Features(Feature);
Features append_Features(Features, Features);
Formals nil_Formals();
Formals single_Formals(Formal);
Formals append_Formals(Formals, Formals);
Expressions nil_Expressions();
Expressions single_Expressions(Expression);
Expressions append_Expressions(Expressions, Expressions);
Cases nil_Cases();
Cases single_Cases(Case);
Cases append_Cases(Cases, Cases);
Program program(Classes);
Class_ class_(Symbol, Symbol, Features, Symbol);
Feature method(Symbol, Formals, Symbol, Expression);
Feature attr(Symbol, Symbol, Expression);
Formal formal(Symbol, Symbol);
Case branch(Symbol, Symbol, Expression);
Expression assign(Symbol, Expression);
Expression static_dispatch(Expression, Symbol, Symbol, Expressions);
Expression dispatch(Expression, Symbol, Expressions);
Expression cond(Expression, Expression, Expression);
Expression loop(Expression, Expression);
Expression typcase(Expression, Cases);
Expression block(Expressions);
Expression let(Symbol, Symbol, Expression, Expression);
Expression plus(Expression, Expression);
Expression sub(Expression, Expression);
Expression mul(Expression, Expression);
Expression divide(Expression, Expression);
Expression neg(Expression);
Expression lt(Expression, Expression);
Expression eq(Expression, Expression);
Expression leq(Expression, Expression);
Expression comp(Expression);
Expression int_const(Symbol);
Expression bool_const(Boolean);
Expression string_const(Symbol);
Expression new_(Symbol);
Expression isvoid(Expression);
Expression no_expr();
Expression object(Symbol);


#endif
