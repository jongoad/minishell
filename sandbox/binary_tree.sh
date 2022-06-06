1 && (2 || (3 && 4)) || (5 && 6)

4
&&
(2
||
(3
&&
1))
||
(6
&&
5)



1&(2|(3&4))|(5&6)


## 

Could maybe keep all the jobs one after the other and simply keep a “format” string as a reference to know when to recurse one level deeper. 
`1 && (2 || (3 && 4)) || (5 && 6)`   ⇒ `1&(2|(3&4))|(5&6)`

exec 1 → & → if 1 is false, stop there → ‘(’ → recurse one more → exec 2 → | → if 2 is true, stop there→ ‘(’ → recurse once more → exec 3 → & → if 3 is false, stop there → exec 4 → ‘)’ → return 4 → ‘)’ → return 2 → | → if return from 2 is true, stop there → ‘(’ → recurse once more → exec 5 → & → if false, stop there → exec 6 → ‘)’ → return 6 → if 6, whole block returns true.