
;;;===================================================
;;; Programa del Mundo de los Bloques usando pilas
;;; cubes_stacks.clp
;;;===================================================

;;;--------------------------
;;; Structuras
;;;--------------------------
(deftemplate goal (slot move)(slot on-top-of))
(deftemplate stack-final (multislot contents))
(deftemplate building (multislot done))

;;;--------------------------
;;;Estado inicial
;;;--------------------------
(deffacts initial-state
	(get-initial-state stacks 3)
	(stack A B C)
	(stack D E F)
	(stack G H I)
	;Objetivo final
	(stack-final (contents A D G B E H C F I))
	(building (done))
)
;;;--------------------------
;;; Reglas de movimiento
;;;--------------------------
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

;;;--------------------------
;;; Limpiar bloque objetivo
;;;--------------------------
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

;;;--------------------------
;;; Generar metas paso a paso
;;;--------------------------
(defrule iniciar-construccion
	?f <- (stack-final (contents $?rest ?base))
	?b <- (building (done))
	(not (goal (move ?block1)(on-top-of ?block2))) ; sin metas activas
	=>
	(retract ?f ?b)
	(assert (goal (move ?base) (on-top-of floor)))
	(assert (stack-final (contents $?rest)))
	(assert (building (done ?base)))
	(printout t "Meta: colocar " ?base " sobre el piso." crlf)
)


(defrule construir-siguiente
  	?sf <- (stack-final (contents $?rest ?next))
  	?b <- (building (done $?rest1 ?base))
  	(not (goal (move ?block1)(on-top-of ?block2))) ; sin metas activas
  	=>
  	(retract ?sf ?b)
  	(assert (goal (move ?next) (on-top-of ?base)))
  	(assert (stack-final (contents $?rest))) 
  	(assert (building (done $?rest1 ?base ?next))) 
  	(printout t "Meta: colocar " ?next " sobre " ?base "." crlf)
)