### 测试

```
make
./main test/sample_src01.jtl
```
词法分析结果储存在 `result.txt` 中。

### 语法

WhileDB + 结构化宏 + 函数过程调用的语法

```
E :=    N | V | -E | E + E | E - E | E * E | E / E | E % E |
        E < E | E <= E | E == E | E != E | E >= E | E > E | 
        E && E | E || E | !E | *E
        malloc(E) | read_int() | read_char() |
        V (E, E, ..., E) | V ()

C :=    var V |
        write_int(E) |
        write_char(E) |
        E = E |
        C; C |
        if (E) then { C } else { C } |
        while (E) do { C } |
        V (E, E, ..., E) | V () |
        return E

D :=    define_expr V (V, V, ..., V) { C } | define_expr V () { C } |
        define_cmd V (V, V, ..., V) { C } | define_cmd V () { C } |
        func V (V, V, ..., V) { C } | func V () { C } |
        proc V (V, V, ..., V) { C } | proc V () { C } |
        D ; D

P :=    C |
        D ; C

```


新增的终结符(token)与非终结符
```
TM_DEF_EXPR TM_DEF_CMD
TM_DEF_FUNC TM_DEF_PROC
TM_RET
TM_COMMA

NT_PROG
NT_DEF
NT_VAR_LIST
NT_EXPR_LIST

```

新增的上下文无关语法
```
NT_VAR_LIST ->  TM_IDENT |
                TM_IDENT TM_COMMA NT_VAR_LIST

NT_EXPR_LIST -> NT_EXPR |
                NT_EXPR TM_COMMA NT_EXPR_LIST

NT_DEF ->       TM_DEF_EXPR TM_IDENT TM_LEFT_PAREN NT_VAR_LIST TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE |
                TM_DEF_EXPR TM_IDENT TM_LEFT_PAREN TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE |
                TM_DEF_CMD TM_IDENT TM_LEFT_PAREN NT_VAR_LIST TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE |
                TM_DEF_CMD TM_IDENT TM_LEFT_PAREN TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE |
                TM_DEF_FUNC TM_IDENT TM_LEFT_PAREN NT_VAR_LIST TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE |
                TM_DEF_FUNC TM_IDENT TM_LEFT_PAREN TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE |
                TM_DEF_PROC TM_IDENT TM_LEFT_PAREN NT_VAR_LIST TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE |
                TM_DEF_PROC TM_IDENT TM_LEFT_PAREN TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE |
                NT_DEF TM_SEMICOL NT_DEF

NT_EXPR ->      TM_IDENT TM_LEFT_PAREN NT_EXPR_LIST TM_RIGHT_PAREN |
                TM_IDENT TM_LEFT_PAREN TM_RIGHT_PAREN

NT_CMD ->       TM_IDENT TM_LEFT_PAREN NT_EXPR_LIST TM_RIGHT_PAREN |
                TM_IDENT TM_LEFT_PAREN TM_RIGHT_PAREN |
                TM_RET NT_EXPR

NT_PROG ->      NT_CMD |
                NT_DEF NT_SEMICOL NT_CMD

NT_WHOLE ->     NT_PROG

        
```
