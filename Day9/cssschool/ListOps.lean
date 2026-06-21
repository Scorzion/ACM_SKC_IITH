import Mathlib



namespace cssschool -- starts a namespace to group the tutorial definitions

-- type variables
def doubleList (α : Type) (xs : List α) : List α :=
  xs ++ xs




-- Implicit arguments and placeholders

#eval doubleList Nat [1, 2, 3] -- should return [1, 2, 3, 1, 2, 3]
#eval doubleList _ ["a"]


-- Guarded placeholders, will supress error messages about failed synthesis of the placeholder, and instead will show the error messages in the output of the evaluation. This is useful for showing the error messages as part of the tutorial, without causing the whole file to fail to compile.

/--
error: don't know how to synthesize placeholder for argument `xs`
context:
⊢ List ℕ
-/
#guard_msgs in
#eval doubleList Nat _


-- Implicit parameters and placeholders
def dblList {α : Type} (xs : List α) : List α :=
  xs ++ xs


#eval dblList [1, 2, 3] -- should return [1, 2, 3, 1, 2, 3]
#eval dblList ["a"] -- should return ["a", "a"]


#eval @dblList Nat [1, 2, 3] -- should return [1, 2, 3, 1, 2, 3]


#eval @dblList _ [1, 2, 3] -- should return [1, 2, 3, 1, 2, 3]



/-!
List of pairs using `do` notation.
The `do` notation is a
convenient way to compose operations that involve iterating over lists.
It allows us to write code that looks more like a traditional imperative style,
 while still being purely functional.
 The same notation and behaviour holds for so-called **Monads** in general.
  We will encounter other monads later, in particular `State` monads and `Option`.
-/

def pairs {α β : Type} (xs : List α) (ys : List β) : List (α × β) := do
  let l1 ← xs
  let l2 ← ys
  pure (l1, l2)
#eval pairs [1, 2] ["a", "b"] -- should return [(1, "a"), (1, "b"), (2, "a"), (2, "b")]


/-!
## Exercise

Using the `do` notation, implement a function `innerPairs` that in a special case corresponds to the following Python list comprehension:

```python
[(x, y) for l in [[1, 2], [3, 4]] for x in l for y in l]`
```
More generally, we are given `ll: List (List α)` and we want `innerPairs` to return a list of all pairs `(x, y)` such that `x` and `y` are both elements of the same inner list in `ll`. In the above example, the answer would be `[(1, 1), (1, 2), (2, 1), (2, 2), (3, 3), (3, 4), (4, 3), (4, 4)]`.
-/

/-!
List of sums using `do` notation. Requires the type `α` to have an instance of the `Add` typeclass to tell Lean how to add elements of type `α`. This example is just a preview of using typeclasses.
-/


/--
Computes all possible sums of elements from two lists using `do` notation.

Typeclass parameter `[Add α]` is used to ensure that the type `α` supports addition. The function iterates over each element `x` in the first list `l₁` and each element `y` in the second list `l₂`, returning a new list containing all possible sums `x + y`.
-/
def sums {α : Type} [Add α] (l₁: List α) (l₂: List α ) : List α := do -- defines `sums`
  let x ← l₁ -- binds an intermediate value for the following expression
  let y ← l₂ -- binds an intermediate value for the following expression
  return x + y -- returns this value from the monadic block



end cssschool -- closes the current namespace or section
/-!
## Next files

First lets see some iductive definitions in IntroInductive.lean
Then Adder.Lean - typeclasses; instances of typeclasses; typeclass inference.
-/
