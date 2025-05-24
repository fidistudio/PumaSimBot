;;;===================================================
;;; Programa del Mundo de los Bloques usando pilas
;;; cubes_stacks.clp
;;;===================================================

;;Plantilla para bloques agrupados
(deftemplate grouped-goal (multislot blocks))

;;Plantilla para objetivos individuales
(deftemplate goal (slot move)(slot on-top-of))

(deffacts initial-state
	(get-initial-state stacks 3)
	(stack A B C)
	(stack D E F)
	(stack G H I)
	;; Objetivos: todos los bloques deben estar en el piso
	(grouped-goal (blocks A B C D E F G H I))
)

;;Función recursiva para expandir la lista de bloques
(deffunction expand-goals (?lst)
	(if (not (eq ?lst nil)) then
		(assert (goal (move (nth$ 1 ?lst)) (on-top-of floor)))
		(expand-goals (rest$ ?lst))
	)
)

;;Regla para activar la expansión
(defrule expand-goal
	?goal <- (grouped-goal (blocks $?blist))
	=>
	(retract ?goal)
	(expand-goals $?blist)
)

(defrule move-directly
	?goal <- (goal (move ?block1) (on-top-of ?block2))
	?stack-1 <- (stack ?block1 $?rest1)
	?stack-2 <- (stack ?block2 $?rest2)
	=>
	(retract ?goal ?stack-1 ?stack-2)
	(assert (stack $?rest1))
	(assert (stack ?block1 ?block2 $?rest2))
	(printout t ?block1 " moved on top of " ?block2 "." crlf)
)

(defrule move-to-floor
	?goal <- (goal (move ?block1) (on-top-of floor))
	?stack-1 <- (stack ?block1 $?rest)
	=>
	(retract ?goal ?stack-1)
	(assert (stack ?block1))
	(assert (stack $?rest))
	(printout t ?block1 " moved on top of floor. " crlf)
)

(defrule clear-upper-block
	(goal (move ?block1))
	(stack ?top $? ?block1 $?)
	=>
	(assert (goal (move ?top)(on-top-of floor)))
)

(defrule clear-lower-block
	(goal (on-top-of ?block1))
	(stack ?top $? ?block1 $?)
	=>
	(assert (goal (move ?top)(on-top-of floor)))
)
