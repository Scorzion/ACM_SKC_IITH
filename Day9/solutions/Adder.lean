/-!
## Prerequisite files

* `ListOps.lean` - implicit and explicit parameters and monadic `do` notation for lists.

## Main concepts introduced

* typeclasses.
* instances of typeclasses.
* typeclass inference.


From Haskell:

In Haskell, a typeclass is a system construct that defines a set of functions or behaviors shared across different data types. It acts like an interface, enabling ad-hoc polymorphism (function overloading). This allows different types to implement the same function name in their own specific ways
-/


namespace cssschool


#eval 3 + 4 -- runs this expression as a tutorial check


#eval "hello " ++ "world" -- runs this expression as a tutorial check
/-
Uncomment the following!
failed to synthesize instance of type class
  HAdd String String ?m.4

Hint: Type class instance resolution failures can be inspected with the `set_option trace.Meta.synthInstance true` command.
-/
-- #guard_msgs in
-- #eval "hello " + "world" -- should give an error, since `+` is not defined for strings

open Add -- opens the namespace for addition, which allows us to use the `+` operator for types that have an instance of the `Add` typeclass.

#eval add 3 4



-- Three kinds of parameters in Lean, explicit, implicit and typeclass parameters. The first two are just syntax, while the last one is a special kind of implicit parameter that is used for typeclass inference.
#check add
/--
error: failed to synthesize instance of type class
  Add String

Hint: Type class instance resolution failures can be inspected with the `set_option trace.Meta.synthInstance true` command.
-/
#guard_msgs in
#check add "Hello " "world" -- checks the type of this expression, which should be `String`


instance : Add String where
  add := λ s1 s2 => s1 ++ s2  -- defines an instance of the `Add` typeclass for the `String` type, specifying that the addition operation for strings is string concatenation.

#eval "hello" + "world" -- should give an error, since `+` is not defined for strings

#eval add "Hello " "world" -- should return "Hello world"



#check (1, 2)


-- A general instance for pairs of types that have an `Add` instance. This allows us to add pairs of numbers component-wise.
-- If suppose we know how to add elements of type `α` and `β`, then we can add pairs of type `(α, β)` by adding their respective components.

-- Does program synthesis


-- instance is a keyword that allows us to define an instance of a typeclass, which can then be used by Lean's typeclass inference system to automatically find the appropriate implementation when we use the `+` operator on pairs of types that have an `Add` instance.
--It is a definition with an annotation called instance :)

instance {α β : Type}[Add α][Add β] : -- provides an instance for typeclass search
  Add (α × β) where
  add := fun (a₁, b₁) (a₂, b₂) ↦
      (a₁ + a₂, b₁ + b₂)



#eval (1, 2) +(3, 4) -- runs this expression as a tutorial check


#eval (1, 2, "Hello") + (3, 4, " world") -- should give an error, since we don't have an instance of `Add` for triples of types.


-- Universes/Hierarchy of types


#check 1
#check String
#check Type
#check Type 2
#check Type 0
#check (0 = 1)


/-!
## Next files
back to Smallest.lean - recursive functions on lists, pattern matching, and proofs by induction on lists.
-/
