# 玩具虚拟机。。。

### example

#### expression
	1+2--->
		push 1
		push 2
		add
	- * / 同上（更换为sub, mul, div)



    : factor '*' factor
    | factor '/' factor
    | factor '+' factor
    | factor '-' factor
    | factor '>' factor
    | factor '<' factor
    | factor '>=' factor
    | factor '<=' factor
    | factor '==' factor
    | factor '!=' factor
    | factor '&&' factor
    | factor '||' factor
    | factor ('?' expr ':' factor)
    | term

grammar Impmon;

// 编译单元
compilation_unit
    : import_stmts tof_defs EOF
    ;

// import声明
import_stmts    
    : import_stmt*
    ;
import_stmt
    : 'import' name ('.'name)* ';'



    ;

// 定义
tof_defs
    : (defunc | defvars)*
    ;
defunc
    : type name '(' (params)? ')' block
    ;
defvars
    : type name ('=' expr (',' name ('=' expr)?)* )?';'
    ;

// stmts
stmts
    : stmt*
    ;
stmt
    : ';'
    | labeled_stmt
    | expr ';'
    | block
    | if_stmt
    | while_stmt
    | for_stmt
    | switch_stmt
    | break_stmt
    | return_stmt
    ;

labeled_stmt
    : IDENTIFIER ':' stmt
    ;
if_stmt 
    : 'if' '(' expr ')' stmt ('else' stmt)?
    ;
while_stmt
    : 'while' '(' expr ')' stmt
    ;
for_stmt
    : 'for' '(' expr ';' expr ';' expr ')' stmt
    ;
switch_stmt
    : 'switch' '(' expr ')' '{' ('case' expr ':' stmts)* ('default' ':' stmt)? '}'
    ;
break_stmt
    : 'break' ';'
    ;
return_stmt
    : 'return' ';'
    | 'return' expr ';'
    ;

// expr
expr
    : term '=' expr
    | factor
    ;

factor
    : factor '*' factor
    | factor '/' factor
    | factor '+' factor
    | factor '-' factor
    | factor '>' factor
    | factor '<' factor
    | factor '>=' factor
    | factor '<=' factor
    | factor '==' factor
    | factor '!=' factor
    | factor '&&' factor
    | factor '||' factor
    | factor ('?' expr ':' factor)
    | term
    ;

term 
    : INTEGER
    | CHARACTER
    | STRING
    | IDENTIFIER
    | '(' expr ')'
    | term '++'
    | term '--'
    | term '[' expr ']'
    | term '.' name
    | term '(' args ')'
    ;


// 其他
type
    : typeref
    ;
typeref 
    : typeref_base '['']'
    | typeref_base '[' INTEGER ']'
    | typeref_base '*'
    | typeref_base
    ; // 差一个函数指针
typeref_base
    : 'void'
    | 'char'
    | 'int'
    ;  

name
    : IDENTIFIER
    ;
       //: 'void' 
params
    : param (',' param)*
    | param (',' '...')? 
    ;
param
    : type name 
    ;
// 临时变量列表 + 语句列表
block
    : '{' defvar_list stmts '}'
    ;
defvar_list
    : defvars*
    ;
member_list
    : '{' (slot ';')* '}'
    ;
slot
    : type name
    ;
args
    : (expr (',' expr)*)?
    ;




// 标识符

IDENTIFIER
    : ([a-z]|[A-Z]|'_')([a-z]|[A-Z]|'_'|[0-9])*; 
INTEGER
    : ([1-9])([0-9])*                     ('U')?('L')?
    | '0'('x'|'X')([0-9]|[a-f]|[A-F])+  ('U')?('L')?
    | '0'([0-7])*                       ('U')?('L')?
    ;
CHARACTER
    : '\'' ~[\r\n]* '\''
    ;
STRING
    : '"' ~[\r\n]* '"'
    ;

// 需要忽略的字符

SKIP_TOKEN
    : [ \t\r\n\f]+ -> skip 
    ;
LINE_COMMENT
    : '//' ~[\r\n]* -> skip
	;
BLOCK_COMMENT
    : '/*' .*? '*/' -> skip
    ;

