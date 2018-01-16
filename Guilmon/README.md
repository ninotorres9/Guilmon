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


#### import 
    import os;
    import lib;
    import abc;

	略。。。

#### defining function
    int add(int lhs, int rhs){
        return lhs + rhs;
    }

--->
	



assertEqual $defunc

$defvars = "
    int main(){
        int i = 0;
        int j = i + 10 *5;
    }
"
assertEqual $defvars

$if_stmt = "
    int main(){
        int i = 0;
        if(i > 0){
            i + 1;
        }
        else{
            i - 1;
        }
    }
"
assertEqual $if_stmt

$while_stmt = "
    int main(){
        while(1){
            int i = 0;
        }
    }
"
assertEqual $while_stmt

$switch_stmt = "
    int main(){
        int result;
        int i = 10;
        switch(i){
            case 1:
                result = 0;
                break;
            case 10:
                result = 10;
                break;
            default:
                result = 5;
        }

    }
"
assertEqual $switch_stmt
