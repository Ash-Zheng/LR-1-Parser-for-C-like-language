# LR(1) Parser for C-Like Language

This our final assignment for Compilers Course.

### C-like Grammar

>Program ::= <类型> < ID>’(‘ ‘)’<语句块> 
>
><类型>::=int | void
>
><ID>::=字母(字母|d数字)*
>
><语句块> ::= ‘{‘<内部声明> <语句串>’}’
>
><内部声明> ::= 空 | <内部变量声明>{; <内部变量声明>}
>
><内部变量声明>::=int <ID>
>
><语句串> ::= <语句> { <语句> }
>
><语句> ::= <if语句> |< while语句> | <return语句> | <赋值语句>
>
><赋值语句> ::= <ID> =<表达式>;
>
><return语句> ::= return [ <表达式> ] （注：[ ]中的项表示可选 
>
><while语句> ::= while ‘( ‘<表达式> ‘)’ <语句块>
>
><if语句> ::= if ‘(‘<表达式>’)’ <语句块> [ else <语句块> ]（注：[ ]中的项表示可选）
>
><表达式>::=<加法表达式>{ relop  <加法表达式> } （注：relop-> <|<=|>|>=|==|!=）
>
><加法表达式> ::= <项> {+ <项> | -<项>}
>
><项> ::= <因子> {* <因子> | /<因子>}
>
><因子> ::=num | ‘(‘<表达式>’)’ |<ID> FTYPE
>
>FTYPE ::= <call> | 空
>
><call> ::=’(’<实参列表> ’) ’
>
><实参> ::=<实参列表> | 空
>
><实参列表> ::= <表达式>{, <表达式>} 



### Action Table and Goto Table format：

```c++
 Action Table & GoTo Table:

        S       C       c       d       $
0       1       2       s7      s9
1                                       acc
2               3       s4      s6
3                                       r1
4               5       s4      s6
5                                       r2
6                                       r3
7               8       s7      s9
8                       r2      r2
9                       r3      r3
```



