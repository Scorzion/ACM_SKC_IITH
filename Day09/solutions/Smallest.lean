import Mathlib


-- LEAN Installation and Setup
-- The most common way to run Lean is using VS Code with the Lean extension.
-- https://leanprover-community.github.io/mathematics_in_lean/C01_Introduction.html#getting-started


-- What are we going to do here
-- Smallest element in a list
-- definitions and types are similar

namespace cssschool


-- def keyword is used to define a function, and the type signature `: Nat` specifies that the function returns a natural number. The function is defined using pattern matching on the input list `xs`. If the list has only one element, it returns that element. If the list has at least two elements, it recursively calls itself on the tail of the list and takes the minimum of the head and the result of the recursive call.

def smallestI (xs : List Nat) :  Nat :=
  match xs with
  | [x] => x
  | x :: y :: zs =>
    min x (smallestI  (y :: zs))
   | [] => 0 -- default value for empty list, since we cannot return a natural number that is less than all elements of the list

-- comeback to this later after we have learned about Typeclasses!


-- def smallestI' (xs : List Nat) :  Nat :=
--   match xs with
--   | [x] => x
--   | x :: y :: zs =>
--     min x (smallestI  (y :: zs))
--   | [] => default


#eval smallestI [5, 3, 8, 1, 4] -- should return 1
#eval smallestI [10] -- should return 10
#eval smallestI [] -- should return 0

namespace natural

-- We can define a smallest function which takes a non-empty list and returns the smallest element in the list.
-- We need to add a proof argument `h : l ≠ []` to ensure that the function is only called on non-empty lists, since it would not make sense to find the smallest element of an empty list.

-- @FunFact : just like terms, we can also have proposotions as arguments to functions, and we can use them to enforce preconditions on the inputs of the function.
-- Something deeper here, called Curry-Howard correspondence.



-- Error: Uncomment for explanation!
def smallest (l: List Nat) (h : l ≠ []) :  Nat :=
  match l with
  | [x] => x
  | x :: y :: zs =>
    min x (smallest (y :: zs) (by simp)) -- we need to provide a proof that the tail of the list is also non-empty, which is done by `by simp` since if the original list is non-empty, then the tail cannot be empty.


-- def smallest (l: List Nat) (h : l ≠ []) :  Nat :=
--   match l with
--   | [x] => x
--   | x :: y :: zs =>
--     min x (smallest (y :: zs) (by simp))


theorem smallest_elem (l : List Nat) (h : l ≠ []) :
    smallest l h ∈ l := by
    fun_induction smallest <;> grind


-- tactic is just a syntax for a sequence of tactics, and `grind` is a custom tactic, which combines several tactics to solve common goals. The `fun_induction` tactic is used to perform induction on the function `smallest`, which allows us to reason about its behavior based on its definition.

-- #print smallest_elem
-- #print smallest_elem._proof_1_3

theorem smallest_elem_expanded (l : List Nat) (h : l ≠ []) :
    smallest l h ∈ l := by
  induction l with
  | nil =>
    -- An empty list contradicts our hypothesis h : l ≠ []
    contradiction
  | cons x xs ih =>
    -- We inspect the tail `xs` to see if l is a singleton or has multiple elements
    cases xs with
    | nil =>
      -- Case 1: l = [x]
      -- By definition, smallest [x] h evaluates directly to x.
      -- We show that x ∈ [x] using the head constructor of membership.
      exact List.Mem.head []

    | cons y zs =>
      -- Case 2: l = x :: y :: zs
      have h_tail_nonempty : y :: zs ≠ [] := by simp
      have ih_hyp := ih h_tail_nonempty

      change min x (smallest (y :: zs) h_tail_nonempty) ∈ x :: y :: zs

      -- Instead of splitting blindly, we introduce the inequality hypothesis manually using the properties already proven for min definition
      -- Note the power of modular proofs and design

      by_cases h_le : x ≤ smallest (y :: zs) h_tail_nonempty
      · -- Subcase A: x is the minimum
        -- Nat.min_eq_left h_le proves that: min x s = x
        rw [Nat.min_eq_left h_le]
        exact List.Mem.head _
      · -- Subcase B: smallest (y :: zs) ... is the minimum
        -- If ¬(x ≤ s), then s ≤ x, so min x s = s
        have h_ge : smallest (y :: zs) h_tail_nonempty ≤ x := Nat.le_of_not_le h_le
        rw [Nat.min_eq_right h_ge]
        exact List.Mem.tail x ih_hyp

    -- After multiple trials!
    -- | cons y zs =>
    --   -- Case 2: l = x :: y :: zs
    --   -- We must supply a proof to the induction hypothesis that the tail is not empty.
    --   have h_tail_nonempty : y :: zs ≠ [] := by simp
    --   have ih_hyp := ih h_tail_nonempty -- Underized IH: smallest (y :: zs) _ ∈ y :: zs

    --   -- We tell Lean to focus on the definitional expansion of this match case:
    --   change min x (smallest (y :: zs) h_tail_nonempty) ∈ x :: y :: zs
    --   -- its like proving a lemma in place



    --   -- min a b is defined under the hood as: if a ≤ b then a else b
    --   unfold min
    --   unfold instMinNat
    --   unfold minOfLe

    -- -- Instead of unfolding, we tell Lean to split whatever if-statement logic is powering 'min'
    --   split_ifs with h_le
    --   · -- Subcase A: x ≤ smallest ...
    --     -- The goal automatically reduces to: x ∈ x :: y :: zs
    --     exact List.Mem.head _
    --   · -- Subcase B: ¬(x ≤ smallest ...)
    --     -- The goal automatically reduces to: smallest ... ∈ x :: y :: zs
    --     exact List.Mem.tail x ih_hyp


    --   split
    --   · -- Subcase A: x ≤ smallest (y :: zs) h_tail_nonempty is TRUE
    --     exact List.Mem.head _
    --   · -- Subcase B: x ≤ smallest (y :: zs) h_tail_nonempty is FALSE
    --     exact List.Mem.tail x ih_hyp

    --   by_cases h_le : x ≤ smallest (y :: zs) h_tail_nonempty
    --   · -- Subcase A: x is less than or equal to the smallest of the tail.
    --     -- The if-then-else statement evaluates to x.
    --     rw [if_pos h_le]
    --     exact List.Mem.head _
    --   · -- Subcase B: x is strictly greater.
    --     -- The if-then-else statement evaluates to the tail's smallest element.
    --     rw [if_neg h_le]
    --     exact List.Mem.tail x ih_hyp








theorem smallest_le_all (l : List Nat) (h : l ≠ []) :
  ∀ x ∈ l, smallest l h ≤ x := by
    fun_induction smallest <;> grind
  -- induction l with
  -- | nil => contradiction
  -- | cons x xs ih =>
  --   cases xs with
  --   | nil =>
  --     -- case 1 : l = [x]
  --     intro x hx
  --     cases hx with
  --     | head => exact Nat.le_refl x
  --     | tail _ h_empty => contradiction
  --   | cons y ys =>



#print smallest_le_all

-- #print smallest_le_all._proof_1_3


#eval smallest [5, 3, 8, 1, 4] (by simp) -- should return 1

macro "smallest%" l:term : term => do
  `(smallest $l (by simp))

#eval smallest% [5, 3, 8, 1, 4] -- should return 1

end natural

--- Next File ListOps.lean, Come back here later ---


namespace general

-- We can generalize the `smallest` function to work with any type that has a linear order, not just natural numbers. This is done by using a typeclass parameter `[LinearOrder α]`, which allows us to use the `min` function and compare elements of type `α`.

-- variable {α : Type} [LinearOrder α]

/--
Implementation of the smallest element in a non-empty list for any type with a linear order.
-/
def smallest {α : Type} [LinearOrder α] (l: List α) (h: l ≠ []) : α := -- defines `smallest`
  match l with -- splits computation into cases by pattern matching
  | x :: [] => x -- matches a singleton list and returns `x`
  | x :: y :: zs => -- matches a list with at least two elements and returns `min x (smallest (y :: zs) (by simp))`
    min x (smallest (y :: zs) (by simp))

#check smallest -- checks the type of `smallest`, which should be `List α → (l ≠ []) → α` for any type `α` with a linear order.


-- Error
-- theorem smallest_elem  (l : List α) (h : l ≠ []) :
--     smallest l h ∈ l := by
--     fun_induction smallest <;> grind

--
theorem smallest_elem  {α : Type} [LinearOrder α] (l : List α) (h : l ≠ []) :
    smallest l h ∈ l := by
    fun_induction smallest <;> grind



-- Error
-- theorem smallest_le_all  (l : List α) (h : l ≠ []) :
--   ∀ x ∈ l, smallest l h ≤ x := by
--     fun_induction smallest <;> grind


-- Corrected version of the theorem for any type with a linear order.
theorem smallest_le_all {α : Type} [LinearOrder α] (l : List α) (h : l ≠ []) :
  ∀ x ∈ l, smallest l h ≤ x := by
    fun_induction smallest <;> grind
  -- induction l with
  -- | nil => contradiction
  -- | cons x xs ih =>
  --   cases xs with
  --   | nil =>
  --     -- case 1 : l = [x]
  --     intro x hx
  --     cases hx with
  --     | head => exact Nat.le_refl x
  --     | tail _ h_empty => contradiction
  --   | cons y ys =>
end general



/-!
## Exercise: Smallest element for partial orders

Define a function analogous to `smallest` for lists of elements of a type with a partial order, and prove the corresponding properties. You will need to use `DecidableLE` to be able to compare elements in the list, and the definition will use `if` expressions in place of `min`.

One of the above theorems is true for partial orders, but the other is not. Which one is it? Prove the one that is true, and give a counterexample for the one that is not using the partial order on `Nat × Nat`.
-/
namespace partial_order -- starts a namespace to group the tutorial definitions
variable {α : Type} [PartialOrder α][DecidableLE α]

def smallest (l: List α) (h: l ≠ []) : α :=
  match l with
  | [x] => x
  | x :: y :: zs =>
    if x ≤ y then
      smallest (x :: zs) (by simp)
    else
      smallest (y :: zs) (by simp)

theorem smallest_elem (l : List α) (h : l ≠ []) : smallest l h ∈ l := by
  induction l with
  | nil => contradiction
  | cons x xs ih =>
    cases xs with
    | nil =>
      exact List.Mem.head []
    | cons y zs =>
      have h_tail1 : x :: zs ≠ [] := by simp
      have h_tail2 : y :: zs ≠ [] := by simp
      dsimp [smallest]
      split_ifs with hxy
      · have ih1 := ih h_tail1
        cases ih1 with
        | head => exact List.Mem.head _
        | tail _ h_mem => exact List.Mem.tail x (List.Mem.tail y h_mem)
      · have ih2 := ih h_tail2
        cases ih2 with
        | head => exact List.Mem.tail x (List.Mem.head _)
        | tail _ h_mem => exact List.Mem.tail x (List.Mem.tail y h_mem)

def counterexample_list : List (Nat × Nat) := [(1, 2), (2, 1)]

theorem counterexample_not_le :
  ¬ (∀ x ∈ counterexample_list, smallest counterexample_list (by decide) ≤ x) := by
  intro h
  have h1 : (2, 1) ∈ counterexample_list := by simp [counterexample_list]
  have h2 : (1, 2) ∈ counterexample_list := by simp [counterexample_list]
  have h_le := h (1, 2) h2
  dsimp [smallest] at h_le
  revert h_le
  decide

end partial_order -- closes the current namespace or section

end cssschool

/-!
## Next files

IsEven.lean
-/
