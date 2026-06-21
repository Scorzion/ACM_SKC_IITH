import Mathlib
-- Source : https://github.com/prathamesh-turaga/bits-and-branches.git
-- Functional Programming and Logic in Lean 4

-- MyBool is a user-defined inductive type that represents a boolean value. It has two constructors, `zero` and `nzero`, which can be thought of as representing "true" and "false" respectively. The `deriving Repr, DecidableEq` part automatically generates instances for the `Repr` and `DecidableEq` typeclasses, allowing us to print values of `MyBool` and compare them for equality.

inductive MyBool where
| zero : MyBool  -- behaves like "true"
| nzero  : MyBool  -- behaves like "false"
deriving Repr, DecidableEq


-- We define a function by allotting a value to each case.
def negate (p : MyBool) : MyBool :=
  match p with
  | .zero => .nzero
  | .nzero => .zero

-- EVALUATION CHECKS
#eval negate .zero            -- expect: nzero
#eval negate .nzero            -- expect: zero
#eval negate (negate .zero)   -- expect: zero

namespace MyBool

#eval negate zero            -- expect: nzero
#eval negate nzero            -- expect: zero
#eval negate (negate zero)   -- expect: zero


def naive_my_and (p : MyBool) (q : MyBool) : MyBool :=
  match p, q with
  | zero, zero => zero
  | zero, nzero => nzero
  | nzero, zero => nzero
  | nzero, nzero => nzero

#eval naive_my_and zero nzero
#eval naive_my_and zero (naive_my_and zero zero)

-- OPTIMIZATION: Using Wildcards (_)
-- We can simplify this. `my_and` is only true if both inputs are true.
-- For all other cases, it is false.

def shorter_my_and (p : MyBool) (q : MyBool) : MyBool :=
  match p, q with
  | zero, zero => zero
  | _, _ => nzero

-- The `_` is a wildcard that matches "anything else".

-- Even shorter: Pattern match only on `p`.
-- If p is true, the result depends entirely on q.
-- If p is false, the result is always false.
def my_and (p : MyBool) (q : MyBool) : MyBool :=
  match p with
  | zero => q
  | _ => nzero

-- DEFINING OR
-- logic: returns true (zero) if at least one input is true.
def my_or (p : MyBool) (q : MyBool) : MyBool :=
  match p with
  | zero => zero -- If p is true, result is true immediately
  | _ => q           -- If p is false, result depends on q

-- -- INFIX NOTATION
-- -- We can assign symbols to these functions.
-- infix:50 " + " => my_or
-- infix:51 " * " => my_and   -- Changed from x to * for standard typing

-- #eval zero + nzero
-- #eval nzero * zero

--we prove out first small theorem, using cases.
-- example keyword is used to state a theorem, and the proof is given after the `:=` symbol. The `by` keyword introduces the proof, which can be done using tactics or by writing a term that constructs a proof.


example (p : MyBool) (q : MyBool) :
  negate (my_and p q) = my_or (negate p) (negate q) :=

   by
    cases p with
    | zero => simp [negate, my_and, my_or]
    | nzero => simp [negate, my_and, my_or]



theorem de_morgan (p : MyBool) (q : MyBool) :
  negate (my_and p q) = my_or (negate p) (negate q) := by
    induction p with
    | zero => simp [negate, my_and, my_or]
     -- case 1: p is zero (true)
    | nzero => simp [negate, my_and, my_or]
     -- case 2: p is nzero (false)

/-
## Next files

 Adder.Lean - typeclasses; instances of typeclasses; typeclass inference.
-/
