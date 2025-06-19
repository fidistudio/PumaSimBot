
;************************************************
;*						*
;*	Initial state 				*
;*						*
;*                      J.Savage, UNAM          *
;*						*
;*                      1/5/20                  *
;*                                              *
;************************************************


(deffacts Initial-state-objects-rooms-zones-actors


; Objects definitions
	( item (type Objects) (name mail_box)(room corridor)(image table)( attributes no-pick brown)(pose 0.25 1.00 0.0))
	( item (type Objects) (name box)(room corridor)(image table)( attributes no-pick brown)(pose 1.58 1.00 0.0))
	( item (type Objects) (name cold_storage)(room kitchen)(image fridge)( attributes no-pick white)(pose 1.76 1.575 0.0))
	( item (type Objects) (name tools_storage)(room deposit)(image shelf)( attributes no-pick brown)(pose 0.4 1.63 0.0))
	( item (type Objects) (name library)(room studio)(image shelf)( attributes no-pick brown)(pose 0.2 0.16 0.0))
	( item (type Objects) (name service_table)(room service)(image table)( attributes no-pick brown)(pose 1.65 0.35 0.0))
	( item (type Objects) (name desk)(room studio)(image desk)( attributes no-pick brown)(pose 0.25 0.74 0.0))
	( item (type Objects) (name bed)(room bedroom)(image bed)( attributes no-pick white)(pose 0.95 0.36 0.0))
	


	( item (type Objects) (name A)(room studio)(zone deposit)(image apple)(attributes pick)(pose 0.20 0.50 0.0))
	( item (type Objects) (name B)(room studio)(zone deposit)(image sushi)(attributes pick)(pose 0.20 0.40 0.0))
	( item (type Objects) (name C)(room studio)(zone deposit)(image milk)(attributes pick)(pose 0.20 0.30 0.0))

	( item (type Objects) (name D)(room corridor)(zone deposit)(image soap)(attributes pick)(pose 0.20 1.0 0.0))
	( item (type Objects) (name E)(room corridor)(zone deposit)(image perfume)(attributes pick)(pose 0.2 1.1 0.0))
	( item (type Objects) (name F)(room corridor)(zone deposit)(image shampoo)(attributes pick)(pose 0.20 1.2 0.0))
	

	( item (type Objects) (name freespace)(room any)(zone any)(image none)(attributes none)(pose 0.0 0.0 0.0))

; Rooms definitions
	( Room (name deposit)(zone any)(zones dummy1 frontexit frontentrance storage dummy2)(center 0.70 1.51))
	( Room (name corridor)(zone mail_box)(zones dummy1 frontexit frontentrance storage dummy2)(center 0.2 1.0))
	( Room (name studio)(zone any)(zones dummy1 frontexit frontentrance storage dummy2)(center 0.40 0.45))
	( Room (name service)(zone service_table)(zones dummy1 frontexit frontentrance storage dummy2)(center 1.65 0.55))
	( Room (name kitchen)(zone cold_storage)(zones dummy1 frontexit frontentrance deposit dummy2)(center 1.60 1.40))
	( Room (name bedroom)(zone any)(zones dummy1 frontexit frontentrance deposit dummy2)(center 1.0 0.55))
	( Room (name corridor2)(zone box)(zones dummy1 frontexit frontentrance storage dummy2)(center 1.58 1.0))

; Robots definitions
	( item (type Robot) (name robot)(zone frontexit)(pose 1.048340 1.107002 0.0))

; Furniture definitions
	( item (type Furniture) (name fridge)(zone kitchen)(image fridge)( attributes no-pick white)(pose 1.50 1.436 0.0))
	( item (type Furniture) (name table)(zone service)(image table)( attributes no-pick brown)(pose 1.65 0.35 0.0))

; Doors definitions
	( item (type Door) (name outsidedoor) (status closed) )

	( Arm (name left))

;stacks definitions
	(stack studio mail_box A B C)
    (stack corridor mail_box D E F)

	(stack kitchen cold_storage)
	(stack bedroom bed)
	(stack service service_table)

	(goal-stack 6 corridor2 box D)
	(goal-stack 5 bedroom bed E)
	(goal-stack 4 deposit any C)
	(goal-stack 3 service service_table A)
	(goal-stack 2 studio any F)
	(goal-stack 1 kitchen cold_storage B)

        (plan (name amazon) (number 0)(duration 0))

)



