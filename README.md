
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

G :=    define_expr V (V, V, ..., V) | define_expr V () |
        define_cmd V (V, V, ..., V) | define_cmd V () |
        func V (V, V, ..., V) { C } | func V () { C } |
        proc V (V, V, ..., V) { C } | proc V () { C } |
        G ; G

P :=    G ; C

```
